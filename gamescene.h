#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QListWidget>
#include <QSound>
#include <QThread>

#include "chessboard.h"
#include "ButtonCtl.h"
#include "chessthink.h"

class GameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = 0);
    ~GameScene();

private:
    void initGame();
    void initChessCtl();

    void clickBoard(QPoint pt);
    void trySelectPieces(int id);
    void tryMovePieces(int killid, int row, int col);
    void movePieces(int moveid, int killid, int row, int col);

    void dealStep();
    void switchTrun();
    void regretChess();

    void gameOver();

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    ChessBorad                  m_chessBorad;
    QPixmap                     m_chessBoard;
    QPixmap                     m_piecesImage;
    QPixmap                     m_selectedImage;

    bool                        m_gameOver;
    int                         m_selectId;
    bool                        m_redTurn;
    bool                        m_sound;

    QVector<ChessStep*>         m_steps;
    QListWidget*                m_listStep;

    QPixmap                     m_playerImage[2];
    QLabel*                     m_player;

    QTimer*                     m_timer;
    QLabel*                     m_sumTimeLabel;
    QLabel*                     m_stepTimeLabel;
    int                         m_stepTime;
    int                         m_sumTime;

    QLabel*                     m_winLabel;

    QSound *                    m_background;

    ChessThink*                 m_chessThink;
    QThread*                    m_thread;

    ButtonCtl*                  m_new;
    ButtonCtl*                  m_regret;
    ButtonCtl*                  m_difficulty;
    ButtonCtl*                  m_soundBtn;

signals:
    void sigChessThink(ChessPieces* pieces);

public slots:
    void slotStartGame();
    void slotRegretChess();
    void slotSound();
    void slotTimeOut();
    void slotAIMove(ChessStep* step);
};

#endif // PLAYSCENE_H
