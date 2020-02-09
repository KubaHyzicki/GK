/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych

/*************************************************************************************/

// #include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <unistd.h>
#include <math.h>


typedef float point3[3];

static GLfloat viewer[]= {0.0, 0.0, 10.0}; 


static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
                               // 0 - nie naciśnięto żadnego klawisza
                               // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old=0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
                                      // i poprzednią kursora myszy 


void spinEgg()
{

    theta[0] -= 0.5;
    if( theta[0] > 360.0 ) theta[0] -= 360.0;

    theta[1] -= 0.5;
    if( theta[1] > 360.0 ) theta[1] -= 360.0;

    theta[2] -= 0.5;
    if( theta[2] > 360.0 ) theta[2] -= 360.0;

    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

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



float calcX(float u, float v){
    float f=(0 - pow(u,5)*90 + pow(u,4)*225 - pow(u,3)*270 + pow(u,2)*180 - u*45) * cos(v*M_PI);
    return f;
}
float calcY(float u, float v){
    float f=0 + pow(u,4)*160 - pow(u,3)*320 + pow(u,2)*160;
    return f;
}

float calcZ(float u, float v){
    float f=(0 - pow(u,5)*90 + pow(u,4)*225 - pow(u,3)*270 + pow(u,2)*180 - u*45) * sin(v*M_PI);
    return f;
}


float calcXu(float u, float v){
    float f=(0 - pow(u,4)*450 + pow(u,3)*900 - pow(u,2)*810 + u*360 - 45) * cos(v*M_PI);
    return f;
}
float calcYu(float u, float v){
    float f=0 + pow(u,3)*640 - pow(u,2)*960 + u*320;
    return f;
}

float calcZu(float u, float v){
    float f=(0 - pow(u,4)*450 + pow(u,3)*900 - pow(u,2)*810 + u*360 - 45) * sin(v*M_PI);
    return f;
}



float calcXv(float u, float v){
    float f=M_PI*(0 + pow(u,5)*90 - pow(u,4)*225 + pow(u,3)*270 - pow(u,2)*180 + u*45) * sin(v*M_PI);
    return f;
}
float calcYv(float u, float v){
    return 0;
}

float calcZv(float u, float v){
    float f=M_PI*(0 + pow(u,5)*90 - pow(u,4)*225 + pow(u,3)*270 - pow(u,2)*180 + u*45) * sin(v*M_PI);
    return f;
}

float *eggVec(float u, float v){
    float vec[3]={calcYu(u,v)*calcZv(u,v)-calcZu(u,v)*calcYv(u,v), calcZu(u,v)*calcXv(u,v)-calcXu(u,v)*calcZv(u,v), calcXu(u,v)*calcYv(u,v)-calcYu(u,v)*calcXv(u,v)};
    return vec;
}

void drawEgg(){
    int N=40;
    float baseVar=1.0/N;
    float u,v;
    float lastU,lastV,lastButOneU,lastButOneV,lastButTwoU,lastButTwoV;
    //point tab[10];

    glBegin(GL_TRIANGLES);

    // for(int i=1;i<=N;i++){
    //  for(int i2=1;i2<=N;i2++){
    //      u=baseVar*i;
    //      v=baseVar*i2;
    //      glColor3f(u, v, u*v);
    //      glVertex3f(calcX(u,v),calcY(u,v)-5,calcZ(u,v));
    //  }
    // }
    for(int i2=0;i2<=N;i2++){
        for(int i=0;i<=N;i++){
            u=baseVar*i;
            v=baseVar*i2;
            if((i == 0 && i2 == 0) || (i == 1 && i2 == 0 ) || (i == 2 && i2 == 0 )) {
                lastButTwoV=lastButOneV;
                lastButTwoU=lastButOneU;
                lastButOneV=lastV;
                lastButOneU=lastU;
                lastV=v;
                lastU=u;
                continue;
            }
            glColor3f(u, v, u*v);
            glVertex3f(calcX(u,v),calcY(u,v)-5,calcZ(u,v));
            glColor3f(lastU, lastV, lastU*lastV);
            glVertex3f(calcX(lastU,lastV),calcY(lastU,lastV)-5,calcZ(lastU,lastV));
            glColor3f(lastButOneU, lastButOneV, lastButOneU*lastButOneV);
            glVertex3f(calcX(lastButOneU,lastButOneV),calcY(lastButOneU,lastButOneV)-5,calcZ(lastButOneU,lastButOneV));

            if(i == 4 && i2 == 0){
                lastButTwoV=lastButOneV;
                lastButTwoU=lastButOneU;
                lastButOneV=lastV;
                lastButOneU=lastU;
                lastV=v;
                lastU=u;
                continue;
            }

            glColor3f(u, v, u*v);
            glVertex3f(calcX(u,v),calcY(u,v)-5,calcZ(u,v));
            glColor3f(lastButOneU, lastButOneV, lastButOneU*lastButOneV);
            glVertex3f(calcX(lastButOneU,lastButOneV),calcY(lastButOneU,lastButOneV)-5,calcZ(lastButOneU,lastButOneV));
            glColor3f(lastButTwoU, lastButTwoV, lastButTwoU*lastButTwoV);
            glVertex3f(calcX(lastButTwoU,lastButTwoV),calcY(lastButTwoU,lastButTwoV)-5,calcZ(lastButTwoU,lastButTwoV));

        }
    }

    glEnd();
}
 

void RenderScene(void)
{
   // for(int i;i<3;i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Czyszczenie okna aktualnym kolorem czyszczącym

        glLoadIdentity();

        gluLookAt(viewer[0],viewer[1],viewer[2], 1.0, 1.0, 0.0, 1.0, 1.0, 0.0);

        glColor3f(1.0f, 1.0f, 1.0f);
        // Ustawienie koloru rysowania na biały

        // glutSolidTeapot(3.0);
        // Narysowanie czajnika
        drawEgg();
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
// Kolor czyszczący (wypełnienia okna) ustawiono na czarny

/*************************************************************************************/

//  Definicja materiału z jakiego zrobiony jest czajnik 
//  i definicja źródła światła

/*************************************************************************************/


/*************************************************************************************/  
// Definicja materiału z jakiego zrobiony jest czajnik 

    GLfloat mat_ambient[]  = {1.0, 1.0, 1.0, 1.0};        
    // współczynniki ka =[kar,kag,kab] dla światła otoczenia

    GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    // współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    // współczynniki ks =[ksr,ksg,ksb] dla światła odbitego                
    
    GLfloat mat_shininess  = {20.0};
    // współczynnik n opisujący połysk powierzchni


/*************************************************************************************/
// Definicja źródła światła


     GLfloat light_position[] = {0.0, 0.0, 10.0, 1.0};    
    // położenie źródła


    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    // składowe intensywności świecenia źródła światła otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};        
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[]= {1.0, 1.0, 1.0, 1.0};
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant  = {1.0};
    // składowa stała ds dla modelu zmian oświetlenia w funkcji 
    // odległości od źródła

    GLfloat att_linear    = {0.05}; 
    // składowa liniowa dl dla modelu zmian oświetlenia w funkcji 
    // odległości od źródła

    GLfloat att_quadratic  = {0.001};
    // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

/*************************************************************************************/                                      
// Ustawienie parametrów materiału i źródła światła

/*************************************************************************************/
// Ustawienie patrametrów materiału


    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


/*************************************************************************************/
// Ustawienie parametrów źródła

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


/*************************************************************************************/
// Ustawienie opcji systemu oświetlania sceny 

    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny 
    glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
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