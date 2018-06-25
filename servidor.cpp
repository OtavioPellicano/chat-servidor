#include "servidor.h"

Servidor::Servidor(QObject *parent) : QTcpServer(parent)
{

}

void Servidor::startServidor()
{
    if(this->listen(QHostAddress::Any, 1312))
    {
        qDebug() << "Servidor iniciado";
        qDebug() << "Conecte pela porta 1312";
    }
    else
    {
        qDebug() << "Erro ao abrir ao abrir a porta 1312";
    }
}

void Servidor::incomingConnection(qintptr descript)
{
    qDebug() << "incomingConnection";
    setConexao(new Conexao(descript, this));


    connect(conexao(), SIGNAL(destroyed(QObject*)), conexao(), SLOT(deleteLater()));

}

Conexao *Servidor::conexao() const
{
    return mConexao;
}

void Servidor::setConexao(Conexao *conexao)
{
    mConexao = conexao;
}
