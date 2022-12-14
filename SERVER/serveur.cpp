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
    connect(mServeur,SIGNAL(newConnection()),this,SLOT(nouvelleConnexion()));
    mServeur->listen(QHostAddress::Any,8585);
}


//THIS FUNCTION AIMS TO WRITE A QJSONOBJECT INTO THE SOCKET THE ANSWER TO THE LOGIN ACTION : ERROR OR OK
void serveur::check_credentials(const QJsonObject &json)
{
    QTcpSocket *sock = (QTcpSocket *) sender();
    QString login = json["login"].toString();
    QString password = json["password"].toString();

    //OPENING FILE WITH CREDENTIALS STORED
    QFile file("credentials.csv");
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
    int cpt=0;
    for(int i = 0 ;i<=wordList.count()/2;i++)
    {
        if (wordList[i]==login && wordList[i+1]==password ){
            //SENDING ANSWER
            //CREATING JSON WITH ANSWER
            QJsonObject  answer_JSON_object;

            answer_JSON_object.insert("action", QJsonValue::fromVariant("answerlogin"));
            mLogin = login;
            answer_JSON_object.insert("state", QJsonValue::fromVariant("ok"));

            QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
            sock->write(answer_QString.toLocal8Bit());
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
        sock->write(answer_QString.toLocal8Bit());
    }

}

//THIS FUNCTION AIMS TO WRITE INTO THE SOCKET THE MESSAGE SENT
void serveur::send_message(const QJsonObject &json)
{
    QTcpSocket *sock = (QTcpSocket *) sender();
    //GETTING VALUES FROM THE JSON
    QString message = json["content"].toString();
    QString receivers ="";
    QJsonArray receivers_Array = json["to"].toArray();
    foreach(const QJsonValue &value, receivers_Array) {

    receivers = value.toString()+",";
    }
    if (receivers.back()==",")
        receivers.chop(1);
    //GETTING CURRENT TIME
    QString date = QDateTime::currentDateTime().toString(" dd/MM/yy hh:mm");

    // WRITE INTO THE FILE TO STORE MESSAGES
    QFile file("messages_1.csv");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream <<message<< ";"<<"Receivers:"+receivers<<";"<<"From:"+mLogin <<";"<<date<<"\n";
        file.close();
    }
    //CREATING JSON
    QJsonObject  answer_JSON_object;
    answer_JSON_object.insert("action", QJsonValue::fromVariant("receive"));
    answer_JSON_object.insert("from", QJsonValue::fromVariant(receivers));
    answer_JSON_object.insert("content", QJsonValue::fromVariant(message));

    answer_JSON_object.insert("datetime", QJsonValue::fromVariant(date));


    QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
    sock->write(answer_QString.toLocal8Bit());
}

//THIS FUNCTION AIMS TO READ THE CONTENT OF THE QJSONOBJECT SENT AND TO RUN THE CORRESPONDING ACTION
void serveur::Proceed(const QJsonObject &json){
    if (json.constFind("action") != json.constEnd()) {
        if (json["action"].toString() == "login") {
            check_credentials(json);
        };
        if (json["action"].toString() == "send") {
            send_message(json);
        };
        if (json["action"].toString() == "get_messages") {
            get_messages(json);
        };

    };
}


void serveur::nouvelleConnexion() {
    qDebug() << "A client has been connected";
    QTcpSocket *connection = mServeur->nextPendingConnection();
    connect(connection, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(connection, SIGNAL(readyRead()), this, SLOT(clientmeparle()));
}

void serveur::sockDisconnected() {
    QTcpSocket *sock = (QTcpSocket *) sender();
    mListeSocks.remove(mListeSocks.key(sock));
    qDebug() << "A client has been diconected";
}

void serveur::get_messages(const QJsonObject &json)
{
    QTcpSocket *sock = (QTcpSocket *) sender();

    //OPENING FILE WITH CREDENTIALS STORED WHERE LOGINS ATE STORED TOO
    QFile file("messages_1.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return  ;
    }
    QStringList wordList;
    // Create a thread to retrieve data from a file
    QTextStream in(&file);
    //Reads the data up to the end of file
    int cpt = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();;
        cpt++;
        // Adding to the model in line with the elements
        // consider that the line separated by semicolons into columns
        for (QString item : line.split(";")) {
            wordList.append(item);


        }}
    QString receivers = json["contact"].toString();

    for(int i = 0 ;i<=wordList.count()-1;)
    {
        if (wordList[i+1] == "Receivers:"+receivers){
            //SENDING ANSWER
            //CREATING JSON WITH ANSWER
            QJsonObject  answer_JSON_object;
            if (wordList[i+2]=="From:"+mLogin){

                answer_JSON_object.insert("messages", QJsonValue::fromVariant(wordList[i+3]+"->"+wordList[i]));
            }
            else {answer_JSON_object.insert("messages", QJsonValue::fromVariant(wordList[i+3]+"<-"+wordList[i]));}
            answer_JSON_object.insert("action", QJsonValue::fromVariant("get_contacts"));
            answer_JSON_object.insert("date", QJsonValue::fromVariant(wordList[i+3]));
            QString answer_QString = QJsonDocument(answer_JSON_object).toJson(QJsonDocument::Compact).toStdString().c_str();
            answer_QString = answer_QString +";";
            sock->write(answer_QString.toLocal8Bit());
            qDebug()<<"Server returned : "<<answer_QString;
        }
        cpt++;
        i=i+4;


    }

}
void serveur::clientmeparle()
{
    QTcpSocket *sock = (QTcpSocket *) sender();
    QByteArray data = sock->readAll();
    if (data.isEmpty()) {
        qDebug() << "No data was currently available for reading from file";
    }
    QJsonObject jsonObject = QJsonDocument::fromJson(data).object();
    Proceed(jsonObject);

}
