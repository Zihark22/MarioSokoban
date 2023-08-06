#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    bool traducteurOk=translator.load(QString("qt_") + locale, QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    if(traducteurOk)
        app.installTranslator(&translator);

    MainWindow fenetre;
    fenetre.setWindowTitle("Mario Sokoban");
    fenetre.show();

    return app.exec();
}
