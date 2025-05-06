#include "character.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>
#include <vector>

using namespace character;

Mario::Mario()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Mario", 800,600, 0, &win, &ren)) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface *surf = IMG_Load("res/Mario.png");
    if (surf == nullptr) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    tex = SDL_CreateTextureFromSurface(ren, surf);
    if (tex == nullptr) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }
    SDL_DestroySurface(surf);
}

Mario::~Mario()
{
    if (tex != nullptr)
        SDL_DestroyTexture(tex);
    if (ren != nullptr)
        SDL_DestroyRenderer(ren);
    if (win != nullptr)
        SDL_DestroyWindow(win);

    SDL_Quit();
}

void Mario::run()
{
    SDL_SetRenderDrawColor(ren, 0,0,0,255);
    SDL_FRect r {200,300, 0, 0};

    SDL_Event event;
    SDL_FRect rect;

    std::vector<CharacterAnimations::Squence> seq = {
            {CharacterAnimations::Action::SMALL_MARIO_STAND, 3},
            {CharacterAnimations::Action::SMALL_MARIO_WALK, 10},
            {CharacterAnimations::Action::SMALL_MARIO_STOP, 6},
            {CharacterAnimations::Action::SMALL_MARIO_JUMP, 20},
            {CharacterAnimations::Action::GROW_SHRINK, 7},
            {CharacterAnimations::Action::BIG_MARIO_STAND, 4},
            {CharacterAnimations::Action::BIG_MARIO_WALK, 10, true},
            {CharacterAnimations::Action::BIG_MARIO_STOP, 6, true},
            {CharacterAnimations::Action::BIG_MARIO_JUMP, 20, true},
            {CharacterAnimations::Action::BIG_MARIO_STAND, 4, true},
            {CharacterAnimations::Action::BIG_MARIO_WALK, 25, true},
    };


    for (auto & s : seq) {
        for (int j = 0; j < s.frame; ++j) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    return;
                }
            }

            auto flip = SDL_FLIP_NONE;
            int k = j;
            if (s.reverse)
                k = s.frame - j;
            else
                k = j;

            if (s.action == CharacterAnimations::Action::SMALL_MARIO_JUMP || s.action == CharacterAnimations::Action::BIG_MARIO_JUMP) {
                if (j < s.frame/2) {
                    r.y -= 3 * CharacterAnimations::SCALE_CHARACTER;
                } else {
                    r.y += 3 * CharacterAnimations::SCALE_CHARACTER;
                }
            }
            if (s.action == CharacterAnimations::Action::SMALL_MARIO_WALK || s.action == CharacterAnimations::Action::BIG_MARIO_WALK) {
                if (s.left) {
                    r.x -= 3 * CharacterAnimations::SCALE_CHARACTER;
                } else r.x += 3 * CharacterAnimations::SCALE_CHARACTER;
            }
            if (s.action == CharacterAnimations::Action::SMALL_MARIO_STOP || s.action == CharacterAnimations::Action::BIG_MARIO_STOP) {
                if (s.left) {
                    r.x -= 1 * CharacterAnimations::SCALE_CHARACTER;
                } else r.x += 1 * CharacterAnimations::SCALE_CHARACTER;
            }
            if (s.left) {
                flip = SDL_FLIP_HORIZONTAL;
            }

        rect = CharacterAnimations::getFrame(CharacterAnimations::MARIO,
                                             s.action,
                                             k);

        r.h = rect.h * CharacterAnimations::SCALE_CHARACTER;
        r.w = rect.w * CharacterAnimations::SCALE_CHARACTER;

        if (s.action == CharacterAnimations::Action::GROW_SHRINK) {
            switch (j) {
            case 1: case 3: case 4: case 6:
                    r.y -= 8 * (s.reverse ? -1 : 1) * CharacterAnimations::SCALE_CHARACTER;
                    break;
                case 2: case 5:
                    r.y += 8 * (s.reverse ? -1 : 1) * CharacterAnimations::SCALE_CHARACTER;
                    break;
                default:
                    break;
            }
        }


            SDL_RenderClear(ren);
        SDL_RenderTextureRotated(
                ren, tex, &rect, &r, 0,
                nullptr, flip);
        SDL_RenderPresent(ren);

        SDL_Delay(70);
        }
    }
}
