#ifndef MORPH_H
#define MORPH_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <algorithm>
#include <array>
#include <QTimeLine>
#include <QSlider>

#include "textures.h"

class morph : public QWidget
{
    Q_OBJECT
public:
    const int l_kroków = 100;

    Textures *tekstury;
    QImage *result;
    QPoint *tmp;
    QSlider *sliderpnt;

    size_t rozmiar;
    QPoint *tmpTriangle;

    double alfa;
    int sliderPos = 0;


    explicit morph(Textures *t, QWidget *parent = nullptr);

    void morf();
    void sortTrojkat();
    void render(std::array<int , 3>& trojkat);
    void teksturujLinie(int x1, int x2, int y);

    void paintEvent(QPaintEvent *ev) override;
    void resizeEvent(QResizeEvent *ev) override;

    void setsuwak(QSlider *ptr);

public slots:
    void sliderCh(int slider);
    void animate();

signals:

};

#endif // MORPH_H
