#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "credentialswidget.h"
#include <openssl/evp.h>
#include <QCryptographicHash>

int decryptFile(const QByteArray & encryptedBytes, QByteArray & decryptedBytes, QByteArray key_qba )
{

    QByteArray iv_qba = QByteArray::fromHex("000102030405060708090a0b0c0d0e0f");
    unsigned char key[32], iv[16];
    memmove(key, key_qba.data(), 32);
    memmove(iv, iv_qba.data(), 16);


    const int bufferLen = 256;
    unsigned char encryptedBuffer[bufferLen] = {0}, decryptedBuffer[bufferLen] = {0};
    int encryptedLen, decryptedLen;
    QDataStream encryptedStream(encryptedBytes);
    QDataStream decryptedStream(&decryptedBytes, QIODevice::ReadWrite);


    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    do {
        encryptedLen = encryptedStream.readRawData((char*)(encryptedBuffer), bufferLen);
        if (!EVP_DecryptUpdate(ctx, decryptedBuffer, &decryptedLen,
                               encryptedBuffer, encryptedLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        decryptedStream.writeRawData((char*)(decryptedBuffer), decryptedLen);
    } while (encryptedLen > 0);

    if (!EVP_DecryptFinal_ex(ctx, decryptedBuffer, &decryptedLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    //фикс того что не дописывало симовлы
    decryptedStream.writeRawData((char*)(decryptedBuffer), decryptedLen);
    EVP_CIPHER_CTX_free(ctx);

    return 1;
}

int encryptFile( QByteArray & encryptedBytes, const QByteArray & decryptedBytes, QByteArray key_qba )
{
    QByteArray iv_qba = QByteArray::fromHex("000102030405060708090a0b0c0d0e0f");
    unsigned char key[32], iv[16];
    memmove(key, key_qba.data(), 32);
    memmove(iv, iv_qba.data(), 16);



    const int bufferLen = 256;
    unsigned char encryptedBuffer[bufferLen] = {0}, decryptedBuffer[bufferLen] = {0};
    int encryptedLen, decryptedLen;
    QDataStream decryptedStream(decryptedBytes);
    QDataStream encryptedStream(&encryptedBytes, QIODevice::ReadWrite);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    do {
        decryptedLen = decryptedStream.readRawData((char*)(decryptedBuffer), bufferLen);
        if (!EVP_EncryptUpdate(ctx, encryptedBuffer, &encryptedLen,
                               decryptedBuffer, decryptedLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        encryptedStream.writeRawData((char*)(encryptedBuffer), encryptedLen);
    } while (decryptedLen > 0);
    if (!EVP_EncryptFinal_ex(ctx, encryptedBuffer, &encryptedLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    encryptedStream.writeRawData((char*)(encryptedBuffer), encryptedLen);
    EVP_CIPHER_CTX_free(ctx);
    return 1;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //qDebug() << ReadJSON("2509");
    //on_lineEdit_textEdited(NULL);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ReadJSON(QString pinCode)
{
    QFile jsonFile("encrypted.json");
    jsonFile.open(QFile::ReadOnly);

    QByteArray hex_encrypted = jsonFile.readAll();

    jsonFile.close();

    QByteArray encrypted = QByteArray::fromHex(hex_encrypted);

    QByteArray decrypted;

    decryptFile(encrypted, decrypted, QCryptographicHash::hash(pinCode.toUtf8(), QCryptographicHash::Sha256));

    //qDebug() << decrypted ;


    //encryptFile(encrypted, decrypted, QCryptographicHash::hash(pinCode.toUtf8(), QCryptographicHash::Sha256));

    //qDebug() << encrypted.toHex() ;

    //QFile encryptedFile("decrypted.json");
    //encryptedFile.open(QIODevice::WriteOnly);
    //encryptedFile.write(encrypted.toHex());
    //encryptedFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(decrypted);
    QJsonObject rootObject = jsonDoc.object();


    if (!jsonDoc.isObject())
        return false;


    m_jsonarray = rootObject["credentials"].toArray();

    if(!m_jsonarray.size())
        return false;

    return true;
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    QString whiteList = arg1;

    ui->listWidget->clear();

    for(int i = 0 ; i < m_jsonarray.size(); i++)
    {
        auto siteValue = m_jsonarray[i].toObject()["site"].toString();
        if(!whiteList.isEmpty() && !siteValue.contains(whiteList))
            continue;

        QListWidgetItem * newItem= new QListWidgetItem();

        credentialswidget * itemWidget =
            new credentialswidget(siteValue, i);
        newItem->setSizeHint({361, 60});
        ui->listWidget->addItem(newItem);
        ui->listWidget->setItemWidget(newItem, itemWidget);
    }
    return;
}




void MainWindow::on_editPin_returnPressed()
{
    auto pinCode = ui->editPin->text();
    if(!pinCode.size() || !ReadJSON(pinCode))
    {
        ui->statusLbl->setStyleSheet("color:red");
        ui->statusLbl->setText("Invalid password");

    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
        on_lineEdit_textEdited(NULL);
    }

    ui->editPin->setText(QString().fill('*', pinCode.size()));
    ui->editPin->clear();

    return;

}

