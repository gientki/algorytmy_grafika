#include <iostream>
#include <QtConcurrent/QtConcurrentMap>
#include <QFuture>
#include <QFutureWatcher>
#include <QTimeLine>
#include "morph.h"

morph::morph(Textures *t, QWidget *parent) : QWidget(parent)
{
    // Inicjalizacja obiektu morph
    tekstury = t;
    result = new QImage(size(), QImage::Format_RGB32);

    rozmiar = tekstury->siatka[0].size();
    tmpTriangle = new QPoint[rozmiar];
}

thread_local QPoint vertsCopy[3];
thread_local std::array<int, 3> oryginalneIndeksy;
thread_local int det;

void morph::morf()
{
    // Wypełnienie obrazu tłem
    result->fill(Qt::white);

    for(int i = 0; i < rozmiar; ++i)
    {
        // Obliczanie nowych współrzędnych trójkątów na podstawie pozycji suwaka
        int dx = tekstury->siatka[1][i].x() - tekstury->siatka[0][i].x();
        int dy = tekstury->siatka[1][i].y() - tekstury->siatka[0][i].y();
        tmpTriangle[i] = { dx * sliderPos / l_kroków + tekstury->siatka[0][i].x(),
                           dy * sliderPos / l_kroków + tekstury->siatka[0][i].y() };
    }

    // Renderowanie trójkątów z wykorzystaniem wielowątkowości
    QtConcurrent::blockingMap(tekstury->trojkaty, [this](std::array<int, 3>& t){morph::render(t);});
}

void morph::render(std::array<int, 3>& trojkat)
{
    // Kopiowanie danych potrzebnych do renderowania trójkąta
    oryginalneIndeksy = trojkat;

    vertsCopy[0] = tmpTriangle[trojkat[0]];
    vertsCopy[1] = tmpTriangle[trojkat[1]];
    vertsCopy[2] = tmpTriangle[trojkat[2]];
    sortTrojkat();

    // Obliczanie wyznacznika trójkąta
    det = (vertsCopy[1].x() - vertsCopy[0].x()) * (vertsCopy[2].y() - vertsCopy[0].y()) -
          (vertsCopy[1].y() - vertsCopy[0].y()) * (vertsCopy[2].x() - vertsCopy[0].x());

    if (det == 0)
        return;

    const double edges[3] = {
        static_cast<double>(vertsCopy[1].x() - vertsCopy[0].x()) / (vertsCopy[1].y() - vertsCopy[0].y()),
        static_cast<double>(vertsCopy[2].x() - vertsCopy[0].x()) / (vertsCopy[2].y() - vertsCopy[0].y()),
        static_cast<double>(vertsCopy[2].x() - vertsCopy[1].x()) / (vertsCopy[2].y() - vertsCopy[1].y())
    };

    int y;
    double x1, x2;
    int lewy, prawy;
    int lewy2, prawy2;

    if (vertsCopy[1].y() == vertsCopy[0].y())
    {
        // Sprawdzenie, czy trójkąt jest poziomy
        x1 = vertsCopy[0].x();
        x2 = vertsCopy[1].x();
        if (x1 > x2)
        {
            std::swap(x1, x2);

            lewy2 = 2;
            prawy2 = 1;
        }
        else
        {
            lewy2 = 1;
            prawy2 = 2;
        }
    }
    else
    {
        x1 = x2 = vertsCopy[0].x();
        if (det > 0)
        {
            lewy = 1;
            prawy = 0;

            lewy2 = lewy;
            prawy2 = 2;
        }
        else
        {
            prawy = 1;
            lewy = 0;

            lewy2 = 2;
            prawy2 = prawy;
        }
    }

    const int imgWidth = result->width();
    const int imgHeight = result->height();
    int xl, xr;
    int yu, ym, yd;

    // Obliczanie granic obszaru, na którym będzie renderowany trójkąt
    yu = std::max(0, std::min(vertsCopy[0].y(), imgHeight));
    yd = std::max(0, std::min(vertsCopy[2].y(), imgHeight));
    ym = std::max(yu, std::min(vertsCopy[1].y(), yd));

    for (y = vertsCopy[0].y(); y < vertsCopy[1].y(); ++y)
    {
        if (yu <= y && y < ym)
        {
            // Renderowanie linii trójkąta w odpowiednim obszarze
            xl = std::max(0, std::min(static_cast<int>(x1), imgWidth));
            xr = std::max(0, std::min(static_cast<int>(x2), imgWidth));
            teksturujLinie(xl, xr, y);
        }
        x1 += edges[lewy];
        x2 += edges[prawy];
    }

    for (; y < vertsCopy[2].y(); ++y)
    {
        if (ym <= y && y < yd)
        {
            // Renderowanie linii trójkąta w odpowiednim obszarze
            xl = std::max(0, std::min(static_cast<int>(x1), imgWidth));
            xr = std::max(0, std::min(static_cast<int>(x2), imgWidth));
            teksturujLinie(xl, xr, y);
        }
        x1 += edges[lewy2];
        x2 += edges[prawy2];
    }

}

void morph::teksturujLinie(int x1, int x2, int y)
{
    const int szerokosc = tekstury->secondScaled->width();
    const int wysokosc  = tekstury->secondScaled->height();

    const uchar* srcUbits1 = tekstury->firstScaled->constBits();
    const uchar* srcUbits2 = tekstury->secondScaled->constBits();
    uchar* dstUbits = result->bits() + ((y * result->width() + x1) << 2);

    for(int x = x1; x <= x2; ++x)
    {
        // Obliczanie wag tekstur dla danego piksela
        const double v = static_cast<double>((x - vertsCopy[0].x()) * (vertsCopy[2].y() - vertsCopy[0].y()) -
                                             (vertsCopy[2].x() - vertsCopy[0].x()) * (y - vertsCopy[0].y())) / det;

        const double w = static_cast<double>((vertsCopy[1].x() - vertsCopy[0].x()) * (y - vertsCopy[0].y()) -
                                             (x - vertsCopy[0].x()) * (vertsCopy[1].y() - vertsCopy[0].y())) / det;

        const double u = 1 - v - w;

        // Obliczanie współrzędnych tekstur dla danego piksela
        const int xt  = tekstury->siatka[0][oryginalneIndeksy[0]].x() * u +
                       tekstury->siatka[0][oryginalneIndeksy[1]].x() * v +
                       tekstury->siatka[0][oryginalneIndeksy[2]].x() * w;
        const int yt  = tekstury->siatka[0][oryginalneIndeksy[0]].y() * u +
                       tekstury->siatka[0][oryginalneIndeksy[1]].y() * v +
                       tekstury->siatka[0][oryginalneIndeksy[2]].y() * w;

        const int xte = tekstury->siatka[1][oryginalneIndeksy[0]].x() * u +
                        tekstury->siatka[1][oryginalneIndeksy[1]].x() * v +
                        tekstury->siatka[1][oryginalneIndeksy[2]].x() * w;
        const int yte = tekstury->siatka[1][oryginalneIndeksy[0]].y() * u +
                        tekstury->siatka[1][oryginalneIndeksy[1]].y() * v +
                        tekstury->siatka[1][oryginalneIndeksy[2]].y() * w;

        if (0 <= xt  && xt  < szerokosc && 0 <= yt  && yt  < wysokosc &&
            0 <= xte && xte < szerokosc && 0 <= yte && yte < wysokosc )
        {
            // Pobieranie wartości pikseli z tekstur i nakładanie ich na wynikową teksturę
            const uchar* src1 = srcUbits1 + ( (yt * szerokosc + xt)  << 2);
            const uchar* src2 = srcUbits2 + ((yte * szerokosc + xte) << 2);

            // Wprowadzenie zmian w sposobie nakładania tekstur
            uchar red   = alfa  * src2[0] + (1-alfa) * src1[0];
            uchar green = alfa  * src2[1] + (1-alfa) * src1[1];
            uchar blue  = alfa  * src2[2] + (1-alfa) * src1[2];

            dstUbits[0] = red;
            dstUbits[1] = green;
            dstUbits[2] = blue;

            dstUbits += 4; // Przesunięcie wskaźnika o 4 bajty do następnego piksela
        }
    }
}


inline void morph::sortTrojkat()
{
    // Sortowanie wierzchołków trójkąta względem osi y

    if (vertsCopy[0].y() > vertsCopy[1].y())
    {
        std::swap(vertsCopy[0], vertsCopy[1]);
        std::swap(oryginalneIndeksy[0], oryginalneIndeksy[1]);
    }

    if (vertsCopy[1].y() > vertsCopy[2].y())
    {
        std::swap(vertsCopy[1], vertsCopy[2]);
        std::swap(oryginalneIndeksy[1], oryginalneIndeksy[2]);
    }

    if (vertsCopy[0].y() > vertsCopy[1].y())
    {
        std::swap(vertsCopy[0], vertsCopy[1]);
        std::swap(oryginalneIndeksy[0], oryginalneIndeksy[1]);
    }
}



void morph::paintEvent(QPaintEvent*)
{
    QPainter pnt(this);
    pnt.drawImage(0,0, *result);
}

void morph::resizeEvent(QResizeEvent *ev)
{

    *result = result->scaled(ev->size());
}

void morph::setsuwak(QSlider *ptr)
{
    sliderpnt =  ptr;
}

void morph::sliderCh(int slider)
{
    sliderPos = slider;
    alfa = slider / 100.;
    morf();
    update();
}

void morph::animate()
{
    QTimeLine *timeline = new QTimeLine(3000);
    timeline->setFrameRange(0, 100);
    timeline->setEasingCurve(QEasingCurve::Linear);
    QObject::connect(timeline, &QTimeLine::frameChanged, sliderpnt, &QSlider::setValue);
    timeline->start();
}
