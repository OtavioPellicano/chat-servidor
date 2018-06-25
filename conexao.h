#ifndef CONEXAO_H
#define CONEXAO_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QDebug>
#include <QAbstractSocket>

class Conexao : public QObject
{
    Q_OBJECT
public:
    explicit Conexao(qintptr descript, QObject *parent = nullptr);

    void startConexao();

private:

    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);
    qintptr descriptor() const;
    void setDescriptor(const qintptr &descriptor);

signals:


public slots:   //signals do TcpScoket:
    void disconnected();
    void readyRead();

private:
    qintptr mDescriptor;
    QTcpSocket *mSocket;



};

#endif // CONEXAO_H
