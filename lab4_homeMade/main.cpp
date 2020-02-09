// lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>


using namespace std;
/*************************************************************************************/


typedef float point3[3];

static GLfloat viewer[]= {0.0, 0.0, 10.0}; 

// inicjalizacja położenia obserwatora
static GLfloat thetaX = 0.0;   // kąt obrotu obiektu
static GLfloat thetaY = 0.0;
static GLfloat thetaZoom = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint statusXY = 0;       // stan klawiszy myszy 
static GLint statusZoom = 0;
                               // 0 - nie naciśnięto żadnego klawisza
                               // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old=0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą

static int y_pos_old=0;       // poprzednia pozycja kursora myszy

static int delta_y = 0;        // różnica pomiędzy pozycją bieżącą
                                      // i poprzednią kursora myszy 
/*************************************************************************************/

// Funkcja rysująca osie układu wspó?rz?dnych

 

void Axes(void)
{

point3  x_min = {-5.0, 0.0, 0.0};
point3  x_max = { 5.0, 0.0, 0.0};
// pocz?tek i koniec obrazu osi x

point3  y_min = {0.0, -5.0, 0.0}; 
point3  y_max = {0.0,  5.0, 0.0};
// pocz?tek i koniec obrazu osi y

point3  z_min = {0.0, 0.0, -5.0};
point3  z_max = {0.0, 0.0,  5.0};
//  pocz?tek i koniec obrazu osi y
glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
glBegin(GL_LINES); // rysowanie osi x
glVertex3fv(x_min);
glVertex3fv(x_max);
glEnd();

glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
glBegin(GL_LINES);  // rysowanie osi y

glVertex3fv(y_min);
glVertex3fv(y_max);                           
glEnd();

glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
glBegin(GL_LINES); // rysowanie osi z

glVertex3fv(z_min);
glVertex3fv(z_max);
glEnd();

}

/*************************************************************************************/

// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba 
// przerysować scenę)
/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)        
    {
        x_pos_old=x;         // przypisanie aktualnie odczytanej pozycji kursora 
        y_pos_old=y;         // przypisanie aktualnie odczytanej pozycji kursora 
                             // jako pozycji poprzedniej
        statusXY = 1;          // wcięnięty został lewy klawisz myszy
    } 
    else
        statusXY = 0;

    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        statusZoom=1;
    }
    else
        statusZoom=0;
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich 
// zmiennych globalnych

void Motion( GLsizei x, GLsizei y )
{
    delta_x=x-x_pos_old;     // obliczenie różnicy położenia kursora myszy
    x_pos_old=x;            // podstawienie bieżącego położenia jako poprzednie
    delta_y=y-y_pos_old;     // obliczenie różnicy położenia kursora myszy
    y_pos_old=y;            // podstawienie bieżącego położenia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/
 

void RenderScene(void)
{

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Czyszczenie okna aktualnym kolorem czyszczącym

glLoadIdentity();
// Czyszczenie macierzy bie??cej

gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// gluLookAt(3.0, 3.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
// Zdefiniowanie położenia obserwatora
Axes();
// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej


if(statusXY == 1)                     // jeśli lewy klawisz myszy wcięnięty
{
   thetaX += delta_x*pix2angle;
   thetaY += delta_y*pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
}                                  // do różnicy położeń kursora myszy

if(statusZoom == 1)
{
    thetaZoom += delta_y;
}

glRotatef(thetaX, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt

glRotatef(thetaY, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kąt

glScalef(thetaZoom/100,thetaZoom/100,thetaZoom/100);

glColor3f(1.0f, 1.0f, 1.0f);
// Ustawienie koloru rysowania na biały

glutWireTeapot(3.0);
// Narysowanie czajnika
glFlush();
// Przekazanie poleceń rysujących do wykonania
     glutSwapBuffers();

 

 }
/*************************************************************************************/

// Funkcja ustalająca stan renderowania

 

void MyInit(void)
{

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszczący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/


// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

 

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0/(float)horizontal;  // przeliczenie pikseli na stopnie

    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej 

    gluPerspective(70, 1.0, 1.0, 30.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);

    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej 

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

 

               

// int main(void)
int main(int argc, char **argv){
    srand(time(NULL));
    glutInit( & argc, argv );
    glutInitWindowSize(300, 300);
    // glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutCreateWindow("Ciastko");
    glutDisplayFunc(RenderScene);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutReshapeFunc(ChangeSize);
    MyInit();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
