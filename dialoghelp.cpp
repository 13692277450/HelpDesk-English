#include "dialoghelp.h"
#include "ui_dialoghelp.h"

DialogHelp::DialogHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    ui->setupUi(this);
    Init_DialogHelp ();
}

DialogHelp::~DialogHelp()
{
    delete ui;
}

void DialogHelp::Init_DialogHelp()
{
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>Super HelpDesk System Help:</strong>&nbsp;</p>"));
    // ui->textEdit_Help->append ("Super HelpDesk System Help: \n");
    ui->textEdit_Help->append ("\nSuper HelpDesk software is published by PavoGroup, this is free version; PavoGroup is focuse on provide high quality products to human beings, and also provide free software to the World. for more information, pls visit www.pavogroup.top. \n\n");
    //ui->textEdit_Help->append ("Super HelpDesk System including 10 modules, they are: \n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>Super HelpDesk System including 3 modules, they are:</strong>&nbsp;</p>"));
    ui->textEdit_Help->append ("\n\n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>Manager Module:</strong>&nbsp;</p>"));
    ui->textEdit_Help->append ("\nManager module details: including night modules to monitor helpdesk team daily operation jobs and build reports, \n - The first is current Open cases distribution by IT Engineer, it can help IT Manager monitor IT Engineers current tasks. \n - The second is current hold tasks distribute by IT Engineer, it can help IT Manager known why some cases could not be closed on time. \n - The third is sort all cases by case status, it can help IT Manager overall to monitor team's job status and control IT resources loading. \n - The forth is all cases distribute by IT Engineer, it will help IT　Manager monitor task loading of each IT Engineer and control how to balance their jobs. \n - The fifth is all Open and Hold cases distribute by department, it can help IT Manager know each department cases quantity then arrange suitable IT support resources to each department. \n - The sixth is all cases distribute in the past 12 months, it can help IT Manager know cases distribute by month and know why there is some quantity of cases dynamic up or down, then can adjust IT resources to meet company business operation. \n - The seventh is all cases distribute by department in the last month, it can help IT　Manager know the case distribution in recent time, then can make decision how to balance IT　resources to meet compan business requirements. \n - The eighth is all cases distribute in the past 12 months by department, it can help IT Manager to conclusion jobs in the last year, the make forcast in the next year. \n - The nineth is IT Engineer account maintianance module, Manager can update/add/delete/check IT　Engineer account... etc. \n\n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>IT Engineer Module:</strong>&nbsp;</p>"));
    ui->textEdit_Help->append ("\n");
    ui->textEdit_Help->append (" - The IT Engineert module,  each IT engineer can see the request from client module then do reponse for each case, they can 'Take Case', 'Handover case','Update case','Close Case'. \n\n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>Client module:</strong>&nbsp;</p>"));
    ui->textEdit_Help->append ("\n");
    ui->textEdit_Help->append ("The client only need configure mysql database server ip address, port (default is 3306), database login name and password, those configuration keep in application runing path with setting sub-folder, which file name is setting.json, after configuration was done for the first user, you can copy this setting.json file to another user's computer who need install Super HelpDesk client, then you need not re-configure for other users again, the database login password was encrypted.\n\n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>Pls feel free contact by email: 13692277450@139.com or access website: www.pavogroup.top to get more support information.</strong>&nbsp;</p>"));
    ui->textEdit_Help->append ("\n\n");
    ui->textEdit_Help->insertHtml (QStringLiteral("<p style='margin:0px;padding:0px;'><strong>PavoGroup @2022, all rights reserved.</strong>&nbsp;</p>"));
    ui->textEdit_Help->moveCursor (QTextCursor::Start);
}

void DialogHelp::on_pushButton_clicked()
{
    this->hide ();
}

