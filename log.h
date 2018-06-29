#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QRunnable>
#include <QDir>
#include <fstream>
#include <QString>
#include <QStringBuilder>
#include <QDateTime>
#include <QDebug>

using std::ifstream;
using std::ofstream;
using std::endl;
using std::ios_base;

class Log : public QObject, public QRunnable
{
    Q_OBJECT
private:

public:
    explicit Log(QObject *parent = nullptr);

private:
    void run();

    void setup();

    QString nomeArqOut() const;
    QDir dirLog() const;

    QString gerarQStrLog(const QString &org, const QString &dst, const QString &msg);
    void salvarArq(const QString &qstrLog);

signals:

public slots:
    void salvarLog(const QString &org, const QString &dst, const QString &msg);

private:

    QDir mDirLog;
    QString mNomeArqOut;

    const QString BROADCAST_CONECTADO = "$c$";      //broadcast: #$$$$#$c$#:user1;user2;user3
    const QString BROADCAST_DESCONECTADO = "$d$";   //broadcast: #$$$$#$d$#:user1;user2;user3

};

#endif // LOG_H
