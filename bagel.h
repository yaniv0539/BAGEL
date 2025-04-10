// Copyright (C) 2025 Moshe Sulamy

#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace bagel
{
	struct Bagel
	{
		bool	DynamicResize = false;
		int		IdBagSize = 5;
		int		InitialEntities = 10;
		int		InitialPackedSize = 5;
		int		MaxComponents = 10;
	};

	template <class T> struct Storage;
	template <class T> class PackedStorage;
	template <class T> class SparseStorage;
	template <class T> class TaggedStorage;

#if __has_include("bagel_cfg.h")
	#define BAGEL_STORAGE(C,T) template <> struct Storage<C> { using type = T<C>; };
	#include "bagel_cfg.h"
	#undef BAGEL_STORAGE
#else
	constexpr Bagel Params{};
#endif

	using id_type = int;
	using ent_type = struct { id_type id; };
	using size_type = int;
	using index_type = int;
	using mask_type =
		std::conditional_t<Params.MaxComponents<=8, std::uint_fast8_t,
		std::conditional_t<Params.MaxComponents<=16, std::uint_fast16_t,
		std::conditional_t<Params.MaxComponents<=32, std::uint_fast32_t,
			std::uint_fast64_t>>>;
	constexpr inline size_type BitsetWidth = sizeof(mask_type)*8;

	class NoInstance { NoInstance() = delete; };
	struct NoCopy {
		NoCopy() = default;
		NoCopy(const NoCopy&) = delete;
		void operator=(const NoCopy&) = delete;
	};

	template <class T, int N>
	class DynamicBag : NoCopy
	{
	public:
		void push(const T& t) {
			if (_size == _capacity) {
				_capacity *= 2;
				_arr = static_cast<T*>(
					realloc(_arr, sizeof(T)*_capacity));
			}
			_arr[_size] = t;
			++_size;
		}
		void ensure(size_type s) {
			if (_capacity < s) {
				_capacity = std::max(s, _capacity*2);
				_arr = static_cast<T*>(
					realloc(_arr, sizeof(T)*_capacity));
			}
		}
		T pop() { return _arr[--_size]; }
		T& operator[](index_type i) { return _arr[i]; }
		const T& operator[](index_type i) const { return _arr[i]; }
		void clear() { _size = 0; }

		size_type size() const { return _size; }
		size_type capacity() const { return _capacity; }

		~DynamicBag() { free(_arr); }
	private:
		T*			_arr = static_cast<T*>(malloc(sizeof(T) * N));
		size_type	_size = 0;
		size_type	_capacity = N;
	};
	template <class T, int N>
	class StaticBag
	{
	public:
		void push(const T& t) { _arr[_size++] = t; }
		T pop() { return _arr[--_size]; }
		T& operator[](index_type i) { return _arr[i]; }
		const T& operator[](index_type i) const { return _arr[i]; }
		void clear() { _size = 0; }

		size_type size() const { return _size; }
		static void ensure(size_type) {}
	private:
		T			_arr[N];
		size_type	_size = 0;
	};
	template <class T, int N>
	using Bag = std::conditional_t<Params.DynamicResize, DynamicBag<T, N>, StaticBag<T,N>>;

	template <class T>
	class SparseStorage final : NoInstance
	{
	public:
		static void add(ent_type e, const T& t) {
			_bag.ensure(e.id);
			_bag[e.id] = t;
		}
		static void del(ent_type) {}
		static T& get(ent_type e) { return _bag[e.id]; }
	private:
		static inline Bag<T,Params.InitialEntities> _bag;
	};
	template <class T>
	class PackedStorage final : NoInstance
	{
	public:
		static void add(ent_type e, const T& t) {
			_entToComp.ensure(e.id);
			_entToComp[e.id] = _comps.size();
			_comps.push(t);
			_compToEnt.push(e);
		}
		static void del(ent_type e) {
			index_type ent_comp_idx = _entToComp[e.id];
			ent_type last_ent = _compToEnt.pop();

			_comps[ent_comp_idx] = _comps.pop();
			_compToEnt[ent_comp_idx] = last_ent;
			_entToComp[last_ent.id] = ent_comp_idx;
		}
		static T& get(ent_type e) {
			return _comps[_entToComp[e.id]];
		}
		static int size() { return _comps.size(); }
		static T& get(index_type idx) {
			return _comps[idx];
		}
		static ent_type entity(index_type idx) {
			return _compToEnt[idx];
		}
	private:
		static inline Bag<T,Params.InitialPackedSize>			_comps;
		static inline Bag<index_type,Params.InitialEntities>	_entToComp;
		static inline Bag<ent_type,Params.InitialPackedSize>	_compToEnt;
	};
	template <class T>
	class TaggedStorage final : NoInstance
	{
	public:
		static void add(ent_type, const T&) {}
		static void del(ent_type) {}
		static T& get(ent_type) = delete;
	};

	template <class T>
	struct Storage final : NoInstance {
		using type = SparseStorage<T>;
	};

	class SingleMask final
	{
	public:
		using bit_type = mask_type;
		static constexpr bit_type bit(index_type idx) { return 1<<idx; }

		void set(bit_type b) { _mask |= b; }

		void clear(bit_type b) { _mask &= ~b; }
		void clear() { _mask = 0; }

		bool test(bit_type b) const { return _mask & b; }
		bool test(SingleMask m) const { return (_mask & m._mask) == m._mask; }
	private:
		mask_type	_mask{0};
	};
	class MultiMask final
	{
	public:
		using bit_type = struct {
			const index_type	index;
			const mask_type		mask;
		};
		static constexpr bit_type bit(index_type idx) {
			return {idx/BitsetWidth, static_cast<mask_type>(1<<(idx%BitsetWidth))};
		}

		void set(bit_type b) { _masks[b.index] |= b.mask; }

		void clear(bit_type b) { _masks[b.index] &= ~b.mask; }
		void clear() { memset(_masks, 0, sizeof(_masks)); }

		bool test(bit_type b) const { return _masks[b.index] & b.mask; }
		bool test(MultiMask m) const {
			for (index_type i = 0; i < Size; ++i)
				if ((_masks[i] & m._masks[i]) != m._masks[i])
					return false;
			return true;
		}
	private:
		static constexpr size_type	Size = (Params.MaxComponents-1)/BitsetWidth + 1;
		mask_type					_masks[Size] ={};
	};
	using Mask = std::conditional_t<Params.MaxComponents<=BitsetWidth, SingleMask, MultiMask>;

	static inline index_type compCounter = -1;
	template <class>
	struct Component final : NoInstance
	{
		static inline const index_type		Index = ++compCounter;
		static inline const Mask::bit_type	Bit = Mask::bit(Index);
	};

	class World final : NoInstance
	{
	public:
		static ent_type createEntity() {
			if (_ids.size() > 0)
				return _ids.pop();
			_masks.push(Mask{});
			return {++_maxId.id};
		}
		static void destroyEntity(ent_type ent) {
			_masks[ent.id].clear();
			_ids.push(ent);
		}
		static const Mask& mask(ent_type e) {
			return _masks[e.id];
		}
		static ent_type maxId() { return _maxId; }

		template <class T>
		static T& getComponent(ent_type e) {
			return Storage<T>::type::get(e);
		}

		template <class T>
		static void addComponent(ent_type e, const T& t) {
			_masks[e.id].set(Component<T>::Bit);
			Storage<T>::type::add(e,t);
		}
		template <class T, class...Ts>
		static void addComponents(ent_type e, const T& t, const Ts&... ts) {
			addComponent(e, t);
			if constexpr (sizeof...(Ts)>0)
				addComponents(e, ts...);
		}

		template <class T>
		static void delComponent(ent_type e) {
			_masks[e.id].clear(Component<T>::Bit);
			Storage<T>::type::del(e);
		}
		template <class T, class ...Ts>
		static void delComponents(ent_type e) {
			delComponent<T>(e);
			if constexpr (sizeof...(Ts)>0)
				delComponents<Ts...>(e);
		}

	private:
		static inline ent_type								_maxId{-1};
		static inline Bag<Mask,		Params.InitialEntities> _masks;
		static inline Bag<ent_type,	Params.IdBagSize>		_ids;
	};

	class Entity
	{

	};

	class MaskBuilder
	{
	public:
		static MaskBuilder create() { return MaskBuilder{}; }

		template <class T>
		MaskBuilder& add() {
			m.set(Component<T>::Bit);
			return *this;
		}

		Mask build() const { return m; }
	private:
		Mask m;
	};
}
