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


private slots:
    void nouveauJoueur();
    void sockDisconnected();
    void joueurMeParle();
    void Proceed(const QJsonObject &json);
    void check_credentials(const QJsonObject &jsons);
    void send_message(const QJsonObject &json);
private:
    QTcpServer* mServeur;
    QList<QTcpSocket*> mListeSocks;
};

#endif // SERVEUR_H
