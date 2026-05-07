#ifndef ENGINEERWINDOW_H
#define ENGINEERWINDOW_H
#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include "dialogupgrade.h"
#include "dialogdonate.h"
#include "dialoghelp.h"
#include <QUdpSocket>
#include <QTimer>
class mainWidget;

namespace Ui
{
    class EngineerWindow;
}

class EngineerWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void mySignal();
private slots:
    // void timerEvent (QTimerEvent *);

    //void changeEvent(QEvent *event);
    void autoUpdateContactList();
    void toDialogSlot();
    void on_readyRead();


    void on_actionWeekly_Report_triggered();

    void on_actionMonthly_Report_triggered();

    void on_actionYearly_Report_triggered();

    void on_actionExit_System_triggered();

    void on_actionAll_My_Cases_triggered();

    void on_actionAll_My_Open_Cases_triggered();

    void on_actionAll_My_Closed_Cases_triggered();

    void on_actionAll_Open_Hold_Cases_triggered();

    void on_actionAll_Closed_Cases_triggered();

    void on_actionAll_Cases_triggered();

    void on_comboBox_ITEngineer_NameList();

    void on_pushButton_Take_Case_clicked();

    void on_pushButton_Handover_Case_clicked();

    void on_pushButton_Update_Case_clicked();

    void on_pushButton_Close_Cases_clicked();

    void on_tableView_AllCases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_Closed_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_Open_Hold_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_AllMy_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_AllMy_Closed_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_My_Open_Hold_Cases_doubleClicked(const QModelIndex &index);

    void on_actionConnect_Database_triggered();

    void on_actionAbout_Super_HelpDesk_triggered();

    void on_actionKnowledge_Center_triggered();

    void on_actionCheck_New_Version_triggered();

    void on_pushButton_Chat_SendMessage_clicked();

    void on_tableView_HelpDesk_Contact_doubleClicked(const QModelIndex &index);

    void on_actionMonitor_Winodw_triggered();

    void on_pushButton_Chat_Search_clicked();

    void on_tableView_Cases_Today_doubleClicked(const QModelIndex &index);
    void closeEvent (QCloseEvent *);

    void on_pushButton_Chat_RefreshList_clicked();

    void on_pushButton_Chat_UpdateIP_clicked();

    void on_tableView_HelpDesk_Contact_activated();


    void on_tableView_Cases_Today_activated();

    void on_pushButton_Chat_Refresh_CasesToday_clicked();


    void on_actionDonate_to_Support_Super_HelpDesk_triggered();

public:
    explicit EngineerWindow(QWidget *parent = nullptr);
    ~EngineerWindow();
    void InitialAllDataEngineer();
    void Double_Clicked_Case_Details(const QSqlIndex &index);
    void initialData_Cases_By_CaseStatus();
    void BindPort();
    QString get_IpAdress();
    void get_UserName();
    void get_MachineName();
    void comboBox_Chat_UpdateIP();
    void updateIPToMySQL();

    //QSqlTableModel *model;

    Ui::EngineerWindow *ui;
    //chat moduel start.......
    QString senderNameEmit;
    QHostAddress senderPeerAddressEmit;
    QString strReceiveWithoutNameEmit;
    QString senderName;
    QHostAddress senderPeerAddress;
    QString senderMessagesChatForm;
    quint16 senderPeerPort;
    QString strReceiveWithoutName;
    QString helpDeskIP;
    QString localIPAddressUpdateIP ;
    QHostAddress peerAddr;
    quint16 peerPort;
    QString sendTitle;
    QString dstIP;
    bool isClickHelpdeskContact = false;
    QTimer *timerUpdateContactList;
    //chat module end ......
    QSqlIndex *index;
    QSqlTableModel *model_Action_All_Closed_Cases;
    QSqlTableModel *model_Action_All_OpenHold_Cases;
    QSqlTableModel *model_Action_All_Cases;
    QSqlTableModel *model_Action_All_My_Cases;
    QSqlTableModel *model_Action_All_My_OpenHold_Cases;
    QSqlTableModel *model_Action_All_My_Closed_Cases;
    QSqlTableModel *model_HelpDesk_Contact;
    QSqlTableModel *model_Cases_Today;

    mainWidget *mainwidget;
    EngineerWindow *engineerWindow;
    DialogHelp *dialogHelp;
    DialogDonate *dialogDonate;
    DialogUpgrade *dialogUpgrade;
    QUdpSocket *m_udpSocket = nullptr;
    QString WindowsUserName;
    QString localHostName1;
    QString localIPAddress;
    QString loginUsernameEngineer;
    //QSqlQueryModel *model;

private:

protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

};

#endif // ENGINEERWINDOW_H
