#include "mainwindow.h"
#include "jeuMario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(LARGEUR_FENET,HAUTEUR_FENET);

    // 1 : Créer le QTabWidget (onglets)
    onglets = new QTabWidget(this);
    onglets->setGeometry(0,30,LARGEUR_FENET, HAUTEUR_FENET-30);
    QPalette palette;
    // 2 : Créer les pages, en utilisant un widget parent pour contenir chacune des pages
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QLabel *page3 = new QLabel; // Comme un QLabel est aussi un QWidget (il en hérite), on peut aussi s'en servir de page

    QLineEdit *lineNom = new QLineEdit(tr("Entrez votre nom"));
    QLabel *label_nom = new QLabel(tr("NOM              :"));
    QLabel *label_prenom = new QLabel(tr("PRENOM       :"));
    QLineEdit *linePrenom = new QLineEdit("Entrez votre prénom");
    QLabel *label_age = new QLabel(tr("AGE               :"));
    QSpinBox *lineAge = new QSpinBox;
        lineAge->setRange(0,100);
        lineAge->setValue(20);
    QLabel *label_nation = new QLabel(tr("NATIONALITE  :"));
    //QLineEdit *lineNation = new QLineEdit(tr("Entrez votre nationnalité"));
    /*QPushButton *lineNation = new QPushButton(tr("Pop&up Button"));
       QMenu *menu = new QMenu(this);
       menu->addAction(tr("&Français"));
       menu->addAction(tr("&Anglais"));
       menu->addAction(tr("&Espagnol"));
       menu->addAction(tr("A&méricain"));
       lineNation->setMenu(menu);*/
    QComboBox *lineNation = new QComboBox;
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
    QPushButton *boutSauvegarder = new QPushButton(tr("Sauvegarder"));
        connect(boutSauvegarder,SIGNAL(clicked()),this,SLOT(save()));
    m_boutonQuitter = new QPushButton(tr("Annuler"));
    connect(m_boutonQuitter, SIGNAL(clicked()),qApp, SLOT(quit()));
    QLabel *label_mail = new QLabel(tr("E-MAIL       :"));
    QLineEdit *lineMail = new QLineEdit(tr("exemple@gmail.com"));

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
        titre->setAlignment(Qt::AlignCenter);
    QPushButton *boutonJouer = new QPushButton(tr("JOUER"));
        connect(boutonJouer,SIGNAL(clicked()),this,SLOT(startGame()));
    QLabel *messageEspace = new QLabel(tr("     "));
    QTextEdit *messageAccueil = new QTextEdit(tr("Bienvenu dans l'application Mario'App créé par Julien Badajoz pour réaliser le jeu mario Mario Sokoban en SDL via un executable et s'inicier à Qt Creator."));
        messageAccueil->setAlignment(Qt::AlignLeft);
        messageAccueil->setFixedHeight(70);
            palette.setColor(QPalette::Text,QColor(255,0,0));
            palette.setColor(QPalette::Base,QColor(0,0,0));
        messageAccueil->setPalette(palette);
        messageAccueil->setStyleSheet("border-radius: 20");
        messageAccueil->setFrameStyle( QFrame::Raised );
        messageAccueil->setReadOnly(true);
    QPushButton *boutonQuitter = new QPushButton(tr("Quitter"));
        connect(boutonQuitter,SIGNAL(clicked()),qApp,SLOT(quit()));
    QLabel *imgCaisse = new QLabel;
        imgCaisse->setPixmap(QPixmap("/Users/badajozj/Documents/Coding/SDL/Mario/mario_sokoban/caisse.jpg"));
        imgCaisse->setAlignment(Qt::AlignCenter);
    QLabel *imgCaisse2 = new QLabel;
        imgCaisse2->setPixmap(QPixmap("/Users/badajozj/Documents/Coding/SDL/Mario/mario_sokoban/caisse.jpg"));
        imgCaisse2->setAlignment(Qt::AlignCenter);

    QGridLayout *layout2 = new QGridLayout;
    layout2->setContentsMargins(5, 5, 5, 40);
    layout2->setColumnMinimumWidth(0,50);
    layout2->setColumnMinimumWidth(1,150);
    layout2->setColumnMinimumWidth(3,150);

    layout2->addWidget(messageEspace,1,0,1,1);
    layout2->addWidget(titre,1,2,1,1);
    layout2->addWidget(boutonJouer,2,2,1,1);
    layout2->addWidget(messageAccueil,3,1,1,3);
    layout2->addWidget(boutonQuitter,4,4,1,2);
    layout2->addWidget(imgCaisse,1,1,1,1);
    layout2->addWidget(imgCaisse2,1,3,1,1);

    page2->setLayout(layout2);

    // Page 3 (je ne vais afficher qu'une image ici, pas besoin de layout)
    page3->setPixmap(QPixmap("/Users/badajozj/Documents/Coding/SDL/Mario/mario_sokoban/instructions.jpg"));
    page3->setAlignment(Qt::AlignCenter);

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
    menuPages=menuBar()->addMenu(tr("Pages"));
    menuContact=menuBar()->addMenu(tr("Contact"));
    menuAide=menuBar()->addMenu(tr("Aide"));

    // barre d'outils
    QToolBar *accueilToolBar = addToolBar("Accueil");

    accueil = new QAction(tr("&Accueil"), this);
    ninjutsu =new QAction(tr("Ninjutsus"), this);
    dodjutsu = new QAction(tr("Dôdjutsus"), this);
    taijutsu = new QAction(tr("Taijutsus"), this);
    ninjas = new QAction(tr("Ninjas"), this);
    kages = new QAction(tr("Kages"), this);
    demons = new QAction(tr("Demons"), this);
    armes = new QAction(tr("Armes"), this);

    menuPages->addAction(accueil);
    accueilToolBar->addAction(accueil);
    accueil->setStatusTip(tr("Appuyer pour retourner à l'accueil"));

    menuJutsus=menuPages->addMenu(tr("Ninjutsu"));
    menuPersonnages=menuPages->addMenu(tr("Personnages"));

    menuJutsus->addAction(ninjutsu);
    menuJutsus->addAction(dodjutsu);
    menuJutsus->addAction(taijutsu);

    menuPersonnages->addAction(ninjas);
    menuPersonnages->addAction(kages);
    menuPersonnages->addAction(demons);

    menuPages->addAction(armes);

    accueilToolBar->addSeparator();

    fermer = new QAction(tr("&Fermer"), this);
    accueilToolBar->addAction(fermer);
    fermer->setStatusTip(tr("Appuyer pour quitter l'appli"));
    connect(fermer, &QAction::triggered,this,&MainWindow::arret);
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Prêt"));
}

void MainWindow::arret()
{
    emit m_boutonQuitter->clicked();
}
void MainWindow::save()
{
    onglets->setCurrentIndex(1);
    onglets->setTabVisible(0,false);
}
int KeyHit()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
        if (e.type == SDL_KEYDOWN)
            return 1;
    return 0;
}
#define TOTAL_POINTS 5000
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
        SDL_SetWindowIcon(window,IMG_Load("/Users/badajozj/Desktop/MarioApp/MarioDocs/caisse.jpg"));

        menu = IMG_Load("/Users/badajozj/Desktop/MarioApp/MarioDocs/menu.jpg");
        text = IMG_Load("/Users/badajozj/Desktop/MarioApp/MarioDocs/titreMario.png");
        instructJeu = IMG_Load("/Users/badajozj/Desktop/MarioApp/MarioDocs/instructions_jeu.png");
        instructEdit = IMG_Load("/Users/badajozj/Desktop/MarioApp/MarioDocs/instructions.jpg");

        SDL_Rect positionMenu={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
        SDL_Rect src1={0, 0, 0, 0};
        SDL_Rect pos={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};

        SDL_Texture* test=SDL_CreateTextureFromSurface(fenetre,text);
        SDL_QueryTexture(test, NULL, NULL, &posTitre.w, &posTitre.h);
        posTitre.y=0;
        posTitre.x=LARGEUR_FENETRE/2-posTitre.w/2;
        SDL_DestroyTexture(test);
        if (text == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        }

        if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
            exit(EXIT_FAILURE);
        }

        Mix_Music* soundA = Mix_LoadMUS("/Users/badajozj/Desktop/MarioApp/MarioDocs/SuperMarioBros.wav"); // charge la musique

        Uint8 volume = MIX_MAX_VOLUME/2; Mix_VolumeMusic(volume); // Mets le volume a 0
        Mix_PlayMusic(soundA,-1); // Joue soundA 1 fois sur le canal 1

        ////////////////////////////////////////  GIF  //////////////////////////////////////////////
       gif = SDLLoadGif("/Users/badajozj/Desktop/MarioApp/MarioDocs/supermario.gif");
       gif2 = SDLLoadGif("/Users/badajozj/Desktop/MarioApp/MarioDocs/key.gif");

        while(!KeyHit())
        {

/**/
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
/**/

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
