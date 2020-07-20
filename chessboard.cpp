#include "chessboard.h"

#include <QStringList>

ChessBorad::ChessBorad()
{
    initChessBorad(true);
}

void ChessBorad::initChessBorad(bool dead)
{
    for(int i=0; i<CHESSPIECES_COUNT; ++i)
    {
        m_chessPieces[i].init(i,dead);
    }
}

QPoint ChessBorad::switchBoardPos(int row, int col)
{
    QPoint pt(CHESSBOARD_CELLWIDTH*col+CHESSBOARD_X, CHESSBOARD_CELLWIDTH*row + CHESSBOARD_Y);
    return pt;
}

bool ChessBorad::switchMousePos(QPoint pt, int &row, int &col)
{
    for(row=0; row<=CHESSBOARD_ROWS; ++row)
    {
        for(col=0; col<=CHESSBOARD_COLUMES; ++col)
        {
            QPoint distance = switchBoardPos(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < CHESSPIECES_RIDIUS* CHESSPIECES_RIDIUS)
                return true;
        }
    }
    return false;
}

void ChessBorad::drawBoard(QPainter& painter, QPixmap pix)
{
    painter.drawPixmap(0,0,750,555,pix);
}

void ChessBorad::drawPieces(QPainter &painter, QPixmap pix)
{
    for(int i=0; i<CHESSPIECES_COUNT; i++)
    {
        if (isDead(i)) continue;
        QPoint pos = switchBoardPos(m_chessPieces[i].m_row, m_chessPieces[i].m_col);
        pos = pos-QPoint(CHESSPIECES_RIDIUS,CHESSPIECES_RIDIUS);

        QRect rcSrc(PIECESITEAM_WIDTH*m_chessPieces[i].m_type,PIECESITEAM_HEIGHT*m_chessPieces[i].m_color,PIECESITEAM_WIDTH,PIECESITEAM_HEIGHT);

        painter.drawPixmap(pos,pix,rcSrc);
    }
}

void ChessBorad::drawChessSelected(QPainter &painter, int id, QPixmap pix)
{
    if(id != -1)
    {
        QPoint pos = switchBoardPos(m_chessPieces[id].m_row, m_chessPieces[id].m_col);
        pos = pos-QPoint(28,28);
        painter.drawPixmap(pos,pix);
    }
}

void ChessBorad::setChessPieces(ChessPieces *pieces)
{
    for(int i=0; i<CHESSPIECES_COUNT; ++i)
    {
        m_chessPieces[i]= pieces[i];
    }
}

ChessPieces *ChessBorad::getChessPieces()
{
    return m_chessPieces;
}

int ChessBorad::getPiecesId(int row, int col)
{
    for(int i=0; i<CHESSPIECES_COUNT; ++i)
    {
        if((m_chessPieces[i].m_row == row) && (m_chessPieces[i].m_col == col) && !m_chessPieces[i].m_dead)
            return i;
    }
    return -1;
}

QString ChessBorad::getPiecesName(int id)
{
    QStringList piecesNameList;
    piecesNameList << "帅" << "仕" << "相" << "马" << "车" << "炮" << "兵" << "将" << "士" << "象" << "马" << "车" << "炮" <<"卒";

    return piecesNameList.at(m_chessPieces[id].m_type + m_chessPieces[id].m_color * 7);
}

bool ChessBorad::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;

    return m_chessPieces[id1].m_color == m_chessPieces[id2].m_color;
}

bool ChessBorad::isRed(int id)
{
    return (m_chessPieces[id].m_color == ChessPieces::COLOR_RED) ? true : false;
}

bool ChessBorad::isDead(int id)
{
    if(id == -1)return true;
    return m_chessPieces[id].m_dead;
}

bool ChessBorad::isPiecesMSX(int id)
{
    if (m_chessPieces[id].m_type == ChessPieces::MA ||
            m_chessPieces[id].m_type == ChessPieces::SHI ||
            m_chessPieces[id].m_type == ChessPieces::XIANG)
    {
        return true;
    }
    return false;
}

bool ChessBorad::isWin(int id)
{
    if(id == -1)return false;
    if(ChessPieces::JIANG == m_chessPieces[id].m_type)
    {
        return true;
    }
    return false;
}

void ChessBorad::killPieces(int id)
{
    if(id==-1) return;
    m_chessPieces[id].m_dead = true;
}

void ChessBorad::relivePieces(int id)
{
    if(id==-1) return;
    m_chessPieces[id].m_dead = false;
}

void ChessBorad::movePieces(int moveid, int row, int col)
{
    m_chessPieces[moveid].m_row = row;
    m_chessPieces[moveid].m_col = col;
}

bool ChessBorad::canMove(int moveid, int killid, int row, int col)
{
    if(sameColor(moveid, killid)) return false;

    switch (m_chessPieces[moveid].m_type)
    {
    case ChessPieces::CHE:
        return canMoveChe(moveid, killid, row, col);

    case ChessPieces::MA:
        return canMoveMa(moveid, killid, row, col);

    case ChessPieces::PAO:
        return canMovePao(moveid, killid, row, col);

    case ChessPieces::BING:
        return canMoveBing(moveid, killid, row, col);

    case ChessPieces::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case ChessPieces::SHI:
        return canMoveShi(moveid, killid, row, col);

    case ChessPieces::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    return false;
}

void ChessBorad::saveStep(int moveid, int killid, int row, int col, QVector<ChessStep *> &steps)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    ChessStep* step = new ChessStep;
    step->m_colFrom = moveCol;
    step->m_colTo = col;
    step->m_rowFrom = moveRow;
    step->m_rowTo = row;
    step->m_moveid = moveid;
    step->m_killid = killid;

    steps.append(step);
}

void ChessBorad::regretChess(ChessStep* step)
{
    relivePieces(step->m_killid);
    movePieces(step->m_moveid, step->m_rowFrom, step->m_colFrom);
}

bool ChessBorad::canMoveChe(int moveid, int, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int ret = getPiecesCountAtLine(moveRow, moveCol, row, col);
    if(ret == 0)
        return true;
    return false;
}

bool ChessBorad::canMoveMa(int moveid, int, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int r = relation(moveRow, moveCol, row, col);
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        if(getPiecesId(moveRow, (col+moveCol)/2) != -1)
            return false;
    }
    else
    {
        if(getPiecesId((row+moveRow)/2, moveCol) != -1)
            return false;
    }

    return true;
}

bool ChessBorad::canMovePao(int moveid, int killid, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int ret = getPiecesCountAtLine(row, col, moveRow, moveCol);
    if(killid != -1)
    {
        if(ret == 1) return true;
    }
    else
    {
        if(ret == 0) return true;
    }
    return false;
}

bool ChessBorad::canMoveBing(int moveid, int, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int r = relation(moveRow, moveCol, row, col);
    if(r != 1 && r != 10) return false;

    if(isRed(moveid))
    {
        if(row > moveRow) return false;
        if(moveRow >= 5 && row == moveRow) return false;
    }
    else
    {
        if(row < moveRow) return false;
        if(moveRow <= 4 && row == moveRow) return false;
    }

    return true;
}

void ChessBorad::getRowCol(int &row, int &col, int id)
{
    row = m_chessPieces[id].m_row;
    col = m_chessPieces[id].m_col;
}

bool ChessBorad::canMoveJiang(int moveid, int killid, int row, int col)
{
    if(killid != -1 && m_chessPieces[killid].m_type == ChessPieces::JIANG)
        return canMoveChe(moveid, killid, row, col);

    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int r = relation(moveRow, moveCol, row, col);
    if(r != 1 && r != 10) return false;

    if(col < 3 || col > 5) return false;
    if(isRed(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool ChessBorad::canMoveShi(int moveid, int, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int r = relation(moveRow, moveCol, row, col);
    if(r != 11) return false;

    if(col < 3 || col > 5) return false;
    if(isRed(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool ChessBorad::canMoveXiang(int moveid, int, int row, int col)
{
    int moveRow,moveCol;
    getRowCol(moveRow, moveCol, moveid);
    int r = relation(moveRow, moveCol, row, col);
    if(r != 22) return false;

    int rEye = (row+moveRow)/2;
    int cEye = (col+moveCol)/2;
    if(getPiecesId(rEye, cEye) != -1)
        return false;

    if(isRed(moveid))
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}

int ChessBorad::getPiecesCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getPiecesId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getPiecesId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

int ChessBorad::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}

QString ChessBorad::getStepName(ChessStep *step,int stepCount)
{
    QStringList redNumList,blackNumList,piecesJTPList,piecesQHList;

    redNumList << "零" << "一" << "二" << "三" << "四" << "五" << "六" << "七" << "八" << "九";
    blackNumList << "０" << "１" << "２" << "３" << "４" << "５" << "６" << "７" << "８" << "９";
    piecesJTPList << "进" << "退" << "平";
    piecesQHList << "前" << "后";

    QString piecesName = getPiecesName(step->m_moveid);

    int format,result,samePiecesCount1 = 0,samePiecesCount2 = 0;
    QString strName;
    if(isRed(step->m_moveid))
    {
        samePiecesCount1 = getSamePiecesCountAtCol1(step->m_moveid,step->m_rowFrom,step->m_colFrom);
        samePiecesCount2 = getSamePiecesCountAtCol2(step->m_moveid,step->m_rowFrom,step->m_colFrom);

        if((samePiecesCount1+samePiecesCount2) > 1)
        {
            format=2;
            result=samePiecesCount1+1;
        }
        else if	(samePiecesCount1+samePiecesCount2 == 1)
        {
            format=1;
            result=samePiecesCount1==0?0:1;
        }
        else
        {
            format=0;
            result=9-step->m_colFrom;
        }

        int piecesJTP = 0;
        if	(step->m_rowFrom > step->m_rowTo)
        {
            piecesJTP=0;
        }
        else if	(step->m_rowFrom < step->m_rowTo)
        {
            piecesJTP=1;
        }
        else
        {
            piecesJTP=2;
        }

        int moveDis = 0;
        if(isPiecesMSX(step->m_moveid))
        {
            moveDis=9-step->m_colTo;
        }
        else
        {
            switch(piecesJTP)
            {
            case 0:
                moveDis = step->m_rowFrom-step->m_rowTo;
                break;
            case 1:
                moveDis = step->m_rowTo-step->m_rowFrom;
                break;
            case 2:
                moveDis = 9-step->m_colTo;
                break;
            default:
                break;
            }
        }

        switch(format)
        {
        case 0:
            strName = QString("%1%2%3%4").arg(piecesName)
                                         .arg(redNumList.at(result))
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(redNumList.at(moveDis));
            break;
        case 1:
            strName = QString("%1%2%3%4").arg(piecesQHList.at(result))
                                         .arg(piecesName)
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(redNumList.at(moveDis));
            break;
        case 2:
            strName = QString("%1%2%3%4").arg(redNumList.at(result))
                                         .arg(piecesName)
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(redNumList.at(moveDis));
            break;
        default:break;
        }
    }
    else
    {
        samePiecesCount1 = getSamePiecesCountAtCol2(step->m_moveid,step->m_rowFrom,step->m_colFrom);
        samePiecesCount2 = getSamePiecesCountAtCol1(step->m_moveid,step->m_rowFrom,step->m_colFrom);

        if((samePiecesCount1+samePiecesCount2) > 1)
        {
            format=2;
            result=samePiecesCount1+1;
        }
        else if	(samePiecesCount1+samePiecesCount2 == 1)
        {
            format=1;
            result=samePiecesCount1==0?0:1;
        }
        else
        {
            format=0;
            result=step->m_colFrom+1;
        }

        int piecesJTP = 0;
        if	(step->m_rowTo > step->m_rowFrom)
        {
            piecesJTP=0;
        }
        else if	(step->m_rowTo < step->m_rowFrom)
        {
            piecesJTP=1;
        }
        else
        {
            piecesJTP=2;
        }

        int moveDis = 0;
        if(isPiecesMSX(step->m_moveid))
        {
            moveDis=step->m_colTo+1;
        }
        else
        {
            switch(piecesJTP)
            {
            case 0:
                moveDis = step->m_rowTo-step->m_rowFrom;
                break;
            case 1:
                moveDis = step->m_rowFrom-step->m_rowTo;
                break;
            case 2:
                moveDis = step->m_colTo+1;
                break;
            default:
                break;
            }
        }

        switch(format)
        {
        case 0:
            strName = QString("%1%2%3%4").arg(piecesName)
                                         .arg(blackNumList.at(result))
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(blackNumList.at(moveDis));
            break;
        case 1:
            strName = QString("%1%2%3%4").arg(piecesQHList.at(result))
                                         .arg(piecesName)
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(blackNumList.at(moveDis));
            break;
        case 2:
            strName = QString("%1%2%3%4").arg(blackNumList.at(result))
                                         .arg(piecesName)
                                         .arg(piecesJTPList.at(piecesJTP))
                                         .arg(blackNumList.at(moveDis));
            break;
        default:break;
        }
    }

    return strName = QString::number(stepCount)+"."+strName;
}

int ChessBorad::getSamePiecesCountAtCol1(int id, int row, int col)
{
    int piecesId,ret = 0;

    for(int i = row - 1; i>=0; --i)
    {
        piecesId = getPiecesId(i, col);
        if (piecesId == -1) continue;
        if((m_chessPieces[piecesId].m_type == m_chessPieces[id].m_type &&
               m_chessPieces[piecesId].m_color == m_chessPieces[id].m_color) ) ++ret;
    }

    return ret;
}

int ChessBorad::getSamePiecesCountAtCol2(int id, int row, int col)
{
    int piecesId, ret = 0;

    for(int i = row + 1; i<=CHESSBOARD_ROWS; ++i)
    {
        piecesId = getPiecesId(i, col);
        if (piecesId == -1) continue;
        if((m_chessPieces[piecesId].m_type == m_chessPieces[id].m_type &&
               m_chessPieces[piecesId].m_color == m_chessPieces[id].m_color) ) ++ret;
    }

    return ret;
}
