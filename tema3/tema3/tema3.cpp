#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <tuple>

#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159265358979323846
// dimensiunea ferestrei in pixeli
#define dim 300
unsigned char prevKey;

class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }


    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    double GetX() 
    {
        return m.x;
    }
    double GetY() 
    {
        return m.y;
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct operator=(CPunct p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    CPunct(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector operator=(CVector p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};


class GrilaCarteziana 
{
public:
    int lines;
    int columns;
    double size = 1;
    double offset_x = -0.5;
    double offset_y = -0.5;

    GrilaCarteziana(int lines, int columns)
    {
        this->lines = lines;
        this->columns = columns;
    }

    C2coord grilaCoordToPixel(int l, int c) 
    {
        double pixel_x = this->offset_x + (double)l / this->lines * this->size; 
        double pixel_y = this->offset_y + (double)c / this->columns * this->size;
        return C2coord(pixel_x, pixel_y);
    }

    void drawGrid() 
    {
        glColor3d(0, 0, 0);

        for (int c = 0; c <= this->columns; c++) 
        {
            double viewport_x = this->offset_x + (double)c / this->columns * this->size;

            glBegin(GL_LINES);
            glVertex2d(viewport_x, this->offset_x);
            glVertex2d(viewport_x, this->offset_x + this->size);
            glEnd();
        }

        for (int l = 0; l <= this->lines; l++) 
        {
            double viewport_y = this->offset_y + (double)l / this->lines * this->size;
                
            glBegin(GL_LINES);
            glVertex2d(this->offset_y, viewport_y);
            glVertex2d(this->offset_y + this->size, viewport_y);
            glEnd();
        }
    }
    //https://gist.github.com/linusthe3rd/803118
    void writePixel(int l, int c)
    {
        C2coord p = this->grilaCoordToPixel(l, c);
        int triangleAmount = 50;
        double radius = 0.02;
        GLfloat twicepi = 2 * PI;

        glLineWidth(2.0);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        for (int i = 1; i <= triangleAmount; i++)
        {
            glVertex2d(p.GetX(), p.GetY());
            glVertex2d(p.GetX() + (radius * cos(i * twicepi / triangleAmount)),
                       p.GetY() + (radius * sin(i * twicepi / triangleAmount))
            );
        }
        glEnd();
    }

    void applyThickness(int x, int y, int thickness)
    {
        if (y > 0)
            writePixel(x, y - thickness);
        if (y < this->lines)
            writePixel(x, y + thickness);

        if (x > 0)
            writePixel(x - thickness, y);
        if (x < this->columns)
            writePixel(x + thickness, y);
    }

    //bresenham midpoint
    //https://www.geeksforgeeks.org/mid-point-line-generation-algorithm/
    //https://gist.github.com/liuerfire/4369039
    void afisaresegmentdreapta3(int x0, int y0, int xmax, int ymax, int thickness)
    {
        C2coord p1 = this->grilaCoordToPixel(x0, y0);
        C2coord p2 = this->grilaCoordToPixel(xmax, ymax);
        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2d(p1.GetX(), p1.GetY());
        glVertex2d(p2.GetX(), p2.GetY());
        glEnd();

        int dy = ymax - y0;
        int dx = xmax - x0;

        int x = x0;
        int y = y0;

        int slope_y = 1;
        int slope_x = 1;
        if (dy < 0)
        {
            slope_y = -1;
            dy = -dy;
        }
        if (dx < 0)
        {
            slope_x = -1;
            dx = -dx;
        }

        writePixel(x, y);
        applyThickness(x, y, thickness);

        if (dy <= dx)
        {
            int d = 2 * dy - dx;
            int dE = 2 * dy;
            int dNE = 2 * (dy - dx);

            while (x * slope_x < xmax)
            {
                if (d <= 0)
                {
                    d += dE;
                }
                else
                {
                    d += dNE;
                    y += slope_y;
                }
                x += slope_x;
                writePixel(x, y);
                applyThickness(x,y,thickness);
            }
        }
        else if (dx < dy)
        {
            int d = 2 * dx - dy;
            int dE = 2 * dx;
            int dNE = 2 * (dx - dy);

            while (y  < ymax)
            {
                if (d <= 0)
                {
                    d += dE;
                }
                else
                {
                    d += dNE;
                    x += slope_x;
                }
                y += slope_y;
                writePixel(x, y);
                applyThickness(x,y,thickness);
            }
        }
        
    }
    
};

GrilaCarteziana grila(15, 15);

void drawCircle()
{
    
}


void Display1() {
    glScaled(1.5, 1.5, 1);
    grila.drawGrid();
    /*for(int i = 0; i <= grila.lines; i++)
        for(int j = 0; j <= grila.columns; j++)
            grila.writePixel(i, j);  */
    
    //grila.afisaresegmentdreapta3(0, 0, 15, 7, 0); 
    //grila.afisaresegmentdreapta3(0, 15, 15, 10, 1);

    grila.afisaresegmentdreapta3(0, 5, 15, 5, 0);
    grila.afisaresegmentdreapta3(15, 10, 0, 4, 0);
    grila.afisaresegmentdreapta3(0, 4, 15, 7, 0);
    grila.afisaresegmentdreapta3(7, 0, 7, 15, 0);
}

void Display2() {
    
}

void Display3() {
    
}

void Display4() {
        
}



void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
  switch(prevKey) {
  case '1':
    glClear(GL_COLOR_BUFFER_BIT);
    Display1();
    break;
  case '2':
    glClear(GL_COLOR_BUFFER_BIT);
    Display2();
    break;
  default:
    break;
  }

  glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}

