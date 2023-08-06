#include "mainwindow.h"
#include "jeuMario.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define TOTAL_POINTS 5000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(LARGEUR_FENET,HAUTEUR_FENET);
    //this->setStyleSheet("QPushbutton{}");

    // 1 : Créer le QTabWidget (onglets)
    onglets = new QTabWidget(this);
    onglets->setGeometry(0,30,LARGEUR_FENET, HAUTEUR_FENET-30);

    QPalette palette;
        palette.setColor(QPalette::Text,QColor(255,255,255));
        palette.setColor(QPalette::Base,QColor(0,0,0));



    // 2 : Créer les pages, en utilisant un widget parent pour contenir chacune des pages
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QLabel *page3 = new QLabel; // Comme un QLabel est aussi un QWidget (il en hérite), on peut aussi s'en servir de page
    page1->setStyleSheet("background-color: rgb(0,0,150);");
    page2->setStyleSheet("background-color: rgb(150,0,0);");
    //page3->setStyleSheet("background-color: lime;");
    QLabel *label_nom = new QLabel(tr("<strong>NOM</strong>              :"));
        lineNom = new QLineEdit(tr("Entrez votre nom"));
    QLabel *label_prenom = new QLabel(tr("<strong>PRENOM </strong>      :"));
        linePrenom = new QLineEdit("Entrez votre prénom");
    QLabel *label_age = new QLabel(tr("<strong>AGE</strong>               :"));
        lineAge = new QSpinBox;
        lineAge->setRange(0,100);
        lineAge->setValue(20);
    QLabel *label_nation = new QLabel(tr("<strong>NATIONALITE</strong>  :"));
    lineNation = new QComboBox;
        lineNation->addItem(tr("Français"));
        lineNation->addItem(tr("Anglais"));
        lineNation->addItem(tr("Espagnol"));
        lineNation->addItem(tr("Américain"));
        lineNation->addItem(tr("Portugais"));
        lineNation->addItem(tr("Japonais"));
        lineNation->addItem(tr("Chinois"));
        lineNation->addItem(tr("Allemand"));
        lineNation->addItem(tr("Italien"));
        lineNation->addItem(tr("Argentin"));
        lineNation->addItem(tr("Brésilien"));
        lineNation->addItem(tr("Australien"));
        lineNation->setMinimumHeight(30);
    QLabel *label_mail = new QLabel(tr("<strong>E-MAIL</strong>       :"));
        lineMail = new QLineEdit(tr("exemple@gmail.com"));
    QPushButton *boutSauvegarder = new QPushButton(tr("Sauvegarder"));
        boutSauvegarder->setMinimumSize(90,20);
        boutSauvegarder->setStyleSheet("background-color:black;border-radius:10;");
        connect(boutSauvegarder,SIGNAL(clicked()),this,SLOT(save()));
        boutSauvegarder->setStatusTip(tr("Appuyer pour sauvegarder vos données et commencer à jouer."));
    m_boutonQuitter = new QPushButton(tr("Annuler"));
        m_boutonQuitter->setMinimumSize(60,20);
        m_boutonQuitter->setStyleSheet("background-color:black;border-radius:10;");
        connect(m_boutonQuitter,SIGNAL(clicked()),qApp,SLOT(quit()));
        m_boutonQuitter->setStatusTip(tr("Appuyer pour quitter l'application."));

    //QVBoxLayout *vbox1 = new QVBoxLayout;
    QGridLayout *layout1 = new QGridLayout;
    layout1->setContentsMargins(5, 5, 5, 5);

    layout1->addWidget(label_nom,0,1);
    layout1->addWidget(lineNom,0,2);
    layout1->addWidget(label_prenom,1,1);
    layout1->addWidget(linePrenom,1,2);
    layout1->addWidget(label_age,2,1);
    layout1->addWidget(lineAge,2,2);
    layout1->addWidget(label_nation,3,1);
    layout1->addWidget(lineNation,3,2);
    layout1->addWidget(label_mail,4,1);
    layout1->addWidget(lineMail,4,2);
    layout1->addWidget(boutSauvegarder,10,3);
    layout1->addWidget(m_boutonQuitter,10,0);

    page1->setLayout(layout1);

    // Page 2
    QLabel *titre = new QLabel("MARIO SOKOBAN");
        titre->setPixmap(QPixmap(chemin((char*)"titreMario.png")));
        titre->setAlignment(Qt::AlignCenter);
        titre->setScaledContents(Qt::KeepAspectRatio);
    QPushButton *boutonJouer = new QPushButton(tr("JOUER"));
        connect(boutonJouer,SIGNAL(clicked()),this,SLOT(startGame()));
        boutonJouer->setStatusTip(tr("Appuyer pour jouer à Mario Sokoban"));
        boutonJouer->setAutoFillBackground(true);
        boutonJouer->update();
        boutonJouer->setStyleSheet("background-color:black;border-radius:20px;color:rgb(255,0,200);font-size:20px;font-family:Arial;text-decoration:bold;");
        boutonJouer->setMinimumHeight(40);
    QTextEdit *messageAccueil = new QTextEdit(tr("Bienvenu dans l'application Mario'App créé par Julien Badajoz à partir du modèl d'OpenClassroom pour réaliser le jeu mario Mario Sokoban en SDL via un executable et s'inicier à Qt Creator."));
        messageAccueil->setAlignment(Qt::AlignLeft);
        messageAccueil->setFixedHeight(70);
        messageAccueil->setPalette(palette);
        messageAccueil->setStyleSheet("border-radius: 20px;font-size:18px;font-family:Times New Roman;");
        messageAccueil->setFrameStyle( QFrame::Raised );
        messageAccueil->setReadOnly(true);
        messageAccueil->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        messageAccueil->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPushButton *boutonQuitter = new QPushButton(tr("Quitter"));
        connect(boutonQuitter,SIGNAL(clicked()),qApp,SLOT(quit()));
        boutonQuitter->setStatusTip(tr("Appuyer pour quitter l'application."));
        boutonQuitter->setStyleSheet("background-color:black;border-radius:10;");
        boutonQuitter->setMaximumWidth(60);
        boutonQuitter->setMinimumHeight(30);
    QLabel *imgCaisse = new QLabel;
        imgCaisse->setPixmap(QPixmap(chemin((char*)"caisse.jpg")));
        imgCaisse->setAlignment(Qt::AlignCenter);
    QLabel *imgCaisse2 = new QLabel;
        imgCaisse2->setPixmap(QPixmap(chemin((char*)"caisse.jpg")));
        imgCaisse2->setAlignment(Qt::AlignCenter);
    QLabel *imgMarioFace=new QLabel;
        imgMarioFace->setPixmap(QPixmap(chemin((char*)"marioFace.png")));
        imgMarioFace->setAlignment(Qt::AlignCenter);
        imgMarioFace->setScaledContents(Qt::KeepAspectRatio);
    QLabel *imgMarioProfil=new QLabel;
        imgMarioProfil->setPixmap(QPixmap(chemin((char*)"marioProfil.png")));
        imgMarioProfil->setAlignment(Qt::AlignCenter);
        imgMarioProfil->setScaledContents(Qt::KeepAspectRatio);


    QGridLayout *layout2 = new QGridLayout;
    layout2->setContentsMargins(5, 5, 5, 40);
    //layout2->setColumnMinimumWidth(0,100);
    layout2->setColumnMinimumWidth(1,50);
    layout2->setColumnMinimumWidth(2,350);
    layout2->setColumnMinimumWidth(3,50);
    //layout2->setColumnMinimumWidth(4,100);

    int nbcol=4,nblign=4;
    //layout2->addWidget(messageEspace,0,0,1,1);
    layout2->addWidget(titre,0,1,2,3);
    layout2->addWidget(boutonJouer,2,2,1,1);
    layout2->addWidget(messageAccueil,3,1,1,3);
    layout2->addWidget(boutonQuitter,nblign,nbcol-1,1,1);
    layout2->addWidget(imgCaisse,2,1,1,1);
    layout2->addWidget(imgCaisse2,2,3,1,1);
    layout2->addWidget(imgMarioFace,0,0,nbcol,1);
    layout2->addWidget(imgMarioProfil,0,nbcol,nbcol,1);

    page2->setLayout(layout2);

    // Page 3
    QGridLayout *layout3 = new QGridLayout;
        layout3->setContentsMargins(5, 5, 5, 40);
    QLabel *lab_jeu=new QLabel(tr("JEU"));
        lab_jeu->setAlignment(Qt::AlignCenter);
        lab_jeu->setMinimumHeight(40);
        lab_jeu->setStyleSheet("background-color:red;font-size:20px;font-family:Arial;");
    QLabel *lab_edit=new QLabel(tr("EDIT"));
        lab_edit->setContentsMargins(0,0,0,0);
        lab_edit->setAlignment(Qt::AlignCenter);
        lab_edit->setMinimumHeight(40);
        lab_edit->setStyleSheet("background-color:blue;font-size:20px;font-family:Arial;");
    QTextEdit *msg_jeu=new QTextEdit(tr("Dans la partie JEU, l'objectif est que Mario déplace toutes les caisses sur chaque objectif(rond vert) de la carte pour passer au niveau suivant. \nIl y a en tout 20 niveaux pré-enregistrés."));
        msg_jeu->setReadOnly(true);
        msg_jeu->setStyleSheet("background-color:red;font-size:13px;");
    QTextEdit *msg_edit=new QTextEdit(tr("Dans la partie EDIT, le joueur peut créer son propre niveau et le sauvegarder pour ensuite le jouer dans la partie JEU(à la suite des 20 premiers niveaux)."));
        msg_edit->setReadOnly(true);
        msg_edit->setStyleSheet("background-color:blue;font-size:13px;");
    QLabel *com_jeu=new QLabel;
        com_jeu->setPixmap(QPixmap(chemin((char*)"regles_jeu.png")));
        com_jeu->setAlignment(Qt::AlignCenter);
        com_jeu->setScaledContents(Qt::KeepAspectRatio);
    QLabel *com_edit=new QLabel;
        com_edit->setPixmap(QPixmap(chemin((char*)"regles_edit.jpg")));
        com_edit->setAlignment(Qt::AlignCenter);
        com_edit->setScaledContents(Qt::KeepAspectRatio);

    layout3->setColumnMinimumWidth(1,40);
    layout3->addWidget(lab_jeu,0,0);
    layout3->addWidget(lab_edit,0,2);
    layout3->addWidget(msg_jeu,1,0);
    layout3->addWidget(msg_edit,1,2);
    layout3->addWidget(com_jeu,2,0);
    layout3->addWidget(com_edit,2,2);

    page3->setLayout(layout3);

    // 4 : ajouter les onglets au QTabWidget, en indiquant la page qu'ils contiennent
    onglets->addTab(page1, tr("Formulaire"));
    onglets->addTab(page2, tr("Jeu"));
    onglets->addTab(page3, tr("Règles"));

    onglets->setCurrentIndex(0);

    createMenus();
    createStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    // Barre de menu
    menu=menuBar()->addMenu(tr("Mario Game"));

    // Barre d'outils
    QToolBar *outils = addToolBar(tr("outils"));
        outils->setStyleSheet("color:red;font-size:15px;font-family:Arial;");

    // Actions
    nouvFenet = new QAction(tr("Nouvelle fenêtre"), this);
    avis = new QAction(tr("Avis"), this);
    fermer = new QAction(tr("Fermer"), this);

    menu->addAction(nouvFenet);
    menu->addAction(avis);
    menu->addAction(fermer);

    outils->addAction(nouvFenet);
    outils->addAction(avis);
    outils->addSeparator();
    outils->addAction(fermer);

    // Indications
    nouvFenet->setStatusTip(tr("Créer une nouvelle fenêtre"));
    avis->setStatusTip(tr("Appuyer pour donner votre avis sur le jeu"));
    fermer->setStatusTip(tr("Appuyer pour quitter l'appli"));

    // Raccourcis
    nouvFenet->setShortcut(QKeySequence("Ctrl+N"));
    avis->setShortcut(QKeySequence("Ctrl+A"));
    fermer->setShortcut(QKeySequence("Ctrl+F"));

    //Connections
    connect(nouvFenet, &QAction::triggered,this,&MainWindow::nvllFenetre);
    connect(avis,&QAction::triggered,this,&MainWindow::donnerAvis);
    connect(fermer, &QAction::triggered,this,&MainWindow::arret);

}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Prêt"));
    statusBar()->setStyleSheet("background-color: black;color:white;");
}
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(nouvFenet);
    menu.addAction(avis);
    menu.addAction(fermer);
    menu.exec(event->globalPos());
}
#endif
void MainWindow::nvllFenetre()
{

}
void MainWindow::arret()
{
    emit m_boutonQuitter->clicked();
}
void MainWindow::save()
{
    int ageJoueur;
    QString nomJoueur;
        string nomJ;
    QString prenomJoueur;
        string prenomJ;
    QString nationJoueur;
        string nationJ;
    QString mailJoueur;
        string mailJ;

    nomJoueur=lineNom->text();
    prenomJoueur=linePrenom->text();
    mailJoueur=lineMail->text();

    if(nomJoueur=="Entrez votre nom" || prenomJoueur=="Entrez votre prénom" || mailJoueur=="exemple@gmail.com"){
        if(nomJoueur=="Entrez votre nom")
            QMessageBox::warning(this,tr("Erreur nom"),tr("Veuillez réessayer en rentrant votre nom"));
        else if(prenomJoueur=="Entrez votre prénom")
            QMessageBox::warning(this,tr("Erreur prénom"),tr("Veuillez réessayer en rentrant votre prénom"));
        else if(mailJoueur=="exemple@gmail.com")
            QMessageBox::warning(this,tr("Erreur mail"),tr("Veuillez réessayer en rentrant votre adresse mail."));
    }
    else{
        ageJoueur=lineAge->value();
        nomJoueur=lineNom->text();
        nomJ = nomJoueur.toStdString();
        prenomJoueur=linePrenom->text();
        prenomJ = prenomJoueur.toStdString();
        nationJoueur=lineNation->currentText();
        nationJ = nationJoueur.toStdString();
        mailJoueur=lineMail->text();
        mailJ = mailJoueur.toStdString();

        onglets->setCurrentIndex(1);

        string const nomFichier(chemin((char*)"joueurs.txt"));
        ofstream flux(nomFichier.c_str(), ios::app);
        if(flux)
        {
            //flux << "Prénom" << "\t" << "Nom" << "\t" << "Age" << "\t" << "Nationalité" << "\t" << "e-mail" << endl;
            flux << prenomJ << "\t" << nomJ << "\t" << ageJoueur << "\t" << nationJ << "\t" << mailJ << endl;
        }
        else
            cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;

        onglets->setTabVisible(0,false);
        QMessageBox::information(this,tr("Message de bienvenu"),QString(tr("Bienvenu, %1 !\nVos informations ont bien été enregistrées.")).arg(prenomJoueur));
    }

}
void MainWindow::donnerAvis()
{
    int nbrVisites=0;
    int nbrLikes=0;
    int nbrDislikes=0;
    int position=0;
    string ligne;
    string const fichier(chemin((char*)"chiffres.txt"));
    string const Fcomments(chemin((char*)"comments.txt"));

    ifstream fluxLecture(fichier.c_str());
    getline(fluxLecture,ligne);
    position=fluxLecture.tellg();
    fluxLecture >> nbrVisites;
    fluxLecture >> nbrLikes;
    fluxLecture >> nbrDislikes;
    fluxLecture.close();

    int reponse=QMessageBox::question(this,tr("Donne ton avis"),tr("As-tu aimer l'appli ?"),QMessageBox::Yes|QMessageBox::No);

    ofstream flux(fichier.c_str(), ios::app);
    if(flux)
    {
        flux.seekp(position, ios::beg);
        //flux << "VISITES" << "\t" << "LIKES" << "\t" << "DISLIKES" << endl;
        if(reponse==QMessageBox::Yes){
            QMessageBox::information(this,tr("Approuve l'appli"),tr("Vous avez aimez."));
            flux << nbrVisites+1 << "\t" << nbrLikes+1 << "\t" << nbrDislikes << endl;
        }
        else if(reponse==QMessageBox::No){
            QMessageBox::information(this,tr("N'approuve pas l'appli"),tr("Vous n'avez pas aimez."));
            QString comments=QInputDialog::getText(this,tr("Commentaires"),tr("Pourquoi n'avez-vous pas aimez ?"));
            string com=comments.toStdString();
            flux << nbrVisites+1 << "\t" << nbrLikes << "\t" << nbrDislikes+1 << endl;
            ofstream fluxComments(Fcomments.c_str(), ios::app);
            if(fluxComments)
                fluxComments << com << endl;
            else
                cout << "Erreur d'ouverture fichier commentaires" << endl;
        }
        else
            QMessageBox::warning(this,tr("Erreur"),tr("La réponse n'a pas été sauvegardé."));
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }
}

int KeyHit()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
        if (e.type == SDL_KEYDOWN)
            return 1;
    return 0;
}

int MainWindow::startGame()
{
    this->hide();
    //this->setWindowModality(Qt::ApplicationModal);


    SDL_Surface *menu = NULL, *instructJeu = NULL, *instructEdit = NULL, *text=NULL;
        SDL_Event event;
        SDL_Surface* tmp,*tmp2,*screen=SDL_CreateRGBSurface (0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
        int continuer = 1;
        SDL_Gif* gif,* gif2;
        SDL_Rect position,position2,pos2,posTitre;
        SDL_Texture *Tri,* Tri2,* texture,* instText;

        SDL_Init(SDL_INIT_VIDEO);

        SDL_Window* window = SDL_CreateWindow("Mario Sokoban", // creates a window
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);

        Uint32 render_flags = SDL_RENDERER_ACCELERATED;
        SDL_Renderer *fenetre = SDL_CreateRenderer(window, -1, render_flags);
        SDL_SetWindowIcon(window,IMG_Load(chemin((char*)"caisse.jpg")));

        menu = IMG_Load(chemin((char*)"menu.jpg"));
        if (menu == NULL)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Erreur Load menu > %s", TTF_GetError());

        text = IMG_Load(chemin((char*)"titreMarioGIF.png"));
        if (text == NULL)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Erreur Load text=>titre Mario > %s", TTF_GetError());

        instructJeu = IMG_Load(chemin((char*)"instructions_jeu.png"));
        if (instructJeu == NULL)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Erreur Load instructions jeu > %s", TTF_GetError());

        instructEdit = IMG_Load(chemin((char*)"instructions.jpg"));
        if (instructEdit == NULL)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Erreur Load instruction edit > %s", TTF_GetError());


        SDL_Rect positionMenu={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
        SDL_Rect src1={0, 0, 0, 0};
        SDL_Rect pos={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};

        SDL_Texture* test=SDL_CreateTextureFromSurface(fenetre,text);
        SDL_QueryTexture(test, NULL, NULL, &posTitre.w, &posTitre.h);
        posTitre.y=0;
        posTitre.x=LARGEUR_FENETRE/2-posTitre.w/2;
        SDL_DestroyTexture(test);


        if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
            exit(EXIT_FAILURE);
        }

        Mix_Music* soundA = Mix_LoadMUS(chemin((char*)"SuperMarioBros.wav")); // charge la musique

        Uint8 volume = MIX_MAX_VOLUME/2; Mix_VolumeMusic(volume); // Mets le volume a 0
        Mix_PlayMusic(soundA,-1); // Joue soundA 1 fois sur le canal 1

        ////////////////////////////////////////  GIF  //////////////////////////////////////////////

        gif = SDLLoadGif(chemin((char*)"supermario.gif"));

        gif2 = SDLLoadGif(chemin((char*)"key.gif"));

        while(!KeyHit())
        {
            tmp2 = SDLGifAutoFrame(gif2);
            tmp = SDLGifAutoFrame(gif);
            Tri=SDL_CreateTextureFromSurface(fenetre, tmp);
            Tri2=SDL_CreateTextureFromSurface(fenetre, tmp2);

            SDL_QueryTexture(Tri, NULL, NULL, &position.w, &position.h);
            SDL_DestroyTexture(Tri);
            // Centre le GIF
            pos.x=LARGEUR_FENETRE/2-5*position.w/8;
            pos.y=HAUTEUR_FENETRE/2-4*position.h/8;
            pos.w=0;
            pos.h=0;
            SDL_BlitSurface(tmp,NULL,screen,&pos);


            SDL_QueryTexture(Tri2, NULL, NULL, &position2.w, &position2.h);
            SDL_DestroyTexture(Tri2);
            // Centre le GIF
            pos2.x=LARGEUR_FENETRE/2-position2.w/2;
            pos2.y=HAUTEUR_FENETRE/2+position.h/2;
            pos2.w=0;
            pos2.h=0;
            SDL_BlitSurface(tmp2,NULL,screen,&pos2);
            SDL_BlitSurface(text, NULL, screen, &posTitre); // Affiche Niveau

            texture= SDL_CreateTextureFromSurface(fenetre, screen);
            SDL_RenderClear(fenetre);
                SDL_RenderCopy(fenetre, texture, NULL, NULL);
            SDL_RenderPresent(fenetre);

        }
        SDL_FreeSurface(text);
        SDLFreeGif(gif);
        SDLFreeGif(gif2);

        texture = SDL_CreateTextureFromSurface(fenetre, menu);
        SDL_QueryTexture(texture, NULL, NULL, &src1.w, &src1.h);
        SDL_FreeSurface(menu);
        /////////////////////////////////////////////////////////////////////////////////////////////
        while (continuer)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: // Veut arrêter le jeu
                            continuer = 0;
                            break;
                        case SDLK_j: // Demande à jouer
                            instText = SDL_CreateTextureFromSurface(fenetre, instructJeu);

                            while(!KeyHit())
                            {
                                SDL_RenderClear(fenetre);
                                SDL_RenderCopy(fenetre, instText, NULL, NULL);
                                SDL_RenderPresent(fenetre);
                            }
                            jouer(fenetre);
                            break;
                        case SDLK_e: //Demande d'édition
                            instText = SDL_CreateTextureFromSurface(fenetre, instructEdit);

                            while(!KeyHit())
                            {
                                SDL_RenderClear(fenetre);
                                SDL_RenderCopy(fenetre, instText, NULL, NULL);
                                SDL_RenderPresent(fenetre);
                            }
                            SDL_DestroyTexture(instText);
                            editeur(fenetre);
                            break;
                    }
                    break;
            }
            SDL_SetRenderDrawColor(fenetre, 255, 255, 255, 255);
            SDL_RenderClear(fenetre);
            SDL_RenderCopy(fenetre, texture, &src1, &positionMenu);
            SDL_RenderPresent(fenetre);
        }
        SDL_FreeSurface(instructJeu);
        SDL_FreeSurface(instructEdit);
        Mix_FreeMusic(soundA);
        Mix_CloseAudio();
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(fenetre);
        SDL_DestroyWindow(window);
        SDL_Quit();

        this->show();
        return EXIT_SUCCESS;
}
