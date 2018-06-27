#ifndef GERENCIACONEXAO_H
#define GERENCIACONEXAO_H

#include <QString>
#include <QStringBuilder>
#include <map>
#include <fstream>
#include <QDir>
#include <QDateTime>
#include <QDebug>


using std::map;
using std::ofstream;
class GerenciaConexao
{
public:
    GerenciaConexao();
    virtual ~GerenciaConexao();

    bool addNickname(const QString &nick, const qintptr &descript);
    void rmNickname(const QString &nickname);
    void rmNickname(const qintptr &descript);
    QString nickname(const qintptr &descript);

private:

    void setNomeArqOut(const QString &nomeArqOut);

    QDir dirLog() const;

    QString nomeArqOut() const;

    enum enumStatus{CONECTADO, DESCONECTADO};

    void salvarLog(const enumStatus &status, const QString &qstrCliente);

    void setupDir();


private:
    map<QString, qintptr> mMapNickDescript;
    QDir mDirLog;
    QString mNomeArqOut;

};

#endif // GERENCIACONEXAO_H
