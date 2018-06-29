#include "gerenciaconexao.h"

GerenciaConexao::GerenciaConexao()
{
    setNomeArqOut("log");
    setupDir();
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
    salvarLog(CONECTADO, nickname(cliente->descriptor()));
    broadcast(CONECTADO, nickname(cliente->descriptor()));

    return true;
}

void GerenciaConexao::rmNickname(const QString &nickname)
{
    salvarLog(DESCONECTADO, nickname);
    broadcast(DESCONECTADO, nickname);
    mMapNickConexao.erase(mMapNickConexao.find(nickname));
}

void GerenciaConexao::rmNickname(const qintptr &descript)
{
    for(auto itMap = mMapNickConexao.begin(); itMap != mMapNickConexao.end(); ++itMap)
    {
        if(itMap->second->descriptor() == descript)
        {
            salvarLog(DESCONECTADO, nickname(descript));
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

/**
 * @brief GerenciaConexao::salvarLog
 * @param status
 * @return
 * Modelo de salvamento
 * yyyy-MM-dd HH:mm:ss -> cliente status
 */

void GerenciaConexao::salvarLog(const enumStatus &status, const QString &qstrCliente)
{
    QString log = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    log = log % ";" % qstrCliente;

    if(status == CONECTADO)
    {
        log = log % ";conectado";
    }
    else if(status == DESCONECTADO)
    {
        log = log % ";desconectado";
    }
    else
    {
        qDebug() << "status invalido!";
        return;
    }


    ifstream arqExist(dirLog().absoluteFilePath(nomeArqOut()).toStdString());
    bool primeiraEscrita;
    if(arqExist.is_open())
    {
        primeiraEscrita = false;
        arqExist.close();
    }
    else
    {
        primeiraEscrita = true;
    }


    ofstream arq(dirLog().absoluteFilePath(nomeArqOut()).toStdString(), ofstream::app);
    if(arq.is_open())
    {
        if(primeiraEscrita)
            arq << "Data-hora;usuário;status" << endl;

        arq << log.toStdString() << endl;
        arq.close();
        qDebug() << "log salvo";
    }
    else
    {
        qDebug() << "impossivel abrir arquivo de log:" << nomeArqOut();
        return;
    }


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

