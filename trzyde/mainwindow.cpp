#include "mainwindow.h"
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <cmath>
#include <QHBoxLayout>
#include <QLabel>
#include <iostream>
#include <QVector>
#include <QSlider>
#include <QWidget>

struct Matrix4x4 {
    double m[4][4] = {0};
    Matrix4x4() {
        for (int i = 0; i < 4; ++i) {
            m[i][i] = 1;
        }
    }
    void pokazuj_to()
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
                std::cout<<m[i][j]<<" ";

            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    // 000x
    // 000y
    // 000z
    // 0000
    void przesuwanko(double tx, double ty, double tz) {
        m[0][3] = tx;
        m[1][3] = ty;
        m[2][3] = tz;
    }
    // duze male
    //x000
    //0y00
    //00z0
    void powienkszanko(double sx, double sy, double sz) {
        m[0][0] = sx;
        m[1][1] = sy;
        m[2][2] = sz;
    }
    // obrrut X
    // 0000
    // 0110
    // 0110
    // 0000
    void obracankoX(double alpha) {
        double cosA = cos(alpha);
        double sinA = sin(alpha);
        m[1][1] = cosA;
        m[1][2] = -sinA;
        m[2][1] = sinA;
        m[2][2] = cosA;
    }
    // obracanko Y
    // 1010
    // 0000
    // 1010
    // 0000
    void obracankoY(double alpha) {
        double cosA = cos(alpha);
        double sinA = sin(alpha);
        m[0][0] = cosA;
        m[0][2] = sinA;
        m[2][0] = -sinA;
        m[2][2] = cosA;
    }
    // obracanko z
    // 1100
    // 1100
    // 0000
    // 0000
    void obracankoZ(double alpha) {
        double cosA = cos(alpha);
        double sinA = sin(alpha);
        m[0][0] = cosA;
        m[0][1] = -sinA;
        m[1][0] = sinA;
        m[1][1] = cosA;
    }
    void wynosi(double& x, double& y, double& z) const {
        double w = 1;
        double newX = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
        double newY = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
        double newZ = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
        x = newX;
        y = newY;
        z = newZ;
    }
};

MyWindow::MyWindow(QWidget *parent) : QWidget(parent), tX(0), tY(0), tZ(0), sX(1), sY(1), sZ(1), alphaX(0), alphaY(0), alphaZ(0) {
    setFixedSize(800, 600);

    im = QImage(600, 600, QImage::Format_RGB32);
    im.fill(Qt::black);
    szer = 600;
    wys = 600;

    punkty.append({220, 176, 0});
    punkty.append({220, 176, 50});
    punkty.append({440, 176, 0});
    punkty.append({440, 176, 50});
    punkty.append({220, 352, 0});
    punkty.append({220, 352, 50});
    punkty.append({440, 352, 0});
    punkty.append({440, 352, 50});

    transXSlider = new QSlider(Qt::Horizontal);
    transYSlider = new QSlider(Qt::Horizontal);

    scaleXSlider = new QSlider(Qt::Horizontal);
    scaleYSlider = new QSlider(Qt::Horizontal);
    scaleZSlider = new QSlider(Qt::Horizontal);
    rotXSlider = new QSlider(Qt::Horizontal);
    rotYSlider = new QSlider(Qt::Horizontal);
    rotZSlider = new QSlider(Qt::Horizontal);

    transXSlider->setRange(-100, 100);
    transYSlider->setRange(-100, 100);

    scaleXSlider->setRange(0, 200);
    scaleYSlider->setRange(0, 200);
    scaleZSlider->setRange(0, 200);
    rotXSlider->setRange(-180, 180);
    rotYSlider->setRange(-180, 180);
    rotZSlider->setRange(-180, 180);

    controlLayout = new QVBoxLayout;
    controlLayout->addWidget(new QLabel("Przesunięcie X:"));
    controlLayout->addWidget(transXSlider);
    controlLayout->addWidget(new QLabel("Przesunięcie Y:"));
    controlLayout->addWidget(transYSlider);
    controlLayout->addWidget(new QLabel("Skalowanie X:"));
    controlLayout->addWidget(scaleXSlider);
    controlLayout->addWidget(new QLabel("Skalowanie Y:"));
    controlLayout->addWidget(scaleYSlider);
    controlLayout->addWidget(new QLabel("Skalowanie Z:"));
    controlLayout->addWidget(scaleZSlider);
    controlLayout->addWidget(new QLabel("Obrót X:"));
    controlLayout->addWidget(rotXSlider);
    controlLayout->addWidget(new QLabel("Obrót Y:"));
    controlLayout->addWidget(rotYSlider);
    controlLayout->addWidget(new QLabel("Obrót Z:"));
    controlLayout->addWidget(rotZSlider);

    hideEdgesCheckbox = new QCheckBox("Ukryj zasłonięte krawędzie");
    controlLayout->addWidget(hideEdgesCheckbox);

    lightCheckbox = new QCheckBox("zapalanko");
    controlLayout->addWidget(lightCheckbox);

    controlWidget = new QWidget;
    controlWidget->setLayout(controlLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    drawingArea = new QWidget(this);
    drawingArea->setFixedSize(600, 600);

    mainLayout->addWidget(drawingArea);
    mainLayout->addWidget(controlWidget);

    connect(transXSlider, &QSlider::valueChanged, this, &MyWindow::trX);
    connect(transYSlider, &QSlider::valueChanged, this, &MyWindow::trY);
    connect(scaleXSlider, &QSlider::valueChanged, this, &MyWindow::scX);
    connect(scaleYSlider, &QSlider::valueChanged, this, &MyWindow::scY);
    connect(scaleZSlider, &QSlider::valueChanged, this, &MyWindow::scZ);
    connect(rotXSlider, &QSlider::valueChanged, this, &MyWindow::rX);
    connect(rotYSlider, &QSlider::valueChanged, this, &MyWindow::rY);
    connect(rotZSlider, &QSlider::valueChanged, this, &MyWindow::rZ);

    connect(hideEdgesCheckbox, &QCheckBox::stateChanged, this, &MyWindow::toggleHiddenEdges);
    connect(lightCheckbox, &QCheckBox::stateChanged, this, &MyWindow::swiatlo);
}

void MyWindow::resizeEvent(QResizeEvent *) {
    QImage imCopy = im.copy();
    im = QImage(600, 600, QImage::Format_RGB32);
    im.fill(Qt::black);

    QPainter p(&im);
    p.drawImage(0, 0, imCopy);

    liczMacierze();
}

void MyWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.drawImage(0, 0, im);
}

void MyWindow::kolorowanko(int x, int y, int R, int G, int B) {
    uchar *pix = im.scanLine(y);
    if (x >= 0 && y >= 0 && x < szer && y < wys) {
        pix[4 * x] = R;
        pix[4 * x + 1] = G;
        pix[4 * x + 2] = B;
    }
}

void MyWindow::linia(int x0, int y0, int x1, int y1, int R,int G,int B) {
    int d, dx, dy, ai, bi, xi, yi;
    int x = x0, y = y0;

    if (x0 < x1) {
        xi = 1;
        dx = x1 - x0;
    } else {
        xi = -1;
        dx = x0 - x1;
    }

    if (y0 < y1) {
        yi = 1;
        dy = y1 - y0;
    } else {
        yi = -1;
        dy = y0 - y1;
    }

    kolorowanko(x, y, R, G, B);

    if (dx > dy) {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;

        while (x != x1) {
            if (d >= 0) {
                x += xi;
                y += yi;
                d += ai;
            } else {
                d += bi;
                x += xi;
            }
            kolorowanko(x, y, R, G, B);
        }
    } else {
        ai = (dx - dy) * 2;
        bi = dx * 2;
        d = bi - dy;

        while (y != y1) {
            if (d >= 0) {
                x += xi;
                y += yi;
                d += ai;
            } else {
                d += bi;
                y += yi;
            }
            kolorowanko(x, y, R, G, B);
        }
    }
}
void MyWindow::malowanko(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b) {

    int minY = std::min({y0, y1, y2, y3});
    int maxY = std::max({y0, y1, y2, y3});

    for (int y = minY; y <= maxY; y++) {
        std::vector<int> intersections;

        if (y0 != y1 && ((y >= y0 && y <= y1) || (y >= y1 && y <= y0))) {
            int x = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
            intersections.push_back(x);
        }

        if (y1 != y2 && ((y >= y1 && y <= y2) || (y >= y2 && y <= y1))) {
            int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
            intersections.push_back(x);
        }


        if (y2 != y3 && ((y >= y2 && y <= y3) || (y >= y3 && y <= y2))) {
            int x = x2 + (y - y2) * (x3 - x2) / (y3 - y2);
            intersections.push_back(x);
        }

        if (y3 != y0 && ((y >= y3 && y <= y0) || (y >= y0 && y <= y3))) {
            int x = x3 + (y - y3) * (x0 - x3) / (y0 - y3);
            intersections.push_back(x);
        }

        if (intersections.size() == 2) {
            int xStart = std::min(intersections[0], intersections[1]);
            int xEnd = std::max(intersections[0], intersections[1]);
            for (int x = xStart; x <= xEnd; x++) {
                linia(x, y, x, y, r, g, b);
            }
        }
    }
}

bool MyWindow::czy_widac(punkt3D p0,punkt3D p1,punkt3D p2,punkt3D p3)
{
    punkt3D kamera = {1, 1, -1000};

    std::cout<<"sciana"<<std::endl;
    std::cout<<"x: "<<p0.x<<" y: "<<p0.y<<" z: "<<p0.z<<std::endl;
    std::cout<<"x: "<<p1.x<<" y: "<<p1.y<<" z: "<<p1.z<<std::endl;
    std::cout<<"x: "<<p2.x<<" y: "<<p2.y<<" z: "<<p2.z<<std::endl;
    std::cout<<"x: "<<p3.x<<" y: "<<p3.y<<" z: "<<p3.z<<std::endl;


    std::cout<<std::endl;
    // linia(p0.x,p0.y,p2.x,p2.y,218, 66, 245);

    // linia(p1.x,p1.y,p3.x,p3.y,218, 66, 245);

    // std::cout<<"x: "<<p0.x-p2.x
    //           <<" y: "<<p0.y-p2.y
    //           <<" z: "<<p0.z-p2.z<<std::endl;

    // std::cout<<"x: "<<p1.x-p3.x
    //           <<" y: "<<p1.y-p3.y
    //           <<" z: "<<p1.z-p3.z<<std::endl;

    //różnice współrzędnych dla obu wektorów
    double u_x = p0.x - p2.x;
    double u_y = p0.y - p2.y;
    double u_z = p0.z - p2.z;

    double a_u = (p0.y- p2.y)/(p0.x - p2.x);

    // wsp kier
    std::cout<<"a_u: "<<a_u<<std::endl;

    double v_x = p1.x - p3.x;
    double v_y = p1.y - p3.y;
    double v_z = p1.z - p3.z;

    double a_v = (p1.y- p3.y)/(p1.x - p3.x);

    // wsp kier
    std::cout<<"a_v: "<<a_v<<std::endl;

    std::cout << "Wektor u: x = " << u_x << " y = " << u_y << " z = " << u_z << std::endl;
    std::cout << "Wektor v: x = " << v_x << " y = " << v_y << " z = " << v_z << std::endl;

    //iloczyn wektorowy
    double w_x = 0;
    double w_y = 0;
    double w_z = 0;

    if(a_u < a_v)
    {
        w_x = u_y * v_z - u_z * v_y;
        w_y = u_z * v_x - u_x * v_z;
        w_z = u_x * v_y - u_y * v_x;
    }
    else
    {
        w_x = u_z * v_y - u_y * v_z;
        w_y = u_x * v_z - u_z * v_x;
        w_z = u_y * v_x - u_x * v_y;
    }
    std::cout<<"iloczyn wektorowy"<<std::endl;
    std::cout<<"x: "<<w_x<<" y: "<<w_y<<" z: "<<w_z<<std::endl;
    std::cout<<"iloczyn skladowych wektora: "<<w_x*w_y*w_z<<std::endl;

    double i_s = w_x*kamera.x + w_y*kamera.y + w_z*kamera.z;

    bool widoczna = true;

    if(i_s <= 0)
    {
        widoczna = false;

        // linia(p0.x,p0.y,p2.x,p2.y,218, 66, 245);
       //  linia(p1.x,p1.y,p3.x,p3.y,0, 233, 0);
        //   std::cout<<"tej nie powinno byc"<<std::endl;
    }
    // std::cout<<widoczna<<std::endl;

    return widoczna;
}
void MyWindow::rysuj(QVector<QPoint> punkty2D, QVector<punkt3D> punkty3D) {
    im.fill(0);

    int indices[][4] = {
        {0, 1, 3, 2},  // dolna ściana
        {4, 5, 7, 6},  // górna ściana
        {0, 1, 5, 4},  // przednia ściana
        {2, 3, 7, 6},  // tylna ściana
        {0, 2, 6, 4},  // lewa ściana
        {1, 3, 7, 5}   // prawa ściana
    };


    QVector<punkt3D> transformedPunkty3D;
    double ZZ=0;

    for (punkt3D p : punkty3D) {
        double x = p.x;
        double y = p.y;
        double z = p.z;

        Skalowanie(x, y, z);
        RotacjaZ(x, y);
        RotacjaY(x, z);
        RotacjaX(y, z);
        Przesuniecie(x, y, z);

        std::cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<std::endl;
        ZZ+=abs(z);
        transformedPunkty3D.append({x, y, z});
    }
    ZZ/=8;
    std::cout<<"srednia ZZ: "<<ZZ<<std::endl;
    std::cout<<std::endl;
    int niewidoczne[4][5][2];
    int j=0;
    for (int i = 0; i < 6; i++) {
        const auto &idx = indices[i];
        std::cout<<std::endl;
        if(czy_widac(transformedPunkty3D[idx[0]],transformedPunkty3D[idx[1]],transformedPunkty3D[idx[2]],transformedPunkty3D[idx[3]])==0)
        {
            std::cout<<"wyznaczanie niewidocznych..."<<std::endl;

            // std::cout<<"x: "<<punkty2D[idx[0]].x()<<" y: "<<punkty2D[idx[0]].y()<<std::endl;
            // std::cout<<"x: "<<punkty2D[idx[1]].x()<<" y: "<<punkty2D[idx[1]].y()<<std::endl;
            // std::cout<<"x: "<<punkty2D[idx[2]].x()<<" y: "<<punkty2D[idx[2]].y()<<std::endl;
            // std::cout<<"x: "<<punkty2D[idx[3]].x()<<" y: "<<punkty2D[idx[3]].y()<<std::endl;

            niewidoczne[0][j][0] = punkty2D[idx[0]].x();
            niewidoczne[0][j][1] = punkty2D[idx[0]].y();
            niewidoczne[1][j][0] = punkty2D[idx[1]].x();
            niewidoczne[1][j][1] = punkty2D[idx[1]].y();
            niewidoczne[2][j][0] = punkty2D[idx[2]].x();
            niewidoczne[2][j][1] = punkty2D[idx[2]].y();
            niewidoczne[3][j][0] = punkty2D[idx[3]].x();
            niewidoczne[3][j][1] = punkty2D[idx[3]].y();

            // std::cout<<"x: "<<niewidoczne[0][j][0]<<" y: "<<niewidoczne[0][j][1]<<std::endl;
            // std::cout<<"x: "<<niewidoczne[1][j][0]<<" y: "<<niewidoczne[1][j][1]<<std::endl;
            // std::cout<<"x: "<<niewidoczne[2][j][0]<<" y: "<<niewidoczne[2][j][1]<<std::endl;
            // std::cout<<"x: "<<niewidoczne[3][j][0]<<" y: "<<niewidoczne[3][j][1]<<std::endl;

            j++;
        }

        double Z0=abs(transformedPunkty3D[idx[0]].z);
        Z0 += abs(transformedPunkty3D[idx[1]].z);
        Z0 += abs(transformedPunkty3D[idx[2]].z);
        Z0 += abs(transformedPunkty3D[idx[3]].z);
        Z0/=4;

        std::cout<<"srednia Z0: "<<Z0<<std::endl;
        std::cout<<"srednia ZZ: "<<ZZ<<std::endl;
        if(hideHiddenEdges || light)
        {
            if(Z0 < ZZ)
            {
                linia(punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), 255,255,255);
                linia(punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), 255,255,255);
                linia(punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), 255,255,255);
                linia(punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), 255,255,255);

                std::cout<<"ta sciana jest widoczna"<<std::endl;
            }
            if(light && (Z0 < ZZ))
            {


                linia(punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), 255,255,255);
                linia(punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), 255,255,255);

                double x1 = abs(punkty2D[idx[1]].x()-punkty2D[idx[3]].x());
                double y1 = abs(punkty2D[idx[1]].y()-punkty2D[idx[3]].y());

                double x2 = abs(punkty2D[idx[0]].x()-punkty2D[idx[2]].x());
                double y2 = abs(punkty2D[idx[0]].y()-punkty2D[idx[2]].y());

                double x = abs(x1-x2);
                double y = abs(y1-y2);

                malowanko(punkty2D[idx[0]].x(), punkty2D[idx[0]].y(),
                          punkty2D[idx[1]].x(), punkty2D[idx[1]].y(),
                          punkty2D[idx[2]].x(), punkty2D[idx[2]].y(),
                          punkty2D[idx[3]].x(), punkty2D[idx[3]].y(),
                          0, 0, (int)(ZZ-Z0)%124+125);

                std::cout<<"ile czerwonosci: "<<x<<' '<<y<<std::endl;

            }
            if(Z0 < ZZ)
            {
                linia(punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), 255,255,255);
                linia(punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), 255,255,255);
                linia(punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), 255,255,255);
                linia(punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), 255,255,255);

            }
        }
        else
        {
            linia(punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), 255,255,255);
            linia(punkty2D[idx[2]].x(), punkty2D[idx[2]].y(), punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), 255,255,255);
            linia(punkty2D[idx[3]].x(), punkty2D[idx[3]].y(), punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), 255,255,255);
            linia(punkty2D[idx[0]].x(), punkty2D[idx[0]].y(), punkty2D[idx[1]].x(), punkty2D[idx[1]].y(), 255,255,255);
        }
    }
    ZZ=0;

    std::cout<<std::endl<<std::endl;
    update();
}

void MyWindow::RotacjaX(double &y, double &z) {
    double y1 = y * cos(alphaX) - z * sin(alphaX);
    double z1 = y * sin(alphaX) + z * cos(alphaX);
    y = y1;
    z = z1;
}
void MyWindow::RotacjaY(double &x, double &z) {
    double x1 = x * cos(alphaY) + z * sin(alphaY);
    double z1 = -x * sin(alphaY) + z * cos(alphaY);
    x = x1;
    z = z1;
}
void MyWindow::RotacjaZ(double &x, double &y) {
    double x1 = x * cos(alphaZ) - y * sin(alphaZ);
    double y1 = x * sin(alphaZ) + y * cos(alphaZ);
    x = x1;
    y = y1;
}
void MyWindow::Skalowanie(double &x, double &y, double &z) {
    x *= sX;
    y *= sY;
    z *= sZ;
}
void MyWindow::Przesuniecie(double &x, double &y, double &z) {
    x += tX;
    y += tY;
    z += tZ;
}

void MyWindow::dwaDtrzyD(QVector<punkt3D> pkt) {
    punkty2D.clear();

    Matrix4x4 q,w,e,r,t,wynik;
    q.powienkszanko(sX, sY, sZ);
    w.obracankoX(alphaX);
    e.obracankoY(alphaY);
    r.obracankoZ(alphaZ);
    t.przesuwanko(tX, tY, tZ);
    wynik = t * r * e * w * q;
    std::cout<<"macierze:"<<std::endl;
    q.pokazuj_to();
    w.pokazuj_to();
    e.pokazuj_to();
    r.pokazuj_to();
    t.pokazuj_to();
    std::cout<<"wynik:"<<std::endl<<std::endl;
    wynik.pokazuj_to();
    std::cout<<"koniec macierzy"<<std::endl<<std::endl;


    for (const punkt3D &p : pkt) {
        double x = p.x;
        double y = p.y;
        double z = p.z;

        wynik.wynosi(x, y, z);

        punkty2D.append(QPoint(static_cast<int>(x), static_cast<int>(y)));

        std::cout << "x: " << x << ' ' << " y: " << y << " z: "<< z <<std::endl;

    }
    std::cout << std::endl;
    rysuj(punkty2D,pkt);
}

void MyWindow::liczMacierze() {
    dwaDtrzyD(punkty);
}

void MyWindow::trX(int val) {
    tX = val;
    liczMacierze();
}

void MyWindow::trY(int val) {
    tY = val;
    liczMacierze();
}

void MyWindow::scX(int val) {
    sX = val / 100.0;
    liczMacierze();
}

void MyWindow::scY(int val) {
    sY = val / 100.0;
    liczMacierze();
}

void MyWindow::scZ(int val) {
    sZ = val / 100.0;
    liczMacierze();
}

void MyWindow::rX(int val) {
    alphaX = val * M_PI / 180.0;
    liczMacierze();
}

void MyWindow::rY(int val) {
    alphaY = val * M_PI / 180.0;
    liczMacierze();
}

void MyWindow::rZ(int val) {
    alphaZ = val * M_PI / 180.0;
    liczMacierze();
}

void MyWindow::toggleHiddenEdges(int state) {
    hideHiddenEdges = (state == Qt::Checked);
    liczMacierze();
}
void MyWindow::swiatlo(int state) {
    light = (state == Qt::Checked);
    liczMacierze();
}
