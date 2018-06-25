#include <QCoreApplication>
#include "servidor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Servidor meuServidor;
    meuServidor.startServidor();

    return a.exec();
}
