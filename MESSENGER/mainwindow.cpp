#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"messenger_main.h"
#include <QMessageBox>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMainWindow>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //SERVER
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
    //login button :
    connect(ui->pushButton_Login,SIGNAL(clicked(bool)),this,SLOT(pushButton_Login_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Automatic connection methhod used for the login button
void MainWindow::pushButton_Login_clicked()
{
    //Getting credentials as a QString
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();




    if(username ==  "test" && password == "test") {
        //Dialog message confirming the sucessful connection
        QMessageBox::information(this, "Login", "Credentials correct : Username and password are correct");

        //CONNECTING TO SERVER IF CREDENTIALS CORRECT
        QTcpSocket* sock = new QTcpSocket(this);
            connect(sock,SIGNAL(readyRead()),this,SLOT(serveurMeParle()));
            connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));



        //Creating JSON with credentials
        QJsonObject  credentials;;
        credentials.insert("action", QJsonValue::fromVariant("login"));
        credentials.insert("login", QJsonValue::fromVariant(username));
        credentials.insert("password", QJsonValue::fromVariant(password));
        qDebug()<<credentials;

        // SENDING JSON TO SERVER
        sock->connectToHost("localhost",8585);
        QJsonDocument doc(credentials);
        QString jsString = doc.toJson(QJsonDocument::Compact);
        qDebug() << sock->readAll();
        sock->write(jsString.toLatin1());

        //CLOSING LOGIN PAGE ( MAINWINDOW)
        hide();

        //OPENING MESSENGER MAIN PAGE

        messenger_main = new Messenger_Main(this);
        messenger_main->show();
    }

    //If credentials incorrect , sending an error dialog message,user has to retry
    else{
         QMessageBox::warning(this,"Login", "Credentials incorrect : Username or Password are not correct");



    }
}
void MainWindow::serveurMeParle()
{
    QTcpSocket * sock = (QTcpSocket*)sender();
    qDebug() << sock->readAll();

}

