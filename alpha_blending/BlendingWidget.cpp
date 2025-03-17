// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "BlendingWidget.h"
#include <QDebug>
#include <iostream>
// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"

using namespace std;

MyWindow::MyWindow(QWidget *parent)
    : QMainWindow(parent),

    szer(600), wys(600), poczX(10), poczY(30)
{
    // Wywolujemy funkcje tworzaca elementy GUI
    // Jej definicja znajduje sie w pliku "ui_mywindow.h"

    resize(800,700);
    // Pobieramy wymiary i wspolrzedne lewego gornego naroznika ramki
    // i ustawiamy wartosci odpowiednich pol
    // Uwaga: ramke "rysujFrame" wykorzystujemy tylko do
    // wygodnego ustaiwenia tych wymiarow. Rysunek bedziemy wyswietlac
    // bezposrednio w glownym oknie aplikacji.

    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).
    img = new QImage(szer, wys, QImage::Format_RGB32);

    img1 = new QImage("C:/Users/swete/OneDrive/Pulpit/4.jpg");
    img2 = new QImage("C:/Users/swete/OneDrive/Pulpit/2.jpg");
    img3 = new QImage("C:/Users/swete/OneDrive/Pulpit/1.jpg");

    mainLayout = new QVBoxLayout(this);
    controlsLayout = new QHBoxLayout(this);

    blendModeComboBox = new QComboBox(this);
    blendModeComboBox->addItem("light", 0);
    blendModeComboBox->addItem("abs", 1);
    blendModeComboBox->addItem("normal", 2);
    blendModeComboBox->addItem("najjaśniejszy pixel", 3);
    blendModeComboBox->addItem("najciemniejszy pixel", 4);
    controlsLayout->addWidget(blendModeComboBox);

    opacitySlider1 = new QSlider(Qt::Horizontal, this);
    opacitySlider1->setRange(0, 100);
    opacitySlider1->setValue(0);
    controlsLayout->addWidget(opacitySlider1);

    opacitySlider2 = new QSlider(Qt::Horizontal, this);
    opacitySlider2->setRange(0, 100);
    opacitySlider2->setValue(0);
    controlsLayout->addWidget(opacitySlider2);

    opacitySlider3 = new QSlider(Qt::Horizontal, this);
    opacitySlider3->setRange(0, 100);
    opacitySlider3->setValue(0);
    controlsLayout->addWidget(opacitySlider3);

    mainLayout->addLayout(controlsLayout);

    imageLabel = new QLabel(this);
    mainLayout->addWidget(imageLabel);

    connect(opacitySlider1, &QSlider::valueChanged, this, &MyWindow::updateImage);
    connect(opacitySlider2, &QSlider::valueChanged, this, &MyWindow::updateImage);
    connect(opacitySlider3, &QSlider::valueChanged, this, &MyWindow::updateImage);
    connect(blendModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MyWindow::updateImage);

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);
    update();
}

void MyWindow::updateImage()
{
    float opacity1 = opacitySlider1->value() / 100.0;
    float opacity2 = opacitySlider2->value() / 100.0;
    float opacity3 = opacitySlider3->value() / 100.0;
    int mode = blendModeComboBox->currentData().toInt();

    blendImages(*img1, opacity1, *img2, opacity2, *img3, opacity3, mode);
    update();
}

void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(poczX, poczY, *img);
}

void MyWindow::kolorowanko( int x,int y, int R, int G, int B)
{
    uchar *ptr = img->bits();
    if (x >= 0 && y >= 0 && x < szer && y < wys)
    {
        ptr[szer * 4 * y + 4 * x] = B;
        ptr[szer * 4 * y + 4 * x + 1] = G;
        ptr[szer * 4 * y + 4 * x + 2] = R;
    }
}
int MyWindow::max(float a,float b)
{
    if (a>b)
        return (int)a;
    else
        return (int)b;
}
int MyWindow::min(float a,float b)
{
    if (a>b)
        return (int)b;
    else
        return (int)a;
}
void dec_to_bin(int liczba)
{
    int i=0,tab[31];

    while(liczba)
    {
        tab[i++]=liczba%2;
        liczba/=2;
    }

    for(int j=i-1;j>=0;j--)
        cout<<tab[j];

    cout<<endl;
}
void MyWindow::blendImages(const QImage &img1, float c1a,
                           const QImage &img2, float c2a,
                           const QImage &img3, float c3a,
                           int mode)
{

    int r, g, b,r1,r2,r3,g1,g2,g3,b1,b2,b3;
        // QRgb color1 = img1.pixel(150,150);
        //  cout<<color1<<endl;
        //  dec_to_bin(color1);
        //  cout<<"cerwony:"<<endl;
        //  dec_to_bin(color1 >> 16);
        //  dec_to_bin(color1 >> 16 & 0xff);
        //  cout<<"niebieski:"<<endl;
        //  dec_to_bin(color1 >> 0);
        //  dec_to_bin(color1 >> 0 & 0xff);
        //  cout<<"zielony:"<<endl;
        //  dec_to_bin(color1 >> 8);
        //  dec_to_bin(color1 >> 8 & 0xff);
    for (int i = 0; i < wys*szer; i++)
    {
        QRgb p1 = img1.pixel(i%szer, i/wys);
        QRgb p2 = img2.pixel(i%szer, i/wys);
        QRgb p3 = img3.pixel(i%szer, i/wys);

        r1 = p1 >> 16 & 0xff;
        r2 = p2 >> 16 & 0xff;
        r3 = p3 >> 16 & 0xff;

        g1 = p1 >> 8 & 0xff;
        g2 = p2 >> 8 & 0xff;
        g3 = p3 >> 8 & 0xff;

        b1 = p1 & 0xff;
        b2 = p2 & 0xff;
        b3 = p3 & 0xff;
        switch (mode)
        {
        case 0: // light
            r = c1a * r1 + c2a * r2 + c3a * r3;
            g = c1a * g1 + c2a * g2 + c3a * g3;
            b = c1a * b1 + c2a * b2 + c3a * b3;

            r = min(255, r);
            g = min(255, g);
            b = min(255, b);

            break;
        case 1: // abs
            r = abs(c1a * r1 - c2a * r2 - c3a * r3);
            g = abs(c1a * g1 - c2a * g2 - c3a * g3);
            b = abs(c1a * b1 - c2a * b2 - c3a *b3);

            r = min(255, r);
            g = min(255, g);
            b = min(255, b);

            break;
        case 2: // normal
            if (2 * c1a < 1)
            {
                r = 2 * c1a * r2 + (1 - 2 * c1a) * r1;
                g = 2 * c1a * g2 + (1 - 2 * c1a) * g1;
                b = 2 * c1a * b2 + (1 - 2 * c1a) * b1;
            }
            else
            {
                r = (2 - 2 * c1a) * r2 + ((c1a - 0.5) / (c1a / 2)) * r3;
                g = (2 - 2 * c1a) * g2 + ((c1a - 0.5) / (c1a / 2)) * g3;
                b = (2 - 2 * c1a) * b2 + ((c1a - 0.5) / (c1a / 2)) * b3;
            }
            r = min(255, r);
            g = min(255, g);
            b = min(255, b);

            break;
        case 3: // znajduje najjaśniejszy
            r = max(c3a * r3, max(c1a * r1, c2a * r2));
            g = max(c3a * g3, max(c1a * g1, c2a * g2));
            b = max(c3a * b3, max(c1a * b1, c2a * b2));

            r = min(255, r);
            g = min(255, g);
            b = min(255, b);

            break;
        case 4: // znajdź najciemniejszy
            r = min(c3a * r3, min(c1a * r1, c2a * r2));
            g = min(c3a * g3, min(c1a * g1, c2a * g2));
            b = min(c3a * b3, min(c1a * b1, c2a * b2));

            r = min(255, r);
            g = min(255, g);
            b = min(255, b);
            break;
        default:
            r = min(c3a * r3, min(c1a * r1, c2a * r2));
            g = min(c3a * g3, min(c1a * g1, c2a * g2));
            b = min(c3a * b3, min(c1a * b1, c2a * b2));

            r = min(255, r);
            g = min(255, g);
            b = min(255, b);
            break;
        }
        kolorowanko(i%szer,i/wys,r,g,b);

    }
}
