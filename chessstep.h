#ifndef STEP_H
#define STEP_H

#include <QObject>

class ChessStep : public QObject
{
    Q_OBJECT
public:
    explicit ChessStep(QObject *parent = 0);
    ~ChessStep();

    int m_moveid;
    int m_killid;
    int m_rowFrom;
    int m_colFrom;
    int m_rowTo;
    int m_colTo;
};

#endif // STEP_H
