#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "managerwindow.h"
#include "engineerwindow.h"

#include <QWidget>
#include <QSqlQuery>
#include <QString>

QT_BEGIN_NAMESPACE
//class DialogLogin;
class ManagerWindow;
class EngineerWindow;
//class DialogUpgrade;

namespace Ui
{
    class mainWidget;
}
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT
signals:
    void mySignal();
public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();
    void connectDB();
    void toManagerSlot();
    void toEngineerSlot();
    void setLoginImageSize();
    ManagerWindow managerWindow;
    EngineerWindow engineerWindow;
    //DialogUpgrade dialogUpgrade;
    QString loginUsername;



public:  //private:
    Ui::mainWidget *ui;
    //  DialogLogin *dialogLogin;
    QSqlQuery loginUserQuery;
    QSqlDatabase db;
    QString loginPassword;
    QString loginTitle;
    int loginIsActive;
    QString dataBaseHostName;
    int dataBasePort;
    QString dataBasePortStr;
    QString dataBaseUserName;
    QString dataBasePassWord;
    bool OpenDBResult;

    //QSqlDatabase loginDB;

private slots:
    void closeEvent (QCloseEvent *);

    void on_loginOKButton_clicked();
    void on_loginCancelButton_clicked();
    void on_passwordEdit_returnPressed();
    void on_pushButton_Save_Settings_clicked();
    void on_pushButton_Exit_Settings_clicked();
    void on_pushButton_MainWidget_Settings_clicked();

public:
    void on_progressBar_valueChanged(int value);
    void on_progressBar_changeLabel();
    QString EncryptionStr(QString str);
    void loadSetting();

};
#endif // MAINWIDGET_H
