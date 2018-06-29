#ifndef GERENCIACONEXAO_H
#define GERENCIACONEXAO_H

#include <QString>
#include <QStringBuilder>
#include <map>
#include <fstream>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include "conexao.h"
#include "log.h"


using std::map;
using std::ofstream;
using std::ifstream;
using std::endl;

class GerenciaConexao: public QObject
{
    Q_OBJECT
private:
    enum enumStatus{CONECTADO, DESCONECTADO};

public:
    explicit GerenciaConexao(QObject *parent = nullptr);
    virtual ~GerenciaConexao();

    bool addNickname(const QString &nick, Conexao *cliente);
    void rmNickname(const QString &nickname);
    void rmNickname(const qintptr &descript);
    QString nickname(const qintptr &descript);
    qintptr descriptor(const QString &nick);
    QString encapsularMsg(const QString &qstrOrigem, const QString &qstrDestino = "", const QString &qstrMsg = "");
    void broadcast(const enumStatus &status, const QString &usuario);
    void validarNickname(QString nick, Conexao *cliente, const bool &valido);

    void redirecionarMensagem(const QString &org, const QString &dst, const QString &msg);

signals:
    void salvarLog(const QString &org, const QString &dst, const QString &msg);

private:

    void setNomeArqOut(const QString &nomeArqOut);

    QDir dirLog() const;

    QString nomeArqOut() const;

    void setupDir();

private:
    QDir mDirLog;
    QString mNomeArqOut;

    map<QString, Conexao*> mMapNickConexao;

    const QString BROADCAST_KEY = "$$$";
    const QString BROADCAST_CONECTADO = "$c$";      //broadcast: #$$$$#$c$#:user1;user2;user3
    const QString BROADCAST_DESCONECTADO = "$d$";   //broadcast: #$$$$#$d$#:user1;user2;user3

    QTcpSocket *mSocket;

    Log *mLog;

};

#endif // GERENCIACONEXAO_H
