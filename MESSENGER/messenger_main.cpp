#include "messenger_main.h"
#include "ui_messenger_main.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextDocument>
#include <QApplication>
#include <QPrinter>

Messenger_Main::Messenger_Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger_Main)

{
    ui->setupUi(this);
    // CONNECT TO SERVER
    QTcpSocket* sock = new QTcpSocket(this);
        connect(sock,SIGNAL(readyRead()),this,SLOT(serveurMeParle()));
        connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
    sock->connectToHost("localhost",8585);
    //Background image

    QPixmap bkgnd("/home/bizzarri/mess2/login_background.jpg");
        bkgnd = bkgnd.scaled(2000,1100, Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);

    //pushButton_Export_PDF
    connect(ui->pushButton_Export_PDF,SIGNAL(clicked(bool)),this,SLOT(Export_PDF()));

    //pushButton_Send_Message
    connect(ui->pushButton_Send_Message,SIGNAL(clicked(bool)),this,SLOT(Send_Message()));

    //pushButton_Add_Contact
    connect(ui->pushButton_Add_Contact,SIGNAL(clicked(bool)),this,SLOT(Add_Contact()));
    //lineEdit_Add_Contact

    //Select Contact from listWidget_Contacts with double click
      connect(ui->listWidget_Contacts,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(Select_Contact(QListWidgetItem*)));

      //label_Contact_Selected

}

Messenger_Main::~Messenger_Main()
{
    delete ui;
}
void Messenger_Main::Add_Contact()
{
    QTcpSocket* sock = new QTcpSocket(this);
    connect(sock,SIGNAL(readyRead()),this,SLOT(serveurMeParle()));
    connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
    sock->connectToHost("localhost",8585);
    //get contact from lineEdit_Add_Contact
    //a method will get all contacts and will __ie__contains return a window with existing contacts , the user select it and it will be added

    QString contact = ui->lineEdit_Add_Contact->text();

    //Adding Contact to the QListWidgetItem
    QListWidgetItem* Contact_Item = new QListWidgetItem(contact);

    //Display in listWidget_Contacts
    ui->listWidget_Contacts->addItem(Contact_Item);

    //Making the lineEdit_message_to_send clear (preparing new message to send)
    ui->lineEdit_Add_Contact->setText("");

}
void Messenger_Main::Export_PDF()
{
}
//Send message to only one user
void Messenger_Main::Send_Message(){
    QString message = ui->lineEdit_message_to_send->text();
if  (message.isNull())
        return; // We don't send empty messages
    else{
    //CONNECTING TO SERVER IF CREDENTIALS CORRECT
    QTcpSocket* sock = new QTcpSocket(this);
        connect(sock,SIGNAL(readyRead()),this,SLOT(serveurMeParle()));
        connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));

    //ContactSelected->text(); is the name of the current user
    qDebug()<<ContactSelected->text();
    //get receiver // TO FIX ONLY ONE NOW

    QString receiver = ContactSelected->text();

    //get message from lineEdit_message_to_send




    QString date_displayed = QDateTime::currentDateTime().toString(" dd/MM/yy hh:mm");
    //QString date = QDateTime::currentDateTime().toString(" dd/mm/yy");
    QString message_formated = date_displayed + ": ->    "+message;

    //Adding message to the QListWidgetItem
    QListWidgetItem* MessageItem = new QListWidgetItem(message_formated);


    //Creating JSON with Message to send
    QJsonObject  message_to_send_JSON;
    message_to_send_JSON.insert("action", QJsonValue::fromVariant("send"));
    message_to_send_JSON.insert("to", QJsonValue::fromVariant(receiver));
    message_to_send_JSON.insert("content", QJsonValue::fromVariant(message));
    qDebug()<<message_to_send_JSON;

    // SENDING JSON TO SERVER
    sock->connectToHost("localhost",8585);
    QJsonDocument doc(message_to_send_JSON);
    QString jsString = doc.toJson(QJsonDocument::Compact);
    qDebug() << sock->readAll();
    sock->write(jsString.toLatin1());
    //Display in listWidget_Messages
        //should get messages with a contact
    ui->listWidget_Messages->addItem(MessageItem);

    //Making the lineEdit_message_to_send clear (preparing new message to send)
    ui->lineEdit_message_to_send->setText("");
}

}
void Messenger_Main::Get_Messages()
{}
void Messenger_Main::Get_Contacts()
{
    //Get contacts from the server, returns a QWidgetList
}
void Messenger_Main::Select_Contact(QListWidgetItem *mContact)
{
    QString contact = mContact->text();
    ContactSelected =mContact;
    ui->label_Contact_Selected->setText(contact);
    //Get the conversation with this contact and display it into : listWidget_Messages
    //for element in conversation :
    ui->listWidget_Messages->addItem("Conversation with :"+contact);
}

void Messenger_Main::serveurMeParle()
{
    QTcpSocket * sock = (QTcpSocket*)sender();
    qDebug() << sock->readAll();

}

void Messenger_Main::serveurConnected(QJsonObject *credentials)
{
    QJsonDocument doc(*credentials);
    QString jsString = QString(doc.toJson());

    QTcpSocket * sock = (QTcpSocket*)sender();
    //credentials->value("action").toString())
    qDebug() << sock->readAll();
    sock->write(jsString.toLatin1());
    sock->write("test");
}
