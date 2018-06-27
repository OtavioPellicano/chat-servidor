#include "gerenciaconexao.h"

GerenciaConexao::GerenciaConexao()
{
    setNomeArqOut("log");
    setupDir();
}

GerenciaConexao::~GerenciaConexao()
{
    mMapNickDescript.clear();
}

bool GerenciaConexao::addNickname(const QString &nick, const qintptr &descript)
{
    if(mMapNickDescript.find(nick) != mMapNickDescript.end())
        return false;

    mMapNickDescript[nick] = descript;
    salvarLog(CONECTADO, nickname(descript));
    return true;

}

void GerenciaConexao::rmNickname(const QString &nickname)
{
    mMapNickDescript.erase(mMapNickDescript.find(nickname));
}

void GerenciaConexao::rmNickname(const qintptr &descript)
{
    for(auto itMap = mMapNickDescript.begin(); itMap != mMapNickDescript.end(); ++itMap)
    {
        if(itMap->second == descript)
        {
            salvarLog(DESCONECTADO, nickname(descript));
            mMapNickDescript.erase(itMap->first);
            return;
        }
    }


}

QString GerenciaConexao::nickname(const qintptr &descript)
{
    for(auto itMap = mMapNickDescript.begin(); itMap != mMapNickDescript.end(); ++itMap)
    {
        if(itMap->second == descript)
        {
            return itMap->first;
        }
    }
    return QString("");
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

    log = log % " -> " % qstrCliente;

    if(status == CONECTADO)
    {
        log = log % " conectado";
    }
    else if(status == DESCONECTADO)
    {
        log = log % " desconectado";
    }
    else
    {
        qDebug() << "status invalido!";
        return;
    }

    ofstream arq(dirLog().absoluteFilePath(nomeArqOut()).toStdString(), ofstream::app);
    if(arq.is_open())
    {
        arq << log.toStdString() << std::endl;
        arq.close();
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
    mNomeArqOut = nomeArqOut % "-" % QDate::currentDate().toString("yyyy-MM-dd") % ".txt";
}


QString GerenciaConexao::nomeArqOut() const
{
    return mNomeArqOut;
}

QDir GerenciaConexao::dirLog() const
{
    return mDirLog;
}

