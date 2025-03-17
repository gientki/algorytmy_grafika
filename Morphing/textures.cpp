#include "textures.h"
#include <QPoint>

Textures::Textures(QString file1, QString file2, QWidget *parent) : QWidget(parent)
{
    // Ładowanie obrazów z plików
    loadImg(file1, &firstSrc);
    loadImg(file2, &secondSrc);

    firstScaled = new QImage();
    secondScaled = new QImage();

    firstView = new QImage(size(), QImage::Format_RGB32);
    secondView = new QImage(size(), QImage::Format_RGB32);

    // Inicjalizacja siatek punktów dla obu obrazów
    siatka[0] = {{0     ,0  }, {615	,0	}, {0   ,305}, {615	,345}, {0	,550},
                 {615	,610}, {203	,673}, {400	,673}, {310	,0  }, {150	,80 },
                 {457	,90 }, {312	,160}, {105	,263}, {468	,257}, {314	,256},
                 {145	,390}, {236	,408}, {398	,404}, {440	,397}, {315	,502},
                 {108	,514}, {432	,534}, {310	,590}, {0	,673}, {615	,673}};

    siatka[1] = {{0     ,0  }, {615	,0	}, {0  	,250}, {615	,250}, {0  	,460},
                 {615	,500}, {240	,672}, {365	,672}, {310	,0  }, {210	,45 },
                 {400	,50 }, {305	,100}, {185	,165}, {420	,170}, {305	,170},
                 {205	,280}, {240	,285}, {367	,285}, {408	,285}, {305	,346},
                 {200	,380}, {415	,395}, {300	,445}, {0   ,672}, {615	,672}};

    // Inicjalizacja trójkątów dla obrazów
    trojkaty = {{0	,8	,9 }, {1	,8	,10}, {8	,9	,11}, {8	,10	,11},
                {0	,9	,12}, {1	,10	,13}, {9	,11	,12}, {10	,11	,13},
                {11	,12	,14}, {11	,13	,14}, {12	,14	,16}, {13	,14	,17},
                {14	,16	,17}, {16	,17	,19}, {12	,15	,16}, {13	,17	,18},
                {15	,16	,19}, {17	,18	,19}, {15	,19	,22}, {18	,19	,22},
                {15	,20	,22}, {18	,21	,22}, {20	,22	,6 }, {21	,22	,7 },
                {22	,6	,7 }, {0	,12	,2 }, {12	,2	,15}, {2	,15	,20},
                {2	,20	,4 }, {20	,4	,6 }, {4	,23	,6 }, {5	,7	,24},
                {21	,5	,7 }, {3	,21	,5 }, {18	,3	,21}, {13	,18	,3 },
                {1	,13	,3 }};
}

int Textures::isHit(QPoint p)
{
    // Sprawdzenie czy punkt należy do którejś z siatek
    for (int i = 0; i < siatka[currImg].size(); ++i) {
        int dx = p.x() - siatka[currImg][i].x();
        int dy = p.y() - siatka[currImg][i].y();

        if (dx * dx + dy * dy <= 25)
            return i;
    }
    return -1;
}

void Textures::loadImg(QString filepath, QImage **img)
{
    if(QFile::exists(filepath))
    {
        *img = new QImage(filepath);
    }
    else
    {
        // Wyświetlenie komunikatu o błędzie w przypadku braku pliku
        QMessageBox(QMessageBox::Icon::Critical, "Plik niedostępny", QString(": plik tekstury(%1) nie jest dostępny.\n "
                    "Aplikacja zostanie zamknięta.").arg(filepath)).exec();
        exit(EXIT_FAILURE);
    }
}

void Textures::paintEvent(QPaintEvent *)
{
   QPainter pnt(this);

   if(currImg == 0)
   {
       QPainter firstP(firstView);
       firstP.drawImage(0, 0, *firstScaled);
       firstP.setPen(QPen(Qt::green));

       // Rysowanie elips na punktach siatki
       for(auto& e : siatka[currImg])
       {
            firstP.drawEllipse(e, 5, 5);
       }

       // Rysowanie trójkątów
       for(auto& e : trojkaty)
       {
           firstP.drawLine(siatka[currImg][e[0]], siatka[currImg][e[1]]);
           firstP.drawLine(siatka[currImg][e[2]], siatka[currImg][e[0]]);
           firstP.drawLine(siatka[currImg][e[1]], siatka[currImg][e[2]]);
       }

       pnt.drawImage(0, 0, *firstView);
   }
   else
   {
       QPainter secondP(secondView);
       secondP.drawImage(0, 0, *secondScaled);
       secondP.setPen(QPen(Qt::green));

       // Rysowanie elips na punktach siatki
       for(auto& e : siatka[currImg])
       {
            secondP.drawEllipse(e, 5, 5);
       }

       // Rysowanie trójkątów
       for(auto& e : trojkaty)
       {
           secondP.drawLine(siatka[currImg][e[0]], siatka[currImg][e[1]]);
           secondP.drawLine(siatka[currImg][e[1]], siatka[currImg][e[2]]);
           secondP.drawLine(siatka[currImg][e[2]], siatka[currImg][e[0]]);
       }

       pnt.drawImage(0, 0, *secondView);
   }
}

void Textures::resizeEvent(QResizeEvent *ev)
{
    // Skalowanie obrazów do rozmiaru okna
    *firstScaled = firstSrc->scaled(ev->size());
    *firstView = firstView->scaled(ev->size());
    *secondScaled = secondSrc->scaled(ev->size());
    *secondView = secondView->scaled(ev->size());
}

void Textures::mousePressEvent(QMouseEvent *ev)
{
    int currIdx;
    if((currIdx = isHit(ev->pos())) != -1)
    {
        currPoint = currIdx;
    }
}

void Textures::mouseMoveEvent(QMouseEvent *ev)
{
    if(currPoint != -1)
    {
        // Przesuwanie punktu siatki w trakcie przeciągania
        siatka[currImg][currPoint] = ev->pos();
        update();
    }
}

void Textures::mouseReleaseEvent(QMouseEvent*)
{
    currPoint = -1;
}

void Textures::zmienObraz()
{
    // Zmiana obrazu na drugi i odświeżenie widoku
    currImg ^= 1;
    update();
}
