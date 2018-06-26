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

    if(!validarEstruturaMensagem(msg))
    {
        qDebug() << "mensagem fora da estrutura";
        conexao()->enviarMensagem("Mensagem fora do padrao");
        return;
    }

    validarNickname(msg);

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

void Servidor::validarNickname(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    str = string(str.begin(),std::remove(str.begin(), str.end(), '\r'));
    str = string(str.begin(),std::remove(str.begin(), str.end(), '\n'));

    if(!str.empty())
    {
        QString keyNickname = QString::fromStdString(string(std::find(str.begin(), str.end(), '#') + 1, std::find(str.begin()+1,str.end(), '#')));

        if(keyNickname == KEY_NICKNAME)
        {
            QString nickname = QString::fromStdString(string(std::find(str.begin(),str.end(), ':') + 1, str.end()));

            if(gerenConexao()->addNickname(nickname, conexao()->descriptor()))
            {
                qDebug() << "nickname: \"" % nickname %"\" adicionado";
                if(!conexao()->enviarMensagem(nickname))
                    qDebug() << "impossivel enviar mensagem!";
            }
            else
            {
                QString qstr = "o nickname ja \"" % nickname % "\" existe!";
                qDebug() << "nickname ja existe";
                if(!conexao()->enviarMensagem(qstr))
                    qDebug() << "impossivel enviar mensagem!";

                delete conexao();
            }
        }
    }

}

bool Servidor::validarEstruturaMensagem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    if(std::count(str.begin(), str.end(),'#') == 2 && std::count(str.begin(), str.end(), ':') == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
