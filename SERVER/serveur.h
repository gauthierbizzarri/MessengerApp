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

public slots:

signals:


private slots:
    void nouvelleConnexion();
    void clientmeparle();
    void sockDisconnected();
    void Proceed(const QJsonObject &json);
    void check_credentials(const QJsonObject &jsons);
    void get_messages(const QJsonObject &json);
    void send_message(const QJsonObject &json);
private:
    QTcpServer* mServeur;
    QMap<QString,QTcpSocket *> mListeSocks;
    QString mLogin="";
    int lignes_csv=0;
};

#endif // SERVEUR_H
