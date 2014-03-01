#include "options.h"
#include "ui_options.h"
#include "constants.h"
#include "QSettings"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QCryptographicHash>

// Constructeur
options::options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::options)
{
    qDebug() << "\nOpening options UI";
    ui->setupUi(this);

    QSettings settings;
    ui->input_folderPath->setText(settings.value(SETTINGS_FOLDERPATH).toString());
    ui->input_autolock->setChecked(settings.value(SETTINGS_AUTOLOCK, true).toBool());
}

// Destructeur
options::~options()
{
    delete ui;
}

// Sélection du dossier via fenêtre de dialogue
void options::on_button_selectFolder_clicked()
{
    QDir folder((ui->input_folderPath->text().isEmpty()) ? "/" : ui->input_folderPath->text());
    QFileDialog fileDialog;

    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setDirectory((folder.exists()) ? folder.path() : "/");
    fileDialog.exec();

    if(fileDialog.result() == QFileDialog::Accepted)
    {
        ui->input_folderPath->setText(fileDialog.directory().path());
    }
}

// OK appuyé
void options::on_button_validOptions_clicked()
{
    qDebug() << "\tTrying to validate options...";
    int errors = 0;
    QSettings settings;

    // Check du dossier
    QDir folder(ui->input_folderPath->text());

    if(ui->input_folderPath->text().isEmpty())
    {
        ui->label_err->setText("You must specify a folder");
        errors++;
    }
    else if(folder.isRoot())
    {
        ui->label_err->setText("You can't specify the system root");
        errors++;
    }
    else if(!folder.exists())
    {
        ui->label_err->setText("This folder does not exist");
        errors++;
    }
    else
    {
        settings.setValue(SETTINGS_FOLDERPATH, ui->input_folderPath->text());
    }

    // Check du password
    // champs non vides ?
    if(!(ui->input_oldPwd->text().isEmpty() && ui->input_newPwd->text().isEmpty()))
    {
        QString oldPwd = QCryptographicHash::hash(ui->input_oldPwd->text().toUtf8(), QCryptographicHash::Md5).toHex();
        QString real_oldPwd = settings.value(SETTINGS_PASSWORD).toString();

        // oldPwd OK ?
        if(oldPwd == real_oldPwd)
        {
            QString newPwd = ui->input_newPwd->text();
            QString newPwd_confirm = ui->input_newPwd_confirm->text();

            // newPwd confirmé?
            if(newPwd == newPwd_confirm)
            {
                newPwd = QCryptographicHash::hash(newPwd.toUtf8(), QCryptographicHash::Md5).toHex();
                settings.setValue(SETTINGS_PASSWORD, newPwd);
            }
            else
            {
                ui->label_err->setText("Confirmation does not match");
                errors++;
            }
        }
        else
        {
            ui->label_err->setText("Old password is wrong");
            errors++;
        }
    }
    else
    {
        // pas de modif du mot de passe
        qDebug() << "\tKeeping password";
    }


    if(errors == 0)
    {
        // Check de l'auto-lock
        settings.setValue(SETTINGS_AUTOLOCK, ui->input_autolock->isChecked());
        qDebug() << "\tOptions all validated, settings updated";
        this->accept();
    }
    else
    {
        qDebug() << "\tOption not validated due to errors";
    }
}


// Cancel
void options::on_button_cancelOptions_clicked()
{
    qDebug() << "\tOptions canceled, settings not updated";
    this->reject();
}
