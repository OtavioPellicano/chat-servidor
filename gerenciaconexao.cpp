#include "gerenciaconexao.h"

GerenciaConexao::GerenciaConexao(QObject *parent)
    :QObject(parent)
{

    mLog = new Log();
    mLog->setAutoDelete(true);

    //Fazendo uma conexao em fila (especificao)
    connect(this, SIGNAL(salvarLog(QString,QString,QString)), mLog, SLOT(salvarLog(QString,QString,QString)), Qt::QueuedConnection);

    QThreadPool::globalInstance()->start(mLog);

}

GerenciaConexao::~GerenciaConexao()
{
    mMapNickConexao.clear();
}


bool GerenciaConexao::addNickname(const QString &nick, Conexao *cliente)
{
    if(mMapNickConexao.find(nick) != mMapNickConexao.end())
    {
        validarNickname(nick, cliente, false);
        delete cliente;
        return false;
    }

    mMapNickConexao[nick] = cliente;
    validarNickname(nick, cliente, true);
    emit salvarLog(nickname(cliente->descriptor()), BROADCAST_CONECTADO, "");
    broadcast(CONECTADO, nickname(cliente->descriptor()));

    return true;
}

void GerenciaConexao::rmNickname(const QString &nickname)
{
    emit salvarLog(nickname, BROADCAST_DESCONECTADO, "");

    broadcast(DESCONECTADO, nickname);
    mMapNickConexao.erase(mMapNickConexao.find(nickname));
}

void GerenciaConexao::rmNickname(const qintptr &descript)
{
    for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
    {
        if(itMap->second->descriptor() == descript)
        {
            emit salvarLog(nickname(descript), BROADCAST_DESCONECTADO, "");
            broadcast(DESCONECTADO, nickname(descript));
            mMapNickConexao.erase(itMap->first);
            return;
        }
    }

}

QString GerenciaConexao::nickname(const qintptr &descript)
{

    for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
    {
        if(itMap->second->descriptor() == descript)
        {
            return itMap->first;
        }
    }
    return QString("");
}

qintptr GerenciaConexao::descriptor(const QString &nick)
{
    return mMapNickConexao[nick]->descriptor();
}

QString GerenciaConexao::encapsularMsg(const QString &qstrOrigem, const QString &qstrDestino, const QString &qstrMsg)
{
    return QString("#%1#%2#:%3").arg(qstrOrigem).arg(qstrDestino).arg(qstrMsg);
}

/**
 * @brief GerenciaConexao::broadcast
 * @param status
 * @param usuario
 * envia um broadCast dos usuarios conectados
 */
void GerenciaConexao::broadcast(const GerenciaConexao::enumStatus &status, const QString &usuario)
{
    //Se tiver apenas um usuário, não há necessidade de broadcast
    if(mMapNickConexao.size() == 1)
        return;

    QString broadMsg;
    QStringList listaUsuarios;
    QString flagBroadcast;

    //carregando lista de usuarios para envio. o ultimo conectado/desconectado sempre sera o ultimo
    for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
    {
        if(itMap->first != usuario)
            listaUsuarios << itMap->first;
    }
    listaUsuarios << usuario;
    broadMsg = listaUsuarios.join(";");

    if(status == CONECTADO)
    {
        flagBroadcast = BROADCAST_CONECTADO;
        for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
        {
            itMap->second->enviarMensagem(encapsularMsg(BROADCAST_KEY, flagBroadcast, broadMsg));
        }
    }
    else if(status == DESCONECTADO)
    {
        flagBroadcast = BROADCAST_DESCONECTADO;
        for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
        {
            if(itMap->first != usuario)
                itMap->second->enviarMensagem(encapsularMsg(BROADCAST_KEY, flagBroadcast, broadMsg));
        }
    }
    else
    {
        qDebug() << "status invalido!";
        return;
    }


}

void GerenciaConexao::validarNickname(const QString& nick,Conexao *cliente, const bool &valido)
{
    if(valido)
    {
        if(!cliente->enviarMensagem(encapsularMsg(nick)))
            qDebug() << "impossivel enviar nickname";
    }
    else
    {
        if(!cliente->enviarMensagem(encapsularMsg("")))
            qDebug() << "impossivel enviar nickname em uso";
    }

}

void GerenciaConexao::redirecionarMensagem(const QString &org, const QString &dst, const QString &msg)
{
    mMapNickConexao[org]->enviarMensagem(encapsularMsg(org, dst, msg));
    mMapNickConexao[dst]->enviarMensagem(encapsularMsg(org, dst, msg));
    emit salvarLog(org, dst, msg);
}

QString GerenciaConexao::origem() const
{
    return mOrigem;
}

void GerenciaConexao::setOrigem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itFirstQuadrado = std::find(str.begin(), str.end(), '#');
    string::iterator itSecondQuadrado = std::find(itFirstQuadrado + 1, str.end(), '#');

    mOrigem = QString::fromStdString(string(itFirstQuadrado + 1, itSecondQuadrado));

}

QString GerenciaConexao::destino() const
{
    return mDestino;
}

void GerenciaConexao::setDestino(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itSecondQuadrado = std::find(str.begin() + 1, str.end(), '#');
    string::iterator itThirdQuadrado = std::find(itSecondQuadrado + 1, str.end(), '#');

    mDestino = QString::fromStdString(string(itSecondQuadrado + 1, itThirdQuadrado));
}

QString GerenciaConexao::mensagem() const
{
    return mMensagem;
}

void GerenciaConexao::setMensagem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    str = string(str.begin(),std::remove(str.begin(), str.end(), '\r'));
    str = string(str.begin(),std::remove(str.begin(), str.end(), '\n'));

    string::iterator itFirst = std::find(str.begin(), str.end(), ':');

    mMensagem = QString::fromStdString(string(itFirst + 1, str.end()));
}

/**
 * @brief Servidor::validarEstruturaMensagem
 * @param msg
 * @return
 * valida a estrutura do protocolo de mensagem, conforme a seguir:
 * #origem#destino#:mensagem
 * o destino pode está vazio, representando assim o input de um nickname
 */
bool GerenciaConexao::validarEstruturaMensagem(const QByteArray &msg)
{
    //     #origem#destino#:mensagem

    string str = QString(msg).toStdString();

    if(!(std::count(str.begin(), str.end(),'#') >= 3 && std::count(str.begin(), str.end(), ':') >= 1))
    {
        return false;
    }

    string::iterator itFirstQuadrado = std::find(str.begin(), str.end(), '#');
    string::iterator itSecondQuadrado = std::find(itFirstQuadrado + 1, str.end(), '#');
    string::iterator itThirdQuadrado = std::find(itSecondQuadrado + 1, str.end(), '#');
    string::iterator itPonto = std::find(str.begin(), str.end(), ':');

    if(itFirstQuadrado == str.begin())
        if(std::distance(itFirstQuadrado, itSecondQuadrado) > 1)    //deve haver pelo menos 1 letra na origem
            if(itThirdQuadrado + 1 == itPonto)                      //o ultimo # deve seguir de :
                return true;

    return false;
}


