/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych

/*************************************************************************************/

// #include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <unistd.h>


typedef float point3[3];

static GLfloat viewer[]= {0.0, 0.0, 10.0}; 
static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
                               // 0 - nie naciśnięto żadnego klawisza
                               // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old=0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
                                      // i poprzednią kursora myszy 
// inicjalizacja położenia obserwatora

/*************************************************************************************/

// Funkcja rysująca osie układu wspó?rz?dnych

 /*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)        
    {
        x_pos_old=x;         // przypisanie aktualnie odczytanej pozycji kursora 
                             // jako pozycji poprzedniej
        status = 1;          // wcięnięty został lewy klawisz myszy
    } 
    else

        status = 0;          // nie został wcięnięty żaden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich 
// zmiennych globalnych

void Motion( GLsizei x, GLsizei y )
{
    
    delta_x=x-x_pos_old;     // obliczenie różnicy położenia kursora myszy

    x_pos_old=x;            // podstawienie bieżącego położenia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

void Axes(void){
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

void RenderScene(void)
{
   // for(int i;i<3;i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Czyszczenie okna aktualnym kolorem czyszczącym

        glLoadIdentity();
        // Czyszczenie macierzy bie??cej

        // if(status == 1)                     // jeśli lewy klawisz myszy wcięnięty
        // {
        //    theta += delta_x*pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
        //     // gluLookAt(viewer[0],viewer[1],viewer[2], 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        // }                                  // do różnicy położeń kursora myszy


        gluLookAt(viewer[0],viewer[1],viewer[2], 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        glColor3f(1.0f, 1.0f, 1.0f);
        // Ustawienie koloru rysowania na biały

        glutSolidTeapot(3.0);
        // Narysowanie czajnika
        glFlush();
        // Przekazanie poleceń rysujących do wykonania
        glutSwapBuffers();
        // sleep(1);
   // }

 

 }
/*************************************************************************************/

// Funkcja ustalająca stan renderowania

 

void MyInit(void)
{

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLfloat mat_ambient[]  = {1.0, 1.0, 1.0, 1.0};        
    GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess  = {10.0};
/*************************************************************************************/
    GLfloat light_position[] = {10.0, 10.0, 10.0, 1.0};
    GLfloat light_ambient[] = {0.4, 0.1, 0.1, 0.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};        
    GLfloat light_specular[]= {1.0, 1.0, 1.0, 1.0};
    GLfloat att_constant  = {1.0};
    GLfloat att_linear    = {0.05}; 
    GLfloat att_quadratic  = {0.001};
/*************************************************************************************/
    GLfloat light_position2[] = {-10.0, -10.0, -10.0, 2.0};    
    GLfloat light_ambient2[] = {0.2, 0.1, 0.1, 0.0};
    GLfloat light_diffuse2[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular2[]= {1.0, 1.0, 1.0, 1.0};
    GLfloat att_constant2  = {1.0};
    GLfloat att_linear2    = {0.05}; 
    GLfloat att_quadratic2  = {0.001};
/*************************************************************************************/
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
/*************************************************************************************/
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
/*************************************************************************************/
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, att_constant2);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, att_linear2);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, att_quadratic2);
/*************************************************************************************/
    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny 
    glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
    glEnable(GL_LIGHT2);     // włączenie źródła o numerze 2
    glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora 

/*************************************************************************************/

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


int main(int argc, char **argv){
    srand(time(NULL));
    glutInit( & argc, argv );
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Ciastko");
    glutDisplayFunc(RenderScene);
    glutMouseFunc(Mouse);
    // mysz
    glutMotionFunc(Motion);

    glutReshapeFunc(ChangeSize);
    MyInit();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}