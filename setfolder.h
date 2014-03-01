#ifndef SETFOLDER_H
#define SETFOLDER_H

#include <QDialog>

namespace Ui {
class SetFolder;
}

class SetFolder : public QDialog
{
    Q_OBJECT

public:
    explicit SetFolder(QWidget *parent = 0);
    ~SetFolder();
    void reject();

private slots:
    void on_button_ok_clicked();

    void on_button_selectFolder_clicked();

private:
    Ui::SetFolder *ui;
};

#endif // SETFOLDER_H
