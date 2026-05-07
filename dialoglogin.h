#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include "mainwidget.h"
#include "managerwindow.h"
#include "engineerwindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QWindow>
#include <QString>

class mainWidget;
namespace Ui
{
    class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT
public:
    explicit DialogLogin(QWidget *parent = nullptr);
    ~DialogLogin();
    void toManagerSlot();
    void toEngineerSlot();
    ManagerWindow managerWindow;
    EngineerWindow engineerWindow;


private slots:
    void on_loginOKButton_clicked();

    void on_loginCancelButton_clicked();

private:
    Ui::DialogLogin *ui;
    mainWidget *mainwidget;

    QString loginUsername;
    QString loginPassword;
    QString loginTitle;
    int loginIsActive;

};

#endif // DIALOGLOGIN_H
