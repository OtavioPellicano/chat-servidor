#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpServer>
#include <QDebug>
#include <algorithm>
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

    GerenciaConexao *gerenConexao() const;
    void setGerenConexao(GerenciaConexao *gerenConexao);

private:

    Conexao *conexao() const;
    void setConexao(Conexao *conexao);


protected:

    void incomingConnection(qintptr descript);

signals:

public slots:
    void readyRead(const QByteArray &msg);
    void disconnected(const qintptr &descrpt);

private:

    const QString KEY_NICKNAME = "$nicknamePass$";

    Conexao *mConexao;

    GerenciaConexao *mGerenConexao;
};

#endif // SERVIDOR_H
