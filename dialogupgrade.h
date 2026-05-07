#ifndef DIALOGUPGRADE_H
#define DIALOGUPGRADE_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>

namespace Ui
{
    class DialogUpgrade;
}

class DialogUpgrade : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpgrade(QWidget *parent = nullptr);
    ~DialogUpgrade();
    void Receive_Json();

private slots:
    void on_pushButton_upgrade_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_Check_Upgrade_clicked();

    void on_progressBar_valueChanged(int value);

private:
    Ui::DialogUpgrade *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QFile *myfile;
    QString Url;
    bool downloadSetup = false;
    bool isCheckUpgradeDone = false;
    bool isDownloadFinished = false;
    qint64 recv_total;
    qint64 all_total;
    void Init();

    void doProcessReadyRead();
    void doProcessFinished();
    void doProcessDownloadProgress(qint64,qint64);
    void doProcessError(QNetworkReply::NetworkError code);
    void Start_Install_New_Application();


};

#endif // DIALOGUPGRADE_H
