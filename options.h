#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class options;
}

class options : public QDialog
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = 0);
    ~options();

private slots:
    void on_button_selectFolder_clicked();
    void on_button_validOptions_clicked();
    void on_button_cancelOptions_clicked();

private:
    Ui::options *ui;
};

#endif // OPTIONS_H
