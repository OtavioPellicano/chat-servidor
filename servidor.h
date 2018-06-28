#ifndef SERVIDOR_H
#define SERVIDOR_H
/**
  Formato da mensagem:
  #origem#destino#:mensagem

  Formato Nickname:
  #origem##:


**/

#include <QTcpServer>
#include <QDebug>
#include <algorithm>
#include <QStringBuilder>
#include "conexao.h"
#include "gerenciaconexao.h"

using std::string;

class Servidor : public QTcpServer
{
    Q_OBJECT
public:
    explicit Servidor(QObject *parent = nullptr);

    virtual ~Servidor();

    void startServidor();

private:
    GerenciaConexao *gerenConexao() const;
    void setGerenConexao(GerenciaConexao *gerenConexao);

    QString origem() const;
    void setOrigem(const QByteArray &msg);

    QString destino() const;
    void setDestino(const QByteArray &msg);

    QString mensagem() const;
    void setMensagem(const QByteArray &msg);

    Conexao *conexao() const;
    void setConexao(Conexao *conexao);

    void validarNickname();
    bool validarEstruturaMensagem(const QByteArray &msg);


protected:

    void incomingConnection(qintptr descript);

signals:

public slots:
    void readyRead(const QByteArray &msg);
    void disconnected(const qintptr &descrpt);

private:

    Conexao *mConexao;

    GerenciaConexao *mGerenConexao;

    QString mOrigem;
    QString mDestino;
    QString mMensagem;
};

#endif // SERVIDOR_H
