#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"messenger_main.h"
#include <QMessageBox>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
       connect(ui->pushButton_Login, SIGNAL(clicked(bool)), this, SLOT(pushButton_Login_clicked()));
       connect(this, SIGNAL(ready_to_start_main_messenger(bool)), this, SLOT(run_main_messenger()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run_main_messenger()
{
    //CLOSING LOGIN PAGE ( MAINWINDOW)
    hide();

    //OPENING MESSENGER MAIN PAGE
    messenger_main = new Messenger_Main(this);
    messenger_main->show();

}

void MainWindow::pushButton_Login_clicked()
{
    //Getting credentials as a QString
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    //Creating JSON with credentials
    QJsonObject  credentials;
    credentials.insert("action", QJsonValue::fromVariant("login"));
    credentials.insert("login", QJsonValue::fromVariant(username));
    credentials.insert("password", QJsonValue::fromVariant(password));

    // SENDING JSON TO SERVER
    QJsonDocument doc(credentials);
    QString jsString = doc.toJson(QJsonDocument::Compact);
    sock->write(jsString.simplified().toLocal8Bit());
}
void MainWindow::serveurMeParle()
{
QByteArray data = sock->readAll();
    qDebug()<<"LE serveur a parlé"<<data;
   if (data.isEmpty()) {
       qDebug() << "No data was currently available for reading from file";
   }
   QJsonObject jsonObject = QJsonDocument::fromJson(data).object();
   if (jsonObject["action"].toString() == "answerlogin") {
       if (jsonObject["state"].toString() == "ok") {
           hide();
           //Dialog message confirming the sucessful connection
           QMessageBox::information(this, "Login", "Credentials correct : Username and password are correct");
           emit ready_to_start_main_messenger(true);
       } else {
           QMessageBox::information(this, "Login", "Credentials not correct : try again");
       }
   }
}


void MainWindow::serveurConnected() {
    sock = (QTcpSocket *) sender();
    sock->write("Je suis co aux Serveurs !");
}


