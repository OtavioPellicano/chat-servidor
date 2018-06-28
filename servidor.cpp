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
        conexao()->enviarMensagem("Mensagem fora da estrutura");
        return;
    }

    setOrigem(msg);
    setDestino(msg);
    setMensagem(msg);

    qDebug() << origem();
    qDebug() << destino();
    qDebug() << mensagem();

    if(destino().isEmpty()) //representa o nickname
    {
        validarNickname();
    }

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

QString Servidor::origem() const
{
    return mOrigem;
}

void Servidor::setOrigem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itFirstQuadrado = std::find(str.begin(), str.end(), '#');
    string::iterator itSecondQuadrado = std::find(itFirstQuadrado + 1, str.end(), '#');

    mOrigem = QString::fromStdString(string(itFirstQuadrado + 1, itSecondQuadrado));

}

QString Servidor::destino() const
{
    return mDestino;
}

void Servidor::setDestino(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    string::iterator itSecondQuadrado = std::find(str.begin() + 1, str.end(), '#');
    string::iterator itThirdQuadrado = std::find(itSecondQuadrado + 1, str.end(), '#');

    mDestino = QString::fromStdString(string(itSecondQuadrado + 1, itThirdQuadrado));
}

QString Servidor::mensagem() const
{
    return mMensagem;
}

void Servidor::setMensagem(const QByteArray &msg)
{
    string str = QString(msg).toStdString();

    str = string(str.begin(),std::remove(str.begin(), str.end(), '\r'));
    str = string(str.begin(),std::remove(str.begin(), str.end(), '\n'));

    string::iterator itFirst = std::find(str.begin(), str.end(), ':');

    mMensagem = QString::fromStdString(string(itFirst + 1, str.end()));
}

Conexao *Servidor::conexao() const
{
    return mConexao;
}

void Servidor::setConexao(Conexao *conexao)
{
    mConexao = conexao;
}

/**
 * @brief Servidor::validarNickname
 * Valida se o nickname ja existe.
 * Caso exista, o cliente é desconectado
 * Caso não exista, o nickname é cadastrado
 */
void Servidor::validarNickname()
{

    if(gerenConexao()->addNickname(origem(), conexao()))
    {
        if(!conexao()->enviarMensagem(gerenConexao()->encapsularMsg(origem())))
            qDebug() << "impossivel enviar mensagem!";
    }
    else
    {
        qDebug() << "nickname ja existe";
        if(!conexao()->enviarMensagem(gerenConexao()->encapsularMsg("")))
            qDebug() << "impossivel enviar mensagem!";

        delete conexao();
    }

}
/**
 * @brief Servidor::validarEstruturaMensagem
 * @param msg
 * @return
 * valida a estrutura do protocolo de mensagem, conforme a seguir:
 * #origem#destino#:mensagem
 * o destino pode está vazio, representando assim o input de um nickname
 */
bool Servidor::validarEstruturaMensagem(const QByteArray &msg)
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
