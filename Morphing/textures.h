#ifndef TEXTURES_H
#define TEXTURES_H

#include <QWidget>
#include <vector>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPen>

class Textures : public QWidget
{
    Q_OBJECT
public:
    int currImg = 0;
    int currPoint = -1;

    QImage *firstView, *secondView, *firstSrc, *secondSrc, *firstScaled, *secondScaled;
    std::vector<QPoint> siatka[2];
    std::vector<std::array<int, 3> > trojkaty;



    explicit Textures(QString file1, QString file2, QWidget *parent = nullptr);
    void loadImg(QString filepath, QImage **img);
    int isHit(QPoint p);

    void paintEvent(QPaintEvent *ev) override;
    void resizeEvent(QResizeEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

public slots:
    void zmienObraz();


signals:

};

#endif // TEXTURES_H
