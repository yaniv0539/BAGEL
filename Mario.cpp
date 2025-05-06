#include "Mario.h"
#include <iostream>
#include "SDL3/SDL.h"
#include "box2d/box2d.h"
#include "bagel.h"
#include "lib/box2d/src/body.h"
#include "SDL3_image/SDL_image.h"

namespace mario
{
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
    }

    SDL_Texture* Mario::getTexture(char* path)
    {
        SDL_Surface *surf = IMG_Load(path);
        if (surf == nullptr) {
            std::cout << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
        if (tex == nullptr) {
            std::cout << SDL_GetError() << std::endl;
            return nullptr;
        }
        return tex;
    }



}