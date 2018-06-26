#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QRunnable>
#include <QDebug>

class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Task();

private:
    void run();

public slots:


};

#endif // TASK_H
