#include "credentialswidget.h"
#include "qjsonobject.h"
#include "ui_credentialswidget.h"
#include "mainwindow.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QClipboard>
credentialswidget::credentialswidget(const QString &site, int json_uid,  QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::credentialswidget)
{
    this->uid = json_uid;
    ui->setupUi(this);
    ui->lblSite->setText(site);
}

credentialswidget::~credentialswidget()
{
    delete ui;
}



void credentialswidget::on_LoginCopy_clicked()
{
    data_key = "login";
    ui->stackedWidget->setCurrentIndex(1);
    ui->PassCopy->setDisabled(true);
    ui->LoginCopy->setDisabled(true);
}

void credentialswidget::on_PassCopy_clicked()
{
    data_key = "password";
    ui->stackedWidget->setCurrentIndex(1);
    ui->PassCopy->setDisabled(true);
    ui->LoginCopy->setDisabled(true);
}


void credentialswidget::on_editPin_returnPressed()
{
    auto pinCode = ui->editPin->text();

    auto encrypted =  QByteArray::fromHex(m_jsonarray[this->uid].toObject()["data"].toString().toStdString().c_str());

    QByteArray decrypted;

    decryptFile(encrypted, decrypted, QCryptographicHash::hash(pinCode.toUtf8(), QCryptographicHash::Sha256));

    QJsonDocument jsonDoc = QJsonDocument::fromJson(decrypted);
    QJsonObject rootObject = jsonDoc.object();

    if(rootObject.isEmpty() || rootObject[data_key].toString().isEmpty())
    {
        ui->statusLbl->setStyleSheet("color:red");
        ui->statusLbl->setText("Invalid password");

    }
    else
    {
        QGuiApplication::clipboard()->setText(rootObject[data_key].toString());

        ui->stackedWidget->setCurrentIndex(0 );
        ui->PassCopy->setDisabled(false);
        ui->LoginCopy->setDisabled(false);

    }

    ui->editPin->setText(QString().fill('*', pinCode.size()));
    ui->editPin->clear();

    return;
}



