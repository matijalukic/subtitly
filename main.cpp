#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication program(argc, argv);
    MainWindow w;
    w.setWindowTitle("Subtitly");
    w.show();

    return program.exec();
}
