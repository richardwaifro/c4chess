#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m_input.h"
#include "../animation/animation.h"
#include "../network/net.h"
#include "../security/debug.h"
#include "../global.h"
#include "archive.h"
#include "event.h"
#include "chess.h"
#include "dot.h"
#include "core.h"
#include "middle.h"

int MIDDLE_TouchToTile(CHESS_CORE_TILE *chess_tile, PP4M_INPUT_POS touch_pos) {

    int result = -1;

    for (int n = 0; n < 64; n++) {

        if (touch_pos.x >= chess_tile[n].rect.x && touch_pos.x <= (chess_tile[n].rect.x + chess_tile[n].rect.w)
        && touch_pos.y >= chess_tile[n].rect.y && touch_pos.y <= (chess_tile[n].rect.y + chess_tile[n].rect.h)) {

            DEBUG_PrintBox(2, "MIDDLE_TouchToTile:");

            if (chess_tile[n].piece != NULL)
                DEBUG_PrintBox(2, "  piece = %p tile[%d] tag[%c%d] name[%d] player[%d]", chess_tile[n].piece, n, chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[n].piece->enum_piece, chess_tile[n].piece->player);
            else
                DEBUG_PrintBox(2, "  piece = %p tile[%d] tag[%c%d]", chess_tile[n].piece, n, chess_tile[n].tag.col, chess_tile[n].tag.row);



            result = n; break;
        }

    }

    return (result);
}

int MIDDLE_TagToTile(CHESS_CORE_TILE_TAG tag) {

    if (tag.col < 97 || tag.col > 104) return (-1);
    if (tag.row < 1 || tag.row > 8) return (-1);

    for (int n = 0; n < 64; n++) {
        if (tag.col == glo_chess_core_tile[n].tag.col && tag.row == glo_chess_core_tile[n].tag.row)
            return (n);
    }

    return (-1);
}

CHESS_CORE_TILE_TAG MIDDLE_TileToTag(int tile) {
    CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
    return (tag);
}

int MIDDLE_ReturnRowTile(int tile) {
    int result;
    result = glo_chess_core_tile[tile].tag.row;
    return (result);
}

int MIDDLE_ReturnColTile(int tile) {
    int result;
    result = glo_chess_core_tile[tile].tag.col;
    return (result);
}

void MIDDLE_UpdatePositionPiece(CHESS_CORE_TILE *chess_tile, int old, int new) {

    if (old == new) return;
    if ((old < 0 || old > 63) || (new < 0 || old > 63)) return;
    if (chess_tile[old].piece == NULL) return;

    DEBUG_PrintBox(2, "MIDDLE_UpdatePositionPiece:");
    DEBUG_PrintBox(2, "  chess_tile[old] = %p, %c%d", chess_tile[old].piece, chess_tile[old].tag.col, chess_tile[old].tag.row);
    DEBUG_PrintBox(2, "  chess_tile[new] = %p, %c%d", chess_tile[new].piece, chess_tile[new].tag.col, chess_tile[new].tag.row);

    if (chess_tile[new].piece != NULL) CORE_GlobalDestroyPiece(chess_tile[new].piece);

    chess_tile[new].piece = chess_tile[old].piece;
    chess_tile[new].piece->rect = chess_tile[new].rect;
    chess_tile[old].piece = NULL;

    return;
}

void MIDDLE_Unsafe_UpdatePositionPiece(CHESS_CORE_TILE *chess_tile, int old, int new) {

    if (old == new) return;
    if ((old < 0 || old > 63) || (new < 0 || old > 63)) return;
    if (chess_tile[old].piece == NULL) return;

    chess_tile[new].piece = chess_tile[old].piece;
    chess_tile[new].piece->rect = chess_tile[old].rect;
    chess_tile[old].piece = NULL;

    return;
}

void MIDDLE_UnsafePosition_Copy(CHESS_CORE_TILE *restrict src, CHESS_CORE_TILE *restrict dst) {
    if (src == NULL) return;

    for (int n = 0; n < 64; n++)
        memcpy(&dst[n], &src[n], sizeof(src[n]));

    return;
}

int MIDDLE_UpdateChangeState(SDL_Event *event, CHESS_CORE_PLAYER *player, int *socket) {

    int result = -1;
    static int position_old = -1;
    static int position_new = -1;

    PP4M_INPUT_POS touch_pos;
    touch_pos = pp4m_INPUT_MouseState(event);

    if (CORE_NET_SocketRedirect(socket, player) == 0) {

        // select choosen piece from mem
        if (touch_pos.iner != -1 && position_old == -1) {
            result = MIDDLE_TouchToTile(glo_chess_core_tile, touch_pos);
            if (result != -1 && glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player == *player) {

                position_old = result;
                CHESS_PiecePattern_RangeAllowed(glo_chess_core_tile, result, *player);
            }
        }

        // deselect choosen piece from mem
        else if (touch_pos.iner != -1 && position_old != -1) {
            result = MIDDLE_TouchToTile(glo_chess_core_tile, touch_pos);

            if (result != -1) {

                if (glo_chess_dot[result].state == true) {

                    // if is a valid move, start changing piece state
                    position_new = result;

                    ARCHIVE_UpdateRegister_PieceState(&glo_chess_core_tile[position_new], position_old, position_new);
                    EVENT_UpdateState_ChessEvent(glo_chess_core_tile, position_old, position_new, *player);
                    CORE_NET_SendRoomState(socket, &result, &position_old, &position_new);

                    MIDDLE_UpdatePositionPiece(glo_chess_core_tile, position_old, position_new);

                    DOT_StateGlobalDotReset();
                    result = -2;

                } else if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[position_old].piece->player == glo_chess_core_tile[result].piece->player) {

                    DOT_StateGlobalDotReset();
                    position_old = result;
                    CHESS_PiecePattern_RangeAllowed(glo_chess_core_tile, result, *player);

                } else if ((glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[position_old].piece->player != glo_chess_core_tile[result].piece->player) || glo_chess_core_tile[result].piece == NULL) {

                    DOT_StateGlobalDotReset();
                    position_old = -1;

                }
            }
        }
    } else {

        result = CORE_NET_RecvRoomState(socket, player, &position_old, &position_new);

    }

    if (result == -2) {

        //CORE_NET_SendRoomState(socket, &result, player, &position_old, &position_new);
        *player = CORE_ReversePlayer_State(*player);

        position_new = -1; position_old = -1;
        DEBUG_PrintBox(1, "CORE_Testing:");
        DEBUG_PrintBox(1, "  player_turn = %d", *player);
    }


    return (result);
}
