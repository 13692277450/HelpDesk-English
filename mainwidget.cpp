#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "managerwindow.h"
#include <QSqlDatabase>
#include <QPropertyAnimation>
#include <QSqlQuery>
#include <QButtonGroup>
#include <QMessageBox>
#include <QSqlRecord>
#include <QIcon>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qbytearray.h>
#include <qstring.h>
#include <QFileInfo>
#include <QTextStreamManipulator>
#include <QProcess>


mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    loadSetting();
    connectDB ();
    setWindowFlags(Qt::Dialog); //取消login 窗口最大化
    this->setWindowIcon(QIcon(":/Icon/bird2.png"));
    setLoginImageSize();
    ui->progressBar->setFixedHeight (15);
    //dialogLogin = new DialogLogin(this);
    // managerWindow = new ManagerWindow(this);
    //connect(ui->loginOKButton,SIGNAL(clicked()),this,SLOT(toManagerSlot()));
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::setLoginImageSize()
{
    QPixmap pixmapUserName(":/Icon/user.png");
    QSize sz(28, 20);
    pixmapUserName = pixmapUserName.scaled(sz, Qt::KeepAspectRatio);
    ui->label_UserName->setPixmap(pixmapUserName);
    QPixmap pixmapPassword(":/Icon/password.png");
    pixmapPassword = pixmapPassword.scaled (sz, Qt::KeepAspectRatio);
    ui->label_Password->setPixmap (pixmapPassword);
}

void mainWidget::connectDB()
{
    QSqlDatabase db;
    QSqlDatabase::removeDatabase("QMYSQL");
    if(QSqlDatabase::contains("HelpDesk"))
    {
        db = QSqlDatabase::database("QMYSQL");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }
    db.setHostName(dataBaseHostName);  //连接本地主机
    db.setPort(dataBasePort);
    db.setDatabaseName("HelpDesk");
    db.setUserName(dataBaseUserName);
    db.setPassword(dataBasePassWord);
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    OpenDBResult = db.open();
    if(!OpenDBResult)
    {
        QMessageBox::warning (this, "Database Error", QString("Could not connect to database, pls check..."));
        qDebug() << "Connected database error";
        return;
    }
    else
    {
        qDebug() << "Connected to database server successful";
        return;
    }
    return;
}


void mainWidget::on_loginCancelButton_clicked()
{
    if(OpenDBResult)
    {
        db.close ();
        QApplication* app;
        app->quit ();
    }
    else
    {
        QApplication* app;
        app->quit ();
    }
}

void mainWidget::closeEvent (QCloseEvent *)
{
    QApplication* app;
    app->quit ();
}

void mainWidget::toEngineerSlot ()
{
    emit mySignal ();
}

void mainWidget::toManagerSlot()
{
    emit mySignal();
}

void mainWidget::on_passwordEdit_returnPressed()
{
    on_loginOKButton_clicked();
}


void mainWidget::on_progressBar_valueChanged(int value)
{
    ui->progressBar->setValue (value);
}

void mainWidget::on_progressBar_changeLabel()
{
    ui->label_ConnectDB->setText ("Connect DB failure!");
    ui->passwordEdit->setDisabled (true);
    ui->usernameEdit->setDisabled (true);
}

void mainWidget::on_pushButton_Save_Settings_clicked()
{
    if(ui->lineEdit_Settings_IP->text ()== "" | ui->lineEdit_Settings_LoginName->text ()== "" | ui->lineEdit_Settings_Password->text ()== "" | ui->lineEdit_Settings_Port->text ()== "")
    {
        QMessageBox::warning (this, "Error", "IP address or login name or password or port could not be empty!");
        return;
    }
    QString filepath =  QCoreApplication::applicationDirPath();
    QFile settingFile2(filepath + "/setting/setting.json");
    //QFileInfo info(settingFile2);
    if(settingFile2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QJsonObject jsonObject;//构建json对象json
        QString encyPassword = EncryptionStr (ui->lineEdit_Settings_Password->text ().trimmed ());
        jsonObject.insert("dataBaseHostName", ui->lineEdit_Settings_IP->text ().trimmed ());
        jsonObject.insert("dataBasePort", ui->lineEdit_Settings_Port->text ().trimmed ());
        jsonObject.insert("dataBaseUserName", ui->lineEdit_Settings_LoginName->text ().trimmed ());//ui->lineEdit_LoginName->text ());
        jsonObject.insert ("dataBasePassWord", encyPassword);
        QJsonDocument document;
        document.setObject(jsonObject);
        QString jsonStr(document.toJson(QJsonDocument::Indented));
        settingFile2.write(jsonStr.toUtf8 ());
        settingFile2.flush();
        QMessageBox::information (this, "Success", "Your configuration were saved successful, pls restart application again!");
    }
    else
    {
        QMessageBox::warning (this, "Error", "Sorry, your configuration were saved failure, pls double check!");
    }
    settingFile2.close();
    //QCoreApplication::quit ();
    qApp->quit ();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}
void mainWidget::loadSetting()
{
    QString filepath =  QCoreApplication::applicationDirPath();
    QFile settingFile(filepath + "/setting/setting.json");
//    QFile settingFile;
//    settingFile.setFileName(":/setting/setting.json");
    if(settingFile.open(QFile::ReadOnly))
    {
        // 在json中添加了注释，所以需要把注释内容去掉
        QByteArray setting = settingFile.readAll().trimmed();
        // 循环删除掉注释内容
        while (setting.contains("/*") && setting.contains("*/"))
        {
            int start = setting.indexOf("/*");
            int end = setting.indexOf("*/") + 1;
            setting =  setting.remove(start, (end - start) + 1);
        }
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(setting, &jsonError));
        if(jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject rootObj = jsonDoc.object();
            dataBaseHostName = rootObj.value("dataBaseHostName").toString (); // 读取配置文件中数据库连接ip地址
            dataBasePort = rootObj.value("dataBasePort").toInt();  // 读取配置文件中数据库连接端口
            dataBasePortStr = rootObj.value ("dataBasePort").toString ();
            dataBaseUserName = rootObj.value("dataBaseUserName").toString(); // 读取配置文件中数据库连接用户名
            dataBasePassWord = EncryptionStr(rootObj.value("dataBasePassWord").toString()); // 读取配置文件中数据库连接密码
        }
        else
        {
            //qDebug() << "Configure json file error!" << jsonError.errorString();
        }
        settingFile.close ();
    }
}
QString mainWidget::EncryptionStr(QString str)
{
    std::wstring wString = str.toStdWString();
    static QByteArray key = "86868686m$M&NOS";
    for (int i = 0; i < wString.size(); i++)
    {
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}




void mainWidget::on_pushButton_Exit_Settings_clicked()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Login);
}



void mainWidget::on_loginOKButton_clicked()
{
    loginUsername = ui->usernameEdit->text().trimmed ();
    loginPassword = ui->passwordEdit->text().trimmed ();
    QSqlQuery query;
    QString strQuery = QString("select * from HelpDesk_Team where userName = '%1' and password = '%2'").
                       arg (loginUsername, loginPassword);
    query.exec(strQuery);
    QSqlRecord rec = query.record();
    if(!query.next())
    {
        QMessageBox::warning (this, "Login Failure", "Sorry, Your username or password error, Pls double check and try again!");
        return;
    }
    else
    {
        rec = query.record ();
        int loginUsernameInt = rec.indexOf ("userName");
        int loginIsActive = rec.indexOf ("isActive");
        int loginTitle = rec.indexOf ("roleTitle");
        loginUsername = query.value (loginUsernameInt).toString ();
        QString loginTitleValue = query.value(loginTitle).toString();
        int logintIsActiveValue = query.value(loginIsActive).toInt ();
        if(logintIsActiveValue == 0)
        {
            QMessageBox::warning (this, "Login Error", "Sorry, Your Account was disabled!");
            return;
        }
        else if(loginTitleValue == "Engineer")
        {
            engineerWindow.loginUsernameEngineer = this->loginUsername;
            engineerWindow.InitialAllDataEngineer ();
            engineerWindow.showMaximized ();
            this->hide();
        }
        else
        {
            managerWindow.loginUsernameManager = this->loginUsername;
            //managerWindow.on_actionMonitor_Window_triggered();
            managerWindow.InitialAllDataManager ();
            managerWindow.Open_Manager_Console ();
            managerWindow.showMaximized ();
            this->hide();
        }
    }
}

void mainWidget::on_pushButton_MainWidget_Settings_clicked()
{
    ui->stackedWidget->setCurrentWidget (ui->page_Setting);
    ui->lineEdit_Settings_IP->setText (dataBaseHostName);
    ui->lineEdit_Settings_Port->setText (dataBasePortStr);
    ui->lineEdit_Settings_LoginName->setText (dataBaseUserName);
}

