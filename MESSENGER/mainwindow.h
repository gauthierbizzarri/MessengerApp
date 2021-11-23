#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void pushButton_Login_clicked();
    void serveurMeParle();
private:
    Ui::MainWindow *ui;
    Messenger_Main *messenger_main;
};

#endif // MAINWINDOW_H
