#include "conexao.h"

Conexao::Conexao(qintptr descript, QObject *parent) : QObject(parent)
{
    setDescriptor(descript);
    startConexao();
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

//    setNickname(socket()->readAll());

//    if(!addNickname(nickname(), descriptor()))
//    {
//        socket()->write("nickname utilizado por outro usuario\r\voce sera desconectado do servidor\r\n");
//        socket()->flush();
//        socket()->waitForBytesWritten();
//        socket()->close();
//        return;
//    }

//    qDebug() << descriptor() << ": " << socket()->readAll();

    emit readyRead(socket()->readAll());

    //Preciso criar uma task (QRunnable)
//    Task *task = new Task();
//    task->setAutoDelete(true);

//    QThreadPool::globalInstance()->start(task);


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
    socket()->write(byteArrayTemp);
    if(!socket()->waitForBytesWritten())
        return false;
    return true;
}


