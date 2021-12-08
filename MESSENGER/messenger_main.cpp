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
#include <QFileDialog>
#include <QInputDialog>
#include <QtWidgets>

Messenger_Main::Messenger_Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Messenger_Main)

{
    ui->setupUi(this);

    //Background image

     ui->lineEdit_message_to_send->setVisible(false);
     qDebug()<<QDir::currentPath();
     QPixmap bkgnd(QDir::currentPath()+"/login_background.jpg");
    bkgnd = bkgnd.scaled(2000,1100, Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    sock = new QTcpSocket(this);
    connect(sock, SIGNAL(readyRead()), this, SLOT(serveurMeParle2()));
    connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
    connect(sock, SIGNAL(connected()), this, SLOT(serveurConnected2()));
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
    //get contact from lineEdit_Add_Contact
    //a method will get all contacts and will __ie__contains return a window with existing contacts , the user select it and it will be added
    bool ok;
    QString contact = QInputDialog::getText(this, tr("Add contact"),
                                             tr("Enter name:  \n Separate users with a ," ), QLineEdit::Normal,
                                             "user1,user2", &ok);
   // QString contact = ui->lineEdit_Add_Contact->text();
    if (ok && !contact.isEmpty()){
        //Adding Contact to the QListWidgetItem
        QListWidgetItem* Contact_Item = new QListWidgetItem(contact);

        //Display in listWidget_Contacts
        ui->listWidget_Contacts->addItem(Contact_Item);
    }}

//THIS FUNCTION AIMS TO EXPORT THE CURRENT CONVERSATION AS PDF
void Messenger_Main::Export_PDF(){
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
               if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
               QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            QTextDocument *doc = new QTextDocument();
            QString html;
            for(int i = 0; i < ui->listWidget_Messages->count(); ++i)
            {
                QListWidgetItem* item = ui->listWidget_Messages->item(i);
                 html = html + "<p>"+item->text()+"</p>";
            }

            doc->setHtml(html);
            doc->print(&printer);
}


void Messenger_Main::Send_Message(){
    QString message = ui->lineEdit_message_to_send->text();
    if  (!message.isNull() ||!ContactSelected->text().isEmpty() || message==""){
        //GETTING RECEIVER
        QString receiver = ContactSelected->text();

        QRegExp regex("(\\;"
                      ")");
        QStringList receivers =receiver.split(regex);
        //get message from lineEdit_message_to_send


        //Creating JSON with Message to send
        QJsonObject  message_to_send_JSON;
        message_to_send_JSON.insert("action", QJsonValue::fromVariant("send"));
        message_to_send_JSON.insert("to", QJsonValue::fromVariant(receivers));
        message_to_send_JSON.insert("content", QJsonValue::fromVariant(message));

        QJsonDocument doc(message_to_send_JSON);
        QString jsString = doc.toJson(QJsonDocument::Compact);
        sock->write(jsString.simplified().toLocal8Bit());

        //Making the lineEdit_message_to_send clear (preparing new message to send)
        ui->lineEdit_message_to_send->setText("");
    }

}
//THIS FUNCTION ALLOW THE TCHATER TO TALK TO THE USER WH
void Messenger_Main::Select_Contact(QListWidgetItem *mContact)
{

    QString contact = mContact->text();
    ContactSelected =mContact;
    ui->lineEdit_message_to_send->setVisible(true);
    ui->listWidget_Messages->clear();
    QMessageBox::information(this, "Messages", "Getting conversation with contact "+contact);
    ui->label_Contact_Selected->setText("Conversation with :"+contact);
    ui->listWidget_Messages->addItem("Conversation with :"+contact);

    QJsonObject  contact_Json;
    contact_Json.insert("contact", QJsonValue::fromVariant(contact));
    contact_Json.insert("action", QJsonValue::fromVariant("get_messages"));

    // SENDING JSON TO SERVER
    QJsonDocument doc(contact_Json);
    QString jsString = doc.toJson(QJsonDocument::Compact);
    sock->write(jsString.simplified().toLocal8Bit());

}

void Messenger_Main::serveurMeParle2()
{

    QByteArray data = sock->readAll();
    qDebug()<<"Server sent a response"<<data;
    if (data.isEmpty()) {
        qDebug() << "No data was currently available for reading from file";
    }

    QString DataAsString = QString(data);
    QStringList messageList;
    for (QString item : DataAsString.split(";"))
    {
        messageList.append(item);
        QJsonObject jsonObject = QJsonDocument::fromJson(item.toUtf8()).object();
        QListWidgetItem* Message =  new QListWidgetItem(jsonObject["messages"].toString());
        ui->listWidget_Messages->addItem(Message);
    }

}

void Messenger_Main::serveurConnected2() {
    sock = (QTcpSocket *) sender();
}
