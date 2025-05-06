#pragma once
#include <iostream>

#include "SDL3/SDL.h"
#include "box2d/box2d.h"
#include "bagel.h"
#include "lib/box2d/src/body.h"
#include "SDL3_image/SDL_image.h"

namespace mario {
    class Mario
    {
    public:
        Mario();
        ~Mario();
        SDL_Texture* getTexture(char* path);

        void run();
    private:
        static constexpr int FPS = 60;
        static constexpr float BOX_SCALE = 10.0f;
        static constexpr float TEX_SCALE = 0.5f;
        static constexpr SDL_FRect BALL_TEX = {404, 580, 76, 76};

        SDL_Texture* tex;
        SDL_Renderer* ren;
        SDL_Window* win;

        b2WorldId world;
    };

    /* ================ Components ================ */

    static constexpr int DEFAULT_CAMERA_WIDTH = 800;
    static constexpr int DEFAULT_CAMERA_HEIGHT = 600;

    /// @brief Components are the data structures that hold the data for each entity.

    /// @brief Position component holds the position of the entity.
    struct Position {
        float x = 0, y = 0; // Position in the game world
    };

    /// @brief Movement component holds the velocity of the entity.
    struct Movement {
        float vx = 0, vy = 0; // Velocity in x and y directions
    };

    /// @brief Physics component holds the physics body and shape.
    struct Physics {
        b2Body* body = nullptr; // Box2D body for physics simulation
        b2Shape* shape = nullptr; // Shape of the body (e.g., box, circle)
        float mass = 1.0f; // Mass of the body
    };

    /// @brief Texture component holds the texture of the entity.
    struct Texture {
        SDL_Texture* texture = nullptr; // SDL texture for rendering
        SDL_Rect src = {0, 0, 0, 0}; // Source rectangle for texture
        SDL_Rect dst = {0, 0, 0, 0};; // Destination rectangle for rendering
    };

    /// @brief AnimatedImage component holds the texture of the entity and the animation frames.
    struct AnimatedImage {
        SDL_Texture* texture = nullptr; // SDL texture for animation
        SDL_Rect src = {0, 0, 0, 0}; // Source rectangle for the current frame
        SDL_Rect dst = {0, 0, 0, 0}; // Destination rectangle for rendering
        int frameCount = 0; // Total number of frames
        int currentFrame = 0; // Current frame index
        float frameTime = 0.1f; // Time per frame
    };

    /// @brief Collider component holds the physics body and shape.
    struct Collider {
        b2Body* body = nullptr; // Box2D body for collision
        b2Shape* shape = nullptr; // Shape used for collision detection
        bool isTrigger = false; // Whether the collider is a trigger
    };

    /// @brief Input component holds the input state of the entity.
    struct Input {
        bool up = false, down = false, left = false, right = false; // Movement inputs
        bool jump = false; // Jump input
        bool shoot = false; // Shoot input
    };

    /// @brief Camera component holds the camera's dimensions and position.
    struct Camera {

    };

    /// @brief EnemyType enum holds the different types of enemies.
    enum class EnemyType {
        Goomba,
        Koopa,
        BulletBill,
        CheepCheep,
        HammerBro,
        Bowser,
    };

    /// @brief Enemy component holds the type of the enemy.
    struct Enemy {
        EnemyType type; // Type of enemy (e.g., Goomba, Koopa)
        float speed = 1.0f; // Movement speed
    };

    struct Player {
        int lives = 3; // Number of lives
        int score = 0; // Player's score
        int coins = 0; // Number of coins collected
    };

    struct MarioState {
        bool isBigMario = false; // Size of Mario (small, big)
        bool isFireMario = false; // Whether Mario is in fire mode
        bool isSuperMario = false; // Whether Mario is invincible
        bool isCrouching = false; // Whether Mario is crouching
    };

    /// @brief State component holds the state of the entity.
    struct State {
        bool isWalking = false; // Whether the entity is walking
        bool isOnGround = true; // Whether the entity is on the ground
        bool isAlive = true; // Whether the entity is alive
        bool isVisible = true; // Whether the entity is visible
    };

    /// @brief BlockType enum holds the different types of blocks.
    enum class BlockType {
        Brick,
        Question,
        Empty,
        Solid
    };

    /// @brief Block component holds the state of a block.
    struct Block {
        BlockType type; // Type of block (e.g., brick, question)
        bool holdsCollectable = false; // Whether the block holds a collectable
        int hits = 1; // Number of hits before the block breaks
        bool isBreakable = true; // Whether the block can be broken
    };

    /// @brief CollectableType enum holds the different types of collectables.
    enum class CollectableType {
        Coin,
        Mushroom,
        FireFlower,
        Star,
        Flag,
        None
    };

    /// @brief Collectable component holds the type and effect of a collectable.
    struct Collectable {
        CollectableType type; // Type of power-up (e.g., mushroom, fire flower)
    };

    /// @brief ScoreValue component holds the score value of the entity.
    struct ScoreValue {
        int value; // Points awarded when the entity is interacted with
    };

    /// @brief Lifetime component holds the remaining lifetime of the entity.
    struct Lifetime {
        float timeRemaining; // Time remaining before the entity is destroyed
    };

    /* ================ Entities ================ */


    /// @brief Creates Mario entity.
    /// @param x,y Position of the entity in the game world.
    /// @param renderer SDL renderer used to create the texture.
    /// @return A `bagel::ent_type` representing the Mario entity.
    inline bagel::ent_type createMario(float x, float y, SDL_Renderer* renderer) {
        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
                    Position{x, y},
                    MarioState{},
                    Movement{},
                    Physics{},
                    Texture{},
                    AnimatedImage{},
                    Collider{},
                    Input{},
                    State{}
                    );

        return entity.entity();
    }

    /// @brief Creates Enemy entity .
    /// @param x,y Position of the entity in the game world.
    /// @param type Type of the enemy (e.g., Goomba, Koopa).
    inline bagel::ent_type createEnemy(float x, float y, EnemyType type, int scoreValue) {
        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
                    Position{x, y},
                    Enemy{type},
                    ScoreValue{scoreValue},
                    Movement{},
                    Physics{},
                    Texture{},
                    AnimatedImage{},
                    Collider{},
                    State{}
                    );

        return entity.entity();
    }

    /// @brief Creates Projectile entity.
    /// @param x,y Position of the entity in the game world.
    /// @param
    inline bagel::ent_type createProjectile(float x, float y) {
        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
                    Position{x, y},
                    Movement{},
                    Physics{},
                    Texture{},
                    Collider{},
                    AnimatedImage{}
                    );

        return entity.entity();
    }

    /// @brief Creates Collectable entity.
    /// @param x,y Position of the entity in the game world.
    /// @param type Type of collectable (e.g., Coin, Mushroom).
    /// @param scoreValue Score value of the collectable.
    inline bagel::ent_type createCollectable(float x, float y, CollectableType type, int scoreValue = 0) {
        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
                    Position{x, y},
                    Collectable{type},
                    Texture{},
                    AnimatedImage{},
                    ScoreValue{scoreValue},
                    Collider{},
                    State{}
                    );

        switch (type) {
            case CollectableType::Mushroom:
                entity.addAll(Movement{}, Physics{});
                break;
            default:
                break;
        }
        return entity.entity();
    }

    /// @brief Creates Block entity.
    /// @param x,y Position of the entity in the game world.
    /// @param type Type of block (e.g., Brick, Question).
    /// @param holdsCollectable Whether the block holds a collectable.
    /// @param hits Number of hits before the block breaks.
    /// @param isBreakable Whether the block can be broken.
    /// @param collectable Type of collectable held by the block.
    inline bagel::ent_type createBlock(float x, float y, BlockType type,
                                     bool holdsCollectable = false, int hits = 1, bool isBreakble = true,
                                     CollectableType collectable = CollectableType::None) {

        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
                    Position{x, y},
                    Block{type, holdsCollectable, hits, isBreakble},
                    Texture{},
                    Collider{}
                    );

        if (holdsCollectable) {
            entity.add(Collectable{collectable});
        }

        if (type == BlockType::Question) {
            entity.addAll(AnimatedImage{});
        }

        if (isBreakble) {
            entity.add(ScoreValue{50});
        }

        return entity.entity();
    }

    /// @brief Creates a Background entity.
    /// @param x,y Position of the background in the game world.
    /// @param renderer SDL renderer used to create the texture.
    /// @param texturePath File path to the texture image.
    inline bagel::ent_type createBackground(float x, float y, SDL_Renderer* renderer, const std::string& texturePath) {
        bagel::Entity entity = bagel::Entity::create();

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(texturePath.c_str()));
        if (!texture) {
            // Handle texture loading error
            SDL_Log("Failed to load texture: %s", SDL_GetError());
            return entity.entity();
        }

        entity.addAll(
            Position{x, y},
            Texture{texture, {0, 0, 0, 0}, {0, 0, 0, 0}}
        );

        return entity.entity();
    }

    /// @brief Creates a Camera entity.
    /// @param x,y Position of the camera in the game world.
    /// @return A `bagel::Entity` representing the camera with position, movement, and camera components.
    inline bagel::ent_type createCamera(float x, float y) {
        bagel::Entity entity = bagel::Entity::create();

        entity.addAll(
            Position{x, y},
            Movement{},
            Camera{}
        );

        return entity.entity();
    }

    /* ================ Systems ================ */

    /// @brief Handles the movement of entities with Position and Movement components.
    class MovemntSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Position>()
                            .set<Physics>()
                            .set<Movement>()
                            .build();
    };

    /// @brief Renders entities with Position and Texture components.
    class RenderSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Position>()
                            .set<Texture>()
                            .build();
    };

    /// @brief Processes input for entities with the Input component.
    class InputSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Input>()
                            .build();
    };

    /// @brief Handles player control by processing Input, Movement, and State components.
    class PlayerControlSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Input>()
                            .set<Movement>()
                            .set<State>()
                            .build();
    };

    /// @brief Manages collision detection and response for entities with Position, Collider, and ScoreValue components.
    class CollisionSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Position>()
                            .set<Physics>()
                            .set<Collider>()
                            .set<ScoreValue>()
                            .build();
    };

    /// @brief Applies power-ups to entities with Collider and Collectable components.
    class PowerUpsSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Collider>()
                            .set<Collectable>()
                            .build();
    };

    /// @brief Updates animations for entities with State and AnimatedImage components.
    class AnimationSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<State>()
                            .set<AnimatedImage>()
                            .build();
    };

    /// @brief Updates the score for entities with the ScoreValue component.
    class ScoreSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<ScoreValue>()
                            .build();
    };

    /// @brief Handles the destruction of entities with the State component when they are no longer alive.
    class DeathSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<State>()
                            .build();
    };

    /// @brief Updates the camera position based on entities with Position and Camera components.
    class CameraSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Position>()
                            .set<Camera>()
                            .build();
    };

    /// @brief Destroys entities with the Lifetime component when their lifetime expires.
    class LifetimeSystem final: bagel::NoInstance
    {
    public:
        static void run() {
            for (bagel::ent_type e = {0}; e.id <= bagel::World::maxId().id; ++e.id) {
                bagel::Entity entity{e};
                if (entity.test(mask)) {
                    // Process the entity
                }
            }
        }
    private:
        static inline bagel::Mask mask = bagel::MaskBuilder()
                            .set<Lifetime>()
                            .build();
    };
}
