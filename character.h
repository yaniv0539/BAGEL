#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>
#include "character_data.h"

namespace character {
    class Mario
    {
    public:
        Mario();
        ~Mario();

        void run();
    private:
        static constexpr int FPS = 60;

        SDL_Texture* tex;
        SDL_Renderer* ren;
        SDL_Window* win;
    };

    class CharacterAnimations {
    public:
        static constexpr int NEXT_FRAME_OFFSET = 2;
        static constexpr int DEFAULT_SQUARE_WIDTH = 32;
        static constexpr int DEFAULT_SQUARE_HEIGHT = 32;
        static constexpr int GROW_AMOUNT = 16;
        static constexpr float SCALE_CHARACTER = 4;

        enum class Action {
            SMALL_MARIO_STAND = 0,
            SMALL_MARIO_WALK,
            SMALL_MARIO_STOP,
            SMALL_MARIO_JUMP,
            SMALL_MARIO_DIE,
            BIG_MARIO_STAND,
            BIG_MARIO_WALK,
            BIG_MARIO_STOP,
            BIG_MARIO_JUMP,
            BIG_MARIO_CROUCH,
            GROW_SHRINK,
            FIRE_MARIO_STAND,
            FIRE_MARIO_WALK,
            FIRE_MARIO_STOP,
            FIRE_MARIO_JUMP,
            FIRE_MARIO_CROUCH,
            FIRE_MARIO_FIRE,
            FIRE,
            EXPLODE,
        };

        struct Character {
            CharacterData::CharacterSpriteInfo *sprites;
        };

        static constexpr Character MARIO = {
                const_cast<CharacterData::CharacterSpriteInfo *>(CharacterData::MARIO_SPRITE)
        };

        // Returns the sprite rectangle for a given action and frame
        static SDL_FRect getFrame(Character cha, Action action, int frame) {

            if (action == Action::SMALL_MARIO_WALK || action == Action::BIG_MARIO_STAND) {
                frame = frame + 2;
            }

            if (action == Action::GROW_SHRINK) {
                switch (frame) {
                    case 0: case 2:
                        action = Action::SMALL_MARIO_STAND;
                        break;
                    case 1: case 3: case 5:
                        action = Action::GROW_SHRINK;
                        break;
                    default:
                        action = Action::BIG_MARIO_STAND;
                        break;
                }
            }

            auto x = static_cast<float>(cha.sprites[static_cast<int>(action)].x +
                    ((frame % cha.sprites[static_cast<int>(action)].frameCount) * (NEXT_FRAME_OFFSET + cha.sprites[static_cast<int>(action)].w)));

            return {x,
                    static_cast<float>(cha.sprites[static_cast<int>(action)].y),
                    static_cast<float>(cha.sprites[static_cast<int>(action)].w),
                    static_cast<float>(cha.sprites[static_cast<int>(action)].h)
            };
        }

        struct Squence {
            Action action;
            int frame;
            bool left = false; // true = right, false = left
            bool reverse = false; // true = reverse, false = normal
        };

    };

}