// Plik naglowkowy klasy MyWindow
// Obiekt tej klasy to glowne okno naszej aplikacji
// Szkielet tego pliku jest tworzony przez QtCreator
// Mozemy do niego dodac deklaracje wlasnych pol i metod

#ifndef MYWINDOW_H
#define MYWINDOW_H

// Dolaczamy plik naglowkowy klasy QMainWindow,
// Klasa QMainWindow posiada swoj wlasny layout.
// latwo mozna do niej dodac pasek menu, widzety dokujace,
// pasek narzedzi i pasek statusu. Na srodku okna
// wyswietlanego przez QMainWindow znajduje sie obszar,
// ktory mozna wypelnic roznymi widgetami.
#include <QMainWindow>

// QPainter to klasa umozliwiajaca niskopoziomowe rysowanie
// na elementach GUI
#include <QPainter>

// QImage to klasa pozwalajaca na niezalezna od sprzetu reprezentacje obrazu.
// Pozwala na bezposredni dostep do poszczegolnych pikseli,
// Bedziemy jej uzywali do tworzenia i przechowywania
// naszych rysunkow
#include <QImage>

// QMouseEvent to klasa obslugujaca zdarzenia zwiazane z myszka
// klikniecia, ruch myszka itp.
#include <QMouseEvent>
//Do liczby PI
#include <QtMath>
//Do stosu
#include <QStack>
//Paleta
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QSlider>

namespace Ui {
class MyWindow;
}

// MyWindow jest podklasa klasy QMainWindow.
class MyWindow : public QMainWindow
{
    // Q_OBJECT jest to makro, ktore musi sie znajdowac
    // we wszystkich klasach definiujacych wlasne sygnaly i sloty
    // W naszej klasie nie jest ono potrzebne,
    // ale QtCreator dodaje je automatycznie do kazdej klasy.
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);

    // Typowa deklaracja konstruktora w Qt.
    // Parametr "parent" okresla rodzica komponenetu.
    // W przypadku naszej klasy parametr ten wskazuje na null
    // co oznacza, ze komponenet nie ma rodzica, jest to
    // komponenet najwyzszego poziomu

    // Deklaracja destruktora

private:
    Ui::MyWindow *ui;
    QImage *img;
    QImage *kopia;
    QLabel *imageLabel;
    QImage *img1;
    QImage *img2;
    QImage *img3;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controlsLayout;
    QComboBox *blendModeComboBox;
    QSlider *opacitySlider1;
    QSlider *opacitySlider2;
    QSlider *opacitySlider3;
    QColor zamien_kolor;
    QColor nowy_kolor;
    QColor kolor;

    int szer;
    int wys;
    int poczX;
    int poczY;
    int wartosc = 0;
    char zmienna = 'r';
    char zmienna2;

    int R, G, B;

    void kopiowanie1();
    void kopiowanie2();
    void paintEvent(QPaintEvent*);
    void kolorowanko(int y, int x, int B, int G, int R);
    void updateImage();
    void blendImages(const QImage &img1, float opacity1,
                     const QImage &img2, float opacity2,
                     const QImage &img3, float opacity3,
                     int mode);
    int max(float,float);
    int min(float,float);

};

#endif // MYWINDOW_H
