#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QDebug>
#include "task.h"
#include <map>

class Conexao : public QObject
{
    Q_OBJECT
public:
    explicit Conexao(qintptr descript, QObject *parent = nullptr);

    qintptr descriptor() const;
    void setDescriptor(const qintptr &descriptor);

    bool enviarMensagem(const QString &msg);

private:

    void startConexao();

    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);



signals:
    void readyRead(const QByteArray &msg);
    void disconnected(const qintptr &descript);

public slots:   //signals do TcpScoket:
    void disconnected();
    void readyRead();

private:
    qintptr mDescriptor;
    QTcpSocket *mSocket;

};

#endif // CONEXAO_H
