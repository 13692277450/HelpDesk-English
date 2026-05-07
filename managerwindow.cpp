#include "managerwindow.h"
#include "ui_managerwindow.h"
#include "mainwidget.h"

//#include "dialoglogin.h"



//表示我们要在下面的代码中使用charts组件的命名空间
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
#include <QStringLiteral>
#include <QHBoxLayout>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QXYSeries>
#include <QBarCategoryAxis>

//using namespace QtCharts;

QT_USE_NAMESPACE

ManagerWindow::ManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Icon/bird2.png"));
    mainwidget = (mainWidget *)parentWidget ();
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket,&QUdpSocket::readyRead,this,&ManagerWindow::on_readyRead);
    BindPort();
    qDebug() << "after bindport start....";
    ui->plainTextEdit_Send_Messages->installEventFilter(this);//设置完后自动调用其eventFilter函数
    QSqlQuery sqlQuery;
    QString sqlString = "select * from HelpDesk_Team";
    timerUpdateContactList = new QTimer(this);
    connect (timerUpdateContactList, &QTimer::timeout,this,&ManagerWindow::autoUpdateContactList);
    timerUpdateContactList->start (600000);
    if(!sqlQuery.exec(sqlString))
    {
    }
    else
    {
        Delay_MSec ();
    }
}

ManagerWindow::~ManagerWindow()
{
    delete ui;
}

void ManagerWindow::toDialogSlot()
{
    emit mySignal();
}

void ManagerWindow::get_UserName()
{
    WindowsUserName = qgetenv("USERNAME");
    if (WindowsUserName.isEmpty())
    {
        WindowsUserName = qgetenv("NoName");
    }
    QTest::qSleep (10);
}
void ManagerWindow::get_MachineName()
{
    localHostName1 = QHostInfo::localHostName();
    QTest::qSleep (10);
}
QString ManagerWindow::get_IpAdress()
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


void ManagerWindow:: InitialAllDataManager ()
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
    //on_comboBox_ITEngineer_NameList();
    updateIPToMySQL();
    on_tableView_HelpDesk_Contact_activated();
    on_tableView_Cases_Today_activated();
    initialData_Cases_By_Maintain_Employees();
    qDebug() << "initial all data end....";
}

void ManagerWindow::autoUpdateContactList()
{
    // timerUpdateContactList->stop ();
    ui->lineEdit_Chat_Search->clear ();
    on_tableView_HelpDesk_Contact_activated();
    on_tableView_Cases_Today_activated();
    qDebug() << "auto refresh contact list and cases today done....";
    // timerUpdateContactList->start (60000);
}

void ManagerWindow::Delay_MSec() //unsigned int msec
{
    ChartData_All_Cases();
    ChartData_Engineer_HoldCases();
    ChartData_Sort_Cases_Type();
    ChartData_OPenCases();
    ChartData_All_Cases_By_Dept();
    ChartData_Monthly_Cases ();
    ChartData_Last_Month_Cases();
    ChartData_Yearly_By_Department_Cases();
    initialData_Cases_By_Engineer();
    initialData_Cases_By_CaseStatus();
    on_comboBox_By_Engineer_activated();
    on_comboBox_By_Cases_Status_activated();
    on_dateEdit_Start_Date_dateTimeChanged();
    on_dateEdit_End_Date_dateTimeChanged();
}


void ManagerWindow::ChartData_OPenCases ()
{
    QPieSeries* pie = new QPieSeries();
    QSqlQuery sqlQuery;
    QString sqlString = "select itEngineer, count(*) from Ticket where issueStatus = 'Open' group by itEngineer";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        QString nameValue = field1.value ().toString () + " (" + field2.value ().toString () + ")";
        pie->append (nameValue, field2.value ().toInt ());
        // pie->append (field1.value ().toString (), field2.value ().toInt ());
    }
    //使用append初始化该饼图系列
    QPieSlice* slice = new QPieSlice;
    slice = pie->slices().at(1);    //得到系列pie中索引为1的切片
    slice->LabelInsideHorizontal;
    slice->setExploded();   //将该切片与饼图分离
    slice->setLabelVisible(true);   //设置标签可见
    slice->setPen(QPen(Qt::darkGreen, 2));  //设置该切片的pen属性, 即绘制该切片边框的钢笔
    slice->setBrush(Qt::green); //设置该切片的brush属性, 即填充切片的画笔颜色
    //创建QChart对象管理pie系列
    QChart* chart = new QChart();
    chart->addSeries(pie);
    QChart::AllAnimations;
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("Open Cases Distribute By IT Engineer");
    chart->legend ()->setAlignment (Qt::AlignLeft);
    ui->graphicsView_Engineer_OpenCases->setChart(chart);
    ui->graphicsView_Engineer_OpenCases->setRenderHint(QPainter::Antialiasing);
}


void ManagerWindow::ChartData_Engineer_HoldCases()
{
    QPieSeries* pie = new QPieSeries();
    QSqlQuery sqlQuery;
    QString sqlString = "select itEngineer, count(*) from Ticket where issueStatus = 'Hold' group by itEngineer";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        QString nameValue = field1.value ().toString () + " (" + field2.value ().toString () + ")";
        pie->append (nameValue, field2.value ().toInt ());
        // pie->append (field1.value ().toString (), field2.value ().toInt ());
    }
    //使用append初始化该饼图系列
    QPieSlice* slice = new QPieSlice;
    slice = pie->slices().at(1);    //得到系列pie中索引为1的切片
    slice->LabelInsideHorizontal;
    slice->setExploded();   //将该切片与饼图分离
    slice->setLabelVisible(true);   //设置标签可见
    slice->setPen(QPen(Qt::darkGreen, 2));  //设置该切片的pen属性, 即绘制该切片边框的钢笔
    slice->setBrush(Qt::yellow); //设置该切片的brush属性, 即填充切片的画笔颜色
    //创建QChart对象管理pie系列
    QChart* chart = new QChart();
    chart->addSeries(pie);
    QChart::AllAnimations;
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("Hold Cases Distribute By IT Engineer");
    chart->legend ()->setAlignment (Qt::AlignLeft);
    ui->graphicsView_Engineer_HoldCases->setChart(chart);
    ui->graphicsView_Engineer_HoldCases->setRenderHint(QPainter::Antialiasing);
}

void ManagerWindow::ChartData_All_Cases()
{
    QPieSeries* pie = new QPieSeries();
    QSqlQuery sqlQuery;
    QString sqlString = "select itEngineer, count(*) from Ticket group by itEngineer";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        QString nameValue = field1.value ().toString () + " (" + field2.value ().toString () + ")";
        pie->append (nameValue, field2.value ().toInt ());
        // pie->append (field1.value ().toString (), field2.value ().toInt ());
    }
    //使用append初始化该饼图系列
    QPieSlice* slice = new QPieSlice;
    slice = pie->slices().at(1);    //得到系列pie中索引为1的切片
    slice->LabelInsideHorizontal;
    slice->setExploded();   //将该切片与饼图分离
    slice->setLabelVisible(true);   //设置标签可见
    slice->setPen(QPen(Qt::darkGreen, 2));  //设置该切片的pen属性, 即绘制该切片边框的钢笔
    slice->setBrush(Qt::red); //设置该切片的brush属性, 即填充切片的画笔颜色
    //创建QChart对象管理pie系列
    QChart* chart = new QChart();
    chart->createDefaultAxes();
    chart->addSeries(pie);
    qDebug() << "Chart data pie all cases...";
    QChart::AllAnimations;
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("All Cases Distribute By IT Engineer");
    chart->legend ()->setAlignment (Qt::AlignLeft);
    ui->graphicsView_All_Cases->setChart(chart);
    ui->graphicsView_All_Cases->setRenderHint(QPainter::Antialiasing);
}

void ManagerWindow::ChartData_Sort_Cases_Type()
{
    QPieSeries* pie = new QPieSeries();
    QSqlQuery sqlQuery;
    QString sqlString = "select issueStatus, count(*) from Ticket group by issueStatus";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        QString nameValue = field1.value ().toString () + " (" + field2.value ().toString () + ")";
        pie->append (nameValue, field2.value ().toInt ());
    }
    //使用append初始化该饼图系列
    QPieSlice* slice = new QPieSlice;
    slice = pie->slices().at(1);    //得到系列pie中索引为1的切片
    slice->LabelInsideHorizontal;
    slice->setExploded();   //将该切片与饼图分离
    slice->setLabelVisible(true);   //设置标签可见
    slice->setPen(QPen(Qt::darkGreen, 2));  //设置该切片的pen属性, 即绘制该切片边框的钢笔
    slice->setBrush(Qt::blue); //设置该切片的brush属性, 即填充切片的画笔颜色
    //创建QChart对象管理pie系列
    QChart* chart = new QChart();
    chart->createDefaultAxes();
    qDebug() << "Sort cases....";
    chart->addSeries(pie);
    QChart::AllAnimations;
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("Sort All Cases By Case Status");
    chart->legend ()->setAlignment (Qt::AlignLeft);
    ui->graphicsView_Sort_Cases_Type->setChart(chart);
    ui->graphicsView_Sort_Cases_Type->setRenderHint(QPainter::Antialiasing);
}

void ManagerWindow::ChartData_All_Cases_By_Dept()
{
    QBarSet* set = new QBarSet("1");
    QBarSeries* series = new QBarSeries();
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QChart* chart = new QChart();
    QSqlQuery sqlQuery;
    QString sqlString = "select employeeDepartment, count(*) from Ticket where issueStatus = 'Open' or issueStatus = 'Hold' group by employeeDepartment";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        int setValue = field2.value ().toInt ();
        QString setName = field1.value ().toString ();
        set->append (setValue);
        axisX->append (setName);
        set->setLabelColor (Qt::red);
    }
    axisX->setLabelsAngle (-90);
    QFont font("Times", 9, QFont::Normal);
    //QFont font = axisX->labelsFont();
    font.setBold(false);
    font.setPointSize(9);
    axisX->setLabelsFont(font);
    series->append (set);
    series->setVisible (true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsInsideBase);
    series->setLabelsVisible(true);
    series->attachAxis(axisX);
    chart->addSeries(series);
    qDebug() << "Series by department";
    chart->legend ()->markers ().at (0)->setVisible(false);
    chart->createDefaultAxes();
    chart->addAxis(axisX, Qt::AlignBottom);
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("All Open And Hold Cases By Department");
    ui->graphicsView_All_Cases_By_Dept->setChart(chart);
    ui->graphicsView_All_Cases_By_Dept->setRenderHint(QPainter::Antialiasing);
}


void ManagerWindow::ChartData_Monthly_Cases()
{
    //设置标题
    QBarSet* set = new QBarSet("2");
    QBarSeries* series = new QBarSeries();
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QChart* chart = new QChart();
    QSqlQuery sqlQuery;
    QString sqlString = "select DATE_FORMAT(issueCreateTime,'%Y-%m') as Value, COUNT(*) as Total from Ticket where issueCreateTime > DATE_SUB(NOW(), INTERVAL 12 MONTH) group by DATE_FORMAT(issueCreateTime,'%Y-%m') order by DATE_FORMAT(issueCreateTime,'%Y-%m');" ;
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        int setValue = field2.value ().toInt ();
        QString setName = field1.value ().toString ();
        set->append (setValue);
        axisX->append (setName);
        set->setLabelColor (Qt::red);
    }
    axisX->setLabelsAngle (-90);
    QFont font = axisX->labelsFont();
    font.setBold(false);
    font.setPointSize(9);
    axisX->setLabelsFont(font);
    series->append (set);
    series->setVisible (true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsInsideBase);
    series->setLabelsVisible(true);
    series->attachAxis(axisX);
    chart->addSeries(series);
    chart->legend ()->markers ().at (0)->setVisible(false);
    chart->createDefaultAxes();
    chart->addAxis(axisX, Qt::AlignBottom);
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("All Cases Distribute In The Past 12 Months");
    ui->graphicsView_Monthly_Cases->setChart(chart);
    ui->graphicsView_Monthly_Cases->setRenderHint(QPainter::Antialiasing);
}

void ManagerWindow::ChartData_Last_Month_Cases()
{
    QBarSet* set = new QBarSet("3");
    QBarSeries* series = new QBarSeries();
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QChart* chart = new QChart();
    QSqlQuery sqlQuery;
    QString sqlString = "select employeeDepartment, COUNT(*) as Total from Ticket where issueCreateTime > DATE_SUB(NOW(), INTERVAL 1 MONTH) group by employeeDepartment";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        int setValue = field2.value ().toInt ();
        QString setName = field1.value ().toString ();
        set->append (setValue);
        axisX->append (setName);
        set->setLabelColor (Qt::red);
    }
    axisX->setLabelsAngle (-90);
    QFont font = axisX->labelsFont();
    font.setBold(false);
    font.setPointSize(9);
    axisX->setLabelsFont(font);
    series->append (set);
    series->setVisible (true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsInsideBase);
    series->setLabelsVisible(true);
    series->attachAxis(axisX);
    chart->addSeries(series);
    chart->legend ()->markers ().at (0)->setVisible(false);
    chart->createDefaultAxes();
    chart->addAxis(axisX, Qt::AlignBottom);
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("All Cases In The Past One Month By Department");
    ui->graphicsView_Last_Month_Cases->setChart(chart);
    ui->graphicsView_Last_Month_Cases->setRenderHint(QPainter::Antialiasing);
}


void ManagerWindow::ChartData_Yearly_By_Department_Cases()
{
    QBarSet* set = new QBarSet("4");
    QBarSeries* series = new QBarSeries();
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QChart* chart = new QChart();
    QSqlQuery sqlQuery;
    QString sqlString = "select employeeDepartment, COUNT(*) as Total from Ticket where issueCreateTime > DATE_SUB(NOW(), INTERVAL 12 MONTH) group by employeeDepartment";
    sqlQuery.exec (sqlString);
    while(sqlQuery.next ())
    {
        QSqlRecord rec = sqlQuery.record ();
        QSqlField field1 = rec.field (0);
        QSqlField field2 = rec.field (1);
        int setValue = field2.value ().toInt ();
        QString setName = field1.value ().toString ();
        set->append (setValue);
        axisX->append (setName);
        set->setLabelColor (Qt::red);
    }
    axisX->setLabelsAngle (-90);
    QFont font = axisX->labelsFont();
    font.setBold(false);
    font.setPointSize(9);
    axisX->setLabelsFont(font);
    series->append (set);
    series->setVisible (true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsInsideBase);
    series->setLabelsVisible(true);
    series->attachAxis(axisX);
    chart->addSeries(series);
    chart->legend ()->markers ().at (0)->setVisible(false);
    chart->createDefaultAxes();
    chart->addAxis(axisX, Qt::AlignBottom);
    //设置图表标题
    QFont serifFont("Times", 10, QFont::Bold);
    chart->setFont (serifFont);
    chart->setTitleFont (serifFont);
    chart->setTitle("All Cases In The Past 12 Months By Department");
    ui->graphicsView_Yearly_Cases ->setChart(chart);
    ui->graphicsView_Yearly_Cases->setRenderHint(QPainter::Antialiasing);
}

void ManagerWindow::on_actionAll_Cases_triggered()
{
    model_Manager_Action_All_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_Cases->setTable("Ticket");
    model_Manager_Action_All_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Manager_Action_All_Cases->select();
    ui->AllCasestableView->setModel(model_Manager_Action_All_Cases);
    model_Manager_Action_All_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->AllCasestableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_Cases);
}





void ManagerWindow::on_actionAll_Open_Cases_triggered()
{
    model_Manager_Action_All_Open_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_Open_Cases->setTable("Ticket");
    model_Manager_Action_All_Open_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Manager_Action_All_Open_Cases->setFilter(("issueStatus = 'Open'"));// % (room_id)));
    model_Manager_Action_All_Open_Cases->select();
    ui->tableView_All_Open_Cases->setModel(model_Manager_Action_All_Open_Cases);
    model_Manager_Action_All_Open_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_Open_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_Open_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_Open_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_Open_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_Open_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_Open_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_Open_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_Open_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_Open_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_Open_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_Open_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_Open_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_Open_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_Open_Cases);
}


void ManagerWindow::on_actionAll_Closed_Cases_triggered()
{
    model_Manager_Action_All_Closed_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_Closed_Cases->setTable("Ticket");
    model_Manager_Action_All_Closed_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Manager_Action_All_Closed_Cases->setFilter(("issueStatus = 'Closed'"));// % (room_id)));
    model_Manager_Action_All_Closed_Cases->select();
    ui->tableView_All_Closed_Cases->setModel(model_Manager_Action_All_Closed_Cases);
    model_Manager_Action_All_Closed_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_Closed_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_Closed_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_Closed_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_Closed_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_Closed_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_Closed_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_Closed_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_Closed_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_Closed_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_Closed_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_Closed_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_Closed_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_Closed_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_Closed_Cases);
}


void ManagerWindow::on_actionAll_Hold_Cases_triggered()
{
    model_Manager_Action_All_Hold_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_Hold_Cases->setTable("Ticket");
    model_Manager_Action_All_Hold_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Manager_Action_All_Hold_Cases->setFilter(("issueStatus = 'Hold'"));// % (room_id)));
    model_Manager_Action_All_Hold_Cases->select();
    ui->tableView_All_Hold_Cases->setModel(model_Manager_Action_All_Hold_Cases);
    model_Manager_Action_All_Hold_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_Hold_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_Hold_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_Hold_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_Hold_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_Hold_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_Hold_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_Hold_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_Hold_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_Hold_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_Hold_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_Hold_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_Hold_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_Hold_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_Hold_Cases);
}



void ManagerWindow::initialData_Cases_By_Engineer()
{
    ui->comboBox_By_Engineer->clear ();
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_By_Engineer->addItem(Engineer_Name);
    }
}


void ManagerWindow::initialData_Cases_By_CaseStatus()
{
    ui->comboBox_By_Cases_Status->clear ();
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_By_Cases_Status->addItem(issueStatus_Name);
    }
}

void ManagerWindow::on_actionCases_By_Engineer_triggered()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Cases_By_Engineer);
    initialData_Cases_By_CaseStatus();
    initialData_Cases_By_Engineer();
}

void ManagerWindow::on_comboBox_By_Engineer_activated()
{
    sortByEngineerName = ui->comboBox_By_Engineer->currentText ().trimmed ();
}


void ManagerWindow::on_comboBox_By_Cases_Status_activated()
{
    sortByIssueType = ui->comboBox_By_Cases_Status->currentText ().trimmed ();
}
//按时间段和工程师名字，Case状态来检索数据
void ManagerWindow::on_pushButton_Search_Cases_clicked()
{
    //check_SqlServer_Connection();
    ui->tableView_SearchByEngineer->clearSelection ();
    model_Manager_Search_By_Engineer_Cases  = new QSqlTableModel(this);
    model_Manager_Search_By_Engineer_Cases->setTable("Ticket");
    model_Manager_Search_By_Engineer_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QDateTime origin_time = QDateTime::fromString("2000-01-01 08:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime startDatetime = ui->dateEdit_Start_Date->dateTime();
    QDateTime endDatetime = ui->dateEdit_End_Date->dateTime();
    qint64 nStartSteps = origin_time.msecsTo(startDatetime);
    qint64 nEndSteps = origin_time.msecsTo(endDatetime);
    if(nStartSteps>nEndSteps)
    {
        QMessageBox mesg;
        mesg.warning(this,"Error","Your start time is later than end time!");
        return;
    }
    QString sFilter = "";//数据库查询语句
    sFilter += tr("issueCreateTime between '\%1\' AND '\%2\'").arg(startDatetime.toString ("yyyy-MM-dd hh:mm:ss")).arg(endDatetime.toString ("yyyy-MM-dd hh:mm:ss"));
    if(ui->comboBox_By_Engineer->currentIndex ()!= -1)
    {
        sFilter +=" AND itEngineer=\'" + ui->comboBox_By_Engineer->currentText() + "\'";
    }
    if (ui->comboBox_By_Cases_Status->currentIndex ()!= -1)
    {
        sFilter +=" AND issueStatus=\'" + ui->comboBox_By_Cases_Status->currentText() + "\'";
    }
    model_Manager_Search_By_Engineer_Cases->setFilter(sFilter);    //查询数据库
    model_Manager_Search_By_Engineer_Cases->select();//选中数据库中所有条目
    ui->tableView_SearchByEngineer->setModel(model_Manager_Search_By_Engineer_Cases);
    model_Manager_Search_By_Engineer_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Search_By_Engineer_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_SearchByEngineer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_Cases_By_Engineer);
}



void ManagerWindow::on_dateEdit_Start_Date_dateTimeChanged() //const QDateTime &dateTime
{
    //sortByEndDate = ui->dateEdit_Start_Date->dateTime ();
}


void ManagerWindow::on_dateEdit_End_Date_dateTimeChanged()
{
    //sortByStartDate = ui->dateEdit_End_Date->dateTime ();
}

void ManagerWindow::check_SqlServer_Connection()
{
}

void ManagerWindow::on_actionHelp_triggered()
{
    dialogHelp = new DialogHelp(this);
    dialogHelp->setModal (false);
    dialogHelp->showNormal ();
}


void ManagerWindow::on_actionAbout_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionPavoGroup_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, the PavoGroup is an international company, software develpment is one of it's core business, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionKnowledge_Library_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionKnowledge_Library_Settings_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionDatabase_Backup_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionHelpDesk_Support_Report_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionCase_Stastics_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionExport_Support_triggered()
{
    QMessageBox::information (this, "Information  Super HelpDesk Ver2.0", "Super HelpDesk software is published by PavoGroup, this is free version, this fuction is only available in professional version, more information, pls visit www.pavogroup.top.");
}


void ManagerWindow::on_actionExit_triggered()
{
    exit(9);
}


void ManagerWindow::on_actionConnect_Database_triggered()
{
    mainWidget().connectDB ();
}


void ManagerWindow::on_actionAll_My_Cases_triggered()
{
    model_Manager_Action_All_My_Cases = new QSqlTableModel(this);
    model_Manager_Action_All_My_Cases->setTable("Ticket");
    model_Manager_Action_All_My_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter ="";
    sFilter += QString("itEngineer = '%1'").arg (loginUsernameManager);
    model_Manager_Action_All_My_Cases->setFilter (sFilter);
    model_Manager_Action_All_My_Cases->select();
    ui->tableView_All_My_Cases->setModel(model_Manager_Action_All_My_Cases);
    model_Manager_Action_All_My_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_My_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_My_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_My_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_My_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_My_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_My_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_My_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_My_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_My_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_My_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_My_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_My_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_My_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_My_Cases);
}


void ManagerWindow::on_actionAll_My_Open_Hold_Cases_triggered()
{
    model_Manager_Action_All_My_OpenHold_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_My_OpenHold_Cases->setTable("Ticket");
    model_Manager_Action_All_My_OpenHold_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter = "";
    sFilter = QString("(issueStatus = 'Hold' or issueStatus = 'Open') and itEngineer = '%1'").arg (loginUsernameManager);
    model_Manager_Action_All_My_OpenHold_Cases->setFilter (sFilter);
    model_Manager_Action_All_My_OpenHold_Cases->select();
    ui->tableView_All_My_OpenHold_Cases->setModel(model_Manager_Action_All_My_OpenHold_Cases);
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(3,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(4,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(6,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_My_OpenHold_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_My_OpenHold_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_My_OpenHold_Cases);
}


void ManagerWindow::on_actionAll_My_Closed_Cases_triggered()
{
    model_Manager_Action_All_My_Closed_Cases  = new QSqlTableModel(this);
    model_Manager_Action_All_My_Closed_Cases->setTable("Ticket");
    model_Manager_Action_All_My_Closed_Cases->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sFilter = "";//数据库查询语句
    sFilter += tr("issueStatus = 'Closed' and itEngineer = '%1'").arg(loginUsernameManager);
    model_Manager_Action_All_My_Closed_Cases->setFilter(sFilter);
    model_Manager_Action_All_My_Closed_Cases->select();
    ui->tableView_All_My_Closed_Cases->setModel(model_Manager_Action_All_My_Closed_Cases);
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(0,Qt::Horizontal,"ID Num");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(1,Qt::Horizontal,"Employee Name");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(2,Qt::Horizontal,"Department");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(3,Qt::Horizontal,"Create Time");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(4,Qt::Horizontal,"Case Type");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(5,Qt::Horizontal,"Description");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(6,Qt::Horizontal,"Case Level");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(7,Qt::Horizontal,"Case Status");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(8,Qt::Horizontal,"Solution");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(9,Qt::Horizontal,"Solved Time");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(10,Qt::Horizontal,"Attachment");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(11,Qt::Horizontal,"HelpDesk");
    model_Manager_Action_All_My_Closed_Cases->setHeaderData(12,Qt::Horizontal,"Remark");
    ui->tableView_All_My_Closed_Cases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget (ui->page_All_My_Closed_Cases);
}


void ManagerWindow::on_actionMaintain_Employees_triggered()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Maintain_Employees);
    ui->comboBox_Add_IsActive->clear ();
    ui->comboBox_Add_LoginTitle->clear ();
    ui->comboBox_Update_IsActive->clear ();
    ui->comboBox_Update_LoginTitle->clear ();
    ui->comboBox_Add_LoginTitle->addItem ("Engineer");
    ui->comboBox_Add_LoginTitle->addItem ("Manager");
    ui->comboBox_Add_IsActive->addItem ("0");
    ui->comboBox_Add_IsActive->addItem ("1");
    ui->comboBox_Update_LoginTitle->addItem ("Engineer");
    ui->comboBox_Update_LoginTitle->addItem ("Manager");
    ui->comboBox_Update_IsActive->addItem ("0");
    ui->comboBox_Update_IsActive->addItem ("1");
    initialData_Cases_By_Maintain_itEngineer();
}


//Maintain employees start at here.........................................................................................

void ManagerWindow::on_pushButton_Add_User_clicked()
{
    QString userName = ui->lineEdit_Add_UserName->text ().trimmed ();
    QString password = ui->lineEdit_Add_Password->text ().trimmed ();
    if(userName.isEmpty ()|| password.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The user name and password can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from HelpDesk_Team where userName =:userName");
    sqlQuery.bindValue (":userName", userName);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The user name is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into HelpDesk_Team(userName, password, roleTitle, isActive)" "values(:userName, :password, :roleTitle, :isActive)");
        sqlQuery.bindValue (":userName", userName);
        sqlQuery.bindValue (":password", password);
        sqlQuery.bindValue (":roleTitle", ui->comboBox_Add_LoginTitle->currentText ().trimmed ());
        sqlQuery.bindValue (":isActive", ui->comboBox_Add_IsActive->currentText ().toInt ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new user account " + userName + " !");
            ui->lineEdit_Add_Password->clear ();
            ui->lineEdit_Add_UserName->clear ();
            initialData_Cases_By_Maintain_itEngineer();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new account, pls check database server!");
        }
    }
}


void ManagerWindow::initialData_Cases_By_Maintain_itEngineer()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    ui->comboBox_Delete_UserName->clear ();
    ui->comboBox_Update_UserName->clear ();
    ui->comboBox_Check_UserName->clear ();
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Delete_UserName->addItem(Engineer_Name);
        ui->comboBox_Update_UserName->addItem(Engineer_Name);
        ui->comboBox_Check_UserName->addItem(Engineer_Name);
    }
}

void ManagerWindow::on_comboBox_Delete_UserName_currentIndexChanged(int index)
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Delete_UserName->currentText ().trimmed ());
    queryEngineer.exec (queryEngineerStr);
    do
    {
        QSqlRecord record = queryEngineer.record ();
        QString password = record.value ("password").toString ().trimmed ();
        QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
        QString isActive = record.value ("isActive").toString ().trimmed ();
        ui->lineEdit_Delete_Password->setText (password);
        ui->lineEdit_Delete_LoginTitle->setText (roleTitle);
        ui->lineEdit_Delete_isActive->setText (isActive);
    }
    while(queryEngineer.next ());
}


void ManagerWindow::on_pushButton_Delete_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Delete_UserName->currentText ().trimmed ());
    if(ui->comboBox_Delete_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose user account first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this account ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This account was deleted successful!");
                initialData_Cases_By_Maintain_itEngineer();
                ui->lineEdit_Delete_Password->clear ();
                ui->lineEdit_Delete_LoginTitle->clear ();
                ui->lineEdit_Delete_isActive->clear ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this account, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void ManagerWindow::on_comboBox_Update_UserName_activated(int index)
{
    if(ui->comboBox_Check_UserName->currentIndex () != -1)
    {
        QSqlQuery queryEngineer;
        QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Update_UserName->currentText ().trimmed ());
        queryEngineer.exec (queryEngineerStr);
        if(queryEngineer.next ())
        {
            QSqlRecord record = queryEngineer.record ();
            QString password = record.value ("password").toString ().trimmed ();
            QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
            QString isActive = record.value ("isActive").toString ().trimmed ();
            ui->lineEdit_Update_Password->setText (password);
            ui->comboBox_Update_LoginTitle->setCurrentText (roleTitle);
            ui->comboBox_Update_IsActive->setCurrentText (isActive);
        }
    }
}


void ManagerWindow::on_comboBox_Check_UserName_activated(int index)
{
    if(ui->comboBox_Check_UserName->currentIndex () != -1)
    {
        QSqlQuery queryEngineer;
        QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Check_UserName->currentText ().trimmed ());
        queryEngineer.exec (queryEngineerStr);
        if(queryEngineer.next ())
        {
            QSqlRecord record = queryEngineer.record ();
            QString password = record.value ("password").toString ().trimmed ();
            QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
            QString isActive = record.value ("isActive").toString ().trimmed ();
            ui->lineEdit_Check_Password->setText (password);
            ui->lineEdit_Check_LoginTitle->setText (roleTitle);
            ui->lineEdit_Check_isActive->setText (isActive);
        }
    }
}


void ManagerWindow::on_pushButton_Check_clicked()
{
    if(ui->comboBox_Check_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Remind", "Pls select user account first!");
        return;
    }
    else
    {
        QSqlQuery queryEngineer;
        QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Check_UserName->currentText ().trimmed ());
        queryEngineer.exec (queryEngineerStr);
        if(queryEngineer.next ())
        {
            QSqlRecord record = queryEngineer.record ();
            QString password = record.value ("password").toString ().trimmed ();
            QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
            QString isActive = record.value ("isActive").toString ().trimmed ();
            ui->lineEdit_Check_Password->setEchoMode (QLineEdit::Normal);
            ui->lineEdit_Check_Password->setText (password);
            ui->lineEdit_Check_LoginTitle->setText (roleTitle);
            ui->lineEdit_Check_isActive->setText (isActive);
        }
    }
}
void ManagerWindow::on_pushButton_Update_clicked()
{
    QString strPassword = ui->lineEdit_Update_Password->text ().trimmed ();
    QString strLoginTitle = ui->comboBox_Update_LoginTitle->currentText ().trimmed ();
    QString strisActive = ui->comboBox_Update_IsActive->currentText ().trimmed ();
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("update HelpDesk.HelpDesk_Team SET password = '%1',roleTitle = '%2',isActive = '%3' where userName = '%4'").arg (strPassword, strLoginTitle, strisActive, ui->comboBox_Update_UserName->currentText ().trimmed ());
    if(ui->comboBox_Update_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose user account first!");
        return;
    }
    else
    {
        if(queryEngineer.exec (queryEngineerStr))
        {
            QMessageBox::information (this, "Information", "This account was updated successful!");
            initialData_Cases_By_Maintain_itEngineer();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to update this account, pls check your server settings.");
            return;
        }
    }
}

void ManagerWindow::on_comboBox_Check_UserName_currentIndexChanged(int index)
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Check_UserName->currentText ().trimmed ());
    queryEngineer.exec (queryEngineerStr);
    do
    {
        QSqlRecord record = queryEngineer.record ();
        QString password = record.value ("password").toString ().trimmed ();
        QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
        QString isActive = record.value ("isActive").toString ().trimmed ();
        ui->lineEdit_Check_Password->setText (password);
        ui->lineEdit_Check_LoginTitle->setText (roleTitle);
        ui->lineEdit_Check_isActive->setText (isActive);
    }
    while(queryEngineer.next ());
}


void ManagerWindow::on_comboBox_Update_UserName_currentIndexChanged(int index)
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("select * from HelpDesk_Team where userName = '%1'").arg (ui->comboBox_Update_UserName->currentText ().trimmed ());
    queryEngineer.exec (queryEngineerStr);
    do
    {
        QSqlRecord record = queryEngineer.record ();
        QString password = record.value ("password").toString ().trimmed ();
        QString roleTitle = record.value ("roleTitle").toString ().trimmed ();
        QString isActive = record.value ("isActive").toString ().trimmed ();
        ui->lineEdit_Update_Password->setText (password);
        ui->comboBox_Update_LoginTitle->setCurrentText (roleTitle);
        ui->comboBox_Update_IsActive->setCurrentText (isActive);
    }
    while(queryEngineer.next ());
}

//Maintain Employee for Employeess start here..............................................

void ManagerWindow::initialData_Cases_By_Maintain_Employees()
{
    QSqlQuery queryEmployee;
    QString queryEmployeeStr = "select * from Employee";
    queryEmployee.exec (queryEmployeeStr);
    QSqlQuery queryTitle;
    QString queryTitleStr = "select * from Title";
    queryTitle.exec (queryTitleStr);
    QSqlQuery queryLevel;
    QString queryLevelStr = "select * from Level";
    queryLevel.exec (queryLevelStr);
    QSqlQuery queryDepartment;
    QString queryDepartmentStr = "select * from Department";
    queryDepartment.exec (queryDepartmentStr);
    ui->comboBox_Maintain_Employee_Add_Department->clear ();
    ui->comboBox_Maintain_Employee_Add_Level->clear ();
    ui->comboBox_Maintain_Employee_Add_Title->clear ();
    ui->comboBox_Maintain_Employee_Update_Department->clear ();
    ui->comboBox_Maintain_Employee_Update_Level->clear ();
    ui->comboBox_Maintain_Employee_Update_Title->clear ();
    ui->comboBox_Maintain_Employee_Delete_UserName->clear ();
    ui->comboBox_Maintain_Employee_Update_UserName->clear ();
    ui->comboBox_Maintain_Employee_Checkout_UserName->clear ();
    while(queryEmployee.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Employee_Name=QString("%1").arg(queryEmployee.value("employeeName").toString());
        ui->comboBox_Maintain_Employee_Delete_UserName->addItem(Employee_Name);
        ui->comboBox_Maintain_Employee_Update_UserName->addItem(Employee_Name);
        ui->comboBox_Maintain_Employee_Checkout_UserName->addItem(Employee_Name);
    }
    while(queryTitle.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Employee_Title=QString("%1").arg(queryTitle.value("employeeTitle").toString());
        ui->comboBox_Maintain_Employee_Add_Title->addItem(Employee_Title);
        ui->comboBox_Maintain_Employee_Update_Title->addItem(Employee_Title);
    }
    while(queryLevel.next())
    {
        //把 Employee 表中的 data 载入到下拉框
        QString Employee_Level=QString("%1").arg(queryLevel.value("employeeLevel").toString());
        ui->comboBox_Maintain_Employee_Add_Level->addItem(Employee_Level);
        ui->comboBox_Maintain_Employee_Update_Level->addItem(Employee_Level);
    }
    while(queryDepartment.next())
    {
        //把 Employee 表中的 data 载入到下拉框
        QString Employee_Department=QString("%1").arg(queryDepartment.value("departmentName").toString());
        ui->comboBox_Maintain_Employee_Add_Department->addItem(Employee_Department);
        ui->comboBox_Maintain_Employee_Update_Department->addItem(Employee_Department);
    }
}

void ManagerWindow::on_pushButton_Maintain_Empoyee_Add_User_clicked()
{
    QString employeeName = ui->lineEdit_Maintain_Employee_Add_EmplyeeName->text ().trimmed ();
    if(employeeName.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The employee name can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from Employee where employeeName =:employeeName");
    sqlQuery.bindValue (":employeeName", employeeName);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The employee name is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into Employee(employeeName, workNumber, employeeDepartment, employeeLevel, employeeTitle)" "values(:employeeName, :workNumber, :employeeDepartment, :employeeLevel, :employeeTitle)");
        sqlQuery.bindValue (":employeeName", ui->lineEdit_Maintain_Employee_Add_EmplyeeName->text ().trimmed ());
        sqlQuery.bindValue (":workNumber", ui->lineEdit_Maintain_Employee_Add_WorkNumber->text ().trimmed ());
        sqlQuery.bindValue (":employeeDepartment", ui->comboBox_Maintain_Employee_Add_Department->currentText ().trimmed ());
        sqlQuery.bindValue (":employeeLevel", ui->comboBox_Maintain_Employee_Add_Level->currentText ().trimmed ());
        sqlQuery.bindValue (":employeeTitle", ui->comboBox_Maintain_Employee_Add_Title->currentText ().trimmed ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new employee account " + employeeName + " !");
            ui->lineEdit_Maintain_Employee_Add_EmplyeeName->clear ();
            ui->lineEdit_Maintain_Employee_Add_WorkNumber->clear ();
            initialData_Cases_By_Maintain_Employees();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new account, pls check database server!");
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Employee_Delete_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from Employee where employeeName = '%1'").arg (ui->comboBox_Maintain_Employee_Delete_UserName->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Employee_Delete_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose user account first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this account ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This account was deleted successful!");
                initialData_Cases_By_Maintain_Employees ();
                ui->lineEdit_Maintain_Employee_Delete_Department->clear ();
                ui->lineEdit_Maintain_Employee_Delete_Title->clear ();
                ui->lineEdit_Maintain_Employee_Delete_Level->clear ();
                ui->lineEdit_Maintain_Employee_Delete_WorkNumber->clear ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this account, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Empoyee_Update_clicked()
{
    QString workNumber = ui->lineEdit_Maintain_Employee_Update_WorkNumber->text ().trimmed ();
    QString employeeDepartment = ui->comboBox_Maintain_Employee_Update_Department->currentText ().trimmed ();
    QString employeeLevel = ui->comboBox_Maintain_Employee_Update_Level->currentText ().trimmed ();
    QString employeeTitle = ui->comboBox_Maintain_Employee_Update_Title->currentText ().trimmed ();
    ui->lineEdit_Maintain_Employee_Update_WorkNumber->setText (workNumber);
    ui->comboBox_Maintain_Employee_Update_Department->setCurrentText (employeeDepartment);
    ui->comboBox_Maintain_Employee_Update_Title->setCurrentText (employeeTitle);
    ui->comboBox_Maintain_Employee_Update_Level->setCurrentText (employeeLevel);
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("update Employee SET workNumber = '%1',employeeTitle = '%2',employeeLevel = '%3', employeeDepartment = '%4' where employeeName = '%5'").arg (workNumber, employeeTitle, employeeLevel, employeeDepartment, ui->comboBox_Maintain_Employee_Update_UserName->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Employee_Update_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose user account first!");
        return;
    }
    else
    {
        if(queryEngineer.exec (queryEngineerStr))
        {
            QMessageBox::information (this, "Information", "This account was updated successful!");
            initialData_Cases_By_Maintain_Employees ();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to update this account, pls check your server settings.");
            return;
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Employee_Check_clicked()
{
    if(ui->comboBox_Maintain_Employee_Checkout_UserName->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Remind", "Pls select user account first!");
        return;
    }
    else
    {
        QSqlQuery queryEngineer;
        QString queryEngineerStr = tr("select * from employeeName where employeeName = '%1'").arg (ui->comboBox_Maintain_Employee_Checkout_UserName->currentText ().trimmed ());
        queryEngineer.exec (queryEngineerStr);
        if(queryEngineer.next ())
        {
            QSqlRecord record = queryEngineer.record ();
            QString employeeDepartment = record.value ("employeeDepartment").toString ().trimmed ();
            QString employeeTitle = record.value ("employeeTitle").toString ().trimmed ();
            QString employeeLevel = record.value ("employeeLevel").toString ().trimmed ();
            QString workNumber = record.value ("workNumber").toString ().trimmed ();
            ui->lineEdit_Maintain_Employee_Checkout_Department->setText (employeeDepartment);
            ui->lineEdit_Maintain_Employee_Checkout_Title->setText (employeeTitle);
            ui->lineEdit_Maintain_Employee_Checkout_Level->setText (employeeLevel);
            ui->lineEdit_Maintain_Employee_Checkout_WorkNumber->setText (workNumber);
        }
    }
}


void ManagerWindow::on_comboBox_Maintain_Employee_Delete_UserName_activated(int index)
{
    QSqlQuery queryEmployee;
    QString queryEmployeeStr = tr("select * from Employee where employeeName = '%1'").arg (ui->comboBox_Maintain_Employee_Delete_UserName->currentText ().trimmed ());
    queryEmployee.exec (queryEmployeeStr);
    do
    {
        QSqlRecord record = queryEmployee.record ();
        QString employeeDepartment = record.value ("employeeDepartment").toString ().trimmed ();
        QString employeeTitle = record.value ("employeeTitle").toString ().trimmed ();
        QString employeeLevel = record.value ("employeeLevel").toString ().trimmed ();
        QString workNumber = record.value ("workNumber").toString ().trimmed ();
        ui->lineEdit_Maintain_Employee_Delete_Department->setText (employeeDepartment);
        ui->lineEdit_Maintain_Employee_Delete_Title->setText (employeeTitle);
        ui->lineEdit_Maintain_Employee_Delete_Level->setText (employeeLevel);
        ui->lineEdit_Maintain_Employee_Delete_WorkNumber->setText (workNumber);
    }
    while(queryEmployee.next ());
}


void ManagerWindow::on_comboBox_Maintain_Employee_Update_UserName_activated(int index)
{
    QSqlQuery queryEmployee;
    QString queryEmployeeStr = tr("select * from Employee where employeeName = '%1'").arg (ui->comboBox_Maintain_Employee_Update_UserName->currentText ().trimmed ());
    queryEmployee.exec (queryEmployeeStr);
    do
    {
        QSqlRecord record = queryEmployee.record ();
        QString employeeDepartment = record.value ("employeeDepartment").toString ().trimmed ();
        QString employeeTitle = record.value ("employeeTitle").toString ().trimmed ();
        QString employeeLevel = record.value ("employeeLevel").toString ().trimmed ();
        QString workNumber = record.value ("workNumber").toString ().trimmed ();
        ui->lineEdit_Maintain_Employee_Update_WorkNumber->setText (workNumber);
        ui->comboBox_Maintain_Employee_Update_Department->setCurrentText (employeeDepartment);
        ui->comboBox_Maintain_Employee_Update_Title->setCurrentText (employeeTitle);
        ui->comboBox_Maintain_Employee_Update_Level->setCurrentText (employeeLevel);
    }
    while(queryEmployee.next ());
}


void ManagerWindow::on_comboBox_Maintain_Employee_Checkout_UserName_activated(int index)
{
    QSqlQuery queryEmployee;
    QString queryEmployeeStr = tr("select * from Employee where employeeName = '%1'").arg (ui->comboBox_Maintain_Employee_Checkout_UserName->currentText ().trimmed ());
    queryEmployee.exec (queryEmployeeStr);
    do
    {
        QSqlRecord record = queryEmployee.record ();
        QString employeeDepartment = record.value ("employeeDepartment").toString ().trimmed ();
        QString employeeTitle = record.value ("employeeTitle").toString ().trimmed ();
        QString employeeLevel = record.value ("employeeLevel").toString ().trimmed ();
        QString workNumber = record.value ("workNumber").toString ().trimmed ();
        ui->lineEdit_Maintain_Employee_Checkout_Department->setText (employeeDepartment);
        ui->lineEdit_Maintain_Employee_Checkout_Title->setText (employeeTitle);
        ui->lineEdit_Maintain_Employee_Checkout_Level->setText (employeeLevel);
        ui->lineEdit_Maintain_Employee_Checkout_WorkNumber->setText (workNumber);
    }
    while(queryEmployee.next ());
}






//Maintain employees at the end of here......................................................................



//Maintain others at the begain of here......................................................................


void ManagerWindow::initialData_Maintain_Others()
{
    QSqlQuery queryTitle;
    QString queryTitleStr = "select * from Title";
    queryTitle.exec (queryTitleStr);
    QSqlQuery queryLevel;
    QString queryLevelStr = "select * from Level";
    queryLevel.exec (queryLevelStr);
    QSqlQuery queryDepartment;
    QString queryDepartmentStr = "select * from Department";
    queryDepartment.exec (queryDepartmentStr);
    QSqlQuery queryCaseType;
    QString queryCastTypeStr = "select * from issueType";
    queryCaseType.exec (queryCastTypeStr);
    ui->comboBox_Maintain_Others_Delete_CaseType->clear ();
    ui->comboBox_Maintain_Others_Delete_Department->clear ();
    ui->comboBox_Maintain_Others_Delete_Level->clear ();
    ui->comboBox_Maintain_Others_Delete_Title->clear ();
    while(queryCaseType.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Issue_Name=QString("%1").arg(queryCaseType.value("issueType").toString());
        ui->comboBox_Maintain_Others_Delete_CaseType->addItem (Issue_Name);
    }
    while(queryTitle.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Title_Name=QString("%1").arg(queryTitle.value("employeeTitle").toString());
        ui->comboBox_Maintain_Others_Delete_Title->addItem (Title_Name);
    }
    while(queryLevel.next())
    {
        //把 Employee 表中的 data 载入到下拉框
        QString Level_Name=QString("%1").arg(queryLevel.value("employeeLevel").toString());
        ui->comboBox_Maintain_Others_Delete_Level->addItem (Level_Name);
    }
    while(queryDepartment.next())
    {
        //把 Employee 表中的 data 载入到下拉框
        QString Department_Name=QString("%1").arg(queryDepartment.value("departmentName").toString());
        ui->comboBox_Maintain_Others_Delete_Department->addItem (Department_Name);
    }
}

void ManagerWindow::on_pushButton_Maintain_Others_Add_Department_clicked()
{
    QString departmentName = ui->lineEdit_Maintain_Others_Add_Department->text ().trimmed ();
    if(departmentName.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The department name can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from Department where departmentName =:departmentName");
    sqlQuery.bindValue (":departmentName", departmentName);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The department name is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into Department(departmentName)" "values(:departmentName)");
        sqlQuery.bindValue (":departmentName", ui->lineEdit_Maintain_Others_Add_Department->text ().trimmed ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new department: " + departmentName + " !");
            ui->lineEdit_Maintain_Others_Add_Department->clear ();
            initialData_Maintain_Others();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new department, pls check database server!");
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Others_Delete_Department_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from Department where departmentName = '%1'").arg (ui->comboBox_Maintain_Others_Delete_Department->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Others_Delete_Department->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose department first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this department ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This department was deleted successful!");
                initialData_Maintain_Others ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this department, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Others_Add_Title_clicked()
{
    QString employeeTitle = ui->lineEdit_Maintain_Others_Add_Title->text ().trimmed ();
    if(employeeTitle.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The title name can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from Title where employeeTitle =:employeeTitle");
    sqlQuery.bindValue (":employeeTitle", employeeTitle);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The title name is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into Title(employeeTitle)" "values(:employeeTitle)");
        sqlQuery.bindValue (":employeeTitle", ui->lineEdit_Maintain_Others_Add_Title->text ().trimmed ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new title: " + employeeTitle + " !");
            ui->lineEdit_Maintain_Others_Add_Title->clear ();
            initialData_Maintain_Others();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new title, pls check database server!");
        }
    }
}


void ManagerWindow::on_pushButton_Maintain_Others_Delete_Title_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from Title where employeeTitle = '%1'").arg (ui->comboBox_Maintain_Others_Delete_Title->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Others_Delete_Title->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose title first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this title ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This title was deleted successful!");
                initialData_Maintain_Others ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this title, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void ManagerWindow::on_pushButton__Maintain_Others_Add_Level_clicked()
{
    QString employeeLevel = ui->lineEdit_Maintain_Others_Add_Level->text ().trimmed ();
    if(employeeLevel.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The Level name can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from Level where employeeLevel =:employeeLevel");
    sqlQuery.bindValue (":employeeLevel", employeeLevel);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The level name is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into Level(employeeLevel)" "values(:employeeLevel)");
        sqlQuery.bindValue (":employeeLevel", ui->lineEdit_Maintain_Others_Add_Level->text ().trimmed ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new level: " + employeeLevel + " !");
            ui->lineEdit_Maintain_Others_Add_Level->clear ();
            initialData_Maintain_Others();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new level, pls check database server!");
        }
    }
}


void ManagerWindow::on_pushButton__Maintain_Others_Delete_Level_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from Level where employeeLevel = '%1'").arg (ui->comboBox_Maintain_Others_Delete_Level->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Others_Delete_Level->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose level first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this level ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This level was deleted successful!");
                initialData_Maintain_Others ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this level, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void ManagerWindow::on_pushButton__Maintain_Others_Add_CaseType_clicked()
{
    QString issueType = ui->lineEdit_Maintain_Others_Add_CaseType->text ().trimmed ();
    if(issueType.isEmpty ())
    {
        QMessageBox::warning (this, "Error", "The caseType name can not be empty!");
        return;
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare ("select * from issueType where issueType =:issueType");
    sqlQuery.bindValue (":issueType", issueType);
    sqlQuery.exec ();
    if(sqlQuery.next ())
    {
        QMessageBox::warning (this, "Error", "The case type is exist already, pls double check!");
    }
    else
    {
        sqlQuery.prepare ("insert into issueType(issueType)" "values(:issueType)");
        sqlQuery.bindValue (":issueType", ui->lineEdit_Maintain_Others_Add_CaseType->text ().trimmed ());
        if(sqlQuery.exec ())
        {
            QMessageBox::information (this, "Information", "Sucess to create new case type! " + issueType + " !");
            ui->lineEdit_Maintain_Others_Add_CaseType->clear ();
            initialData_Maintain_Others();
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to create new case type, pls check database server!");
        }
    }
}


void ManagerWindow::on_pushButton__Maintain_Others_Delete_CaseType_clicked()
{
    QSqlQuery queryEngineer;
    QString queryEngineerStr = tr("delete from issueType where issueType = '%1'").arg (ui->comboBox_Maintain_Others_Delete_CaseType->currentText ().trimmed ());
    if(ui->comboBox_Maintain_Others_Delete_CaseType->currentIndex ()==-1)
    {
        QMessageBox::warning (this, "Error", "Pls choose casetype first!");
        return;
    }
    else
    {
        int ret = QMessageBox::warning (this, "Remind", "Are you sure to delete this caseType ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(ret == 16384)
        {
            if(queryEngineer.exec (queryEngineerStr))
            {
                QMessageBox::information (this, "Information", "This caseType was deleted successful!");
                initialData_Maintain_Others ();
            }
            else
            {
                QMessageBox::warning (this, "Error", "Failure to delete this caseType, pls check your server settings.");
                return;
            }
        }
        else
        {
            return;
        }
    }
}




//Maintain others at the end of here............................................................................

void ManagerWindow::on_AllCasestableView_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_pushButton_Details_Take_Case_clicked()
{
    QString PreItEngineer = ui->lineEdit_Details_ITEngineer->text ().trimmed ();
    ui->lineEdit_Details_ITEngineer->setText (loginUsernameManager);
    QSqlQuery sqlQuery;
    QSqlQuery sqlQueryRemark;
    QString sqlStr = QString("UPDATE HelpDesk.Ticket SET itEngineer='%1' WHERE id=%2").arg (loginUsernameManager).arg (ui->lineEdit_Details_ID->text ().toInt ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update time is %1; Action: Take Case; Updated IT Engineer from %2 to %3．\n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (PreItEngineer).arg (ui->lineEdit_Details_ITEngineer->text ().trimmed ());
        ui->textEdit_Details_Remark->append (updateRemark);
        QString remarkResult = ui->textEdit_Details_Remark->toPlainText ().trimmed ();
        QString sqlRemarkStr = QString("UPDATE Ticket SET remarkInformation='%1' WHERE id=%2").arg (remarkResult).arg (ui->lineEdit_Details_ID->text ().toInt ());
        if(sqlQueryRemark.exec (sqlRemarkStr))
        {
            QMessageBox::information (this, "Information", "Sucess to update records!");
        }
        else
        {
            QMessageBox::warning (this, "Error", "Failure to update record, pls double check your database server settings!");
        }
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update record, pls double check your server!");
    }
}


void ManagerWindow::on_pushButton_Details_Updat_Case_clicked()
{
    QString caseStatus = ui->comboBox_Details_Case_Status->currentText ().trimmed ();
    QString solutionStr = ui->textEdit_Details_Solution->toPlainText ().trimmed ();
    QString remarkStr = ui->textEdit_Details_Remark->toPlainText ().trimmed ();
    QSqlQuery sqlQuery;
    QString sqlStr = tr("update Ticket SET issueStatus = '%1', solutionDescription = '%2', remarkInformation = '%3' where id = %4").arg (caseStatus).arg (solutionStr).arg (remarkStr).arg (ui->lineEdit_Details_ID->text ().trimmed ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1; Action: Updated; Updated case to case status %2 and updated case solution with remark by IT Engineer %3. \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (caseStatus).arg (loginUsernameManager);
        ui->textEdit_Details_Remark->append (updateRemark);
        QMessageBox::information (this, "Information", "Sucess to update the case!");
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update case, pls double check your server!");
    }
}


void ManagerWindow::on_pushButton_Details_Close_Case_clicked()
{
    QString caseStatus = ui->comboBox_Details_Case_Status->currentText ().trimmed ();
    QSqlQuery sqlQuery;
    QString solutionStr = ui->textEdit_Details_Solution->toPlainText ().trimmed ();
    QString remarkStr = ui->textEdit_Details_Remark->toPlainText ().trimmed ();
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString DateTimestr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString sqlStr = tr("update Ticket SET issueStatus = 'Closed', solutionDescription = '%1', remarkInformation = '%2', issueSolvedTime = '%3', itEngineer ='%4' where id = %5").arg (solutionStr).arg (remarkStr).arg(DateTimestr).arg (loginUsernameManager).arg (ui->lineEdit_Details_ID->text ().trimmed ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1; Action: Closed; Closed this case from Case Status: %2 to Case Status: Closed and updated solution with remark by IT Engineer: %4. \n*****************************************************").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg(caseStatus).arg (loginUsernameManager);
        ui->textEdit_Details_Remark->append (updateRemark);
        ui->lineEdit_Details_SolvedTime->setText (DateTimestr);
        QMessageBox::information (this, "Information", "Sucess to close the case!");
        ui->comboBox_Details_Case_Status->setCurrentText ("Closed");
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to close case, pls double check your server!");
    }
}


void ManagerWindow::on_pushButton_Details_Handover_Case_clicked()
{
    if(ui->comboBox_Details_HandoverTo->currentIndex ()==-1)
    {
        QMessageBox::information (this, "Information", "Pls select handover IT Engineer first!");
        return;
    }
    QString PreItEngineer = ui->lineEdit_Details_ITEngineer->text ().trimmed ();
    QSqlQuery sqlQuery;
    QString sqlStr = tr("update Ticket SET itEngineer = '%1' where id = %2").arg (ui->comboBox_Details_HandoverTo->currentText ().trimmed ()).arg (ui->lineEdit_Details_ID->text ().trimmed ());
    if(sqlQuery.exec (sqlStr))
    {
        QString updateRemark = tr("Update Time is %1; Action: Handover; Handover case from IT Engineer: %2 to IT Engineer: %3 by IT Engineer: %4. \n***************************************************** ").arg (QDateTime().currentDateTime ().toString ().trimmed ()).arg (PreItEngineer).arg (ui->comboBox_Details_HandoverTo->currentText ().trimmed ()).arg (loginUsernameManager);
        ui->textEdit_Details_Remark->append (updateRemark);
        QMessageBox::information (this, "Information", "Sucess to handover the case!");
        ui->lineEdit_Details_ITEngineer->setText (ui->comboBox_Details_HandoverTo->currentText ().trimmed ());
    }
    else
    {
        QMessageBox::warning (this, "Error", "Failure to update record, pls double check your server!");
    }
}


void ManagerWindow::on_tableView_All_Closed_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_Closed_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_All_Hold_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_Hold_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_All_My_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_My_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_All_My_Closed_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_My_Closed_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_All_My_OpenHold_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_My_OpenHold_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_All_Open_Cases_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Action_All_Open_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_tableView_SearchByEngineer_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_Manager_Search_By_Engineer_Cases->record(index.row()); //关键步骤
    ui->lineEdit_Details_ID->clear ();
    ui->lineEdit_Details_EmployeeName->clear ();
    ui->lineEdit_Details_Department->clear ();
    ui->lineEdit_Details_CaseLevel->clear ();
    ui->lineEdit_Details_CaseType->clear ();
    ui->lineEdit_Details_CreateTime->clear ();
    ui->lineEdit_Details_SolvedTime->clear ();
    ui->lineEdit_Details_ITEngineer->clear ();
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}


void ManagerWindow::on_actionCheck_Upgrade_triggered()
{
    dialogUpgrade = new DialogUpgrade(this);
    dialogUpgrade->setModal (false);
    dialogUpgrade->show ();
}


void ManagerWindow::on_actionMonitor_Window_triggered()
{
    ui->stackedWidget->setCurrentWidget (ui->page_InitalData_Chart);
    ChartData_All_Cases();
    ChartData_Engineer_HoldCases();
    ChartData_Sort_Cases_Type();
    ChartData_OPenCases();
    ChartData_All_Cases_By_Dept();
    ChartData_Monthly_Cases ();
    ChartData_Last_Month_Cases ();
    ChartData_Yearly_By_Department_Cases();
}



void ManagerWindow::Open_Manager_Console()
{
    ui->stackedWidget->setCurrentWidget (ui->page_InitalData_Chart);
    ChartData_All_Cases();
    ChartData_Engineer_HoldCases();
    ChartData_Sort_Cases_Type();
    ChartData_OPenCases();
    ChartData_All_Cases_By_Dept();
    ChartData_Monthly_Cases ();
    ChartData_Last_Month_Cases ();
    ChartData_Yearly_By_Department_Cases();
}

void ManagerWindow::closeEvent (QCloseEvent *)
{
    QApplication* app;
    app->quit ();
}

void ManagerWindow::on_actionChat_Window_triggered()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Chat);
}

//Chat module start here.........................................................

void ManagerWindow::BindPort()
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

void ManagerWindow::on_readyRead()
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

void ManagerWindow::on_pushButton_Chat_SendMessage_clicked()
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
    qDebug() << "dstIP is: " << dstIP;
    m_udpSocket->writeDatagram(str,dstAddr,dstPort);
    QTextCharFormat fmt;
    //字体色
    fmt.setForeground(QBrush(Qt::darkRed));
    QString msgLocal = ui->plainTextEdit_Send_Messages->toPlainText ();
    ui->plainTextEdit_Chat_Messages->mergeCurrentCharFormat(fmt);
    ui->plainTextEdit_Chat_Messages->appendPlainText("[To " + chatWithName + "-" + QTime::currentTime ().toString () + "]: "+msgLocal);
    ui->plainTextEdit_Send_Messages->clear ();
}

void ManagerWindow::on_pushButton_Chat_RefreshList_clicked()
{
    ui->lineEdit_Chat_Search->clear ();
    on_tableView_HelpDesk_Contact_activated();
}
void ManagerWindow::comboBox_Chat_UpdateIP()
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
void ManagerWindow::on_pushButton_Chat_UpdateIP_clicked()
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
void ManagerWindow::updateIPToMySQL()
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
bool ManagerWindow::eventFilter(QObject *target, QEvent *event)
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




void ManagerWindow::on_pushButton_Chat_Search_clicked()
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


void ManagerWindow::on_tableView_HelpDesk_Contact_activated()
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





void ManagerWindow::on_tableView_Cases_Today_activated()
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


void ManagerWindow::on_pushButton_Chat_Refresh_CasesToday_clicked()
{
    on_tableView_Cases_Today_activated ();
}



//Chat moduel end here............................................................




void ManagerWindow::on_tableView_HelpDesk_Contact_doubleClicked(const QModelIndex &index)
{
    QSqlRecord record = model_HelpDesk_Contact->record(index.row ()); //关键步骤
    //update data
    ui->label_ChatWithName->setText (record.value ("employeeName").toString ());
    helpDeskIP = record.value ("ipAddress").toString ();
    dstIP = helpDeskIP;
}


void ManagerWindow::on_tableView_Cases_Today_doubleClicked(const QModelIndex &index)
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
    ui->textEdit_Details_Description->clear ();
    ui->textEdit_Details_Attachment->clear ();
    ui->textEdit_Details_Solution->clear ();
    ui->textEdit_Details_Remark->clear ();
    ui->comboBox_Details_HandoverTo->clear ();
    ui->comboBox_Details_Case_Status->clear ();
    //update data
    QSqlQuery queryCaseStatus;
    QString queryCaseStatusStr = "select * from issueStatus";
    queryCaseStatus.exec (queryCaseStatusStr);
    while(queryCaseStatus.next())
    {
        //把 ticket 表中的 issueStatusName 载入到下拉框
        QString issueStatus_Name=QString("%1").arg(queryCaseStatus.value("issueStatusName").toString());
        ui->comboBox_Details_Case_Status->addItem(issueStatus_Name);
    }
    //
    QSqlQuery queryEngineer;
    QString queryEngineerStr = "select * from HelpDesk_Team";
    queryEngineer.exec (queryEngineerStr);
    while(queryEngineer.next())
    {
        //把 HelpDesk_Team 表中的 username 载入到下拉框
        QString Engineer_Name=QString("%1").arg(queryEngineer.value("userName").toString());
        ui->comboBox_Details_HandoverTo->addItem(Engineer_Name);
    }
    //
    ui->comboBox_Details_Case_Status->setCurrentText (record.value ("issueStatus").toString ().trimmed ());
    ui->lineEdit_Details_ID->setText (record.value("id").toString ());
    ui->lineEdit_Details_EmployeeName->setText (record.value ("employeeName").toString ());
    ui->lineEdit_Details_Department->setText (record.value("employeeDepartment").toString ());
    ui->lineEdit_Details_CaseLevel->setText (record.value("issueLevel").toString ());
    ui->lineEdit_Details_CaseType->setText (record.value("issueType").toString ());
    ui->lineEdit_Details_CreateTime->setText (record.value("issueCreateTime").toString ());
    ui->lineEdit_Details_SolvedTime->setText (record.value("issueSolvedTime").toString ());
    ui->lineEdit_Details_ITEngineer->setText (record.value("itEngineer").toString ());
    ui->textEdit_Details_Description->setText (record.value("issueDescription").toString ());
    ui->textEdit_Details_Solution->setText (record.value("solutionDescription").toString ());
    ui->textEdit_Details_Attachment->setText (record.value("attachedFile").toString ());
    ui->textEdit_Details_Remark->setText (record.value("remarkInformation").toString ());
    ui->stackedWidget->setCurrentWidget (ui->page_Case_Details);
}










void ManagerWindow::on_actionMaintain_Department_Level_Title_triggered()
{
    initialData_Maintain_Others();
    ui->stackedWidget->setCurrentWidget (ui->page_Department_Level_Title);
}




void ManagerWindow::on_actionDonate_to_support_Super_Helpdesk_triggered()
{
//    QString Url = "www.pavogroup.top";
//    QDesktopServices::openUrl (Url);
    dialogDonate = new DialogDonate(this);
    dialogDonate->setModal (false);
    dialogDonate->showNormal ();
}

