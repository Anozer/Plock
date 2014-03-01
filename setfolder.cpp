#include "setfolder.h"
#include "ui_setfolder.h"
#include "constants.h"
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

// Constructeur
SetFolder::SetFolder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFolder)
{
    ui->setupUi(this);
}

// Destructeur
SetFolder::~SetFolder()
{
    delete ui;
}


// Surcharge de reject pour empêcher fermeture mauvaise data
void SetFolder::reject()
{
    QApplication::beep();
}

// Show folder
void SetFolder::on_button_selectFolder_clicked()
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

// Valid modal
void SetFolder::on_button_ok_clicked()
{
    QDir folder(ui->input_folderPath->text());

    if(ui->input_folderPath->text().isEmpty())
    {
        ui->label_err->setText("You must specify a folder");
    }
    else if(folder.isRoot())
    {
        ui->label_err->setText("You can't specify the system root");
    }
    else if(!folder.exists())
    {
        ui->label_err->setText("This folder does not exist");
    }
    else
    {
        QSettings settings;
        settings.setValue(SETTINGS_FOLDERPATH, ui->input_folderPath->text());
        this->accept();
    }
}
