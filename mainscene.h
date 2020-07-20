#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>

class MainScene : public QWidget
{
    Q_OBJECT
public:
    explicit MainScene(QWidget *parent = 0);

protected:
    //绘图事件
    void paintEvent(QPaintEvent *);


signals:

public slots:
};

#endif // MAINSCENE_H
