#include <stdio.h>

#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../global.h"

#include "middle.h"
#include "event.h"
#include "core.h"
#include "chess.h"

int glo_chess_event_availmo = -1;

bool glo_chess_event_layer[64];
bool glo_chess_event_king_uatk;

void EVENT_BlankLayer_Global(void) {
    for (int n = 0; n < 64; n++) {
        glo_chess_event_layer[n] = false;
    }
    return;
}

// instead of searching for eveery tile, search for glo_chess_core_piece[]
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {
    for (int n = 0; n < 64; n++) {
        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            for (int i = 0; i < 64; i++) chess_tile[n].piece->range[i] = false;
        }
    }

    return;
}

void EVENT_UpdateState_ChessEvent(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {
    if (MIDDLE_CheckTilePos(position_old, position_new) == -1) return;

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) == 0) {
        CHESS_CheckState_PawnEnPassant(chess_tile, position_old, position_new, player);
        CHESS_CheckState_PawnPromotion(chess_tile, position_old, position_new, player);
    } else if (CHESS_Redirect_EnumKing(chess_tile, position_old) == 0)
        CHESS_CheckState_KingCastling(chess_tile, position_old, position_new, player);
    else if (CHESS_Redirect_EnumRook(chess_tile, position_old) == 0)
        CHESS_CheckState_RookCastling(chess_tile, position_old, position_new, player);

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) != 0)
        _glo_chess_tile_passant = -1;

    return;
}

void EVENT_CheckKing_UnderAttack(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {

        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            if (chess_tile[n].piece->enum_piece == KING || chess_tile[n].piece->enum_piece == BKING) {

                if (glo_chess_event_layer[n] == true) glo_chess_event_king_uatk = true;
                else glo_chess_event_king_uatk = false;
                return;
            }
        }
    }

    return;
}

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state;
    if (!check_state) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state = true;
    }

    if (pl_bak != player) {

        EVENT_BlankLayer_Global();

        for (int n = 0; n < 64; n++)
        {
            // piece range copy
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player != player) {

                for (int i = 0; i < 64; i++) {

                    if (chess_tile[n].piece->range[i] == true) {
                        glo_chess_event_layer[i] = true;
                    }

                    // better to create a visible layer
                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[i].tag.col, chess_tile[i].tag.row, chess_tile[n].piece->range[i], glo_chess_event_layer[i]);
                }
            }
        }

        pl_bak = player;
        EVENT_CheckKing_UnderAttack(chess_tile, player);
    }

    if (glo_chess_event_king_uatk == true) return 2;
    return 0;
}

int EVENT_HandlePopup_Pause(int *running) {

    int result = 0;
    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

    while (1) {

        if (*running == -1) break;

        PP4M_HOOK *hook_list_pw = GUI_PopupWindow_Init(440, 180);

        GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 15, 410, 70);
        GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -2, "Esci dal gioco", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 95, 410, 70);

        result = GUI_PopupWindow_Core(hook_list_pw, txr_snapshot);

        if (result == -1) break;
        if (result == -3) { *running = -1; break; }

        SDL_Texture *txr_snapshot2 = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
        PP4M_HOOK *hook_list_pw_exit = GUI_PopupWindow_Init(400, 165);

        GUI_PopupWindow_Title(hook_list_pw_exit, OPENSANS_REGULAR, "Sei sicuro?", PP4M_WHITE, 32);
        GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -1, "Annulla", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 85, 185, 70);
        GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -2, "Okay", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 205, 85, 185, 70);

        if (GUI_PopupWindow_Core(hook_list_pw_exit, txr_snapshot2) == -2) *running = -1;

        hook_list_pw_exit = NULL;
    }

    return *running;
}

int EVENT_HandleKingState(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {
    int result = -1;
    glo_chess_event_availmo = 0;

    for (int n = 0; n < 64; n++)
        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player)
            for (int i = 0; i < 64; i++)
                if (chess_tile[n].piece->range[i] == true)
                    if (chess_tile[i].piece == NULL || chess_tile[i].piece->player != player)
                        glo_chess_event_availmo += 1;

    if (glo_chess_event_availmo == 0)
        for (int n = 0; n < 64; n++)
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player)
                if (CHESS_Redirect_EnumKing(chess_tile, n) == 0) {
                    if (glo_chess_event_layer[n] == true) result = EVENT_HandlePopup_Checkmate(NULL, player);
                    else result = EVENT_HandlePopup_Stalemate(NULL);

                    break;
                }

    return result;
}

int EVENT_HandlePopup_Stalemate(char *comment) {
    int result = -1;

    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
    PP4M_HOOK *hook_list = GUI_PopupWindow_Init(450, 150);

    GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, "Stalemate", PP4M_WHITE, 32);

    // leaving this for now
    if (comment != NULL) GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, comment, PP4M_WHITE, 32);
    GUI_PopupWindow_Button(hook_list, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 90, 430, 50);

    result = GUI_PopupWindow_Core(hook_list, txr_snapshot);
    return result;
}

int EVENT_HandlePopup_Checkmate(char *comment, CHESS_CORE_PLAYER player) {
    (void)player;

    int result = -1;

    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
    PP4M_HOOK *hook_list = GUI_PopupWindow_Init(450, 150);

    GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, "Checkmate", PP4M_WHITE, 32);

    // leaving this for now
    if (comment != NULL) GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, comment, PP4M_WHITE, 32);
    GUI_PopupWindow_Button(hook_list, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 90, 430, 50);

    result = GUI_PopupWindow_Core(hook_list, txr_snapshot);
    return result;
}
