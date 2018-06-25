#include "conexao.h"

Conexao::Conexao(qintptr descript, QObject *parent) : QObject(parent)
{
    setDescriptor(descript);
    startConexao();
}

void Conexao::startConexao()
{
    setSocket(new QTcpSocket(this));

    connect(socket(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket(), SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket()->setSocketDescriptor(descriptor());

    qDebug() << descriptor() << ": cliente conectado!";

}

void Conexao::disconnected()
{
    qDebug() << descriptor() << ": cliente desconectado!";
}

void Conexao::readyRead()
{
    //qDebug() << "readyRead";
    qDebug() << descriptor() << ": " << socket()->readAll();

}

QTcpSocket *Conexao::socket() const
{
    return mSocket;
}

void Conexao::setSocket(QTcpSocket *socket)
{
    mSocket = socket;
}

qintptr Conexao::descriptor() const
{
    return mDescriptor;
}

void Conexao::setDescriptor(const qintptr &descriptor)
{
    mDescriptor = descriptor;
}

