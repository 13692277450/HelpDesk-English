#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include "mainwidget.h"
#include <QSqlQuery>
#include <QButtonGroup>
#include <QMessageBox>
#include <QSqlRecord>
#include <QIcon>



DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog); //取消login 窗口最大化
    this->setWindowIcon(QIcon(":/Icon/bird.png"));
    mainwidget = (mainWidget *)parentWidget ();
    //connect(ui->loginOKButton,SIGNAL(clicked()),this,SLOT(toManagerSlot()));
}

DialogLogin::~DialogLogin()
{
    delete ui;
}
void DialogLogin::toEngineerSlot ()
{
}

void DialogLogin::toManagerSlot()
{
}

void DialogLogin::on_loginOKButton_clicked()
{
    loginUsername = ui->usernameEdit->text().trimmed ();
    loginPassword = ui->passwordEdit->text().trimmed ();
    QSqlQuery query;
    QString strQuery = QString("select * from HelpDesk_Team where userName = '%1' and password = '%2'").
                       arg (loginUsername).arg(loginPassword);
    query.exec(strQuery);
    QSqlRecord rec = query.record();
    qDebug() << "This is DialogUI query....";
    if(!query.next())
    {
        QMessageBox::warning (this, "Login Failure", "Sorry, Your username or password error, Pls double check and try again!");
        return;
    }
    else
    {
        rec = query.record ();
        int loginIsActive = rec.indexOf ("isActive");
        int loginTitle = rec.indexOf ("roleTitle");
        QString loginTitleValue = query.value(loginTitle).toString();
        int logintIsActiveValue = query.value(loginIsActive).toInt ();
        if(logintIsActiveValue == 0)
        {
            QMessageBox::warning (this, "Login Error", "Sorry, Your Account was disabled!");
            return;
        }
        else if(loginTitleValue == "Engineer")
        {
            engineerWindow.showMaximized ();
            this->hide();
        }
        else
        {
            qDebug() << "Login sucessful";
            qDebug() << loginUsername << loginPassword << loginTitle << loginIsActive <<"Manually input";
            managerWindow.showMaximized ();
            this->hide();
        }
    }
}
void DialogLogin::on_loginCancelButton_clicked()
{
    mainWidget().db.close ();
    exit (0);
}
