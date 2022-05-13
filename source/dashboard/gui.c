#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../chess/core.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"

#include "../global.h"
#include "gui.h"

/*
GUI_TextureAlias *GUI_PopupWindow_Window(SDL_Color color, int x, int y, int w, int h) {

    GUI_TextureAlias *ttr_alias = GUI_TextureAlias_InitRect(x, y, w, h, FULL);
    ttr_alias->texture = pp4m_DRAW_TextureInitColor(glo_render, color, &ttr_alias->rect, x, y, w, h);

    return (ttr_alias);
}

SDL_Texture *GUI_PopupWindow_Title(char *title, SDL_Rect *rect, SDL_Color color, SDL_Rect window_pos) {
    if (strlen(title) > 255) return (NULL);

    SDL_Texture *texture_title = NULL;
    texture_title = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, color, 24, rect, 0, 0, title);

    int w = 0, h = 0;
    SDL_QueryTexture(texture_title, NULL, NULL, &w, &h);

    rect->x = (window_pos.w / 2) - (w / 2) + window_pos.x;
    rect->y = window_pos.y;

    return (texture_title);
}

GUI_TextureAlias *GUI_CreateTexture_BackgroundInit(SDL_Color color) {

    // initializing variables
    GUI_TextureAlias *background = GUI_TextureAlias_InitRect(0, 0, glo_screen_w, glo_screen_h, FULL);
    background->texture = pp4m_DRAW_TextureInitColor(glo_render, color, &background->rect, 0, 0, glo_screen_w, glo_screen_h);

    return (background);
}
*/
GUI_TextureAlias *GUI_Create_Texture_BackgroundInit(GUI_TextureAlias *ttr_alias, SDL_Color color, int alpha) {

    // initializing variables
    ttr_alias->rect = pp4m_DRAW_InitRect(0, 0, glo_screen_w, glo_screen_h);
    ttr_alias->texture = pp4m_DRAW_TextureInitColor(glo_render, color, &ttr_alias->rect, 0, 0, glo_screen_w, glo_screen_h);

    // blending the texture for trasparent filter
    SDL_SetTextureBlendMode(ttr_alias->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(ttr_alias->texture, alpha);

    return (ttr_alias);
}

SDL_Texture *GUI_CreateTexture_BackgroundInit(SDL_Color color, int alpha) {

    // initializing variables
    SDL_Texture *texture;
    texture = pp4m_DRAW_TextureInitColor(glo_render, color, NULL, 0, 0, glo_screen_w, glo_screen_h);

    // blending the texture for trasparent filter
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, alpha);

    return (texture);
}

/*
GUI_TextureAlias GUI_CreateTexture_Button(char *title, SDL_Color color, int x, int y, int w, int h) {

    // initializing variables
    GUI_TextureAlias button;

    button.texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color, &button.rect, x, y, w, h);

    SDL_Texture *texture = NULL;
    SDL_Rect rect;

    SDL_SetRenderTarget(glo_render, button.texture);

    texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, 24, &rect, 0, 0, title);

    int w_text = 0;
    int h_text = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w_text, &h_text);

    rect.x = (w / 2) - (w_text / 2);
    rect.y = (h / 2) - (h_text / 2);

    SDL_RenderCopy(glo_render, texture, NULL, &rect);

    SDL_SetRenderTarget(glo_render, NULL);

    SDL_DestroyTexture(texture);
    texture = NULL;

    return (button);
}

int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, char *title, SDL_Color color_text, SDL_Color color_button, int point, int x_pp, int y_pp, int w, int h) {
    if (head == NULL) return (-1);

    GUI_TextureAlias *crt_alias = head->next->next->ptr;

    int x = crt_alias->rect.x + x_pp;
    int y = crt_alias->rect.y + y_pp;

    // initializing variables
    GUI_TextureAlias button;
    button.texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color_button, &button.rect, x, y, w, h);

    SDL_Texture *texture = NULL;
    SDL_Rect rect;

    SDL_SetRenderTarget(glo_render, button.texture);

    texture = pp4m_TTF_TextureFont(glo_render, path, color_text, point, &rect, 0, 0, title);

    int w_text = 0;
    int h_text = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w_text, &h_text);

    rect.x = (w / 2) - (w_text / 2);
    rect.y = (h / 2) - (h_text / 2);

    SDL_RenderCopy(glo_render, texture, NULL, &rect);

    SDL_SetRenderTarget(glo_render, NULL);

    SDL_DestroyTexture(texture);
    texture = NULL;

    pp4m_HOOK_Next(head, &button);

    return (0);
}
*/

PP4M_HOOK *GUI_PopupWindow_Init(int w, int h) {

    int x = (w / 2) - glo_screen_w / 2;
    int y = (h / 2) - glo_screen_h / 2;

    // background cloudy/blurred/polarized
    SDL_Texture *background = GUI_CreateTexture_BackgroundInit(PP4M_BLACK, 150);

    // popup window
    //GUI_TextureAlias *PopupWindow = GUI_PopupWindow_Window(PP4M_GREY_NORMAL, x, y, w, h);

    PP4M_HOOK *head = pp4m_HOOK_Init();

    pp4m_HOOK_Next(head, background);

    return (head);
}

int GUI_PopupWindow_CoreTest(PP4M_HOOK *head, SDL_Texture *background) {

    PP4M_HOOK *current = head;
    int val = pp4m_HOOK_Size(head);

    SDL_Texture *ttr_alias = current->ptr;
    current = current->next;
    //SDL_Rect *rect = current->ptr;

    SDL_Event event;
    int result = 0;

    while(result == 0) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) result = -1;

        //current = head;
        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, background, NULL, NULL);
        SDL_RenderCopy(glo_render, ttr_alias, NULL, NULL);

        /*
        if (current->next != NULL)
            for (int n = 0; n <= val; n++) {
                current = current->next;
                ttr_alias = current->ptr;

                SDL_RenderCopy(glo_render, ttr_alias->texture, NULL, &ttr_alias->rect);
            }
        */

        SDL_RenderPresent(glo_render);
    }

    // missing exiting animation
    for (int n = 0; n <= val; n++)
        pp4m_HOOK_Remove(head);

    return (result);
}

/*
int GUI_PopupWindow_Core(PP4M_HOOK *list_hook, int x, int y, int w, int h) {
    SDL_Event event;

    // background cloudy/blurred/polarized
    GUI_TextureAlias BackgroundPolar;
    BackgroundPolar = GUI_CreateTexture_BackgroundPolarize(NULL, PP4M_BLACK, 150);

    // popup window
    GUI_TextureAlias PopupWindow;
    GUI_TextureAlias_InitRect(&PopupWindow, x, y, w, h, FULL);
    PopupWindow.texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &PopupWindow.rect, x, y, w, h);

    // button continue
    GUI_TextureAlias ButtonContinue;
    ButtonContinue = GUI_CreateTexture_Button("Continua", PP4M_GREY_HEAVY, PopupWindow.rect_als.x + 10, PopupWindow.rect_als.y + 10, 420, 75);

    // button exit
    GUI_TextureAlias ButtonExit;
    ButtonExit = GUI_CreateTexture_Button("Esci dal gioco", PP4M_GREY_HEAVY, PopupWindow.rect_als.x + 10, PopupWindow.rect_als.y + 95, 420, 75);

    while(1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        SDL_RenderClear(glo_render);

        int val = pp4m_HOOK_Size(list_hook);
        PP4M_HOOK *current = list_hook;
        CHESS_CORE_TILE *hook_tile = NULL;

        for (int n = 0; n <= val; n++) {

            if (n == 0) {
                SDL_RenderCopy(glo_render, list_hook->ptr, NULL, NULL);
                hook_tile= current->ptr;
                current = current->next;
                continue;
            }

            hook_tile= current->ptr;
            current = current->next;

            SDL_RenderCopy(glo_render, hook_tile->texture, NULL, &hook_tile->rect);
            if (hook_tile->piece != NULL) SDL_RenderCopy(glo_render, hook_tile->piece->texture, NULL, &hook_tile->rect);
        }

        SDL_RenderCopy(glo_render, BackgroundPolar.texture, NULL, &BackgroundPolar.rect);
        SDL_RenderCopy(glo_render, PopupWindow.texture, NULL, &PopupWindow.rect);

        SDL_RenderCopy(glo_render, ButtonContinue.texture, NULL, &ButtonContinue.rect);
        SDL_RenderCopy(glo_render, ButtonExit.texture, NULL, &ButtonExit.rect);
        SDL_RenderPresent(glo_render);

    }

    // missing exiting animation

    SDL_DestroyTexture(BackgroundPolar.texture);
    SDL_DestroyTexture(PopupWindow.texture);

    SDL_DestroyTexture(ButtonContinue.texture);
    SDL_DestroyTexture(ButtonExit.texture);

    return 0;
}


void GUI_Testing(void) {

    SDL_Texture *texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 100);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, 100);

    SDL_Rect rect = { (glo_screen_w/2-50), (glo_screen_h/2-50), 100, 100};

    //pp4m_DRAW_TextureDrawCircle_Filled(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int circle_center_x, int circle_center_y, int radius)
    pp4m_DRAW_TextureDrawCircle_Filled(glo_render, texture, PP4M_RED, (rect.w / 2), (rect.h / 2), 50);

    SDL_Event event;
    while(1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, texture, NULL, &rect);
        SDL_RenderPresent(glo_render);

    }

    return;
}
*/
