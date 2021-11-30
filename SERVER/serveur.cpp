#include "serveur.h"
#include <QJsonArray>
#include <QtSql>
#include <iostream>
#define q2c(string) string.toStdString()
serveur::serveur(QObject *parent) : QObject(parent)
{
    mServeur = new QTcpServer(this);
    connect(mServeur,SIGNAL(newConnection()),this,SLOT(nouveauJoueur()));
    mServeur->listen(QHostAddress::Any,8585);
}
void serveur::check_credentials(const QJsonObject &json)
{
    QTcpSocket *sock = (QTcpSocket *) sender();
       QString login = json["login"].toString();
       QString password = json["password"].toString();
    qDebug()<<"PASSWORD SELECTED"<<password<<"LOGIN SELECTED"<<login;
    //qDebug()<<"Finding file with sotred credentials : "<<QDir::currentPath();

    //OPENING FILE WITH CREDENTIALS STORED
    QFile file("test.csv");
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << file.errorString();
            return  ;
        }
        QStringList wordList;
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            wordList.append(line.split(',').first());
             //qDebug() <<"EXISTING CREDENTIALS:"<< wordList;
        }

        if (wordList[0]==login){
            if (wordList[1]==password){
                //SENDING ANSWER
                //CREATING JSON WITH ANSWER
                QJsonObject  answer_JSON_object;
                answer_JSON_object.insert("action", QJsonValue::fromVariant("answerlogin"));
                answer_JSON_object.insert("state", QJsonValue::fromVariant("ok"));

                QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
                //qDebug()<<"JSON RESPOSNE LOGIN"<<answer;
                qDebug()<<"STRING SENT" <<answer_QString.toLocal8Bit();
                sock->write(answer_QString.toLocal8Bit());
                qDebug()<<"dans le readall"<<sock->readAll();

        }
        }
        else {
            //SENDING ANSWER
            //CREATING JSON WITH ANSWER
            QJsonObject  answer_JSON_object;
            answer_JSON_object.insert("action", QJsonValue::fromVariant("answerlogin"));
            answer_JSON_object.insert("state", QJsonValue::fromVariant("notok"));

            QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
            //qDebug()<<"JSON RESPOSNE LOGIN"<<answer;
            qDebug()<<"STRING SENT" <<answer_QString.toLocal8Bit();
            sock->write(answer_QString.toLocal8Bit());
            qDebug()<<"dans le readall"<<sock->readAll();
            }

}
void serveur::Proceed(const QJsonObject &json){
        if (json.constFind("action") != json.constEnd()) {
            if (json["action"].toString() == "login") {
                check_credentials(json);
            };
            if (json["action"].toString() == "send") {
                qDebug() << "j'envoie un message" << json;
            };
        };
    }

void serveur::nouveauJoueur()
{
    QTcpSocket *connection = mServeur->nextPendingConnection();
    connect(connection, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(connection,SIGNAL(readyRead()),this,SLOT(joueurMeParle()));
    //connect(connection,SIGNAL(readyRead()),this,SLOT(joueurMeParle()));
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
        QTcpSocket *sock = (QTcpSocket *) sender();
        QByteArray data = sock->readAll();
        if (data.isEmpty()) {
            qDebug() << "No data was currently available for reading from file";
        }
        QJsonObject jsonObject = QJsonDocument::fromJson(data).object();
        Proceed(jsonObject);

}
