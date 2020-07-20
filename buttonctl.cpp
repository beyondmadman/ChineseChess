#include "buttonctl.h"
#include <QDebug>
#include <QPropertyAnimation>

ButtonCtl::ButtonCtl(QString normalImg,QString pressImg)
    :normalImgPath(normalImg),
      pressImgPath(pressImg)
{
    //创建QPixmap对象
    QPixmap pix;

    //加载图片
    bool ret = pix.load(normalImg);
    if (!ret)
    {
        qDebug() << "加载图片失败";
    }

    //设置按钮大小
    this->setFixedSize(pix.width(),pix.height());

    //设置按钮样式
    setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    setIconSize(QSize(pix.width(),pix.height()));
}
void ButtonCtl::zoom(ButtonActon ac)
{
    //创建动画
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");

    //设置间隔
    animation->setDuration(200);

    //设置起始位置
    int top = (ac == ButtonUp) ? this->y() + 10 : this->y();
    animation->setStartValue(QRect(this->x(),top,this->width(),this->height()));

    //设置结束位置
    top = (ac == ButtonUp) ? this->y() : this->y() + 10;
    animation->setEndValue(QRect(this->x(),top,this->width(),this->height()));

    //设置效果
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //启动动画
    animation->start();
}

void ButtonCtl::mousePressEvent(QMouseEvent *e)
{
    if (pressImgPath != "")
    {
        //创建QPixmap对象
        QPixmap pix;

        //加载图片
        bool ret = pix.load(pressImgPath);
        if (!ret)
        {
            qDebug() << "加载图片失败";
        }

        //设置按钮大小
        this->setFixedSize(pix.width(),pix.height());

        //设置按钮样式
        setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        setIconSize(QSize(pix.width(),pix.height()));
    }

    QPushButton::mousePressEvent(e);
}

void ButtonCtl::mouseReleaseEvent(QMouseEvent *e)
{
    if (normalImgPath != "")
    {
        //创建QPixmap对象
        QPixmap pix;

        //加载图片
        bool ret = pix.load(normalImgPath);
        if (!ret)
        {
            qDebug() << "加载图片失败";
        }

        //设置按钮大小
        this->setFixedSize(pix.width(),pix.height());

        //设置按钮样式
        setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        setIconSize(QSize(pix.width(),pix.height()));
    }

    QPushButton::mouseReleaseEvent(e);
}

