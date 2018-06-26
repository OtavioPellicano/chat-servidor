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
    qDebug() << "incomingConnection";
    setConexao(new Conexao(descript, this));
    connect(conexao(), SIGNAL(destroyed(QObject*)), conexao(), SLOT(deleteLater()));
    connect(conexao(), SIGNAL(readyRead(QByteArray)), this, SLOT(readyRead(QByteArray)));
    connect(conexao(), SIGNAL(disconnected(qintptr)), this, SLOT(disconnected(qintptr)));


}

void Servidor::readyRead(const QByteArray &msg)
{
    qDebug() << msg;

    string str = QString(msg).toStdString();
    //auto itStr = str.begin();

    QString keyNickname = QString::fromStdString(string(std::find(str.begin(), str.end(), '#') + 1, std::find(str.begin()+1,str.end(), '#')));

    if(keyNickname == KEY_NICKNAME)
    {
        QString nickname = QString::fromStdString(string(std::find(str.begin(),str.end(), ':') + 1, str.end()));
        qDebug() << "conexao nickname";
        if(gerenConexao()->addNickname(nickname, conexao()->descriptor()))
        {
            qDebug() << "nickname adicionado";
        }
        else
        {
            qDebug() << "nickname ja existe";
            if(!conexao()->enviarMensagem(nickname))
                qDebug() << "impossivel enviar mensagem!";
        }
    }

}

void Servidor::disconnected(const qintptr &descrpt)
{
    qDebug() << "nickname removido";
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
