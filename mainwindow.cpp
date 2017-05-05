#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){


    ui->setupUi(this);

    // Konekcija za otvaranje titla
    connect(ui->actionLoad_Subtitle, &QAction::triggered, this, &MainWindow::loadSubRipTitle);
    connect(ui->actionLoad_Subtitle_2, &QAction::triggered, this, &MainWindow::loadTitle);
    connect(ui->actionLoad_MPlayer_Subtitle, &QAction::triggered, this, &MainWindow::loadMPlayer);
    connect(ui->actionLoad_MicroDVD_Subtitle, &QAction::triggered, this, &MainWindow::loadMicroDVD);

    // Konekcije za pamcenje titla
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveTitle);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsTitle);

    // Konekcije za edit menu
    connect(ui->actionMerge, &QAction::triggered, this, &MainWindow::openMergeDialog);
    connect(ui->actionSplit, &QAction::triggered, this, &MainWindow::openSplitDialog);
    connect(ui->actionSplit_Consecutive, &QAction::triggered, this, &MainWindow::openSplitConDialog);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::openShiftDialog);

    // Podesavanja tabele
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0, ui->tableWidget->width() * 0.2);
    ui->tableWidget->setColumnWidth(1, ui->tableWidget->width() * 0.2);
    ui->tableWidget->setColumnWidth(2, ui->tableWidget->width() * 0.55);

    QStringList columnNames;
    columnNames << "Time Appearance(s)" << "Time Removal(s)" << "Title text";
    ui->tableWidget->setHorizontalHeaderLabels(columnNames);
    ui->tableWidget->verticalHeader()->hide();


    // input maske za inpute
    ui->timeApp->setInputMask(tr("00:00:00,000"));
    ui->timeRem->setInputMask(tr("00:00:00,000"));
    ui->timeStampFrom->setInputMask(tr("00:00:00,000"));
    ui->timeStampTo->setInputMask(tr("00:00:00,000"));
    ui->jumpTime->setInputMask(tr("00:00:00,000"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
// Otvaranje dialoga za shift
void
MainWindow::openShiftDialog(){
    try{
        if(subtitleProcess.loadedTitle.titlesNumber() < 1)
            throw ErrSubtitle();

        bool dialogOK;
        double shiftTime = QInputDialog::getDouble(this,
                                                   tr("Shift all titles"),
                                                   tr("Shift by(s):"),
                                                   0, -99999, 99999, 3, &dialogOK);

        if(dialogOK){
            Timestamp shiftBy(shiftTime);

            SubtitleProcessing process(subtitleProcess.loadedTitle);

            process.shift(shiftBy);
            titleInserting(subtitleProcess.loadedTitle);
        }

    }
    catch(ErrSubtitle){
        subtitleChangedAlert = new QMessageBox(QMessageBox::Warning,
                                               tr("Shift failed"),
                                               tr("Shifting of the titles failed please try again."));
        subtitleChangedAlert->show();
    }
}

// Otvaranje dialoga za merge
void
MainWindow::openMergeDialog(){
    try{
        // Ako nema titlova
        if(subtitleProcess.loadedTitle.titlesNumber() <= 1)
            throw ErrSubtitle();


        mergeDialogBox = new mergeDialog(this);
        mergeDialogBox->show();

        // Ako je ok na dialogu
        if(mergeDialogBox->exec() == QDialog::Accepted){
            double minDiffSeconds = mergeDialogBox->getMinTime();
            int minCharacters = mergeDialogBox->getMinChars();

            SubtitleProcessing::minimalCharatersInSubtitle = minCharacters;
            SubtitleProcessing::titleTimeDifference = Timestamp(minDiffSeconds);

            SubtitleProcessing process(subtitleProcess.loadedTitle);
            process.merge();

            titleInserting(subtitleProcess.loadedTitle);
        }

    }
    catch(ErrSubtitle e){
        subtitleChangedAlert = new QMessageBox(QMessageBox::Warning,
                                               tr("Subtitle failed"),
                                               tr("Not enough titles to work with!"));
        subtitleChangedAlert->show();
    }
}

// Otvaranje dialoga za merge
void
MainWindow::openSplitDialog(){
    try{
        if(subtitleProcess.loadedTitle.titlesNumber() < 1)
            throw ErrSubtitle();
        bool dialogOK;
        int maxChars = QInputDialog::getInt(this,
                                            tr("Split titles"),
                                            tr("Maximum number of charactes in title:"),
                                            SubtitleProcessing::maximumCharatersInSubtitle,  10, 1000, 1, &dialogOK);
        // Ako je potvrdno
        if(dialogOK){
            SubtitleProcessing::maximumCharatersInSubtitle = maxChars;

            SubtitleProcessing process(subtitleProcess.loadedTitle);
            process.split();

            titleInserting(subtitleProcess.loadedTitle);
        }
    }
    catch(ErrSubtitle e){
        subtitleChangedAlert = new QMessageBox(QMessageBox::Warning,
                                               tr("Subtitle failed"),
                                               tr("Not enough titles to work with!"));
        subtitleChangedAlert->show();
    }
}


// Otvaranje za uzastopne
void
MainWindow::openSplitConDialog(){
    try{
        if(subtitleProcess.loadedTitle.titlesNumber() < 1)
            throw ErrSubtitle();

        bool dialogOK;
        double timeDouble = QInputDialog::getDouble(this,
                                                    tr("Split consecutive titles"),
                                                    tr("Shift title by(s)"),
                                                    0., 0., 10.,3,&dialogOK);
        Timestamp moveBy(timeDouble);
        if(dialogOK){
            SubtitleProcessing process(subtitleProcess.loadedTitle);

            process.splitConsecutive(moveBy);
            titleInserting(subtitleProcess.loadedTitle);

        }
    }
    catch(ErrSubtitle e){
        subtitleChangedAlert = new QMessageBox(QMessageBox::Warning,
                                               tr("Spliting failed"),
                                               tr("Time of the splitt is to big or there is no titles to split"));
        subtitleChangedAlert->show();
    }

}

// Load za sve tipove
void MainWindow::loadTitle(){

    QString subtitleName = QFileDialog::getOpenFileName(
                this,
                "Open Subtitle File",
                "",
                "Any file (*.*)");

    processSubtitle(subtitleName);
}
// SRT fajlovi
void MainWindow::loadSubRipTitle(){

    QString subtitleName = QFileDialog::getOpenFileName(
                this,
                "Open Subtitle File",
                "",
                "Subrip title (*.srt)");


    processSubtitle(subtitleName);
}
// MicroDVD tip
void MainWindow::loadMicroDVD(){
    QString subtitleName = QFileDialog::getOpenFileName(
                this,
                "Open Subtitle File",
                "",
                "MicroDVD title (*.sub *.txt)");


    processSubtitle(subtitleName);
}
// MPlayer tip
void MainWindow::loadMPlayer(){
    QString subtitleName = QFileDialog::getOpenFileName(
                this,
                "Open Subtitle File",
                "",
                "MPlayer title (*.sub)");


    processSubtitle(subtitleName);
}
// Ubacivanje titlova u tabelu
void MainWindow::titleInserting(Subtitles& subsToInsert){
    // Podesavanja tabele
    ui->tableWidget->setRowCount(subsToInsert.titlesNumber());

    // Za svaki title
    int currentRow = 0;
    for(const Subtitle& sub: subsToInsert.getSubtitles()){
        // fetcha and convert data
        QString timeApp, timeRem;
        timeApp = QString::fromStdString(sub.getTimeOfAppearance().timestampStirng());
        timeRem = QString::fromStdString(sub.getTimeOfRemoval().timestampStirng());
        QTableWidgetItem * firstColumn = new QTableWidgetItem(timeApp);
        firstColumn->setTextAlignment(Qt::AlignRight);
        QTableWidgetItem * secondColumn = new QTableWidgetItem(timeRem);
        secondColumn->setTextAlignment(Qt::AlignLeft);
        QTableWidgetItem * thirdColumn = new QTableWidgetItem(QString::fromStdString(sub.getString()));
        thirdColumn->setTextAlignment(Qt::AlignCenter);


        // set in table
        ui->tableWidget->setItem(currentRow, 0, firstColumn);
        ui->tableWidget->setItem(currentRow, 1, secondColumn);
        ui->tableWidget->setItem(currentRow++, 2, thirdColumn);
    }
}

// Procesuiranje titla sa aplikacijom za svaki tip title
void MainWindow::processSubtitle(QString& subtitleName){
    try{

        const string fileName = subtitleName.toStdString();
        subtitleProcess.loadTitle(fileName);

        // Ubacivanje u tabelu
        titleInserting(subtitleProcess.loadedTitle);

        QString newTitle = "Subtitly  -" + subtitleName;
        setWindowTitle(newTitle);

    }
    catch(IOError){

        fileTypeAlert = new QMessageBox(QMessageBox::Warning, tr("File type error"),tr("Selected file is not compatibile"), QMessageBox::Ok);

        fileTypeAlert->show();
    }
}

// Pamcenje titlova
void MainWindow::saveTitle(){
    try{
        QString filePath = QString::fromStdString(subtitleProcess.getSubtitleName());

        // Ako jos nismo sacuvali prvi put
        if(filePath == "")
            saveAsTitle();
        else
            subtitleProcess.saveTitle(filePath.toStdString());
    }
    catch(IOError){
        fileTypeAlert = new QMessageBox(QMessageBox::Warning, tr("File saving error"),tr("File cann\'t be saved! "), QMessageBox::Ok);

        fileTypeAlert->show();
    }
}

void MainWindow::saveAsTitle(){
    try{
        QString newSubtitleName = QFileDialog::getSaveFileName(this,
                                                           tr("Save title as"),
                                                           "",
                                                           tr("SubRip (*.srt);;MicroDVD(*.txt *.sub);;MPlayer(*.sub)"));
        if(newSubtitleName == "")
            throw IOError();

        string  subtitleName = newSubtitleName.toStdString();
        string extension = subtitleName.substr(subtitleName.find_last_of('.')+1);

        // Tip na osnovu ekstenzija
        if(extension == "srt")
        {
            subtitleProcess.saveTitleAs(subtitleName);
        }
        else if( extension == "txt"){
            subtitleProcess.saveTitleAs(subtitleName, SubtitleIO::MicroDVD);
        }
        // .sub
        else{
            // Ako smo ucitali MicroDVD pamtimo ga u tom formatu
            if(subtitleProcess.getType() == SubtitleIO::MicroDVD)
                subtitleProcess.saveTitleAs(subtitleName, SubtitleIO::MicroDVD);
            else
                subtitleProcess.saveTitleAs(subtitleName, SubtitleIO::MPlayer);
        }
    }
    catch(IOError){
        fileTypeAlert = new QMessageBox(QMessageBox::Warning, tr("File saving error"),tr("File cann\'t be saved! "), QMessageBox::Ok);
        fileTypeAlert->show();
    }
}

// Menjanje titlova
void MainWindow::on_tableWidget_cellChanged(int titleNumber, int column)
{
    try{
        Subtitle& changingSubtitle = subtitleProcess.loadedTitle.get(titleNumber);
        QString& newValue = ui->tableWidget->item(titleNumber, column)->text();

        QString oldValue;
        if(column == 0)
            oldValue = QString::fromStdString(changingSubtitle.getTimeOfAppearance().timestampStirng());
        else if(column == 1) // ako je removal
            oldValue = QString::fromStdString(changingSubtitle.getTimeOfRemoval().timestampStirng());
        else
            oldValue = QString::fromStdString(changingSubtitle.getString());

        if(oldValue != newValue){
            // Menjanje texta
            if(column == 2)
                changingSubtitle.setString(newValue.toStdString());


            // Menjanje pojavljivanja
            else if(column == 0){
                QString& oldValue = QString::fromStdString(changingSubtitle.getTimeOfAppearance().timestampStirng());

                try{
                    Timestamp newTimestamp(newValue.toStdString());

                    // ako vreme pojavljivanja nije pre vremena odjave - greska
                    if(!newTimestamp.isBefore(changingSubtitle.getTimeOfRemoval()))
                        throw ErrTimeFormatFault(ErrTimeFormatFault::NUMSEG);

                    changingSubtitle.setTimeOfAppearance(newTimestamp);
                }

                catch(ErrTimeFormatFault error){
                    ui->tableWidget->item(titleNumber, column)->setText(oldValue);
                    QString errorTitle = "Time format fault!";

                    QString errorMessage;

                    switch(error.errorType){
                        case ErrTimeFormatFault::STRERR:
                        errorMessage = "Invalid time format";
                        break;
                        case ErrTimeFormatFault::NUMSEG:
                        errorMessage = "Time appearance is not before time removal.";
                        break;
                    }

                    timeFormatAlert = new QMessageBox(QMessageBox::Warning, errorTitle, errorMessage);
                    timeFormatAlert->show();
                }

            }
            // Menjanje uklanjanja
            else if( column == 1){

                try{
                    Timestamp newTimestamp(newValue.toStdString());

                    // ako je uklanjanje posle sledeceg uklanjanja onda greska
                    if(titleNumber + 1 <= subtitleProcess.loadedTitle.titlesNumber() &&
                            subtitleProcess.loadedTitle.get(titleNumber+1).getTimeOfRemoval().isBefore(newTimestamp))
                        throw ErrTimeFormatFault(ErrTimeFormatFault::NUMSEG);

                    changingSubtitle.setTimeOfRemoval(newTimestamp);
                }

                catch(ErrTimeFormatFault error){
                    ui->tableWidget->item(titleNumber, column)->setText(oldValue);
                    QString errorTitle = "Time format fault!";
                    QString errorMessage;

                    if(error.errorType == ErrTimeFormatFault::STRERR)
                        errorMessage= "Invalid time format of the changed field.";
                    else
                        errorMessage = "Removal time must be before next time of removal.";

                    timeFormatAlert = new QMessageBox(QMessageBox::Warning, errorTitle, errorMessage, QMessageBox::Close);
                    timeFormatAlert->show();
                }
            }
        }
    }
    catch(ErrSubtitle){
        subtitleChangedAlert = new QMessageBox(QMessageBox::Warning, tr("Subtitle Changed Error"), tr("Can\'t  change this title"));
        subtitleChangedAlert->show();
    }
}

void MainWindow::on_insertSubtitleBtn_clicked()
{
    try{
        QString timeApp, timeRem, subText;

        timeApp = ui->timeApp->text();
        timeRem = ui->timeRem->text();
        subText = ui->titleText->toPlainText();

        // Prazan title
        if(subText == "")
            throw ErrSubtitle();

        Subtitle newSubtitle = Subtitle(Timestamp(timeApp.toStdString()), Timestamp(timeRem.toStdString()), subText.toStdString());

        if(subtitleProcess.loadedTitle.titlesNumber() > 0)
        {
            int place = subtitleProcess.loadedTitle.findPlace(newSubtitle);
            // ako smo pronasli poziciju
            if(place != -1)
                subtitleProcess.loadedTitle.insertAt(place, newSubtitle);
            else
                subtitleProcess.loadedTitle.addBack(newSubtitle);
        }
        else
            subtitleProcess.loadedTitle.addBack(newSubtitle);

        titleInserting(subtitleProcess.loadedTitle);

    }
    catch(ErrSubtitle e){
        subtitleInsertAlert = new QMessageBox(QMessageBox::Warning, tr("Insertition failed"), tr("Cannot insert that format"));
        subtitleInsertAlert->show();

    }
    catch(ErrTimeFormatFault e){
        timeFormatAlert = new QMessageBox(QMessageBox::Warning, tr("Time format"), tr("Cannot insert that format"));
        timeFormatAlert->show();
    }

}

void MainWindow::on_deleteBtn_clicked()
{
    try{
        QString timeFrom, timeTo;
        timeFrom = ui->timeStampFrom->text();
        timeTo = ui->timeStampTo->text();


        Timestamp fromTime(timeFrom.toStdString());
        Timestamp toTime(timeTo.toStdString());

        // Ako nisu ispravna vremena
        if(!fromTime.isBefore(toTime))
            throw ErrTimeFormatFault();

        subtitleProcess.loadedTitle.erase(fromTime, toTime);

        titleInserting(subtitleProcess.loadedTitle);
    }
    catch(ErrTimeFormatFault e){
        timeFormatAlert = new QMessageBox(QMessageBox::Warning, tr("Time format"), tr("Cannot use that time format"));
        timeFormatAlert->show();

    }
}

// Kliknuto na celiju
void MainWindow::on_tableWidget_cellClicked(int row, int column){
    ui->timeApp->setText(ui->tableWidget->item(row,0)->text());
    ui->timeRem->setText(ui->tableWidget->item(row,1)->text());
    ui->titleText->setText(ui->tableWidget->item(row,2)->text());
    ui->timeStampFrom->setText(ui->tableWidget->item(row,0)->text());
    ui->timeStampTo->setText(ui->tableWidget->item(row,1)->text());
}
// Skok na vreme
void MainWindow::on_jumpToBtn_clicked()
{
    try{

        QString timeJump = ui->jumpTime->text();
        Timestamp jumpTime(timeJump.toStdString());

        int indexToJump = subtitleProcess.loadedTitle.jumpTo(jumpTime);

        ui->tableWidget->setCurrentCell(indexToJump, 2);

    }
    catch(ErrTimeFormatFault){
        timeFormatAlert = new QMessageBox(QMessageBox::Warning, tr("Time format"), tr("Cannot use that time format"));
        timeFormatAlert->show();
    }

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "25")
        SubtitleIO::framesPerSecond = 25.;
    else
        SubtitleIO::framesPerSecond = 23.976;

}
