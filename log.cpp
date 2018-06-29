#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{
    setup();
}

void Log::run()
{
    while(1){}  //loop onde esta rodando minha task
}

void Log::setup()
{
    mDirLog.mkdir("log");
    mDirLog.cd("log");
    mNomeArqOut =  "log-" % QDate::currentDate().toString("yyyy-MM-dd") % ".csv";
}

void Log::salvarLog(const QString &org, const QString &dst, const QString &msg)
{
    salvarArq(gerarQStrLog(org, dst, msg));

}

QDir Log::dirLog() const
{
    return mDirLog;
}

QString Log::nomeArqOut() const
{
    return mNomeArqOut;
}

QString Log::gerarQStrLog(const QString &org, const QString &dst, const QString &msg)
{
    QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString mensagem;

    if(dst == BROADCAST_CONECTADO)
    {
        mensagem = "conectado";
    }
    else if(dst == BROADCAST_DESCONECTADO)
    {
        mensagem = "desconectado";
    }
    else
    {
        mensagem = msg;
    }

    return qstrLog % ";" % org % ";" % dst % ";" % mensagem;
}

void Log::salvarArq(const QString &qstrLog)
{
    bool primeiraEscrita;
    ifstream arqExist(dirLog().absoluteFilePath(nomeArqOut()).toStdString());
    if(arqExist.is_open())
    {
        primeiraEscrita = false;
        arqExist.close();
    }
    else
    {
        primeiraEscrita = true;
    }

    ofstream arq(dirLog().absoluteFilePath(nomeArqOut()).toStdString(), ios_base::app);
    if(arq.is_open())
    {
        if(primeiraEscrita)
            arq << "data-hora;origem;destino;mensagem" << endl;


        arq << qstrLog.toStdString() << endl;
        arq.close();
    }
    else
    {
        qDebug() << "impossivel abrir arquivo de log:" << nomeArqOut();
    }

}
