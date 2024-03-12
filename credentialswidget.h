#ifndef CREDENTIALSWIDGET_H
#define CREDENTIALSWIDGET_H

#include <QDialog>

namespace Ui {
class credentialswidget;
}

class credentialswidget : public QDialog
{
    Q_OBJECT

public:
    explicit credentialswidget(const QString &site, int json_uid,  QWidget *parent = nullptr);
    ~credentialswidget();

private slots:

    void on_LoginCopy_clicked();

    void on_editPin_returnPressed();

    void on_PassCopy_clicked();

private:
    Ui::credentialswidget *ui;
    int uid;
    QString data_key = "";
};

#endif // CREDENTIALSWIDGET_H
