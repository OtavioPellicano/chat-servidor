#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpServer>
#include <QDebug>
#include <algorithm>
#include <QStringBuilder>
#include "conexao.h"
#include "gerenciaconexao.h"

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

    Conexao *conexao() const;
    void setConexao(Conexao *conexao);

protected:
    void incomingConnection(qintptr descript);

signals:

public slots:
    void readyRead(const QByteArray &msg);
    void disconnected(const qintptr &descrpt);

private:

    Conexao *mConexao;

    GerenciaConexao *mGerenConexao;

};

#endif // SERVIDOR_H
