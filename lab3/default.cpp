// lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>

using namespace std;
/*************************************************************************************/



// Funkcaja określająca, co ma być rysowane
// (zawsze wywoływana, gdy trzeba przerysować scenę)


float rand01Val() {
    float result = rand() % 100;
    result = result / 100;
    return result;
}

float randVal(float max) {
    int range = (int)max * 2 % 1;
    float result = rand() % range - max;
    return result;
}

void recktangle(int x, int y, int length, int aberration) {
    glBegin(GL_QUADS);
    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x, y);

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x, y + length);

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + length, y + length);

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + length, y);


    /*
    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + randVal(aberration), y + randVal(aberration));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + randVal(aberration), y + length + randVal(aberration));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + length + randVal(aberration), y + length + randVal(aberration));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(x + length + randVal(aberration), y + randVal(aberration));
    */
    glEnd();
    glFlush();
}

void fractal(int baseX, int baseY, int baseLength, int baseAberration) {
    for (int ix = 0; ix < 3; ix++) {
        for (int iy = 0; iy < 3; iy++) {
            if (ix == 1 && iy == 1) continue;
            recktangle(baseX + ix * baseLength, baseY + iy * baseLength, baseLength, baseAberration);
        }
    }
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    int depth = 3;
    int baseX = 0;
    int baseY = 0;
    int baseLength = 10;
    int baseAberration = 1;
    int X=0, Y=0;
    for (int d = 0; d < depth; d++) {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                baseX = baseX + 3 * baseLength;
                baseY = baseY + 3 * baseLength;
                fractal(X, Y, baseLength, baseAberration);
            }
        }
    }
    glFlush();
}

void RenderScene_lastWorking(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    int depth = 3;
    int baseX = 0;
    int baseY = 0;
    int baseLength = 10;
    int baseAberration = 1;
    for (int d = 0; d < depth; d++) {
        fractal(baseX, baseY, baseLength, baseAberration);
        baseX = baseX + 3 * baseLength;
        baseY = baseY + 3 * baseLength;
    }
    glFlush();
}

void Strange_Recktangle(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(-50.0f + randVal(10), -50.0f + randVal(10));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(50.0f + randVal(10), -50.0f + randVal(10));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(50.0f + randVal(10), 50.0f + randVal(10));

    glColor3f(rand01Val(), rand01Val(), rand01Val());
    glVertex2f(-50.0f + randVal(10), 50.0f + randVal(10));

    glEnd();

    glFlush();
}
void Triangles(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 50.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(50.0f, 0.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 50.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-50.0f, 0.0f);

    glEnd();

    glFlush();
}



void MyInit(void)

{

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // Kolor okna wnętrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;


    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna urządzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)


    glMatrixMode(GL_PROJECTION);
    // Określenie układu współrzędnych obserwatora

    glLoadIdentity();
    // Określenie przestrzeni ograniczającej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie współczynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // określenie okna obserwatora.
    // Pozwala to zachować właściwe proporcje rysowanego obiektu
    // Do określenia okna obserwatora służy funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Określenie układu współrzędnych

    glLoadIdentity();

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

//void recktangle() {
//	return
//}

// int main(void)
int main(int argc, char **argv){
    srand(time(NULL));
    glutInit( & argc, argv );
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Ciastko");
    // glutDisplayFunc(Triangles);
    glutReshapeFunc(ChangeSize);
    MyInit();
    glutMainLoop();
}
