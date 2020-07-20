#ifndef BUTTONCTL_H
#define BUTTONCTL_H

#include <QPushButton>

enum ButtonActon { ButtonUp, ButtonDown};

class ButtonCtl : public QPushButton
{
    Q_OBJECT
public:
    //构造函数
    ButtonCtl(QString normalImg,QString pressImg = "");

protected:
    //鼠标事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QString normalImgPath;   //默认图片路径
    QString pressImgPath;    //按下图片路径

public:
    void zoom(ButtonActon ac);

signals:

public slots:
};

#endif // BUTTONCTL_H
