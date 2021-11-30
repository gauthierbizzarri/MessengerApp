#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QMainWindow>
#include <stdexcept>
#include <regex>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <QTextCodec>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include "messenger_main.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



public
    slots:
    void run_main_messenger();
    void pushButton_Login_clicked();

    signals:
    void ready_to_start_main_messenger(bool);

private
    slots:
    void serveurMeParle();
    void serveurConnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket *sock;
    Messenger_Main *messenger_main;
};

#endif // MAINWINDOW_H
