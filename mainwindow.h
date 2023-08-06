#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QContextMenuEvent>

#include<QTabWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QTextBrowser>
#include <QProgressBar>
#include <QSlider>
#include <QColor>
#include <QComboBox>
#include <QString>
#include <QFile>

#include <QMessageBox>
#include <QInputDialog>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "jeuMario.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <string>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using std::cout; using std::endl;
using std::copy; using std::string;

#define LARGEUR_FENET 800
#define HAUTEUR_FENET 500

#define WIDTHSCREEN 800
#define HEIGHTSCREEN 600
#define TOTAL_POINTS 5000

int KeyHit();

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void arret();
    void save();
    int startGame();
    void donnerAvis();
    void nvllFenetre();
//signals:

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event)override;
#endif
// Onglets
    QTabWidget *onglets;
// Boutons
    QPushButton *m_boutonQuitter;
// Menus
    QMenu *menu;
// Actions
    QAction *fermer;
    QAction *nouvFenet;
    QAction *avis;
// Widgets
    QSpinBox *lineAge;
    QLineEdit *lineMail;
    QLineEdit *lineNom;
    QLineEdit *linePrenom;
    QComboBox *lineNation;

private:
    void createMenus();
    void createStatusBar();
};
#endif // MAINWINDOW_H
