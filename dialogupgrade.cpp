#include "dialogupgrade.h"
#include "ui_dialogupgrade.h"

//网络相关头文件
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
//JSON相关头文件

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTextStreamManipulator>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QFile>
#include <QTest>
#include <QFileInfo>
#include <unistd.h>
#include <QSaveFile>
#include <QThread>
#include <QProcess>




DialogUpgrade::DialogUpgrade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpgrade)
{
    ui->setupUi(this);
    Init ();
    // loadSetting ();
    Receive_Json();
}

DialogUpgrade::~DialogUpgrade()
{
    delete ui;
}

void DialogUpgrade::Init()
{
    manager = new QNetworkAccessManager(this);
    myfile = new QFile(this);
    ui->progressBar->setFixedHeight (15);
    ui->progressBar->setValue (0);
    ui->progressBar->setAlignment (Qt::AlignVCenter);
    ui->lineEdit_Json->setVisible (false);
    ui->lineEdit->setVisible (false);
    ui->textEdit->append ("The Super HelpDesk current version is: Ver2.0 \n");
    //ui->pushButton_upgrade->setDisabled (true);
    ui->pushButton_upgrade->setVisible (false);
}


void DialogUpgrade::on_pushButton_upgrade_clicked()
{
    if(reply->isRunning ())
    {
        return;
    }
    else
    {
        reply->reset ();
        myfile->close ();
        QNetworkRequest request;
        ui->lineEdit->setText ("http://www.pavogroup.top/superhelpdesk/superhelpdesk.exe");
        QString Url = ui->lineEdit->text();
        request.setUrl(QUrl(Url));
        reply = manager->get(request);              //发送请求
        connect(reply,&QNetworkReply::readyRead,this,&DialogUpgrade::doProcessReadyRead);                //可读
        connect(reply,&QNetworkReply::finished,this,&DialogUpgrade::doProcessFinished);                  //结束
        connect(reply,&QNetworkReply::downloadProgress,this,&DialogUpgrade::doProcessDownloadProgress);  //大小
        // connect(reply,QOverload<QNetworkReply::NetworkError>:: of(&QNetworkReply::error), this,&DialogUpgrade::doProcessError); //error
        QStringList list = Url.split("/");
        QString filename = list.at(list.length()-1);
        QString file = "C:/" + filename;
        myfile->setFileName(file);
        bool ret = myfile->open(QIODevice::WriteOnly|QIODevice::Truncate);    //创建文件
        if(!ret)
        {
            QMessageBox::warning(this,"Warning","Download upgrade file failure, pls try again later!");
            return;
        }
        downloadSetup = true;
        ui->progressBar->setValue(0);             //设置进度条为0
        ui->progressBar->setMinimum(0);
    }
}

void DialogUpgrade::doProcessReadyRead()             //读取写入
{
    while(!reply->atEnd())
    {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
}

void DialogUpgrade::doProcessFinished()
{
    myfile->close();
    if(downloadSetup == true)
    {
        isDownloadFinished = true;
        Start_Install_New_Application();
    }
}

void DialogUpgrade::doProcessDownloadProgress(qint64 recv_total, qint64 all_total)      //显示
{
    ui->progressBar->setMaximum(all_total);
    ui->progressBar->setValue(recv_total);
//    QStringList list = ui->lineEdit->text().split("/");
//    QString filename = list.at(list.length()-1);
//    QString data = "--" + filename;
}

void DialogUpgrade::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
}




void DialogUpgrade::on_pushButton_exit_clicked()
{
    myfile->close ();
    this->hide ();
}



void DialogUpgrade::on_pushButton_Check_Upgrade_clicked()
{
    ui->textEdit->clear ();
    ui->textEdit->append ("The Super HelpDesk current version is: Ver1.0 \n");
    ui->textEdit->append ("The lastest upgrade infromation: \n \n");
    QFile settingFile;
    settingFile.setFileName ("C:/superhelpdeskupgrade.json");
    if(settingFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QByteArray setting = settingFile.readAll ().trimmed ();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(setting, &jsonError));
        QJsonObject rootObj = jsonDoc.object();
        QString LatestVersion = rootObj.value("LastestVersion").toString ();
        QString UpdateTime = rootObj.value("UpdateTime").toString ();
        QString ReleaseNote = rootObj.value("ReleaseNote").toString();
        QString Url = rootObj.value("Url").toString();
        ui->textEdit->append ("Lastest Version : " + LatestVersion + "\n");
        ui->textEdit->append ("Update Time : " + UpdateTime + "\n");
        ui->textEdit->append ("Release Notes : " + ReleaseNote + "\n");
        ui->textEdit->append ("\nSuccess to read upgrade version file! you can click download & upgrade button to download new version application, the new application installation package will be kept in C:\. \n");
        ui->pushButton_upgrade->setVisible (true);
        isCheckUpgradeDone = true;
    }
    else
    {
        ui->textEdit->append ("Sorry, Open upgrade version file failure! \n \n");
    }
}

void DialogUpgrade::Receive_Json()
{
    ui->textEdit->clear ();
    ui->textEdit->append ("The Super HelpDesk current version is: Ver2.0 \n");
    myfile->close ();
    QNetworkRequest request;
    ui->lineEdit->setText ("http://www.pavogroup.top/superhelpdesk/superhelpdeskupgrade.json");
    QString Url = ui->lineEdit->text();
    request.setUrl(QUrl(Url));
    reply = manager->get(request);              //发送请求
    QTest::qSleep (100);
    connect(reply,&QNetworkReply::readyRead,this,&DialogUpgrade::doProcessReadyRead);                //可读
    connect(reply,&QNetworkReply::finished,this,&DialogUpgrade::doProcessFinished);                  //结束
    connect(reply,&QNetworkReply::downloadProgress,this,&DialogUpgrade::doProcessDownloadProgress);  //大小
    // connect(reply,QOverload<QNetworkReply::NetworkError>:: of(&QNetworkReply::error), this,&DialogUpgrade::doProcessError); //error
    QStringList list = Url.split("/");
    QString filename = list.at(list.length()-1);
    QString file = "C:/" + filename;
    myfile->setFileName(file);
    bool ret = myfile->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text);    //创建文件
    if(!ret)
    {
        ui->textEdit->append ("Can not connect to upgrade server, pls check your connection or try again later!");
        return;
    }
    else
    {
        ui->textEdit->append ("Connect Super HelpDesk home website www.pavogroup.top successed. \n \n");
        return;
    }
    myfile->aboutToClose ();
    myfile->flush ();
    ui->progressBar->setValue(0);             //设置进度条初始化为0
    ui->progressBar->setMinimum(0);
    return;
}

void DialogUpgrade::on_progressBar_valueChanged(int value)
{
}

void DialogUpgrade::Start_Install_New_Application()
{
    ui->textEdit->append ("Application download finished and keep installation file is C:\superhelpdesk.rar");
    qDebug() << "Start exit program and install new....";
    QProcess process(this);
    QString str ="C:/superhelpdesk.rar";//加可执行文件路径
    //qApp->setApplicationName (str);
    qApp->quit ();
    process.startDetached(str);//启动可执行文件
    // QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

