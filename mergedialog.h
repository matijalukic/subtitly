#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>
namespace Ui {
class mergeDialog;
}

class mergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mergeDialog(QWidget *parent = 0);
    ~mergeDialog();
    double getMinTime() const;
    int getMinChars() const;
private slots:
    //void on_mergeDialog_accepted();

private:
    Ui::mergeDialog *ui;
};

#endif // MERGEDIALOG_H
