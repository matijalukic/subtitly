#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "SubtitleApp.h"
#include "mergedialog.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // Sva ucitavanja titlova
    void loadTitle();
    void loadSubRipTitle();
    void loadMicroDVD();
    void loadMPlayer();


    // Pamcenje titlova
    void saveTitle();
    void saveAsTitle();

    // procesuiranje titlova
    void processSubtitle(QString&);
    // ubacivanje titlova
    void titleInserting(Subtitles&);

    // Dijalozi za procese
    void openMergeDialog();
    void openSplitDialog();
    void openSplitConDialog();
    void openShiftDialog();

private slots:
    void on_tableWidget_cellChanged(int row, int column);

    void on_insertSubtitleBtn_clicked();

    void on_deleteBtn_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_jumpToBtn_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    SubtitleApp subtitleProcess;
    QMessageBox * fileTypeAlert;
    QMessageBox * subtitleChangedAlert;
    QMessageBox * timeFormatAlert;
    QMessageBox * subtitleInsertAlert;
    int matija =0;
    Ui::MainWindow *ui;
    mergeDialog * mergeDialogBox;
};

#endif // MAINWINDOW_H
