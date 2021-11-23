#include <QCoreApplication>
#include "serveur.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    serveur s;
    return a.exec();
}
