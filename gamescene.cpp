#include "gamescene.h"
#include "chessboard.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QStringList>
#include <QDebug>

GameScene::GameScene(QWidget *parent) : QMainWindow(parent)
{
    //设置标题
    this->setWindowTitle("中国象棋");

    //设置大小
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置应用图标
    setWindowIcon(QIcon(":/res/logo.png"));

    m_chessBoard = QPixmap(":/res/chessborad.png");
    m_piecesImage = QPixmap(":/res/CHESS_ITEM.png");
    m_selectedImage =  QPixmap(":res/selected1.png");

    initGame();
    initChessCtl();

    m_gameOver = true;

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));

    m_chessThink = new ChessThink;
    m_thread = new QThread(this);
    m_chessThink->moveToThread(m_thread);
    m_thread->start();
    connect(this,&GameScene::sigChessThink,m_chessThink,&ChessThink::movePieces);
    connect(m_chessThink,&ChessThink::updataMovePieces,this,&GameScene::slotAIMove);

    //背景音效
    m_background = new QSound(":/res/bgmusic.wav",this);
    m_background->setLoops(-1);
    m_background->play();

}

GameScene::~GameScene()
{
    m_thread->quit();
    m_thread->wait();
    delete m_chessThink;
}

void GameScene::initGame()
{
    m_stepTime = 0;
    m_sumTime = 0;
    m_selectId = -1;
    m_redTurn = true;
}

void GameScene::initChessCtl()
{
    m_listStep = new QListWidget(this);
    m_listStep->setFixedSize(100,150);
    m_listStep->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_listStep->width()*0.5,this->height()*0.05);
    m_listStep->setStyleSheet("QListWidget{background:rgb(202,150,100);border:3px solid rgb(83,22,29); color:black; }"
    "QListWidget::Item{padding-top:-1px; padding-bottom:-1px;padding-left:-1px;padding-right:-1px;}"
    "QListWidget::Item:hover{background:transparent;}"
    "QListWidget::item:selected{background:transparent;color:red; }"
    "QScrollBar::vertical {margin:0px 0px 0px 0px;background-color:rgb(186,134,75);border:none;width:12px;}"
    "QScrollBar::handle:vertical {background-color:rgb(102,32,20);width:12px;}");

    QFont font;
    font.setFamily("宋体");
    font.setBold(true);
    font.setPointSize(12);

    m_sumTimeLabel=new QLabel("局时: 00:00:00",this);
    m_sumTimeLabel->setFont(font);
    m_sumTimeLabel->setFixedSize(125,35);
    m_sumTimeLabel->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_sumTimeLabel->width()*0.5+10,this->height()*0.35);

    m_player = new QLabel(this);
    QPixmap pixPlayer;
    pixPlayer.load(":/res/redchess.png");
    m_playerImage[0] = pixPlayer.scaled(pixPlayer.width()*0.7,pixPlayer.height()*0.7);
    pixPlayer.load(":/res/blackchess.png");
    m_playerImage[1] = pixPlayer.scaled(pixPlayer.width()*0.7,pixPlayer.height()*0.7);
    m_player->setFixedSize(35,35);
    m_player->setPixmap(m_playerImage[0]);
    m_player->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_player->width()-m_sumTimeLabel->width()*0.5,this->height()*0.37);

    m_stepTimeLabel = new QLabel("步时: 00:00:00",this);
    m_stepTimeLabel->setFont(font);
    m_stepTimeLabel->setFixedSize(125,35);
    m_stepTimeLabel->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_stepTimeLabel->width()*0.5+10,this->height()*0.4);

    m_new = new ButtonCtl(":/res/new.jpg");
    m_new->setParent(this);
    m_new->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_new->width()*0.5,this->height()*0.5);

    m_regret = new ButtonCtl(":/res/regret.jpg");
    m_regret->setParent(this);
    m_regret->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_regret->width()*0.5,this->height()*0.6);

    m_difficulty = new ButtonCtl(":/res/difficulty.jpg");
    m_difficulty->setParent(this);
    m_difficulty->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_difficulty->width()*0.5,this->height()*0.7);

    m_soundBtn = new ButtonCtl(":/res/opensound.png");
    m_soundBtn->setParent(this);
    m_soundBtn->move(CHESSBOARD_WIDTH+(GAME_WIDTH-CHESSBOARD_WIDTH)*0.5-m_soundBtn->width()*0.5,this->height()*0.8);
    m_sound = true;

    m_winLabel = new QLabel(this);
    QPixmap pixWin;
    pixWin.load(":/res/win.png");
    m_winLabel->setGeometry(0,0,pixWin.width(),pixWin.height());
    m_winLabel->setPixmap(pixWin);
    m_winLabel->move((CHESSBOARD_WIDTH - pixWin.width())*0.5 , -pixWin.height());

    connect(m_new,SIGNAL(clicked()),this,SLOT(slotStartGame()));
    connect(m_regret,SIGNAL(clicked()),this,SLOT(slotRegretChess()));
    connect(m_soundBtn,SIGNAL(clicked()),this,SLOT(slotSound()));
}

void GameScene::paintEvent(QPaintEvent *)
{
    //创建画家
    QPainter painter(this);

    //绘制棋盘
    m_chessBorad.drawBoard(painter, m_chessBoard);

    //绘制棋子
    m_chessBorad.drawPieces(painter, m_piecesImage);

    //选择标识
    m_chessBorad.drawChessSelected(painter, m_selectId, m_selectedImage);
}

void GameScene::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !m_gameOver)
    {
        clickBoard(event->pos());

        update();
    }
}

void GameScene::slotStartGame()
{
    QSound * startSound = new QSound(":/res/new.wav",this);
    startSound->play();

    initGame();
    m_chessBorad.initChessBorad(false);

    m_gameOver = false;

    m_steps.clear();
    m_listStep->clear();
    m_winLabel->move((CHESSBOARD_WIDTH - m_winLabel->width())*0.5 , -m_winLabel->height());

    m_timer->start(1000);

    update();
}

void GameScene::slotRegretChess()
{
    QSound * regretSound = new QSound(":/res/regret.wav",this);
    regretSound->play();

    if(!m_redTurn || m_gameOver) return;
    regretChess();
    regretChess();
    update();
}

void GameScene::slotSound()
{
    m_sound = !m_sound;
    if (m_sound)
    {
        m_soundBtn->setIcon(QIcon(":/res/opensound.png"));
        m_background->play();
    }
    else
    {
        m_soundBtn->setIcon(QIcon(":/res/closesound.png"));
        m_background->stop();
    }
}

void GameScene::slotTimeOut()
{
    m_stepTime++;
    m_sumTime++;
    QString stepTime="步时："+QString("%1").arg(m_stepTime/3600%60, 2, 10,QChar('0'))
            +":"+QString("%2").arg(m_stepTime/60%60, 2, 10, QChar('0'))
            +":"+QString("%3").arg(m_stepTime%60, 2, 10, QChar('0'));
    m_stepTimeLabel->setText(stepTime);

    QString sumTime="局时："+QString("%1").arg(m_sumTime/3600%60, 2, 10,QChar('0'))
            +":"+QString("%2").arg(m_sumTime/60%60, 2, 10, QChar('0'))
            +":"+QString("%3").arg(m_sumTime%60, 2, 10, QChar('0'));
    m_sumTimeLabel->setText(sumTime);
}

void GameScene::slotAIMove(ChessStep* step)
{
    movePieces(step->m_moveid, step->m_killid, step->m_rowTo, step->m_colTo);
    update();
    delete step;
}

void GameScene::clickBoard(QPoint pt)
{
    int row, col;
    bool bClicked = m_chessBorad.switchMousePos(pt, row, col);
    if(!bClicked) return;

    int id = m_chessBorad.getPiecesId(row, col);
    if(m_selectId == -1)
    {
        trySelectPieces(id);
    }
    else
    {
        tryMovePieces(id, row, col);
    }
}

void GameScene::trySelectPieces(int id)
{
    bool red = m_chessBorad.isRed(id);
    if((id == -1) || (m_redTurn != red))
    {
        return;
    }

    m_selectId = id;

    QSound * selectSound = new QSound(":/res/check.wav",this);
    selectSound->play();
}

void GameScene::movePieces(int moveid, int killid, int row, int col)
{
    m_chessBorad.saveStep(moveid, killid, row, col, m_steps);

    dealStep();

    m_chessBorad.killPieces(killid);
    m_chessBorad.movePieces(moveid, row, col);

    if (m_chessBorad.isWin(killid))
    {
        gameOver();
        return;
    }

    QSound * moveSound = new QSound(":/res/down.wav",this);
    moveSound->play();

    switchTrun();

}

void GameScene::dealStep()
{
    int index = m_steps.count();
    QString str=m_chessBorad.getStepName(m_steps.last(),index);

    m_listStep->insertItem(index,str);
    m_listStep->setCurrentRow(index-1);
}

void GameScene::switchTrun()
{
    m_redTurn = !m_redTurn;
    m_stepTime = 0;
    int i = m_redTurn == true ? 0:1;
    m_player->setPixmap(m_playerImage[i]);
    m_selectId = -1;

    if (!m_redTurn)
    {
       emit sigChessThink(m_chessBorad.getChessPieces());
    }
}


void GameScene::regretChess()
{
    int count = m_steps.count();
    if(count == 0) return;

    ChessStep* step = m_steps.last();
    m_steps.removeLast();
    m_chessBorad.regretChess(step);
    delete step;

    m_listStep->takeItem(count-1);
}

void GameScene::gameOver()
{
     m_gameOver = true;
     m_selectId = -1;
     m_timer->stop();

     //胜利动画
     if(m_redTurn)
     {
        QPropertyAnimation* winAnimation = new QPropertyAnimation(m_winLabel,"geometry");
        winAnimation->setDuration(1000);
        winAnimation->setStartValue(QRect(m_winLabel->x(),m_winLabel->y(),m_winLabel->width(),m_winLabel->height()));
        winAnimation->setEndValue(QRect(m_winLabel->x(),m_winLabel->y()+(CHESSBOARD_HEIGHT+m_winLabel->height())*0.5,m_winLabel->width(),m_winLabel->height()));
        winAnimation->setEasingCurve(QEasingCurve::OutBounce);
        winAnimation->start();

        QSound * winSound = new QSound(":/res/win.wav",this);
        winSound->play();
     }
     else
     {
         QSound * lostSound = new QSound(":/res/lost.wav",this);
         lostSound->play();
     }
}

void GameScene::tryMovePieces(int killid, int row, int col)
{
    if(killid != -1 && m_chessBorad.sameColor(killid, m_selectId))
    {
        trySelectPieces(killid);
        return;
    }

    bool ret = m_chessBorad.canMove(m_selectId, killid, row, col);
    if(ret)
    {
        movePieces(m_selectId, killid, row, col);
    }
}


