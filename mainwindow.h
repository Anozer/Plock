#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int setFolderLockState(int state);
    int lockFolder(void);
    int unlockFolder(void);
    int toggleLock(void);
    void displayFolderStatus(void);
    bool checkPassword(void);
    void updateDatas(void);

private slots:
    void on_input_show_clicked();
    void on_input_toggleLock_clicked();
    void on_input_pwd_returnPressed();
    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;

    QDir    folder;
    QString password;
    bool    autolock;
};

#endif // MAINWINDOW_H
