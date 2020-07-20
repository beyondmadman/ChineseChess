#ifndef CHESSTHINK_H
#define CHESSTHINK_H

#include <QObject>
#include <QThread>

#include "chessboard.h"
#include "chessstep.h"

class ChessThink : public QObject
{
    Q_OBJECT
public:
    explicit ChessThink(QObject *parent = 0);

    void movePieces(ChessPieces* pieces);

private:
    ChessStep* getBestMove();
    void getAllPossibleMove(QVector<ChessStep *> &steps);
    int getMinScore(int level, int curMin);
    int getMaxScore(int level, int curMax);
    int score();

    void fakeMove(ChessStep* step);
    void unfakeMove(ChessStep* step);

private:
    int                         m_level;
    ChessBorad                  m_chessBorad;

signals:
     void updataMovePieces(ChessStep* step);

public slots:
};

#endif // CHESSTHINK_H
