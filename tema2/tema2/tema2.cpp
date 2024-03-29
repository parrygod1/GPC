#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1);
    double ratia = 0.05;

    // calculul valorilor maxime/minime ptr. x si y
    // aceste valori vor fi folosite ulterior la scalare
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
    double pi = 4 * atan(1);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}

//2.1
double func(double x)
{
    if (x == 0)
        return 1;
    else
    {
        double closest = round(x);
        double dist = closest - x;
        return fabs(dist)/x;
    }
}

void Display3()
{
    double ymax = -15;
    double ymin = 5;
    double xmax = 20;
    double ratia = 0.05;

    for (double x = 0; x < 100; x += ratia) {
        double result = func(x);
        if (result > ymax) ymax = result;
        if (result < ymin) ymin = result;
    }
    
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);

    for (double x = 0; x < 100; x += ratia) {
        double y;
        y = func(x) / ymax;

        glVertex2d(x / xmax, y);
    }
    glEnd();
}

double melc_x(double t, double a, double b)
{
    return 2 * (a * cos(t) + b) * cos(t);
}

double melc_y(double t, double a, double b)
{
    return 2 * (a * cos(t) + b) * sin(t);
}
//2.2.1 melc
void Display4()
{
    double a = 0.3;
    double b = 0.2;
    double ymax = -1;
    double ymin = 1;
    double xmax = -1;
    double xmin = 1;
    double ratia = 0.05;
    double pi = 4 * atan(1);

    for (double t = pi; t < pi; t +=ratia)
    {
        double x = melc_x(t,a,b);
        double y = melc_y(t,a,b);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_LOOP);

    for (double t = -pi; t < pi; t += ratia) {
        double x = melc_x(t, a, b);
        double y = melc_y(t, a, b);
        glVertex2d(x / xmax, y / ymax);
    }
    glEnd();
}

//222 longchamps
double longchamps_x(double t, double a)
{
    return a / (4 * cos(t)* cos(t) - 3);
}

double longchamps_y(double t, double a)
{
    return a * tan(t) / (4 * cos(t) * cos(t) - 3);
}

//sa ne fereasca domnu sa folosim pow
void Display5()
{
    double a = 0.2;
    double ymax = -1;
    double ymin = 1;
    double xmax = -1;
    double xmin = 1;
    double ratia = 0.05;
    double pi = 4 * atan(1);

    //-pi/6 pentru cadranul stanga sus
    for (double t = -pi/2 + ratia; t < -pi/6; t += ratia)
    {
        if (t != pi / 6 && t != -pi / 6)
        {
            double x = longchamps_x(t, a);
            double y = longchamps_y(t, a);
            if (x < xmin) xmin = x;
            if (x > xmax) xmax = x;
            if (y < ymin) ymin = y;
            if (y > ymax) ymax = y;
        }
        
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(0, 0, 1.0);
    glBegin(GL_LINE_STRIP);

    for (double t = -pi / 2 + ratia; t < -pi / 6; t += ratia)
    {
        if (t != pi / 6 && t != -pi / 6)
        {
            double x = longchamps_x(t, a);
            double y = longchamps_y(t, a);
            glVertex2d(x / xmax, y / ymax);
        }
    }
    glEnd();

    glColor3f(1.0, 0, 0);

    double step = 0.00002;
    double start = 0;
    bool drawingtri = false;

    for (double t = -pi / 2 + ratia; t < -pi / 6; t += ratia)
    {
        if (t != pi / 6 && t != -pi / 6)
        {
            double x = longchamps_x(t, a);
            double y = longchamps_y(t, a);

            if (!drawingtri)
            {
                glBegin(GL_TRIANGLES);
                glVertex2d(-1, 1);
                glVertex2d(x / xmax, y / ymax);
                start = y / ymax;
                drawingtri = true;
            }
            else if (fabs(y / ymax - start) >= step)
            {
                glVertex2d(x / xmax, y / ymax);
                glEnd();
                drawingtri = false;
            }
        }
    }
    glEnd();
    
}


double cicloida_x(double t, double a, double b)
{
    return a * t - b * sin(t);
}

double cicloida_y(double t, double a, double b)
{
    return a - b * cos(t);
}


void Display6()
{
    double a = 0.1;
    double b = 0.2;
    double ymax = -9999;
    double ymin = 9999;
    double xmax = -9999;
    double xmin = 9999;
    double ratia = 0.05;

    for (double t = -10; t <= 10; t += ratia)
    {
        double x = cicloida_x(t, a, b);
        double y = cicloida_y(t, a, b);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_STRIP);

    for (double t = -10; t <= 10; t += ratia)
    {
        double x = cicloida_x(t, a, b);
        double y = cicloida_y(t, a, b);
        glVertex2d(x / xmax/ 1.1, y / ymax/2); 
    }
    glEnd();
}

double epicicloidala_x(double t, double r, double R)
{
    return (R + r) * cos(r / R * t) - r * cos(t + (r / R * t));
}

double epicicloidala_y(double t, double r, double R)
{
    return (R + r) * sin(r / R * t) - r * sin(t + (r / R * t));
}

void Display7()
{
    double R = 0.1;
    double r = 0.3;
    double ymax = -9999;
    double ymin = 9999;
    double xmax = -9999;
    double xmin = 9999;
    double ratia = 0.05;

    double domaina = 0;
    double domainb = 4 * atan(1) * 2;

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = epicicloidala_x(t, r, R);
        double y = epicicloidala_y(t, r, R);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_LOOP);

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = epicicloidala_x(t, r, R);
        double y = epicicloidala_y(t, r, R);
        glVertex2d(x / xmax, y / ymax);
    }
    glEnd();
}

double hipocicloidala_x(double t, double r, double R)
{
    return (R - r) * cos(r / R * t) - r * cos(t - (r / R * t));
}

double hipocicloidala_y(double t, double r, double R)
{
    return (R - r) * sin(r / R * t) - r * sin(t - (r / R * t));
}

void Display8()
{
    double R = 0.1;
    double r = 0.3;
    double ymax = -9999;
    double ymin = 9999;
    double xmax = -9999;
    double xmin = 9999;
    double ratia = 0.05;

    double domaina = 0;
    double domainb = 4 * atan(1) * 2;

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = hipocicloidala_x(t, r, R);
        double y = hipocicloidala_y(t, r, R);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_LOOP);

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = hipocicloidala_x(t, r, R);
        double y = hipocicloidala_y(t, r, R);
        glVertex2d(x / xmax, y / ymax);
    }
    glEnd();
}

double lamniscata_x(double t, double a)
{
    return a * sqrt(2 * cos(2 * t)) * cos(t);
}

double lamniscata_y(double t, double a)
{
    return a * sqrt(2 * cos(2 * t)) * sin(t);
}

void Display9()
{
    double a = 0.4;
    double ymax = -9999;
    double ymin = 9999;
    double xmax = -9999;
    double xmin = 9999;
    double ratia = 0.05;

    double domaina = -4 * atan(1) / 4;
    double domainb = 4 * atan(1)/4;

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = lamniscata_x(t, a);
        double y = lamniscata_y(t, a);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_STRIP);

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = -lamniscata_x(t, a);
        double y = -lamniscata_y(t, a);
        glVertex2d(x / xmax / 1.3, y / ymax / 3);
    }
    glEnd();

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = lamniscata_x(t, a);
        double y = lamniscata_y(t, a);
        glVertex2d(x / xmax / 1.3, y / ymax / 3);
    }
    glEnd();
}

double spirala_x(double t, double a)
{
    return a * exp(1 + t) * cos(t);
}

double spirala_y(double t, double a)
{
    return a * exp(1 + t) * sin(t);
}

void Display10()
{
    double a = 0.02;
    double ymax = -9999;
    double ymin = 9999;
    double xmax = -9999;
    double xmin = 9999;
    double ratia = 0.05;

    double domaina = 0;
    double domainb = 4 * atan(1) * 2;

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = spirala_x(t, a);
        double y = spirala_y(t, a);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_STRIP);

    for (double t = domaina; t <= domainb; t += ratia)
    {
        double x = -spirala_x(t, a);
        double y = -spirala_y(t, a);
        glVertex2d(x / xmax , y / ymax/3);
    }
    glEnd();
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    case '9':
        Display9();
        break;
    case '0':
        Display10();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}