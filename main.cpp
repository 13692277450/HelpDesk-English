#include "mainwidget.h"
#include "managerwindow.h"
#include "engineerwindow.h"
#include "dialogupgrade.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSqlDatabase>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QtCharts>
#include <QTest>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "HelpDesk3_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    //==========================
    //=================================
//    QSqlDatabase db;
//    QSqlDatabase::removeDatabase("QMYSQL");
//    if(QSqlDatabase::contains("HelpDesk"))
//    {
//        db = QSqlDatabase::database("QMYSQL");
//        qDebug() << "QSqldatabase contains QMYSQL....";
//    }
//    else
//    {
//        db = QSqlDatabase::addDatabase("QMYSQL");
//        qDebug() << "QSqlDatabase add database....";
//    }
//    db.setHostName("108.166.195.86");  //连接本地主机
//    db.setPort(3306);
//    db.setDatabaseName("HelpDesk");
//    db.setUserName("root");
//    db.setPassword("86868686mM");
//    bool OpenDBResult = db.open();
//    if(!OpenDBResult)
//    {
//        QMessageBox::warning (NULL, "Database Error", QString("Could not connect to database, pls check..."));
//        qDebug() << "Connected database error";
//        //return;
//    }
//    else
//    {
//        qDebug() << "Connected to database server successful";
//        //return;
//    }
    mainWidget mainWidgetWindow;
    // mainWidgetWindow.loadSetting ();
    // mainWidgetWindow.connectDB ();
    QPropertyAnimation *animation = new QPropertyAnimation(&mainWidgetWindow,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    if(!mainWidgetWindow.OpenDBResult)
    {
        mainWidgetWindow.on_progressBar_valueChanged (0);
        mainWidgetWindow.on_progressBar_changeLabel();
    }
    else
    {
        mainWidgetWindow.on_progressBar_valueChanged (100);
    }
    mainWidgetWindow.showNormal ();
    return a.exec();
}




