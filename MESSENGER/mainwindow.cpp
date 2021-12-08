#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"messenger_main.h"
#include "messenger_main.h"
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
#include <QCryptographicHash>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet("QMainWindow {@mixin sp-layout {
                        @media screen and (max-width: 750px) {
                          @content;
                        }
                      }

                      body {
                        display: flex;
                        justify-content: center;
                        align-items: center;
                        min-height: 100vh;
                         background: radial-gradient(ellipse at bottom, #0d1d31 0%, #0c0d13 100%);
                        overflow: hidden;
                      }

                      @function random_range($min, $max) {
                        $rand: random();
                        $random_range: $min + floor($rand * (($max - $min) + 1));
                        @return $random_range;
                      }

                      .stars {
                        position: fixed;
                        top: 0;
                        left: 0;
                        width: 100%;
                        height: 120%;
                        transform: rotate(-45deg);
                      }

                      .star {
                        $star-count: 50;
                        --star-color: var(--primary-color);
                        --star-tail-length: 6em;
                        --star-tail-height: 2px;
                        --star-width: calc(var(--star-tail-length) / 6);
                        --fall-duration: 9s;
                        --tail-fade-duration: var(--fall-duration);

                        position: absolute;
                        top: var(--top-offset);
                        left: 0;
                        width: var(--star-tail-length);
                        height: var(--star-tail-height);
                        color: var(--star-color);
                        background: linear-gradient(45deg, currentColor, transparent);
                        border-radius: 50%;
                        filter: drop-shadow(0 0 6px currentColor);
                        transform: translate3d(104em, 0, 0);
                        animation: fall var(--fall-duration) var(--fall-delay) linear infinite, tail-fade var(--tail-fade-duration) var(--fall-delay) ease-out infinite;

                        @include sp-layout {
                          // For mobile performance, tail-fade animation will be removed QAQ
                          animation: fall var(--fall-duration) var(--fall-delay) linear infinite;
                        }

                        @for $i from 1 through $star-count {
                          &:nth-child(#{$i}) {
                            --star-tail-length: #{random_range(500em, 750em) / 100};
                            --top-offset: #{random_range(0vh, 10000vh) / 100};
                            --fall-duration: #{random_range(6000, 12000s) / 1000};
                            --fall-delay: #{random_range(0, 10000s) / 1000};
                          }
                        }

                        &::before, &::after {
                          position: absolute;
                          content: '';
                          top: 0;
                          left: calc(var(--star-width) / -2);
                          width: var(--star-width);
                          height: 100%;
                          background: linear-gradient(45deg, transparent, currentColor, transparent);
                          border-radius: inherit;
                          animation: blink 2s linear infinite;
                        }

                        &::before {
                          transform: rotate(45deg);
                        }

                        &::after {
                          transform: rotate(-45deg);
                        }
                      }

                      @keyframes fall {
                        to {
                          transform: translate3d(-30em, 0, 0);
                        }
                      }

                      @keyframes tail-fade {
                        0%, 50% {
                          width: var(--star-tail-length);
                          opacity: 1;
                        }

                        70%, 80% {
                          width: 0;
                          opacity: 0.4;
                        }

                        100% {
                          width: 0;
                          opacity: 0;
                        }
                      }

                      @keyframes blink {
                        50% {
                          opacity: 0.6;
                        }
                      } }");

    /*
    //Background image
    qDebug()<<QDir::current();
    QPixmap bkgnd(QDir::currentPath()+"/login_background.jpg");
    bkgnd = bkgnd.scaled(2000,1100, Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
*/
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
    QString password =  QCryptographicHash::hash(ui->lineEdit_password->text().toUtf8(),
                                                 QCryptographicHash::Sha3_512).toHex();

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
    qDebug()<<"Server sent a response"<<data;
    if (data.isEmpty()) {
        qDebug() << "No data was currently available for reading from file";
    }
    QJsonObject jsonObject = QJsonDocument::fromJson(data).object();

    //WE GET AN ANSWER LOGIN FROM THE SERVER
    if (jsonObject["action"].toString() == "answerlogin") {
        if (jsonObject["state"].toString() == "ok") {
            hide();
            //Dialog message confirming the sucessful connection
            QMessageBox::information(this, "Login", "Credentials correct : Username and password are correct");
            emit ready_to_start_main_messenger(true);
        } else {
            //Dialog message asking for an other try
            QMessageBox::information(this, "Login", "Credentials not correct : try again");
        }
    }
}



void MainWindow::serveurConnected() {
    sock = (QTcpSocket *) sender();
}


