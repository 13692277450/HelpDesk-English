
#include "engineerwindow.h"
#include "ui_engineerwindow.h"
#include "mainwidget.h"
QT_USE_NAMESPACE
#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QPieSlice>
#include <QPieSeries>
#include <QGraphicsView>
#include <QChartView>
#include <QtCharts>
#include <QChart>
#include <QChar>
#include <QMessageBox>
#include <QSqlRecord>
#include <QComboBox>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QSqlField>
#include <QSqlDatabase>
#include <QTimer>
#include <QTest>
#include <QLabel>
#include <QBarSeries>
#include <QBarSet>
#include <QColor>
#include <QFont>
#include <QLineSeries>
#include <QLine>
#include <QValueAxis>
#include <QtMath>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QSqlTableModel>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QModelIndex>
#include <QPlainTextEdit>

EngineerWindow::EngineerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EngineerWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Icon/bird2.png"));
    mainwidget = (mainWidget *)parentWidget ();
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket,&QUdpSocket::readyRead,this,&EngineerWindow::on_readyRead);
    BindPort();
    qDebug() << "after bindport start....";
    ui->plainTextEdit_Send_Messages->installEventFilter(this);//设置完后自动调用其eventFilter函数
    timerUpdateContactList = new QTimer(this);
    connect (timerUpdateContactList, &QTimer::timeout,this,&EngineerWindow::autoUpdateContactList);
    timerUpdateContactList->start (600000);
//    comboBox_Chat_UpdateIP();
//    on_comboBox_ITEngineer_NameList();
//    updateIPToMySQL();
//    on_tableView_HelpDesk_Contact_activated();
//    tableView_Cases_Today();
//    QSqlQuery sqlQuery;
//    QString sqlString = "select * from HelpDesk_Team";
//    if(!sqlQuery.exec(sqlString))
//    {
//    }
//    else
//    {
    //  InitialAllDataEngineer();
//    }
}

EngineerWindow::~EngineerWindow()
{
    delete ui;
}

void EngineerWindow::toDialogSlot()
{
    emit mySignal();
}


void EngineerWindow::get_UserName()
{
    WindowsUserName = qgetenv("USERNAME");
    if (WindowsUserName.isEmpty())
    {
        WindowsUserName = qgetenv("NoName");
    }
    QTest::qSleep (10);
}
void EngineerWindow::get_MachineName()
{
    localHostName1 = QHostInfo::localHostName();
    QTest::qSleep (10);
}
QString EngineerWindow::get_IpAdress()
{
    localIPAddress = "";
    QList<QHostAddress>listAddress = QNetworkInterface::allAddresses();
    for(int j = 0; j < listAddress.size(); j++)
    {
        if(!listAddress.at(j).isNull() &&
                listAddress.at(j).protocol() ==  QAbstractSocket::IPv4Protocol
                && listAddress.at(j) != QHostAddress::LocalHost)
        {
            localIPAddress = listAddress.at(j).toString();
            // ui->lineEdit_IPAddress->setText (localIPAddress);
            return localIPAddress;
        }
    }
    return localIPAddress;
    //本地主机名
    //    QString hostName = QHostInfo::localHostName();
    //    //本机IP地址
    //    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    //    //IP地址列表
    //    QList<QHostAddress> addrList = hostInfo.addresses();
    //    for(int i=0; i<addrList.count(); i++)
    //    {
    //        QHostAddress host = addrList.at(i);
    //        if(QAbstractSocket::IPv4Protocol == host.protocol())
    //        {
    //            QString ip = host.toString();
    //            ui->lineEdit_IPAddress->setText (ip);
    //        }
    //    }
}


void EngineerWindow:: InitialAllDataEngineer ()
{
    qDebug() << "initial all data start....";
    QSqlQuery query;
    QString sqlStr =  tr("select * from HelpDesk_Team");
    int k = 0;
    if(!query.exec(sqlStr) && k <5)
    {
        mainwidget->connectDB ();
        QTest::qSleep (30000);
        k++;
        if(k == 4)
        {
            QMessageBox::information (this, "Connectiong error!", "Try 5 times to reconnect but could not connect to MySQL database server, pls check your connection!");
            return;
        }
    }
    get_UserName ();
    get_IpAdress ();
    get_MachineName ();
    comboBox_Chat_UpdateIP();
    on_comboBox_ITEngineer_NameList();
    updateIPToMySQL();
    on_tableView_HelpDesk_Contact_activated();
    on_tableView_Cases_Today_activated();
    qDebug() << "initial all data end....";
}

void EngineerWindow::autoUpdateContactList()
{
    // timerUpdateContactList->stop ();
    ui->lineEdit_Chat_Search->clear ();
    on_tableView_HelpDesk_Contact_activated();
    on_tableView_Cases_Today_activated();
    qDebug() << "auto refresh contact list and cases today done....";
    // timerUpdateContactList->start (60000);
}
//void EngineerWindow::timerEvent (QTimerEvent *t)
//{
//    //timerUpdateContactList->start (60000);
//}

void EngineerWindow::on_actionWeekly_Report_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void EngineerWindow::on_actionMonthly_Report_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void EngineerWindow::on_actionYearly_Report_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void EngineerWindow::on_actionExit_System_triggered()
{
    exit(1);
}



void EngineerWindow::on_actionAll_Cases_triggered()
{
    model_Action_All_Cases  = new QSqlTableModel(this);
    model_Action_All_Cases->setTable("Ticket");
    model_Action_All_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Action_All_Cases->select();
    ui->tableView_AllCases->setModel(model_Action_All_Cases);
    model_Action_All_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
//    ui->tableView_AllCases->horizontalHeader()->setStyleSheet("QHeaderView::section {"
//            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_AllCases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // ui->tableView_AllCases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewAllCases->setColumnWidth(4,359);
//    ui->tableView_AllCases->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView_AllCases->setContextMenuPolicy(Qt::CustomContextMenu);
//    ui->tableView_AllCases->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
//    ui->tableView_AllCases->verticalHeader ()->setDefaultAlignment (Qt::AlignVCenter);
//    ui->tableView_AllCases->verticalHeader ()->hide ();
    ui->stackedWidget->setCurrentWidget (ui->page_AllCases);
}



void EngineerWindow::on_actionAll_My_Cases_triggered()
{
    model_Action_All_My_Cases = new QSqlTableModel(this);
    model_Action_All_My_Cases->setTable("Ticket");
    model_Action_All_My_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter ="";
    sFilter += QString("itEngineer = '%1'").arg (loginUsernameEngineer);
    model_Action_All_My_Cases->setFilter (sFilter);
    model_Action_All_My_Cases->select();
    ui->tableView_AllMy_Cases->setModel(model_Action_All_My_Cases);
    model_Action_All_My_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_My_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_My_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_My_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_My_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_My_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_My_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_My_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_My_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_My_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_My_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_My_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_My_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_AllMy_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_AllMyCases);
}



void EngineerWindow::on_actionAll_My_Open_Cases_triggered()
{
    model_Action_All_My_OpenHold_Cases  = new QSqlTableModel(this);
    model_Action_All_My_OpenHold_Cases->setTable("Ticket");
    model_Action_All_My_OpenHold_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter = "";
    sFilter = QString("(issueStatus = 'Hold' or issueStatus = 'Open') and itEngineer = '%1'").arg (loginUsernameEngineer);
    model_Action_All_My_OpenHold_Cases->setFilter (sFilter);
    model_Action_All_My_OpenHold_Cases->select();
    ui->tableView_All_My_Open_Hold_Cases->setModel(model_Action_All_My_OpenHold_Cases);
    model_Action_All_My_OpenHold_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_My_OpenHold_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_My_OpenHold_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_My_OpenHold_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_My_OpenHold_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_My_OpenHold_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_My_OpenHold_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_My_OpenHold_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_My_OpenHold_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_My_OpenHold_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_My_OpenHold_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_My_OpenHold_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_My_OpenHold_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_All_My_Open_Hold_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_AllMyOpenHoldCases);
}






void EngineerWindow::on_actionAll_My_Closed_Cases_triggered()
{
    model_Action_All_My_Closed_Cases  = new QSqlTableModel(this);
    model_Action_All_My_Closed_Cases->setTable("Ticket");
    model_Action_All_My_Closed_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter = "";//数据库查询语句
    sFilter += tr("issueStatus = 'Closed' and itEngineer = '%1'").arg(loginUsernameEngineer);
    model_Action_All_My_Closed_Cases->setFilter(sFilter);
    model_Action_All_My_Closed_Cases->select();
    ui->tableView_AllMy_Closed_Cases->setModel(model_Action_All_My_Closed_Cases);
    model_Action_All_My_Closed_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_My_Closed_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_My_Closed_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_My_Closed_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_My_Closed_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_My_Closed_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_My_Closed_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_My_Closed_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_My_Closed_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_My_Closed_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_My_Closed_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_My_Closed_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_My_Closed_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_AllMy_Closed_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_AllMyClosedCases);
}



void EngineerWindow::on_actionAll_Open_Hold_Cases_triggered()
{
    model_Action_All_OpenHold_Cases  = new QSqlTableModel(this);
    model_Action_All_OpenHold_Cases->setTable("Ticket");
    model_Action_All_OpenHold_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Action_All_OpenHold_Cases->setFilter(("issueStatus = 'Hold' or issueStatus = 'Open'"));// % (room_id)));
    model_Action_All_OpenHold_Cases->select();
    ui->tableView_All_Open_Hold_Cases->setModel (model_Action_All_OpenHold_Cases);
    model_Action_All_OpenHold_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_OpenHold_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_OpenHold_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_OpenHold_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_OpenHold_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_OpenHold_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_OpenHold_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_OpenHold_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_OpenHold_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_OpenHold_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_OpenHold_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_OpenHold_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_OpenHold_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_All_Open_Hold_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_AllOpenHoldCases);
}



void EngineerWindow::on_actionAll_Closed_Cases_triggered()
{
    model_Action_All_Closed_Cases  = new QSqlTableModel(this);
    model_Action_All_Closed_Cases->setTable("Ticket");
    model_Action_All_Closed_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Action_All_Closed_Cases->setFilter(("issueStatus = 'Closed'"));
    model_Action_All_Closed_Cases->select();
    ui->tableView_All_Closed_Cases->setModel(model_Action_All_Closed_Cases);
    model_Action_All_Closed_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Action_All_Closed_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Action_All_Closed_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Action_All_Closed_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Action_All_Closed_Cases->setHeaderData(4,Qt::Horizontal,"Description");
    model_Action_All_Closed_Cases->setHeaderData(5,Qt::Horizontal,"Create Time");
    model_Action_All_Closed_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Action_All_Closed_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Action_All_Closed_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Action_All_Closed_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Action_All_Closed_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Action_All_Closed_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Action_All_Closed_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    //    ui->AllCasestableView->horizontalHeader()->setStyleSheet("QHeaderView::section {"
    //            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_All_Closed_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_AllClosedCases);
    // model->createIndex (int arow, int acolumn, quintptr aid);
}


void EngineerWindow::on_tableView_AllCases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}

void EngineerWindow::initialData_Cases_By_CaseStatus()
{
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Case_Status->addItem(issueStatus_Name);
    }
}

void EngineerWindow::on_comboBox_ITEngineer_NameList()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString itEngineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        //ui->comboBox_ITEngineer->clear ();
        ui->comboBox_ITEngineer->addItem(itEngineer_Name);
    }
}


void EngineerWindow::on_pushButton_Take_Case_clicked()
{
    QString PreItEngineer = ui->lineEdit_Details_ITEngineer->text ().trimmed ();
    QSqlQuery sqlQuery;
    QSqlQuery sqlQueryRemark;
    QString sqlStr = QString("UPDATE HelpDesk.Ticket SET itEngineer='%1' WHERE id=%2").arg (loginUsernameEngineer).arg (ui->lineEdit_Details_ID->text ().toInt ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update time is %1, Take case from %2 to %3． \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (PreItEngineer).arg (loginUsernameEngineer);
        ui->plainTextEdit_Remark->appendPlainText (updateRemark);
        // ui->plainTextEdit_SystemLog->appendPlainText (updateRemark);
        QString remarkResult = ui->plainTextEdit_Remark->toPlainText ().toUtf8 ().trimmed ();
        qDebug() <<"Remark result: " << remarkResult;
        QString sqlRemarkStr = QString("UPDATE HelpDesk.Ticket SET remarkInformation='%1' WHERE id=%2").arg (remarkResult).arg (ui->lineEdit_Details_ID->text ().toInt ());
        if(sqlQueryRemark.exec (sqlRemarkStr))
        {
            QMessageBox::information (this, "Information", "Sucess to update records!");
            ui->lineEdit_Details_ITEngineer->setText (loginUsernameEngineer);
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to update record, pls double check your server connection!");
        }
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update record, pls double check your server!");
    }
}


void EngineerWindow::on_pushButton_Handover_Case_clicked()
{
    if(ui->comboBox_ITEngineer->currentIndex () == -1)
    {
        QMessageBox::information (this, "Information", "Pls select handover IT Engineer first!");
        return;
    }
    QString PreItEngineer = ui->lineEdit_Details_ITEngineer->text ().trimmed ();
    QSqlQuery sqlQuery;
    QString sqlStr = tr("update HelpDesk.Ticket SET itEngineer = '%1'  where id = %2").arg (ui->comboBox_ITEngineer->currentText ().trimmed ()).arg (ui->lineEdit_Details_ID->text ().trimmed ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1, Handover case from IT Engineer: %2 to IT Engineer: %3 by IT Engineer: %4. \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (PreItEngineer).arg (ui->comboBox_ITEngineer->currentText ().trimmed ()).arg (loginUsernameEngineer);
        ui->plainTextEdit_Remark->appendPlainText (updateRemark);
        QMessageBox::information (this, "Information", "Sucess to handover the case!");
        ui->lineEdit_Details_ITEngineer->setText (ui->comboBox_ITEngineer->currentText ().trimmed ());
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update record, pls double check your server!");
    }
}


void EngineerWindow::on_pushButton_Update_Case_clicked()
{
    QString castStatus = ui->comboBox_Case_Status->currentText ().trimmed ();
    QSqlQuery sqlQuery;
    QString solutionStr = ui->plainTextEdit_Solution->toPlainText ().trimmed ();
    QString remarkStr = ui->plainTextEdit_Remark->toPlainText ().trimmed ();
    QString sqlStr = tr("update HelpDesk.Ticket SET issueStatus = '%1', solutionDescription = '%2', remarkInformation = '%3' where id = %4").arg (ui->comboBox_Case_Status->currentText ().trimmed ()).arg(solutionStr).arg(remarkStr).arg(ui->lineEdit_Details_ID->text ().toInt ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1, updated case status to %2 and updated case solution with remark by IT Engineer: %3. \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (ui->comboBox_Case_Status->currentText ().trimmed ()).arg (loginUsernameEngineer);
        ui->plainTextEdit_Remark->appendPlainText (updateRemark);
        QMessageBox::information (this, "Information", "Sucess to update the case!");
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update case, pls double check your server!");
    }
}


void EngineerWindow::on_pushButton_Close_Cases_clicked()
{
    QString caseStatus = ui->comboBox_Case_Status->currentText ().trimmed ();
    QSqlQuery sqlQuery;
    QString solutionStr = ui->plainTextEdit_Solution->toPlainText ().trimmed ();
    QString remarkStr = ui->plainTextEdit_Remark->toPlainText ().trimmed ();
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString DateTimestr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString sqlStr = tr("update HelpDesk.Ticket SET issueStatus = 'Closed', solutionDescription = '%1', remarkInformation = '%2', issueSolvedTime = '%3', itEngineer = '%4' where id = %5").arg (solutionStr).arg (remarkStr).arg (DateTimestr).arg (loginUsernameEngineer).arg (ui->lineEdit_Details_ID->text ().trimmed ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1, Closed this case from Case Status: %2 to Case Status: Closed and updated solution with remark by IT Engineer: %4. \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg(caseStatus).arg (loginUsernameEngineer);
        ui->plainTextEdit_Remark->appendPlainText (updateRemark);
        ui->lineEdit_Details_SolvedTime->setText (DateTimestr);
        QMessageBox::information (this, "Information", "Sucess to close the case!");
        ui->comboBox_Case_Status->setCurrentText ("Closed");
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to close case, pls double check your server!");
    }
}


void EngineerWindow::on_tableView_All_Closed_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_Closed_Cases->record(index.row ()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("caseStatus").toString ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}



void EngineerWindow::on_tableView_All_Open_Hold_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_OpenHold_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("caseStatus").toString ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void EngineerWindow::on_tableView_AllMy_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_My_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void EngineerWindow::on_tableView_AllMy_Closed_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_My_Closed_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void EngineerWindow::on_tableView_All_My_Open_Hold_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Action_All_My_OpenHold_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void EngineerWindow::on_actionConnect_Database_triggered()
{
    mainWidget().connectDB ();
    //ui->plainTextEdit_SystemLog->appendPlainText ("Try to re-connect to mysql database at " + QDateTime::currentDateTime ().toString ());
}


void EngineerWindow::on_actionAbout_Super_HelpDesk_triggered()
{
    dialogHelp = new DialogHelp(this);
    dialogHelp->setModal (false);
    dialogHelp->show ();
}


void EngineerWindow::on_actionKnowledge_Center_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, and knowledge module is only be available in professional version, more information, pls visit www.pavogroup.top.");
}


void EngineerWindow::on_actionCheck_New_Version_triggered()
{
    dialogUpgrade = new DialogUpgrade(this);
    dialogUpgrade->setModal (false);
    dialogUpgrade->show ();
}




void EngineerWindow::BindPort()
{
    //ui->lineEdit_LocalPort->setText ("3000");
    ui->plainTextEdit_Chat_Messages->appendPlainText ("Local Hostname: " + localHostName1);
    ui->plainTextEdit_Chat_Messages->appendPlainText ("Login Username: " + WindowsUserName);
    ui->plainTextEdit_Chat_Messages->appendPlainText ("IP Address    : " + localIPAddress);
    m_udpSocket->abort();
    qint16 port =8686; // = ui->lineEdit_LocalPort->text ().toInt ();
    QString portString = "8686";
    if(m_udpSocket->bind(QHostAddress::AnyIPv4,port))
    {
        // ui->plainTextEdit_Chat_Messages->appendPlainText ("**Sucessed to bind port**");
        QString portAdd = "**Succeeded bind port: "+QString::number(m_udpSocket->localPort())+" at all IP Addresses**";
        ui->plainTextEdit_Chat_Messages->appendPlainText (portAdd);
        // ui->pushButton_BindPort->setEnabled(false);
        //  ui->pushButton_UnBindPort->setEnabled(true);
    }
    else
    {
        ui->plainTextEdit_Chat_Messages->appendPlainText("**Failure to bind port: " + portString + "**");
    }
}

void EngineerWindow::on_readyRead()
{
    //是否还有待读取的传入数据报
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray data;
        //返回待读取的数据报的字节数
        data.resize(m_udpSocket->pendingDatagramSize());
        //        QHostAddress peerAddr;
        //        quint16 peerPort;
        //读取数据报的内容
        m_udpSocket->readDatagram(data.data(),data.size(),&peerAddr,&peerPort);
        senderPeerAddress = peerAddr;
        QString str = data.data();
        QStringList list = str.split("///");
        senderName = list.at(list.length()-1);
        strReceiveWithoutName = list.at(list.length ()-2);
        //  QString peer = "[From ] +"+peerAddr.toString()+":"+QString::number(peerPort)+"] ";
        sendTitle = "[From "+senderName+ "-" + QTime::currentTime ().toString () + "]: ";
        //  ui->plainTextEdit_Chat_Messages->appendPlainText(peer+str);
        QTextCharFormat fmt;
        //字体色
        strReceiveWithoutNameEmit = strReceiveWithoutName;
        senderNameEmit = senderName;
        senderPeerAddressEmit = senderPeerAddress;
        QString messageForwardToChatForm = sendTitle + strReceiveWithoutName;
        //on_readyReadEmit();
        ui->label_ChatWithName->setText (senderName);
        fmt.setForeground(QBrush(Qt::blue));
        ui->plainTextEdit_Chat_Messages->mergeCurrentCharFormat(fmt);
        ui->plainTextEdit_Chat_Messages->appendPlainText(sendTitle+strReceiveWithoutName);
        dstIP = peerAddr.toString ();
        // V2.5 code .........................................................................................
        //        if((ui->label_ChatWithName->text () == "") | (ui->label_ChatWithName->text () == senderName))
        //        {
        //            qDebug() << "Engter into else...";
        //            ui->label_ChatWithName->setText (senderName);
        //            fmt.setForeground(QBrush(Qt::blue));
        //            ui->plainTextEdit_Chat_Messages->mergeCurrentCharFormat(fmt);
        //            ui->plainTextEdit_Chat_Messages->appendPlainText(sendTitle+strReceiveWithoutName);
        //        }
        //        else
        //        {
        //            qDebug() << "come into if for...";
        //            if(senderNameList.isEmpty ())
        //            {
        //                ChatForm *senderNameChatForm = new ChatForm(0, senderName, senderPeerAddress, senderPeerPort, WindowsUserName);
        //                qDebug() << "new ChatForm already....";
        //                senderNameChatForm->showNormal ();
        //                QTest::qSleep (5000);
        //                emit senderMessages(senderName, senderPeerAddress, strReceiveWithoutName);
        //                senderNameList.append (senderName);
        //                qDebug() << "Emit with chatform out";
        //            }
        //            else
        //            {
        //                for(int i=0; i<senderNameList.size(); i++)
        //                {
        //                    if(senderNameList.at(i) == senderName)
        //                    {
        //                        emit senderMessages(senderNameEmit, senderPeerAddressEmit, strReceiveWithoutName);
        //                        qDebug() << "Emit message out without show chatform";
        //                    }
        //                    else
        //                    {
        //                        ChatForm *senderNameChatForm = new ChatForm(0, senderName, senderPeerAddress, senderPeerPort, WindowsUserName);
        //                        senderNameChatForm->showNormal ();
        //                        QTest::qSleep (5000);
        //                        emit senderMessages(senderName, senderPeerAddress, strReceiveWithoutName);
        //                        senderNameList.append (senderName);
        //                        qDebug() << "Emit with chatform out......";
        //                    }
        //                }
        //            }
        //        }
        // V2.5 code .........................................................................................
    }
}
//void EngineerWindow::on_readyReadEmit()
//{
//    if((ui->label_ChatWithName->text () == "") | (ui->label_ChatWithName->text () == senderName))
//    {
//        qDebug() << "Engter into else...";
//        QTextCharFormat fmt;
//        ui->label_ChatWithName->setText (senderName);
//        fmt.setForeground(QBrush(Qt::blue));
//        ui->plainTextEdit_Chat_Messages->mergeCurrentCharFormat(fmt);
//        ui->plainTextEdit_Chat_Messages->appendPlainText(sendTitle+strReceiveWithoutName);
//    }
//    else
//    {
//        qDebug() << "come into if for...";
//        if(senderNameList.isEmpty ())
//        {
//            ChatForm *senderNameChatForm = new ChatForm(0, senderName, senderPeerAddress, senderPeerPort, WindowsUserName);
//            senderNameChatForm->show ();
//            QTest::qSleep (5000);
//            emit senderMessages(senderName, senderPeerAddress, strReceiveWithoutName);
//            senderNameList.append (senderName);
//            qDebug() << "Emit with chatform out";
//        }
//        else
//        {
//            for(int i=0; i<senderNameList.size(); i++)
//            {
//                if(senderNameList.at(i) == senderName)
//                {
//                    emit senderMessages(senderNameEmit, senderPeerAddressEmit, strReceiveWithoutName);
//                    qDebug() << "Emit message out without show chatform";
//                }
//                else
//                {
//                    ChatForm *senderNameChatForm = new ChatForm(0, senderName, senderPeerAddress, senderPeerPort, WindowsUserName);
//                    senderNameChatForm->show ();
//                    emit senderMessages(senderName, senderPeerAddress, strReceiveWithoutName);
//                    senderNameList.append (senderName);
//                    qDebug() << "Emit with chatform out......";
//                }
//            }
//        }
//    }
//}

void EngineerWindow::on_pushButton_Chat_SendMessage_clicked()
{
    if(ui->plainTextEdit_Send_Messages->document ()->isEmpty ())
    {
        QMessageBox::warning (this, "Remind", "You could not send empty message out!");
        return;
    }
    if(ui->label_ChatWithName->text ()== "")
    {
        QMessageBox::information (this, "Remind","Pls double click on the contact list to choose who are you want to talk first.");
        return;
    }
    //    if(ui->label_ChatWithName->text ()!="" && isClickHelpdeskContact == false)
    //    {
    //        dstIP = peerAddr.toString ();
    //        isClickHelpdeskContact = false;
    //        // qDebug() << "dstIP: " << dstIP;
    //    }
    //    else if(senderPeerAddress.isNull ())
    //    {
    //        dstIP = helpDeskIP; // = ui->lineEdit_SendTo_IP->text ();
    //        isClickHelpdeskContact = false;
    //        // qDebug() << "dstIP: " << helpDeskIP;
    //    }
    //    else
    //    {
    //        dstIP = senderPeerAddress.toString ();
    //        isClickHelpdeskContact = false;
    //    }
    QString chatWithName = ui->label_ChatWithName->text ();
    QHostAddress dstAddr(dstIP);
    //目标端口
    quint16 dstPort = 8686; //= ui->lineEdit_SendTo_Port->text().toInt ();
    QString msg = ui->plainTextEdit_Send_Messages->toPlainText () + "///" + WindowsUserName;
    QByteArray str = msg.toUtf8();
    //发出数据报
    m_udpSocket->writeDatagram(str,dstAddr,dstPort);
    QTextCharFormat fmt;
    //字体色
    fmt.setForeground(QBrush(Qt::darkRed));
    QString msgLocal = ui->plainTextEdit_Send_Messages->toPlainText ();
    ui->plainTextEdit_Chat_Messages->mergeCurrentCharFormat(fmt);
    ui->plainTextEdit_Chat_Messages->appendPlainText("[To " + chatWithName + "-" + QTime::currentTime ().toString () + "]: "+msgLocal);
    ui->plainTextEdit_Send_Messages->clear ();
}


void EngineerWindow::on_tableView_HelpDesk_Contact_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_HelpDesk_Contact->record(index.row ()); //关键步骤
    //update data
    ui->label_ChatWithName->setText (record.value ("employeeName").toString ());
    helpDeskIP = record.value ("ipAddress").toString ();
    dstIP = helpDeskIP;
}



void EngineerWindow::on_actionMonitor_Winodw_triggered()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Chat);
    //on_tableView_HelpDesk_Contact_activated();
}








void EngineerWindow::on_tableView_Cases_Today_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Cases_Today->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->plainTextEdit_Description->clear ();
    ui->plainTextEdit_Solution->clear ();
    ui->plainTextEdit_Attachment->clear ();
    ui->plainTextEdit_Remark->clear ();
    ui->comboBox_Case_Status->clear ();
    ui->comboBox_ITEngineer->clear ();
    initialData_Cases_By_CaseStatus();
    on_comboBox_ITEngineer_NameList();
    //update data
    ui->comboBox_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->plainTextEdit_Description->appendPlainText (record.value("issueDescription").toString ());
    ui->plainTextEdit_Solution->appendPlainText (record.value("solutionDescription").toString ());
    ui->plainTextEdit_Attachment->appendPlainText (record.value("attachedFile").toString ());
    ui->plainTextEdit_Remark->appendPlainText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}

void EngineerWindow::closeEvent (QCloseEvent *)
{
    QApplication* app;
    app->quit ();
}

//Chat module start............................................................................
void EngineerWindow::on_pushButton_Chat_RefreshList_clicked()
{
    ui->lineEdit_Chat_Search->clear ();
    on_tableView_HelpDesk_Contact_activated();
}
void EngineerWindow::comboBox_Chat_UpdateIP()
{
    ui->comboBox_Chat_UpdateIP->clear ();
    localIPAddressUpdateIP = "";
    QList<QHostAddress>listAddress = QNetworkInterface::allAddresses();
    for(int j = 0; j < listAddress.size(); j++)
    {
        if(!listAddress.at(j).isNull() &&
                listAddress.at(j).protocol() ==  QAbstractSocket::IPv4Protocol
                && listAddress.at(j) != QHostAddress::LocalHost)
        {
            localIPAddressUpdateIP = listAddress.at(j).toString();
            ui->comboBox_Chat_UpdateIP->addItem (localIPAddressUpdateIP );
        }
    }
}
void EngineerWindow::on_pushButton_Chat_UpdateIP_clicked()
{
    QSqlQuery sqlQuery;
    QString sqlStr = tr("update ChatList SET ipAddress = '%1', computerName = '%2' where employeeName = '%3'").arg (ui->comboBox_Chat_UpdateIP->currentText ().trimmed ()).arg (localHostName1).arg (WindowsUserName);
    if(sqlQuery.exec (sqlStr))
    {
        on_tableView_HelpDesk_Contact_activated ();
        BindPort ();
        ui->plainTextEdit_Chat_Messages->appendPlainText ("Your new IP address has been updated to server and all users will auto update in 10 minutes.");
    }
    else
    {
        ui->plainTextEdit_Chat_Messages->appendPlainText ("Sorry, update IP address failure, pls contact with IT.");
    }
}
void EngineerWindow::updateIPToMySQL()
{
    QSqlQuery sqlQuery;
    QString sqlStr = tr("update ChatList SET ipAddress = '%1', computerName = '%2' where employeeName = '%3'").arg (ui->comboBox_Chat_UpdateIP->currentText ().trimmed ()).arg (localHostName1).arg (WindowsUserName);
    if(sqlQuery.exec (sqlStr))
    {
        on_tableView_HelpDesk_Contact_activated ();
        ui->plainTextEdit_Chat_Messages->appendPlainText ("Your new IP address has been updated to server and all users will auto update in 10 minutes.");
    }
    else
    {
        on_tableView_HelpDesk_Contact_activated ();
        ui->plainTextEdit_Chat_Messages->appendPlainText ("Sorry, update IP address failure, pls contact with IT.");
    }
}
bool EngineerWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->plainTextEdit_Send_Messages)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)
            {
                on_pushButton_Chat_SendMessage_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}




void EngineerWindow::on_pushButton_Chat_Search_clicked()
{
    if(ui->lineEdit_Chat_Search->text ().isEmpty ())
    {
        QMessageBox::information (this, "Remind", "Pls input search name first!");
        return;
    }
    // model_HelpDesk_Contact  = new QSqlTableModel(this) ;
    model_HelpDesk_Contact = new QSqlTableModel(this);
    //ui->tableView_HelpDesk_Contact->setMaximumWidth (200);
    model_HelpDesk_Contact->setTable("ChatList");
    model_HelpDesk_Contact->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_HelpDesk_Contact->setFilter(tr("employeeName like '%%1%'").arg (ui->lineEdit_Chat_Search->text ().trimmed ()));
    model_HelpDesk_Contact->select();
    ui->tableView_HelpDesk_Contact->setModel(model_HelpDesk_Contact);
    model_HelpDesk_Contact->setHeaderData(1,Qt::Horizontal,"User Name");
    model_HelpDesk_Contact->setHeaderData(2,Qt::Horizontal,"IP Address");
    ui->tableView_HelpDesk_Contact->hideColumn (0);
    ui->tableView_HelpDesk_Contact->hideColumn (5);
    ui->tableView_HelpDesk_Contact->hideColumn (3);
    ui->tableView_HelpDesk_Contact->hideColumn (4);
    ui->tableView_HelpDesk_Contact->hideColumn (6);
    ui->tableView_HelpDesk_Contact->verticalHeader ()->setVisible (false);
    ui->tableView_HelpDesk_Contact->horizontalHeader()->setStyleSheet("QHeaderView::section {"
            "color: blue;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_HelpDesk_Contact->verticalHeader ()->setDefaultAlignment (Qt::AlignVCenter);
    ui->tableView_HelpDesk_Contact->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_HelpDesk_Contact->setEditTriggers (QTableView::NoEditTriggers);
    //ui->tableView_HelpDesk_Contact->sortByColumn (4,Qt::DescendingOrder);
    ui->tableView_HelpDesk_Contact->sortByColumn (1,Qt::AscendingOrder);
}
//Chat module end..............................................................................




void EngineerWindow::on_tableView_HelpDesk_Contact_activated()
{
    // model_HelpDesk_Contact  = new QSqlTableModel(this);
    model_HelpDesk_Contact = new QSqlTableModel(this);
    //ui->tableView_HelpDesk_Contact->setMaximumWidth (200);
    model_HelpDesk_Contact->setTable("ChatList");
    model_HelpDesk_Contact->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // model_HelpDesk_Contact->setFilter("select userName and ipAddress");
    model_HelpDesk_Contact->select();
    ui->tableView_HelpDesk_Contact->setModel(model_HelpDesk_Contact);
    model_HelpDesk_Contact->setHeaderData(1,Qt::Horizontal,"User Name");
    model_HelpDesk_Contact->setHeaderData(2,Qt::Horizontal,"IP Address");
    ui->tableView_HelpDesk_Contact->hideColumn (0);
    ui->tableView_HelpDesk_Contact->hideColumn (5);
    ui->tableView_HelpDesk_Contact->hideColumn (3);
    ui->tableView_HelpDesk_Contact->hideColumn (4);
    ui->tableView_HelpDesk_Contact->hideColumn (6);
    ui->tableView_HelpDesk_Contact->verticalHeader ()->setVisible (false);
    ui->tableView_HelpDesk_Contact->horizontalHeader()->setStyleSheet("QHeaderView::section {"
            "color: blue;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_HelpDesk_Contact->verticalHeader ()->setDefaultAlignment (Qt::AlignVCenter);
    ui->tableView_HelpDesk_Contact->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_HelpDesk_Contact->setEditTriggers (QTableView::NoEditTriggers);
    //ui->tableView_HelpDesk_Contact->sortByColumn (4,Qt::DescendingOrder);
    ui->tableView_HelpDesk_Contact->sortByColumn (1,Qt::AscendingOrder);
}





void EngineerWindow::on_tableView_Cases_Today_activated()
{
    //QDate dateToday = QDate::currentDate ();
    model_Cases_Today  = new QSqlTableModel(this);
    model_Cases_Today->setTable("Ticket");
    model_Cases_Today->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter = tr("date_format(issueCreateTime, '%y-%m-%d') = CURRENT_DATE()");//.arg(dateToday.toString ("yyyy-MM-dd"));
    //QString sFilter = tr("DateDiff(DATE_Format(issueCreateTime, '%Y-%m-%d'),CURRENT_DATE())=0;");//.arg(dateToday.toString ("yyyy-MM-dd"));
    model_Cases_Today->setFilter (sFilter);
    model_Cases_Today->select();
    ui->tableView_Cases_Today->setModel(model_Cases_Today);
    model_Cases_Today->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Cases_Today->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Cases_Today->setHeaderData(2,Qt::Horizontal,"Department");
    model_Cases_Today->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Cases_Today->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Cases_Today->setHeaderData(5,Qt::Horizontal,"Description");
    model_Cases_Today->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Cases_Today->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Cases_Today->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Cases_Today->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Cases_Today->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Cases_Today->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Cases_Today->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_Cases_Today->hideColumn (3);
    ui->tableView_Cases_Today->hideColumn (4);
    ui->tableView_Cases_Today->hideColumn (5);
    ui->tableView_Cases_Today->hideColumn (7);
    ui->tableView_Cases_Today->hideColumn (8);
    ui->tableView_Cases_Today->hideColumn (9);
    ui->tableView_Cases_Today->hideColumn (10);
    ui->tableView_Cases_Today->hideColumn (11);
    ui->tableView_Cases_Today->hideColumn (12);
    ui->tableView_Cases_Today->horizontalHeader()->setStyleSheet("QHeaderView::section {"
            "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    ui->tableView_Cases_Today->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // ui->tableView_AllCases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewAllCases->setColumnWidth(4,359);
    //    ui->tableView_AllCases->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    ui->tableView_AllCases->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->tableView_Cases_Today->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_Cases_Today->verticalHeader ()->setDefaultAlignment (Qt::AlignVCenter);
    ui->tableView_Cases_Today->setEditTriggers (QTableView::NoEditTriggers);
}


void EngineerWindow::on_pushButton_Chat_Refresh_CasesToday_clicked()
{
    on_tableView_Cases_Today_activated();
}




void EngineerWindow::on_actionDonate_to_Support_Super_HelpDesk_triggered()
{
    dialogDonate = new DialogDonate(this);
    dialogDonate->setModal (false);
    dialogDonate->showNormal ();
}

