#include "messenger_main.h"
#include <mainwindow.h>
#include "ui_messenger_main.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextDocument>
#include <QApplication>
#include <QPrinter>
#include <QFile>
#include <QDir>
#include <QStringList>



Messenger_Main::Messenger_Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger_Main)

{
    ui->setupUi(this);
    //Background image

    QPixmap bkgnd("/home/bizzarri/mess2/login_background.jpg");
        bkgnd = bkgnd.scaled(2000,1100, Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);

    sock = new QTcpSocket(this);
    connect(sock, SIGNAL(readyRead()), this, SLOT(serveurMeParle()));
    connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
    connect(sock, SIGNAL(connected()), this, SLOT(serveurConnected()));
    sock->connectToHost("localhost", 8585);
    //pushButton_Export_PDF
    connect(ui->pushButton_Export_PDF,SIGNAL(clicked(bool)),this,SLOT(Export_PDF()));

    //pushButton_Send_Message
    connect(ui->pushButton_Send_Message,SIGNAL(clicked(bool)),this,SLOT(Send_Message()));

    //pushButton_Add_Contact
    connect(ui->pushButton_Add_Contact,SIGNAL(clicked(bool)),this,SLOT(Add_Contact()));
    //lineEdit_Add_Contact

    //Select Contact from listWidget_Contacts with double click
      connect(ui->listWidget_Contacts,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(Select_Contact(QListWidgetItem*)));


}

Messenger_Main::~Messenger_Main()
{
    delete ui;
}
//THIS FUNCTION WILL
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

//THIS FUNCTION AIMS TO EXPORT THE CURRENT CONVERSATION AS PDF
void Messenger_Main::Export_PDF(){}

//THIS FUNCTION AIMS TO SEND A MESSAGE TO ONLY ONE USER ....
void Messenger_Main::Send_Message(){
    QString message = ui->lineEdit_message_to_send->text();
if  (message.isNull())
        return; // We don't send empty messages
    else{
    //GETTING RECEIVER
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

    QJsonDocument doc(message_to_send_JSON);
    QString jsString = doc.toJson(QJsonDocument::Compact);
    qDebug()<<jsString;
    sock->write(jsString.simplified().toLocal8Bit());

    //Display in listWidget_Messages
        //should get messages with a contact
    ui->listWidget_Messages->addItem(MessageItem);

    //Making the lineEdit_message_to_send clear (preparing new message to send)
    ui->lineEdit_message_to_send->setText("");
}

}
//THIS FUNCTION IS CALLED TO LOAD MESSAGES SOTRED IN THE DATABASE
void Messenger_Main::Get_Messages()
{
    //OPENING FILE WITH MESSAGES STORED
    QFile file("messages.csv");
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
}

//THIS FUNCTION AIMS TO GET ALL CONTACTS STORED IN THE DATABASE
void Messenger_Main::Get_Contacts()
{
    //Get contacts from the server, returns a QWidgetList
}

//THIS FUNCTION ALLOW THE TCHATER TO TALK TO THE USER WH
void Messenger_Main::Select_Contact(QListWidgetItem *mContact)
{
    QString contact = mContact->text();
    ContactSelected =mContact;
    ui->label_Contact_Selected->setText(contact);
    //Get the conversation with this contact and display it into : listWidget_Messages
    //for element in conversation :
    ui->listWidget_Messages->addItem("Conversation with :"+contact);
}





