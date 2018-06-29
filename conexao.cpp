#include "conexao.h"

Conexao::Conexao(qintptr descript, QObject *parent) : QObject(parent)
{
    setDescriptor(descript);
    startConexao();
}

Conexao::~Conexao()
{
    socket()->close();
}


void Conexao::startConexao()
{
    setSocket(new QTcpSocket(this));

    socket()->flush();//limpando o buffer que possa ter

    connect(socket(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket(), SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket()->setSocketDescriptor(descriptor());

    qDebug() << descriptor() << ": cliente conectado!";

}

void Conexao::disconnected()
{
    qDebug() << descriptor() << ": cliente desconectado!";
    emit disconnected(descriptor());
}

void Conexao::readyRead()
{

    emit readyRead(socket()->readAll());

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

bool Conexao::enviarMensagem(const QString &msg)
{

    QByteArray byteArrayTemp = msg.toLocal8Bit();
    byteArrayTemp.append("\r\n\r\n\r\n");           //para garantir o envio!
    socket()->write(byteArrayTemp);
    if(!socket()->waitForBytesWritten())
        return false;

    qDebug() << descriptor() << ":enviado servidor:" << byteArrayTemp;

    socket()->flush();
    QTest::qSleep(50);  //Para garantir envio individual de mensagem

    return true;
}


