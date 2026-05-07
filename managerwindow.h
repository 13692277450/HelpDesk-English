#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H
#include <QMainWindow>
#include <QSqlTableModel>
#include <QDate>
#include <QDateTime>
#include <QComboBox>
#include <QUdpSocket>
#include "dialogupgrade.h"
#include "dialoghelp.h"
#include "dialogdonate.h"
class mainWidget;


namespace Ui
{
    class ManagerWindow;
}

class ManagerWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void mySignal();
private slots:

    void toDialogSlot();

    void on_actionAll_Cases_triggered();

    void on_actionCases_By_Engineer_triggered();

    void on_actionAll_Open_Cases_triggered();

    void on_actionAll_Closed_Cases_triggered();


    void on_comboBox_By_Engineer_activated();

    void on_comboBox_By_Cases_Status_activated();

    void on_actionAll_Hold_Cases_triggered();

    void on_pushButton_Search_Cases_clicked();

    void on_dateEdit_Start_Date_dateTimeChanged();

    void on_dateEdit_End_Date_dateTimeChanged();

    void Delay_MSec();

    //void ChartData_All_Cases_By_Dept_333333333();



    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void on_actionPavoGroup_triggered();

    void on_actionKnowledge_Library_triggered();

    void on_actionKnowledge_Library_Settings_triggered();

    void on_actionDatabase_Backup_triggered();

    void on_actionHelpDesk_Support_Report_triggered();

    void on_actionCase_Stastics_triggered();

    void on_actionExport_Support_triggered();

    void on_actionExit_triggered();

    void on_actionConnect_Database_triggered();

    void on_actionAll_My_Cases_triggered();

    void on_actionAll_My_Open_Hold_Cases_triggered();

    void on_actionAll_My_Closed_Cases_triggered();

    void on_actionMaintain_Employees_triggered();


    void on_pushButton_Add_User_clicked();

    void on_comboBox_Delete_UserName_currentIndexChanged(int index);

    void on_pushButton_Delete_clicked();

    void on_comboBox_Update_UserName_activated(int index);

    void on_comboBox_Check_UserName_activated(int index);

    void on_pushButton_Check_clicked();

    void on_pushButton_Update_clicked();

    void on_comboBox_Check_UserName_currentIndexChanged(int index);

    void on_comboBox_Update_UserName_currentIndexChanged(int index);

    void on_AllCasestableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_Details_Take_Case_clicked();

    void on_pushButton_Details_Updat_Case_clicked();

    void on_pushButton_Details_Close_Case_clicked();

    void on_pushButton_Details_Handover_Case_clicked();

    void on_tableView_All_Closed_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_Hold_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_My_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_My_Closed_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_My_OpenHold_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_All_Open_Cases_doubleClicked(const QModelIndex &index);

    void on_tableView_SearchByEngineer_doubleClicked(const QModelIndex &index);

    void on_actionCheck_Upgrade_triggered();

    void on_actionMonitor_Window_triggered();
    void closeEvent (QCloseEvent *);


    void on_actionChat_Window_triggered();

    void on_tableView_HelpDesk_Contact_activated();

    void on_tableView_HelpDesk_Contact_doubleClicked(const QModelIndex &index);

    void on_tableView_Cases_Today_doubleClicked(const QModelIndex &index);
    void on_tableView_Cases_Today_activated();

    void on_pushButton_Chat_SendMessage_clicked();
    void on_pushButton_Chat_Refresh_CasesToday_clicked();
    void on_pushButton_Chat_Search_clicked();
    void on_readyRead();
    void on_pushButton_Chat_RefreshList_clicked();
    void on_pushButton_Chat_UpdateIP_clicked();

    void on_pushButton_Maintain_Empoyee_Add_User_clicked();

    void on_pushButton_Maintain_Employee_Delete_clicked();

    void on_pushButton_Maintain_Empoyee_Update_clicked();

    void on_pushButton_Maintain_Employee_Check_clicked();

    void on_comboBox_Maintain_Employee_Delete_UserName_activated(int index);

    void on_comboBox_Maintain_Employee_Update_UserName_activated(int index);

    void on_comboBox_Maintain_Employee_Checkout_UserName_activated(int index);


    void on_actionMaintain_Department_Level_Title_triggered();


    void on_pushButton_Maintain_Others_Add_Department_clicked();

    void on_pushButton_Maintain_Others_Delete_Department_clicked();

    void on_pushButton_Maintain_Others_Add_Title_clicked();

    void on_pushButton_Maintain_Others_Delete_Title_clicked();

    void on_pushButton__Maintain_Others_Add_Level_clicked();

    void on_pushButton__Maintain_Others_Delete_Level_clicked();

    void on_pushButton__Maintain_Others_Add_CaseType_clicked();

    void on_pushButton__Maintain_Others_Delete_CaseType_clicked();

    void on_actionDonate_to_support_Super_Helpdesk_triggered();

public:
    explicit ManagerWindow(QWidget *parent = nullptr);
    ~ManagerWindow();

    void ChartData_All_Cases();
    void ChartData_Engineer_HoldCases();
    void ChartData_Yealy_Cases_Sum();
    void ChartData_Sort_Cases_Type();
    void ChartData_Engineer_Open_Cases();
    void ChartData_Monthly_Cases();
    void ChartData_Close_AvarageTime();
    void ChartData_OPenCases();
    void ChartData_Last_Month_Cases();
    void ChartData_Yearly_By_Department_Cases();
    void initialData_Cases_By_Engineer();
    void initialData_Cases_By_CaseStatus();
    void check_SqlServer_Connection();
    void ChartData_All_Cases_By_Dept();
    void initialData_Cases_By_Maintain_itEngineer();
    void Open_Manager_Console();
    void initialData_Cases_By_Maintain_Employees();
    void initialData_Maintain_Others();


protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器


public:
    Ui::ManagerWindow *ui;


    void BindPort();
    QString get_IpAdress();
    void get_UserName();
    void get_MachineName();
    void comboBox_Chat_UpdateIP();
    void updateIPToMySQL();
    void InitialAllDataManager();
    void autoUpdateContactList();

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
    QSqlTableModel *model_HelpDesk_Contact;
    QSqlTableModel *model_Cases_Today;
    QUdpSocket *m_udpSocket = nullptr;
    QString WindowsUserName;
    QString localHostName1;
    QString localIPAddress;

    //chat module end ......

    mainWidget *mainwidget;
    ManagerWindow *managerWindow;
    DialogUpgrade *dialogUpgrade;
    DialogHelp *dialogHelp;
    DialogDonate *dialogDonate;
    //QSqlQueryModel *model;
    QString sortByEngineerName;
    QString sortByIssueType;
    QString sortByStartDate;
    QString sortByEndDate;
    QString loginUsernameManager;
    QSqlTableModel *model_Manager_Action_All_Closed_Cases;
    QSqlTableModel *model_Manager_Action_All_Open_Cases;
    QSqlTableModel *model_Manager_Action_All_Hold_Cases;

    QSqlTableModel *model_Manager_Action_All_Cases;
    QSqlTableModel *model_Manager_Action_All_My_Cases;
    QSqlTableModel *model_Manager_Action_All_My_OpenHold_Cases;

    QSqlTableModel *model_Manager_Action_All_My_Closed_Cases;
    QSqlTableModel *model_Manager_Search_By_Engineer_Cases;

    QComboBox combobox;

};

#endif // MANAGERWINDOW_H
