#include "servidor.h"

Servidor::Servidor(QObject *parent) : QTcpServer(parent)
{
    setGerenConexao(new GerenciaConexao());
}

Servidor::~Servidor()
{
    delete gerenConexao();
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
    setConexao(new Conexao(descript, this));
    connect(conexao(), SIGNAL(destroyed(QObject*)), conexao(), SLOT(deleteLater()));
    connect(conexao(), SIGNAL(readyRead(QByteArray)), this, SLOT(readyRead(QByteArray)));
    connect(conexao(), SIGNAL(disconnected(qintptr)), this, SLOT(disconnected(qintptr)));

}

void Servidor::readyRead(const QByteArray &msg)
{

    if(!gerenConexao()->validarEstruturaMensagem(msg))
    {
        qDebug() << "mensagem fora da estrutura";
        conexao()->enviarMensagem("Mensagem fora da estrutura");
        return;
    }

    gerenConexao()->setOrigem(msg);
    gerenConexao()->setDestino(msg);
    gerenConexao()->setMensagem(msg);

    if(gerenConexao()->destino().isEmpty()) //representa o nickname
    {
        gerenConexao()->addNickname(gerenConexao()->origem(), conexao());
        return;
    }

    if(!gerenConexao()->origem().isEmpty() && !gerenConexao()->destino().isEmpty() && !gerenConexao()->mensagem().isEmpty())
    {
        //redirecionar a mensagem
        gerenConexao()->redirecionarMensagem(gerenConexao()->origem(), gerenConexao()->destino(), gerenConexao()->mensagem());
        return;
    }

}

void Servidor::disconnected(const qintptr &descrpt)
{
    gerenConexao()->rmNickname(descrpt);
}

GerenciaConexao *Servidor::gerenConexao() const
{
    return mGerenConexao;
}

void Servidor::setGerenConexao(GerenciaConexao *gerenConexao)
{
    mGerenConexao = gerenConexao;
}

Conexao *Servidor::conexao() const
{
    return mConexao;
}

void Servidor::setConexao(Conexao *conexao)
{
    mConexao = conexao;
}
