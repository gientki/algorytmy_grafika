#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Tworzenie głównego okna
    QWidget *wnd = new QWidget();
    setCentralWidget(wnd);

    // Tworzenie obiektu klasy Textures
    Textures *tekstury = new Textures("ścieżka do pliku1", "ścieżka do pliku1");

    // Tworzenie obiektu klasy morph i przekazywanie do niego obiektu tekstury
    morph *zmora = new morph(tekstury);

    // Ustawienie polityki rozszerzania dla obiektów tekstury i zmora
    tekstury->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    zmora->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Tworzenie siatki
    QGridLayout *grid = new QGridLayout(wnd);

    // Ustawianie odstępów między elementami w siatce
    grid->setSpacing(30);

    // Dodawanie obiektów tekstury i zmora do siatki
    grid->addWidget(tekstury, 0, 0);
    grid->addWidget(zmora, 0 , 1);

    // Tworzenie suwaka, przycisku zmiany i przycisku animacji
    QSlider *klatki = new QSlider(Qt::Horizontal);
    QPushButton *zmiana = new QPushButton("Zmiana");
    QPushButton *animate = new QPushButton("Animacja");

    // Ustawianie wartości maksymalnej i minimalnej suwaka
    klatki->setMaximum(100);
    klatki->setMinimum(0);

    // Dodawanie suwaka, przycisku zmiany i przycisku animacji do siatki
    grid->addWidget(zmiana, 1, 0, Qt::AlignCenter);
    grid->addWidget(klatki, 1, 1);
    grid->addWidget(animate, 2, 0, 1, 2, Qt::AlignCenter);

    // Przekazywanie suwaka do obiektu zmora
    zmora->setsuwak(klatki);

    // Tworzenie połączeń sygnałów i slotów
    QObject::connect(zmiana, &QPushButton::pressed, tekstury, &Textures::zmienObraz);
    QObject::connect(klatki, &QSlider::valueChanged, zmora, &morph::sliderCh);
    QObject::connect(animate, &QPushButton::pressed, zmora, &morph::animate);

    // Ustawianie rozmiaru okna
    this->resize(1280, 800);

    // Ustawianie wartości początkowej i zerowanie suwaka
    klatki->setValue(1);
    klatki->setValue(0);
}

MainWindow::~MainWindow()
{
}
