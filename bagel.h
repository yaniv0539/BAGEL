// Copyright (C) 2025 Moshe Sulamy

#pragma once
#include <cstdlib>
#include <type_traits>

namespace bagel
{
	struct Bagel
	{
		bool	DynamicResize = false;
		int		IdBagSize = 5;
		int		InitialEntities = 10;
		int		InitialPackedSize = 5;
	};
	constexpr Bagel Params{};

	using id_type = int;
	using ent_type = struct { id_type id; };
	using size_type = int;
	using index_type = int;

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
		void ensure(size_type) const {}
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
		static Bag<T,Params.InitialEntities> _bag;
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
	private:
		static inline Bag<T,Params.InitialPackedSize>			_comps;
		static inline Bag<index_type,Params.InitialEntities>	_entToComp;
		static inline Bag<ent_type,Params.InitialPackedSize>	_compToEnt;
	};

	template <class T>
	struct Storage final : NoInstance {
		using type = SparseStorage<T>;
	};

	class World final : NoInstance
	{
	public:
		static ent_type createEntity() {
			if (_ids.size() > 0)
				return _ids.pop();
			return {++_maxId.id};
		}
		static void destroyEntity(ent_type entity) {
			_ids.push(entity);
		}
	private:
		static inline Bag<ent_type,Params.IdBagSize>	_ids;
		static inline ent_type							_maxId{-1};
	};
}
