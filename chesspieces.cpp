#include "chesspieces.h"
#include "common.h"

static struct InitPos
{
    int row, col;
    ChessPieces::CHESS_TYPE type;
}initPos[CHESSPIECES_HALFCOUNT] = {
                {0, 0, ChessPieces::CHE},
                {0, 1, ChessPieces::MA},
                {0, 2, ChessPieces::XIANG},
                {0, 3, ChessPieces::SHI},
                {0, 4, ChessPieces::JIANG},
                {0, 5, ChessPieces::SHI},
                {0, 6, ChessPieces::XIANG},
                {0, 7, ChessPieces::MA},
                {0, 8, ChessPieces::CHE},

                {2, 1, ChessPieces::PAO},
                {2, 7, ChessPieces::PAO},
                {3, 0, ChessPieces::BING},
                {3, 2, ChessPieces::BING},
                {3, 4, ChessPieces::BING},
                {3, 6, ChessPieces::BING},
                {3, 8, ChessPieces::BING},
                };


ChessPieces::ChessPieces()
{

}

ChessPieces::~ChessPieces()
{

}

void ChessPieces::init(int id, bool dead)
{
    if(id < CHESSPIECES_HALFCOUNT)
    {
        this->m_col = initPos[id].col;
        this->m_row = initPos[id].row;
        this->m_type = initPos[id].type;
        this->m_color = COLOR_BLACK;
    }
    else
    {
        this->m_col = CHESSBOARD_COLUMES-initPos[id-CHESSPIECES_HALFCOUNT].col;
        this->m_row = CHESSBOARD_ROWS-initPos[id-CHESSPIECES_HALFCOUNT].row;
        this->m_type = initPos[id-CHESSPIECES_HALFCOUNT].type;
        this->m_color = COLOR_RED;
    }

    this->m_dead = dead;

}

