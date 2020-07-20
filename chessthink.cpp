#include "chessthink.h"

#include <QDebug>

ChessThink::ChessThink(QObject *parent) : QObject(parent)
{
    m_level = 5;
}

void ChessThink::movePieces(ChessPieces* pieces)
{
    m_chessBorad.setChessPieces( pieces);

    ChessStep* step = getBestMove();

    emit updataMovePieces(step);

}

ChessStep* ChessThink::getBestMove()
{
    ChessStep* ret = NULL;
    QVector<ChessStep*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        ChessStep* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(m_level-1, maxInAllMinScore);
        unfakeMove(step);

        if(minScore > maxInAllMinScore)
        {
            if(ret) delete ret;

            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

int ChessThink::score()
{
    static int s[] = {1000, 499, 501, 200, 15000, 100, 100};
    int scoreBlack = 0;
    int scoreRed = 0;

    ChessPieces* pieces =  m_chessBorad.getChessPieces();

    for(int i=0; i<16; ++i)
    {
        if(pieces[i].m_dead) continue;
        scoreRed += s[pieces[i].m_type];
    }
    for(int i=16; i<32; ++i)
    {
        if(pieces[i].m_dead) continue;
        scoreBlack += s[pieces[i].m_type];
    }
    return scoreBlack - scoreRed;
}

int ChessThink::getMinScore(int level, int curMin)
{
    if(level == 0)
        return score();

    QVector<ChessStep*> steps;
    getAllPossibleMove(steps);
    int minInAllMaxScore = 300000;

    while(steps.count())
    {
        ChessStep* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int maxScore = getMaxScore(level-1, minInAllMaxScore);
        unfakeMove(step);
        delete step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                ChessStep* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }

    }
    return minInAllMaxScore;
}
int ChessThink::getMaxScore(int level, int curMax)
{
    if(level == 0)
        return score();

    QVector<ChessStep*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        ChessStep* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(level-1, maxInAllMinScore);
        unfakeMove(step);
        delete step;

        if(minScore >= curMax)
        {
            while(steps.count())
            {
                ChessStep* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;
}


void ChessThink::fakeMove(ChessStep *step)
{
    m_chessBorad.killPieces(step->m_killid);
    m_chessBorad.movePieces(step->m_moveid, step->m_rowTo, step->m_colTo);
}

void ChessThink::unfakeMove(ChessStep *step)
{
    m_chessBorad.relivePieces(step->m_killid);
    m_chessBorad.movePieces(step->m_moveid, step->m_rowFrom, step->m_colFrom);
}

void ChessThink::getAllPossibleMove(QVector<ChessStep *> &steps)
{
   ChessPieces* pieces =  m_chessBorad.getChessPieces();

    for(int i = 0; i< CHESSPIECES_HALFCOUNT; i++)
    {
        if(pieces[i].m_dead) continue;
        for(int row = 0; row <= CHESSBOARD_ROWS; ++row)
        {
            for(int col = 0; col <= CHESSBOARD_COLUMES; ++col)
            {
                int killid = m_chessBorad.getPiecesId(row, col);
                if(m_chessBorad.sameColor(i, killid)) continue;

                if(m_chessBorad.canMove(i, killid, row, col))
                {
                    m_chessBorad.saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}
