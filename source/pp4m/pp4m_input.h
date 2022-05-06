#ifndef PP4M_INPUT_H
#define PP4M_INPUT_H

#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int iner;
} PP4M_INPUT_POS;

int             pp4m_INPUT_KeyboardHit(void);
PP4M_INPUT_POS  pp4m_INPUT_MouseState(SDL_Event *event);

#endif // PP4M_INPUT_H
