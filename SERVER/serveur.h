#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class serveur : public QObject
{
    Q_OBJECT
public:
    explicit serveur(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void nouveauJoueur();
    void sockDisconnected();
    void joueurMeParle();
private:
    QTcpServer* mServeur;
    QList<QTcpSocket*> mListeSocks;
};

#endif // SERVEUR_H
