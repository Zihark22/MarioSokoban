#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QTranslator>

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>

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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "jeuMario.h"

#define LARGEUR_FENET 600
#define HAUTEUR_FENET 400

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
    char source[60]="/Users/badajozj/Desktop/MarioApp/MarioDocs/";
public slots:
    void arret();
    void save();
    int startGame();
//signals:
protected:
// Onglets
    QTabWidget *onglets;
// Boutons
    QPushButton *m_boutonQuitter;
// Menus
    QMenu *menuPages;
    QMenu *menuContact;
    QMenu *menuAide;

    QMenu *menuJutsus;
    QMenu *menuPersonnages;
QAction *fermer;
//Pages
    QAction *accueil;
    QAction *histoire;
    QAction *ninjutsu;
    QAction *dodjutsu;
    QAction *taijutsu;
    QAction *armes;
    //Persos
        QAction *ninjas;
        QAction *kages;
        QAction *demons;

private:
    void createMenus();
    void createStatusBar();
};
#endif // MAINWINDOW_H
