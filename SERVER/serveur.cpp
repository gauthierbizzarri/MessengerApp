#include "serveur.h"

serveur::serveur(QObject *parent) : QObject(parent)
{
    mServeur = new QTcpServer(this);
    connect(mServeur,SIGNAL(newConnection()),this,SLOT(nouveauJoueur()));

    mServeur->listen(QHostAddress::Any,8585);
}

void serveur::nouveauJoueur()
{
    QTcpSocket *connection = mServeur->nextPendingConnection();
    connect(connection, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(connection,SIGNAL(readyRead()),this,SLOT(joueurMeParle()));
    mListeSocks << connection;
}

void serveur::sockDisconnected()
{
    QTcpSocket * sock = (QTcpSocket*)sender();
    mListeSocks.removeAll(sock);
    delete sock;
}

void serveur::joueurMeParle()
{
    QTcpSocket * sock = (QTcpSocket*)sender();
    qDebug() << sock->readAll();
    sock->write("OK");
}
