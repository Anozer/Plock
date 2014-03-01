#ifndef SETPASSWORD_H
#define SETPASSWORD_H

#include <QDialog>

namespace Ui {
class SetPassword;
}

class SetPassword : public QDialog
{
    Q_OBJECT

public:
    explicit SetPassword(QWidget *parent = 0);
    ~SetPassword();
    void reject();

private slots:
    void on_button_ok_clicked();

private:
    Ui::SetPassword *ui;
};

#endif // SETPASSWORD_H
