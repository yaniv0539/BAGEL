#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

struct Position {
	int x;
	int y;
};
struct Direction {
	int dx;
	int dy;
};


#include "bagel.h"
using namespace std;
using namespace bagel;

void run_tests();


int main()
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		cout << SDL_GetError() << endl;
		return -1;
	}

	SDL_Window *win;
	SDL_Renderer *ren;

	if (!SDL_CreateWindowAndRenderer(
		"Pong", 800,600, 0, &win, &ren)) {
		cout << SDL_GetError() << endl;
		return -1;
	}
	SDL_Surface *surf = IMG_Load("res/OSK.jpg");
	if (surf == nullptr) {
		cout << SDL_GetError() << endl;
		return -1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
	if (tex == nullptr) {
		cout << SDL_GetError() << endl;
		return -1;
	}
	SDL_DestroySurface(surf);

	SDL_SetRenderDrawColor(ren, 255,0,0,255);
	SDL_FRect r{0,0,100,100};

	for (int i = 0; i < 1000; ++i) {
		SDL_RenderClear(ren);

		SDL_RenderTexture(ren, tex, &r, &r);
		SDL_RenderPresent(ren);

		r.x += .1f;
		r.y += .1f;
		SDL_Delay(5);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
