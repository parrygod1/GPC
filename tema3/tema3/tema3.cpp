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

    C2coord(C2coord& p)
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

    double static GetViewportCoordOnGrid(int lin_col_size, double offset, double grid_size, int lin_col) 
    {
        double quotent = (double)lin_col / lin_col_size;
        return offset + quotent * grid_size;
    }
    void DrawSelf() {
        glColor3d(0, 0, 0);

        // Draw lines
        for (int l = 0; l <= this->lines; l++) {
            double viewport_x = GetViewportCoordOnGrid(this->lines, this->offset_x, this->size, l);

            glBegin(GL_LINES);
            glVertex2d(viewport_x, this->offset_x);
            glVertex2d(viewport_x, this->offset_x + this->size);
            glEnd();
        }

        // Draw columns
        for (int c = 0; c <= this->columns; c++) {
            double viewport_y = GetViewportCoordOnGrid(this->columns, this->offset_y, this->size, c);
                
            glBegin(GL_LINES);
            glVertex2d(this->offset_y, viewport_y);
            glVertex2d(this->offset_y + this->size, viewport_y);
            glEnd();
        }
    }

    void writePixel(int l, int c, double radius = 0.045)
    {
        double viewport_x, viewport_y;
        std::tie(viewport_x, viewport_y) = this->GetViewportFromInteger(l, c);
        int triangleAmount = 50;

        glLineWidth(2.0);
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINES);
        for (int i = 0; i <= triangleAmount; i++)
        {
            glVertex2d(viewport_x, viewport_y);
            glVertex2d(viewport_x + (radius * cos(i * 2 * PI / triangleAmount)),
                viewport_y + (radius * sin(i * 2 * PI / triangleAmount)));
        }
        glEnd();
    }

    std::pair<double, double> GetViewportFromInteger(int x, int y) {
        double viewport_x = GetViewportCoordOnGrid(this->lines, this->offset_x, this->size, x);
        double viewport_y = GetViewportCoordOnGrid(this->lines, this->offset_y, this->size, y);

        return std::make_pair(viewport_x, viewport_y);
    }
};

GrilaCarteziana grila(5, 5);

void drawCircle()
{
    
}


void Display1() {
    glScaled(1.5, 1.5, 1);
    grila.DrawSelf();
    for(int i = 0; i <= grila.lines; i++)
        for(int j = 0; j <= grila.columns; j++)
            grila.writePixel(i, j, 0.02);  
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

