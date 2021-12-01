#include "serveur.h"
#include <QJsonArray>
#include <QtSql>
#include <iostream>
#include <QTextStream>
using namespace std;
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
    //qDebug()<<"PASSWORD SELECTED"<<password<<"LOGIN SELECTED"<<login;
    //qDebug()<<"Finding file with sotred credentials : "<<QDir::currentPath();

    //OPENING FILE WITH CREDENTIALS STORED
    QFile file("test.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return  ;
    }
    QStringList wordList;
    // Create a thread to retrieve data from a file
    QTextStream in(&file);
    //Reads the data up to the end of file
    while (!in.atEnd())
    {
        QString line = in.readLine();
        // Adding to the model in line with the elements
        // consider that the line separated by semicolons into columns
        for (QString item : line.split(";")) {
            wordList.append(item);

        }

    }
    qDebug() <<"EXISTING CREDENTIALS:"<< wordList;
    int cpt=0;
    for(int i = 0 ;i<=wordList.count()/2;i++)
    {
        qDebug()<<"LOGIN"<<wordList[i]<<"=?"<<login;
        qDebug()<<"PASSWORD"<<wordList[i+1]<<"=?"<<password;
        if (wordList[i]==login && wordList[i+1]==password ){
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
                cpt++;

            }
    }

    if (cpt==0){
    //SENDING ANSWER
    //CREATING JSON WITH ANSWER
    QJsonObject  answer_JSON_object;
    answer_JSON_object.insert("action", QJsonValue::fromVariant("answerlogin"));
    answer_JSON_object.insert("state", QJsonValue::fromVariant("notok"));

    QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
    //qDebug()<<"JSON RESPOSNE LOGIN"<<answer;
    //qDebug()<<"STRING SENT" <<answer_QString.toLocal8Bit();
    sock->write(answer_QString.toLocal8Bit());
    //qDebug()<<"dans le readall"<<sock->readAll();
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
