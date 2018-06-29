#include "gerenciaconexao.h"

GerenciaConexao::GerenciaConexao(QObject *parent)
    :QObject(parent)
{
    setNomeArqOut("log");
    setupDir();

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

void GerenciaConexao::validarNickname(QString nick,Conexao *cliente, const bool &valido)
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
}

void GerenciaConexao::setupDir()
{
    mDirLog.mkdir("log");
    mDirLog.cd("log");
}


void GerenciaConexao::setNomeArqOut(const QString &nomeArqOut)
{
    mNomeArqOut = nomeArqOut % "-" % QDate::currentDate().toString("yyyy-MM-dd") % ".csv";
}


QString GerenciaConexao::nomeArqOut() const
{
    return mNomeArqOut;
}

QDir GerenciaConexao::dirLog() const
{
    return mDirLog;
}

