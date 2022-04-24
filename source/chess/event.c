#include <stdio.h>

#include "event.h"
#include "core.h"
#include "chess.h"

bool glo_chess_event_layer[64];
bool glo_chess_event_layer_[64];

void EVENT_BlankLayer_Global(void) {
    for (int n = 0; n < 64; n++) {
        glo_chess_event_layer[n] = false;
    }
    return;
}

// instead of searching for eveery tile, search for glo_chess_core_piece[]
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile) {
    for (int n = 0; n < 64; n++) {
        if (chess_tile[n].piece != NULL) {
            for (int i = 0; i < 64; i++) chess_tile[n].piece->range[i] = false;
        }
    }

    return;
}

void EVENT_CheckDrawState(void) {

    for (int n = 0; n < 32; n++) {
        if (glo_chess_core_piece[n].texture != 0 && glo_chess_core_piece[n].enum_piece == NONE) { printf("  dead mouse = %d\n\n", n); }
    }

    return;
}

void EVENT_CheckOppositeAttack(CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {
        if (glo_chess_core_tile[n].piece != NULL && glo_chess_core_tile[n].piece->player != player) {
            for (int i = 0; i < 64; i++) ;
        }
    }

    return;
}

void EVENT_CheckKingState(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {

        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            if (chess_tile[n].piece->enum_piece == KING || chess_tile[n].piece->enum_piece == BKING) {

                if (glo_chess_event_layer[n] == true) {
                    printf("\n\nKING UNDER ATTACK\n\n\n"); //check enabled, a func;

                    glo_chess_event_king_uatk = true;

                    //if (player == WHITE_PLAYER) glo_chess_event_wking_uatk = true;
                    //else if (player == BLACK_PLAYER) glo_chess_event_bking_uatk = true;
                    //glo_chess_event_king_uatk = true;

                    //CORE_Testing_InterposeAttack();
                    return;
                }

                //if (player == WHITE_PLAYER) glo_chess_event_wking_uatk = false;
                //else if (player == BLACK_PLAYER) glo_chess_event_bking_uatk = false;
                //glo_chess_event_king_uatk = false;
                break;
            }
        }
    }

    return;
}

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    // for sake of testing other things, let me point out this bool can expire
    static bool check_state;
    if (!check_state) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state = true;
    }

    if (pl_bak != player) {

        EVENT_BlankLayer_Global();
        EVENT_BlankLayer_Piece(chess_tile);

        for (int n = 0; n < 64; n++)
        {
            // piece range copy
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player != player) {

                CHESS_RedirectPiecePattern(chess_tile, n, pl_bak, CHECK);

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
        EVENT_CheckDrawState();
        EVENT_CheckKingState(chess_tile, player);
    }

    if (glo_chess_event_king_uatk == true) return 2;
    return 0;
}
