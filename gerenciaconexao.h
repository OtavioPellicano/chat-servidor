#ifndef GERENCIACONEXAO_H
#define GERENCIACONEXAO_H

#include <QString>
#include <map>

using std::map;

class GerenciaConexao
{
public:
    GerenciaConexao();
    virtual ~GerenciaConexao();

    bool addNickname(const QString &nickanme, const qintptr &descript);
    void rmNickname(const QString &nickname);
    void rmNickname(const qintptr &descript);


private:
    map<QString, qintptr> mMapNickDescript;
};

#endif // GERENCIACONEXAO_H
