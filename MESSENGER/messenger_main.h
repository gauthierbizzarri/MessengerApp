#ifndef MESSENGER_MAIN_H
#define MESSENGER_MAIN_H
#include<QListWidgetItem>
#include<QListWidget>
#include <QMainWindow>
#include <QTcpSocket>
namespace Ui {
class Messenger_Main;
}

class Messenger_Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Messenger_Main(QWidget *parent = nullptr);
    ~Messenger_Main();

public slots:

private slots:
     void serveurMeParle2();
     void serveurConnected2();
    void Export_PDF();
    void Send_Message();
    void Get_Messages();
    void Add_Contact();
    void Get_Contacts();
    void Select_Contact(QListWidgetItem *contact);

private:
    Ui::Messenger_Main *ui;
    QTcpSocket *sock;
    QListWidgetItem* MessageItem = nullptr;
    QListWidgetItem* ContactSelected = nullptr;
    QList <QMap<QString,QString>>* Conversations = nullptr;
};

#endif // MESSENGER_MAIN_H
