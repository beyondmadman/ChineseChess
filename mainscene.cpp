#include "mainscene.h"
#include "buttonctl.h"
#include "gamescene.h"

#include <QPainter>
#include <QIcon>
#include <QTimer>

MainScene::MainScene(QWidget *parent) : QWidget(parent)
{
    //设置标题
    this->setWindowTitle("中国象棋");

    //设置大小
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置应用图标
    setWindowIcon(QIcon(":/res/logo.png"));


    //创建按钮
    ButtonCtl* singleplayerBtn = new ButtonCtl(":/res/singleplayer.png");
    singleplayerBtn->setParent(this);
    singleplayerBtn->move(this->width()*0.5-singleplayerBtn->width()*0.5,this->height()*0.65);

    //游戏场景
    GameScene* gameScene = new GameScene;

    //开始音效
    QSound * startSound = new QSound(":/res/start.wav",this);


    //按钮响应
    connect(singleplayerBtn,&ButtonCtl::clicked,[=]()
    {
        //播放音效
        startSound->play();

        //按钮特效
        singleplayerBtn->zoom(ButtonActon::ButtonDown);
        singleplayerBtn->zoom(ButtonActon::ButtonUp);

        //延时进入选择场景
        QTimer::singleShot(500,[=]()
        {
            //隐藏主场景
            this->hide();

            //设置位置
            gameScene->setGeometry(this->geometry());

            //显示游戏场景
            gameScene->show();
        });
    });

}

void MainScene::paintEvent(QPaintEvent *)
{
    //创建画家，指定绘图设备
    QPainter painter(this);

    //创建QPixmap对象
    QPixmap pix;

    //加载背景图片
    pix.load(":/res/mainback.png");

    //绘制背景
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}

