#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "core.h"

bool INPUT_Exit(SDL_Event *event) {

  if (event->type == SDL_QUIT) return true;

  return false;
}

int INPUT_ConvertPositionToTile(int x, int y) {

  int z = 0;
  for (int n = 0; n < 8; n++) {

    if (y >= tile[z].pp4m.rect.y && y < (tile[z].pp4m.rect.y)+50)
    for (int i = 0; i < 8; i++) {

        if (x >= tile[z].pp4m.rect.x && x < (tile[z].pp4m.rect.x)+50) return z;

        z += 1;
      }
    z += 8;
  }

  return -1;
}

int INPUT_TouchInteractPiece(SDL_Event *event) {

    int foo = -1;
    if (event->type == SDL_FINGERDOWN) {

          int touch_x, touch_y;
          SDL_GetMouseState(&touch_x, &touch_y);

          foo = INPUT_ConvertPositionToTile(touch_x, touch_y);

        }

  return foo;
}

int INPUT_MouseInteractPiece(SDL_Event *event) {

    int foo = -1;
    if (event->type == SDL_MOUSEBUTTONDOWN) {

          int touch_x, touch_y;
          SDL_GetMouseState(&touch_x, &touch_y);

          foo = INPUT_ConvertPositionToTile(touch_x, touch_y);

        }

  return foo;
}