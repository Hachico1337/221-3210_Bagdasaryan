#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qjsonarray.h"
#include <QMainWindow>
inline QJsonArray m_jsonarray;
inline int decryptFile(const QByteArray & encryptedBytes , QByteArray &decryptedBytes, QByteArray key_qba);
inline int encryptFile( QByteArray & encryptedBytes, const QByteArray & decryptedBytes, QByteArray key_qba );
class Cridential
{
    QString hostname;
    QString login;
    QString password;

};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool ReadJSON(QString pinCode);

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_editPin_returnPressed();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
