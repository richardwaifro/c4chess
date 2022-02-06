#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"

#include "../global.h"
#include "event.h"
#include "chess.h"
#include "dot.h"
#include "touch.h"
#include "middle.h"
#include "core.h"

CHESS_CORE_PLAYER glo_chess_core_player;
CHESS_CORE_TILE glo_chess_core_tile[64];
CHESS_CORE_PIECE glo_chess_core_piece[32];

#define TEX_WKING "resources/wking.png"
#define TEX_WPAWN "resources/wpawn.png"
#define TEX_WKNIGHT "resources/wknight.png"
#define TEX_WBISHOP "resources/wbishop.png"
#define TEX_WROOK "resources/wrook.png"
#define TEX_WQUEEN "resources/wqueen.png"

#define TEX_BKING "resources/bking.png"
#define TEX_BPAWN "/resources/bpawn.png"
#define TEX_BKNIGHT "resources/bknight.png"
#define TEX_BBISHOP "resources/bbishop.png"
#define TEX_BROOK "resources/brook.png"
#define TEX_BQUEEN "resources/bqueen.png"

char chess_initpiece[8][8] = {
 "        ",
 "      P ",
 "   K    ",
 "        ",
 "        ",
 "    pp  ",
 " k      ",
 "        "
};

int CORE_InitPiece(CHESS_CORE_PIECE *piece, int tile, CHESS_CORE_ENUM_PIECE name, CHESS_CORE_PLAYER player) {

    char pathfile[256];
    strcpy(pathfile, glo_current_dir);

    printf("player = %p\n", &player);

    if (glo_chess_core_player == WHITE_PLAYER) {

        if (player == WHITE_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN:
                break;
                case NONE:
                break;
            }
        }

        else if (player == BLACK_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN: 
                    strcat(pathfile, TEX_BPAWN);
                    piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, pathfile, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN:
                break;
                case NONE:
                break;
            }
        }

    }

    else if (glo_chess_core_player == BLACK_PLAYER) {

        if (player == WHITE_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN:
                break;
                case NONE:
                break;
            }
        }

        else if (player == BLACK_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN:
                break;
                case NONE:
                break;
            }
        }

    }

    if (piece->texture != NULL) printf("piece->texture = %p\n", &piece->texture);
    else {
        printf("piece->texture not initialized\n");
        return (EXIT_FAILURE);
    }

    glo_chess_core_tile[tile].piece = piece;
    glo_chess_core_tile[tile].piece->player = player;
    glo_chess_core_tile[tile].piece->enum_piece = name;

    return (EXIT_SUCCESS);
}

int CORE_ReadArrayInitPiece(char array[8][8], CHESS_CORE_PLAYER player) {

    int result = -1;

    CHESS_CORE_PLAYER pl_bak = player;
    int count_core_piece;

    if (pl_bak == WHITE_PLAYER) {

        int j = 0;

        for (int n = 0; n < 8; n++) {

            for (int i = 0; i < 8; i++) {

                if (array[n][i] != ' ') {
                    switch(array[n][i]) {
                        case 'K':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'k':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'P':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BPAWN, player);
                        count_core_piece++;
                        break;
                        case 'p':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, PAWN, player);
                        count_core_piece++;
                        break;
                        case 'N':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'n':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'B':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'b':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'R':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'r':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'Q':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                        case 'q':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                    }

                    printf("fuck white %p\n", &player);
                    if (result != EXIT_SUCCESS) return (EXIT_FAILURE);
                }
                j += 1;
            }
        }
    }

    else if (pl_bak == BLACK_PLAYER) {

        int j = 63;

        for (int n = 0; n < 8; n++) {
            for (int i = 0; i < 8; i++) {

                if (array[n][i] != ' ') {
                    switch(array[n][i]) {
                        case 'K':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'k':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'P':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, PAWN, player);
                        count_core_piece++;
                        break;
                        case 'p':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BPAWN, player);
                        count_core_piece++;
                        break;
                        case 'N':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'n':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'B':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'b':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'R':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'r':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'Q':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                        case 'q':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                    }

                    printf("fuck black %p\n", &player);
                    if (result != EXIT_SUCCESS) return (EXIT_FAILURE);
                }
                j -= 1;
            }
        }
    }

    return (EXIT_SUCCESS);
}

void CORE_ChessCreateBoard(void) {

    int size_tile = 50;
    int x_b = glo_screen_w / 2 - (size_tile * 4);
    int y_b = glo_screen_h / 2 - (size_tile * 4);

    int colomn = 1;

    bool toggle = false;

    for(int n = 0; n < 64; n++) {

        if (colomn > 8) {
            y_b += size_tile;
            x_b = glo_screen_w / 2 - (size_tile * 4);
            toggle ^= 1;
            colomn = 1;
        }

        if (toggle == false) {
            toggle = true;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        else if (toggle == true) {
            toggle = false;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_RED, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        x_b += size_tile; colomn += 1;
    }

    return;
}

void CORE_ChessInitTag(CHESS_CORE_PLAYER player) {

    char colomn[] = "abcdefgh";

    if (player == WHITE_PLAYER) {

        int colomn_pos = 0;
        int row = 8;

        for (int n = 0; n < 64; n++) {

            glo_chess_core_tile[n].tag.col = colomn[colomn_pos];
            glo_chess_core_tile[n].tag.row = row;

            colomn_pos++;
            if (colomn_pos > 7) { colomn_pos = 0; if (row != 1) row--; }

        }

    }

    else if (player == BLACK_PLAYER) {

        int colomn_pos = 7;
        int row = 1;

        for (int n = 0; n < 64; n++) {

            glo_chess_core_tile[n].tag.col = colomn[colomn_pos];
            glo_chess_core_tile[n].tag.row = row;

            colomn_pos--;
            if (colomn_pos < 0) { colomn_pos = 7; if (row != 8) row++; }

        }

    }

    return;
}

/*
void CORE_ChessInitPlacePiece(void) {

    int i = 0;
    for (int n = 0; n < 32; n++) {

        if (i == 16) i += 32;
        glo_chess_core_tile[i].piece = &glo_chess_core_piece[n];
        i++;

    }

    return;
}
*/

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE *piece) {

    if (piece != NULL) {
        printf("CORE_GlobalDestroyPiece:\n  destroy piece = %p\n", piece);
        SDL_DestroyTexture(piece->texture);
        piece->enum_piece = NONE;
    }

    return;
}

void CORE_GlobalClearCorePiece(void) {
    for (int n = 0; n < 32; n++) if (sizeof(glo_chess_core_piece[n]) != 0) CORE_GlobalDestroyPiece(&glo_chess_core_piece[n]);
    return;
}

void CORE_GlobalClearChessTile(void) {

    for (int n = 0; n < 64; n++) {
        if (sizeof(glo_chess_core_tile[n].piece) != sizeof(CHESS_CORE_PIECE*)) CORE_GlobalDestroyPiece(glo_chess_core_tile[n].piece);
        SDL_DestroyTexture(glo_chess_core_tile[n].texture);
    }

    return;
}

void CORE_InitGlobalChessPiece(void) {

    for (int n = 0; n < 32; n++) {

        glo_chess_core_piece[n].player =        -1;
        glo_chess_core_piece[n].enum_piece =    NONE;
        glo_chess_core_piece[n].texture =       NULL;

        glo_chess_core_piece[n].rect.x =        0;
        glo_chess_core_piece[n].rect.y =        0;
        glo_chess_core_piece[n].rect.w =        0;
        glo_chess_core_piece[n].rect.h =        0;

        glo_chess_core_piece[n].lock =          -1;

    }

    return;
}

void CORE_Testing(CHESS_CORE_PLAYER player) {    

    glo_chess_core_player = player;

    CORE_ChessCreateBoard();
    CORE_ChessInitTag(player);

    DOT_InitGlobalDot();

    // set glo_chess_core_piece to initial state
    CORE_InitGlobalChessPiece();

    if (CORE_ReadArrayInitPiece(chess_initpiece, player) != EXIT_SUCCESS) return;

    SDL_Event event;
    player = WHITE_PLAYER;

    while(1) {
        SDL_PollEvent(&event);

        EVENT_CheckPieceLayer(player);

        if (MIDDLE_UpdateChangeState(&event, player) == 0) { player ^= 1; printf("CORE_Testing:\n  player_turn = %d\n", player); }

        SDL_RenderClear(glo_render);
        for (int n = 0; n < 64; n++) {
            SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
            if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].rect);

            DOT_StateGlobalDot(n);
        }
        SDL_RenderPresent(glo_render);

        if (event.type == SDL_QUIT) { break; }
    }

    return;
}
