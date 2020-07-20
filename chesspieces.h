#ifndef CHESSPIECES_H
#define CHESSPIECES_H

class ChessPieces
{
public:

    //棋子类型
    enum CHESS_TYPE{JIANG = 0, SHI, XIANG, MA, CHE, PAO, BING};
    //棋子颜色
    enum CHESS_COLOR{COLOR_RED = 0,COLOR_BLACK};

    ChessPieces();
    ~ChessPieces();

public:
    //初始化棋子
    void init(int id, bool dead);

    int                 m_row;                            //棋子的行
    int                 m_col;                            //棋子的列
    CHESS_TYPE          m_type;                           //棋子类型
    CHESS_COLOR         m_color;                          //棋子颜色
    bool                m_dead;                           //棋子状态

};

#endif // CHESSPIECES_H
