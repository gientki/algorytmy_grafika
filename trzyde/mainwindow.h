#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QImage>
#include <QSlider>
#include <QVBoxLayout>
#include <QVector>
#include <QPoint>
#include <QCheckBox>
struct punkt3D {
    double x, y, z;
};

class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void trX(int val);
    void trY(int val);
    void scX(int val);
    void scY(int val);
    void scZ(int val);
    void rX(int val);
    void rY(int val);
    void rZ(int val);

private:
    void drawPixel(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void rysuj(QVector<QPoint> punkty);
    void dwaDtrzyD(QVector<punkt3D> pkt);
    void liczMacierze();
    void kolorowanko(int x, int y, int R, int G, int B);
    void linia(int x0, int y0, int x1, int y1, int R,int G,int B);
    void RotacjaX(double &y, double &z);
    void RotacjaY(double &x, double &z);
    void RotacjaZ(double &x, double &y);
    void Skalowanie(double &x, double &y, double &z);
    void Przesuniecie(double &x, double &y, double &z);
    void rysuj(QVector<QPoint> punkty2D, QVector<punkt3D> punkty3D);
    bool czyWidoczna(QVector3D normalna, punkt3D kamera, punkt3D p);
    bool czy_widac(punkt3D p0,punkt3D p1,punkt3D p2,punkt3D p3);
    void toggleHiddenEdges(int state);
    void swiatlo(int state);
    void malowanko(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);
    QImage im;
    QVector<punkt3D> punkty;
    QVector<QPoint> punkty2D;
    QVector3D obliczNormalna(punkt3D p1, punkt3D p2, punkt3D p3);
    QSlider *transXSlider;
    QSlider *transYSlider;
    QSlider *scaleXSlider;
    QSlider *scaleYSlider;
    QSlider *scaleZSlider;
    QSlider *rotXSlider;
    QSlider *rotYSlider;
    QSlider *rotZSlider;

    QCheckBox *hideEdgesCheckbox;
    QCheckBox *lightCheckbox;

    QWidget *controlWidget;
    QVBoxLayout *controlLayout;
    QWidget *drawingArea;

    int szer,wys;
    double tX, tY, tZ;
    double sX, sY, sZ;
    double alphaX, alphaY, alphaZ;
    bool hideHiddenEdges;
    bool light;
};

#endif // MYWINDOW_H
