#include "gerenciaconexao.h"

GerenciaConexao::GerenciaConexao()
{

}

GerenciaConexao::~GerenciaConexao()
{
    mMapNickDescript.clear();
}

bool GerenciaConexao::addNickname(const QString &nickanme, const qintptr &descript)
{
    if(mMapNickDescript.find(nickanme) != mMapNickDescript.end())
        return false;

    mMapNickDescript[nickanme] = descript;
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
            mMapNickDescript.erase(itMap->first);
            return;
        }

    }


}
