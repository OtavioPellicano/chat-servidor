#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QRunnable>

class Log : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

private:
    void run();

signals:

public slots:
};

#endif // LOG_H
