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


GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{

/*************************************************************************************/

// Struktura dla nagłówka pliku  TGA


    #pragma pack(1)            
    typedef struct                        
    {
        GLbyte    idlength;              
        GLbyte    colormaptype;           
        GLbyte    datatypecode;             
        unsigned short    colormapstart;  
        unsigned short    colormaplength; 
        unsigned char     colormapdepth;   
        unsigned short    x_orgin;         
        unsigned short    y_orgin;         
        unsigned short    width;          
        unsigned short    height;         
        GLbyte    bitsperpixel;                   
        GLbyte    descriptor;             
    }TGAHEADER;
    #pragma pack(8)

    FILE *pFile;                    
    TGAHEADER tgaHeader;            
    unsigned long lImageSize;        
    short sDepth;                    
    GLbyte    *pbitsperpixel = NULL; 

           
/*************************************************************************************/ 

// Wartości domyślne zwracane w przypadku błędu 

    *ImWidth = 0;                
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;
    
    pFile = fopen(FileName, "rb");
    if(pFile == NULL)
      return NULL;

/*************************************************************************************/ 
// Przeczytanie nagłówka pliku 


    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
                

/*************************************************************************************/

// Odczytanie szerokości, wysokości i głębi obrazu 

    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;


/*************************************************************************************/
// Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity) 
    
    if(tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
        return NULL;

/*************************************************************************************/

// Obliczenie rozmiaru bufora w pamięci


    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


/*************************************************************************************/   

// Alokacja pamięci dla danych obrazu


     pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    
      if(pbitsperpixel == NULL)
        return NULL;

    if(fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
        {
        free(pbitsperpixel);
        return NULL;
        }
   

/*************************************************************************************/

// Ustawienie formatu OpenGL


    switch(sDepth)

        {

        case 3:    

            *ImFormat = GL_BGR_EXT;

            *ImComponents = GL_RGB8;

            break;

        case 4:

            *ImFormat = GL_BGRA_EXT;

            *ImComponents = GL_RGBA8;

            break;

        case 1:

            *ImFormat = GL_LUMINANCE;

            *ImComponents = GL_LUMINANCE8;

            break;

        };

     

    fclose(pFile);

       

    return pbitsperpixel;

}

/*************************************************************************************/ 

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

// glColor3f(1.0f, 1.0f, 1.0f);
// Ustawienie koloru rysowania na biały
if(statusXY == 1)                     // jeśli lewy klawisz myszy wcięnięty
{
   thetaX += delta_x*pix2angle;
   thetaY += delta_y*pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
}                                  // do różnicy położeń kursora myszy

if(statusZoom == 1)
{
    thetaZoom += delta_y;

glScalef(thetaZoom/100,thetaZoom/100,thetaZoom/100);
}

glRotatef(thetaX, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt

glRotatef(thetaY, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kąt

glCullFace(GL_FRONT_AND_BACK);
glDisable(GL_CULL_FACE);
glBegin(GL_TRIANGLES);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glTexCoord2f(2.0f, 0.0f);
        glVertex3f(2.0f, 0.0f, 0.0f);
        
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(1.0f, 2.0f, 0.0f);
    
glEnd();

// Narysowanie czajnika
glFlush();
// Przekazanie poleceń rysujących do wykonania
     glutSwapBuffers();

 

 }
/*************************************************************************************/

// Funkcja ustalająca stan renderowania

 

void MyInit(void)
{
    /*************************************************************************************/

// Zmienne dla obrazu tekstury

 

 GLbyte *pBytes;
 GLint ImWidth, ImHeight, ImComponents;
 GLenum ImFormat;


           // ..................................       
           //       Pozostała część funkcji MyInit()

           // ..................................

/*************************************************************************************/

// Teksturowanie będzie prowadzone tyko po jednej stronie ściany 

 glEnable(GL_CULL_FACE);


/*************************************************************************************/

//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

 pBytes = LoadTGAImage("N1_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);


 /*************************************************************************************/

// Zdefiniowanie tekstury 2-D 

 glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

/*************************************************************************************/

// Zwolnienie pamięci
       
 free(pBytes);


/*************************************************************************************/

// Włączenie mechanizmu teksturowania

 glEnable(GL_TEXTURE_2D);

/*************************************************************************************/

// Ustalenie trybu teksturowania

 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

/*************************************************************************************/

// Określenie sposobu nakładania tekstur

 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszczący (wypełnienia okna) ustawiono na czarny



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
