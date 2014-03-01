#include "setpassword.h"
#include "ui_setpassword.h"
#include "constants.h"
#include <QSettings>
#include <QCryptographicHash>

// Constructeur
SetPassword::SetPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPassword)
{
    ui->setupUi(this);
}

// Destructeur
SetPassword::~SetPassword()
{
    delete ui;
}

// OK
void SetPassword::on_button_ok_clicked()
{
    QString pwd = ui->input_newPwd->text();
    QString pwd_confirmed = ui->input_confirmNewPwd->text();


    if(pwd != pwd_confirmed)
    {
        ui->label_badConfirmation->setText("Password not confirmed");
    }
    else if (pwd == "")
    {
        ui->label_badConfirmation->setText("You must enter a password");
    }
    else
    {
        pwd = QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Md5).toHex();
        QSettings settings;
        settings.setValue(SETTINGS_PASSWORD, pwd);
        this->accept();
    }
}

// surcharge de reject pour empêcher fermeture mauvais pwd
void SetPassword::reject()
{
    QApplication::beep();
}
