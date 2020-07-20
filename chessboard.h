#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "chesspieces.h"
#include "chessstep.h"

#include <QPoint>
#include <QPainter>
#include <QString>

class ChessBorad
{
public:
    ChessBorad();
    void initChessBorad(bool dead);

    QPoint switchBoardPos(int row, int col);
    bool switchMousePos(QPoint pt, int &row, int &col);

    void drawBoard(QPainter& painter,QPixmap pix);
    void drawPieces(QPainter &painter,QPixmap pix);
    void drawChessSelected(QPainter &painter, int id,QPixmap pix);

    void setChessPieces(ChessPieces* pieces);
    ChessPieces* getChessPieces();
    int getPiecesId(int row, int col);
    QString getPiecesName(int id);
    QString getStepName(ChessStep *step, int stepCount);

    bool sameColor(int id1, int id2);
    void killPieces(int id);
    void relivePieces(int id);
    void movePieces(int moveid, int row, int col);

    bool isRed(int id);
    bool isDead(int id);
    bool isPiecesMSX(int id);
    bool isWin(int id);

    bool canMove(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<ChessStep*>& steps);
    void regretChess(ChessStep* step);

private:
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    void getRowCol(int &row, int &col, int id);
    int getPiecesCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);

    int getSamePiecesCountAtCol1(int id, int row, int col);
    int getSamePiecesCountAtCol2(int id, int row, int col);

private:
     ChessPieces                 m_chessPieces[CHESSPIECES_COUNT];

};

#endif // BOARD_H
