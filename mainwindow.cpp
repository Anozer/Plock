#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "options.h"
#include "setpassword.h"
#include "setfolder.h"
#include <QDebug>
#include <iostream>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QCryptographicHash>
#include <QApplication>
#include <QSettings>


/*************************************
 *   Constructeurs et destructeurs   *
 *************************************/

// Constructeur (setup)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Construction de l'UI
    qDebug() << "\nBuilding UI";
    ui->setupUi(this);


    // Récup des infos dans les prefs
    this->updateDatas();

    // Configs de l'UI
    this->displayFolderStatus();

    // Focus sur l'input du pwd
    ui->input_pwd->setFocus();
}

// Destructeur (lock systématique)
MainWindow::~MainWindow()
{
    if(this->autolock)
    {
        this->lockFolder();
    }
    qDebug() << "\nDeleting UI";
    delete ui;
}


/****************
 *   Methodes   *
 ****************/

// Verrouillage ou déverrouillage du dossier selon le CHMOD
int MainWindow::setFolderLockState(int state)
{
    QProcess    chmodProcess;
    QStringList chmodProcess_arguments;
    int         chmodNumber;

    if(state == FOLDER_LOCKED)
    {
        chmodNumber = CHMOD_LOCKED;
        qDebug() << "\nAttempting to LOCK the folder";
    }
    else if (state == FOLDER_UNLOCKED)
    {
        chmodNumber = CHMOD_UNLOCKED;
        qDebug() << "\nAttempting to UNLOCK the folder";
    }
    else
    {
        qDebug() << "\nERROR : Unknown state... chmod will not be performed.";
        return -1;
    }

    chmodProcess_arguments << QString::number(chmodNumber) << this->folder.path();
    chmodProcess.start("chmod", chmodProcess_arguments);
    chmodProcess.waitForFinished(30000);
    qDebug() << "\tCommand \"chmod\" with arguments" << chmodProcess_arguments;

    this->folder.refresh();

    qDebug() << "\tFolder is" << ((this->folder.isReadable()) ? "unlocked" : "locked");
    if((state == FOLDER_LOCKED && this->folder.isReadable()) ||
            (state == FOLDER_UNLOCKED && !this->folder.isReadable()))
    {
        qDebug() << "\t ERROR: the folder is not in the correct state...";
        return -1;
    }

    return 0;
}
// Verrouillage du dossier via CHMOD
int MainWindow::lockFolder(void)
{
    return this->setFolderLockState(FOLDER_LOCKED);
}
// Déverrouillage du dossier via CHMOD
int MainWindow::unlockFolder(void)
{
    return this->setFolderLockState(FOLDER_UNLOCKED);
}
// Inversion de l'état de verrouillage du dossier
int MainWindow::toggleLock(void)
{
    if(this->folder.isReadable())   return this->lockFolder();
    else                            return this->unlockFolder();
}

// Affiche l'état (locked/unlocked) du dossier dans la batte d'état de l'app
void MainWindow::displayFolderStatus(void)
{
    bool isFolderReadable = this->folder.isReadable();

    ui->statusBar->showMessage((isFolderReadable) ? "Folder is unlocked" : "Folder is locked");
    ui->statusBar->setStyleSheet((isFolderReadable) ? "color:green;" : "color:blue;");
}

// Vérification du mot de passe tapé
bool MainWindow::checkPassword()
{
    QString passwordToCheck = QCryptographicHash::hash(ui->input_pwd->text().toUtf8(), QCryptographicHash::Md5).toHex();

    qDebug() << "\nTest: " << passwordToCheck << " == " << this->password << " ?";

    if(passwordToCheck == this->password)
    {
        ui->input_show->setEnabled(true);
        ui->input_toggleLock->setEnabled(true);
        ui->statusBar->clearMessage();

        qDebug() << "\tOK, password is valid";
        return true;
    }
    else
    {
        QApplication::beep();

        int initX = this->x();
        int initY = this->y();
        int deplacement = 0;

        for(int i=0; i<8; i++)
        {
            deplacement = ((i%3)-1)*5;
            this->move(initX+deplacement, initY);
        }

        ui->input_pwd->setFocus();
        ui->input_pwd->selectAll();
        ui->statusBar->showMessage("Invalid Password");
        ui->statusBar->setStyleSheet("color:red;");

        qDebug() << "\tKO, password is not valid";
        return false;
    }
}

void MainWindow::updateDatas()
{
    // Chargement des préférences de l'app
    QSettings settings;
    qDebug() << "\nLoading settings :";

    // Récup du dossier de travail
    QString folderPath_ = settings.value(SETTINGS_FOLDERPATH, "nosettings").toString();

    if (folderPath_ == "nosettings")
    {
        SetFolder folder_Window(this);
        folder_Window.exec();
        this->folder.setPath(settings.value(SETTINGS_FOLDERPATH, "nosettings").toString());
    }
    else
    {
        this->folder.setPath(folderPath_);
    }

    qDebug() << "\tWorking on " << this->folder.path();



    // Récup du mdp de l'utilisateur
    QString password_ = settings.value(SETTINGS_PASSWORD, "nosettings").toString();

    if (password_ == "nosettings")
    {
        SetPassword pwd_Window(this);
        pwd_Window.exec();

        this->password = settings.value(SETTINGS_PASSWORD, "nosettings").toString();
    }
    else
    {
        this->password = password_;
    }

    qDebug() << "\tUser password is " << this->password;


    // Récup de l'info d'autolock (default = true)
    this->autolock = settings.value(SETTINGS_AUTOLOCK, true).toBool();

    qDebug() << "\tAutolock is " << ((this->autolock)?"Enabled":"Disabled");
}


/**********************
 *   Slots / Events   *
 **********************/

// Click sur "show"
void MainWindow::on_input_show_clicked()
{
    QUrl folderUrl = "file://"+this->folder.path();
    QDesktopServices::openUrl(folderUrl);
}


// Click sur "toggle click"
void MainWindow::on_input_toggleLock_clicked()
{
    this->toggleLock();
    this->displayFolderStatus();
}

// "Entré" pressé sur le champ password
void MainWindow::on_input_pwd_returnPressed()
{
    if(checkPassword())
    {
        this->unlockFolder();
        this->displayFolderStatus();
    }
}

// Ouverture des prefs
void MainWindow::on_actionPreferences_triggered()
{
    options prefWindow(this);
    prefWindow.exec();

    this->updateDatas();
}
