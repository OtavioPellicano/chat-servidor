#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpServer>
#include <QDebug>
#include "conexao.h"


class Servidor : public QTcpServer
{
    Q_OBJECT
public:
    explicit Servidor(QObject *parent = nullptr);

    void startServidor();

private:

    Conexao *conexao() const;
    void setConexao(Conexao *conexao);

protected:

    void incomingConnection(qintptr descript);

signals:

public slots:

private:
    Conexao *mConexao;
};

#endif // SERVIDOR_H
