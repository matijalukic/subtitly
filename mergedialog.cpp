#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "mainwindow.h"
#include "SubtitleProcessing.h"
mergeDialog::mergeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mergeDialog)
{
    ui->setupUi(this);
    ui->minimalCharacters->setValue(SubtitleProcessing::minimalCharatersInSubtitle);
    ui->minimalTime->setValue(SubtitleProcessing::titleTimeDifference.getSeconds());
}

mergeDialog::~mergeDialog()
{
    delete ui;
}


double
mergeDialog::getMinTime() const{
    return ui->minimalTime->value();
}

int
mergeDialog::getMinChars() const{
    return ui->minimalCharacters->value();
}
