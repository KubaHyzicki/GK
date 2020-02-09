#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <GL/glut.h>
#include <random>
#include <chrono>
#include <functional>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };  // na z przybliżenie widoku.
                                               // inicjalizacja położenia obserwatora

static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angleX;
static GLfloat pix2angleY;

static GLfloat theta = 0.0;   // azymut
static float fi = 0.0;  // kšt elewacji
float radius = 15.0;

bool side[5] = {true, true, true, true, true};
bool texture = false;
static GLint status = 0;       // stan klawiszy myszy
                               // 0 - nie nacinięto żadnego klawisza
                               // 1 - nacinięty lewy klawisz
                               // 2 - nacinięty prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;       // poprzednia pozycja kursora wzdłóż osi OY
static int zoom_pos_old = 0;

static int delta_x = 0;        // różnica pomiędzy pozycjš bieżšcš i poprzedniš kursora myszy
static int delta_y = 0;
static int delta_zoom = 0;

const float pi = M_PI;

void Axes(void)
{
    point3  x_min = { -5.0, 0.0, 0.0 };
    point3  x_max = { 5.0, 0.0, 0.0 };

    point3  y_min = { 0.0, -5.0, 0.0 };
    point3  y_max = { 0.0,  5.0, 0.0 };

    point3  z_min = { 0.0, 0.0, -5.0 };
    point3  z_max = { 0.0, 0.0,  5.0 };

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

// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
// FileName, alokuje pamięć i zwraca wskanik (pBits) do bufora w którym
// umieszczone sš dane.
// Ponadto udostępnia szerokoć (ImWidth), wysokoć (ImHeight) obrazu
// tekstury oraz dane opisujšce format obrazu według specyfikacji OpenGL
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytujšcej dane z pliku TGA.
// Działa tylko dla obrazów wykorzystujšcych 8, 24, or 32 bitowy kolor.
// Nie obsługuje plików w formacie TGA kodowanych z kompresjš RLE.

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
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

    // Wartoci domylne zwracane w przypadku błędu

    *ImWidth = 0;
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;

    pFile = fopen(FileName, "rb");
    if (pFile == NULL)
        return NULL;
    // Przeczytanie nagłówka pliku

    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Odczytanie szerokoci, wysokoci i głębi obrazu

    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;

    // Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)

    if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
        return NULL;

    // Obliczenie rozmiaru bufora w pamięci

    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

    // Alokacja pamięci dla danych obrazu

    pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

    if (pbitsperpixel == NULL)
        return NULL;

    if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
    {
        free(pbitsperpixel);
        return NULL;
    }

    // Ustawienie formatu OpenGL

    switch (sDepth)
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

struct PyramidPoints {
    double x;
    double y;
    double z;
};

void Pyramid(PyramidPoints point1, PyramidPoints point2, PyramidPoints point3, PyramidPoints point4, PyramidPoints point5) {

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLES);
    // Trójkšt z przodu
    if (side[0]) {
        glNormal3f(point1.x, point1.y, point1.z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(point1.x, point1.y, point1.z);
        glNormal3f(point2.x, point2.y, point2.z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(point2.x, point2.y, point2.z);
        glNormal3f(point3.x, point3.y, point3.z);
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(point3.x, point3.y, point3.z);
    }
    if (side[1]) {
        // Trójkšt prawy
        glNormal3f(point1.x, point1.y, point1.z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(point1.x, point1.y, point1.z);
        glNormal3f(point3.x, point3.y, point3.z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(point3.x, point3.y, point3.z);
        glNormal3f(point4.x, point4.y, point4.z);
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(point4.x, point4.y, point4.z);
    }
    if (side[2]) {
        // Trójkšt z tyłu
        glNormal3f(point1.x, point1.y, point1.z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(point1.x, point1.y, point1.z);
        glNormal3f(point4.x, point4.y, point4.z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(point4.x, point4.y, point4.z);
        glNormal3f(point5.x, point5.y, point5.z);
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(point5.x, point5.y, point5.z);
    }
    if (side[3]) {
        // Trójkšt lewy
        glNormal3f(point1.x, point1.y, point1.z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(point1.x, point1.y, point1.z);
        glNormal3f(point5.x, point5.y, point5.z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(point5.x, point5.y, point5.z);
        glNormal3f(point2.x, point2.y, point2.z);
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(point2.x, point2.y, point2.z);
        glEnd();
    }
    if (side[4]) {
        glBegin(GL_QUADS);
        glNormal3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(point5.x, point5.y, point5.z);
        glNormal3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(point4.x, point4.y, point4.z);
        glNormal3f(1.0, -1.0, 1.0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(point3.x, point3.y, point3.z);
        glNormal3f(1.0, -1.0, 1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(point2.x, point2.y, point2.z);
        glEnd();
    }
}
void DrawPyramid(PyramidPoints point1, PyramidPoints point2, PyramidPoints point3, PyramidPoints point4, PyramidPoints point5) {

        Pyramid(point1, point2, point3, point4, point5);
}

// Funkcja "bada" stan myszy i ustawia wartoci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
        y_pos_old = y;

        status = 1;          // wcięnięty został lewy klawisz myszy
    }
    else if (btn = GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        zoom_pos_old = y; // poruszanie myszš wzdłuż osi OY

        status = 2;
    }
    else

        status = 0;          // nie został wcięnięty żaden klawisz
}

// Funkcja "monitoruje" położenie kursora myszy i ustawia wartoci odpowiednich miennych globalnych
void Motion(GLsizei x, GLsizei y)
{
    delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
    delta_y = y - y_pos_old;
    delta_zoom = y - zoom_pos_old;

    //std::cout << viewer[2] << "\n";

    x_pos_old = x;            // podstawienie bieżšcego położenia jako poprzednie
    y_pos_old = y;
    zoom_pos_old = y;

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

void TextureSide() {
    if (texture) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

// Funkcja okrelajšca co ma być rysowane (zawsze wywoływana, gdy trzeba przerysować scenę)
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczšcym

    glLoadIdentity();
    // Czyszczenie macierzy bieżšcej

    //std::cout << fi << " " << th << " " << viewZ << "\n";


    if (status == 1) {                 // jeli lewy klawisz myszy wcięnięty
        fi += delta_y * pix2angleY;
        theta += delta_x * pix2angleX;    // modyfikacja kšta obrotu o kat proporcjonalny

        if (theta >= 360.0) { theta = 0.0; }
        if (fi >= 360.0) { fi = 0.0; }
    }
    else if (status == 2) {
        radius += 0.1* delta_zoom; //zmiana kšta obserwacji przez obserwatora
        if (radius >= 25.0) { radius = 25.0; }
        if (radius <= 10.0) { radius = 10.0; }
    }

    glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kšt
                                      //glRotatef(fi, 1.0, 0.0, 0.0);

    viewer[0] = radius * cos(theta) * cos(fi);
    viewer[1] = radius * sin(fi);
    viewer[2] = radius * sin(theta) * cos(fi);

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);
    // Zdefiniowanie położenia obserwatora
    TextureSide();
    Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej

    DrawPyramid({ 0.0, 4.0, 0.0 }, { -4.0, -4.0, 4.0 }, { 4.0, -4.0, 4.0 }, { 4.0, -4.0, -4.0 }, { -4.0, -4.0, -4.0 }); // Rysowawanie piramidy o zadanych punktach

    glFlush();
    // Przekazanie poleceń rysujšcych do wykonania
    glutSwapBuffers();

}

void Keys(unsigned char key, int x, int y) { // Funkcja zwrotna, wybór wywietlenia jajka przez klawisz
    if (key == 'q') { side[0] = !side[0]; }
    if (key == 'w') { side[1] = !side[1]; }
    if (key == 'e') { side[2] = !side[2]; }
    if (key == 'r') { side[3] = !side[3]; }
    if (key == 't') { side[4] = !side[4]; }
    if (key == 'y') { texture = !texture; }
    std::cout << texture;
    RenderScene();
}

// Funkcja ustalajšca stan renderowania
void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszczšcy (wypełnienia okna) ustawiono na czarny

    GLbyte *pBytes;
    GLint ImWidth, ImHeight, ImComponents;
    GLenum ImFormat;


    // Definicja materiału z jakiego zrobiony jest przedmiot
    //-------------------------------------------------------
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    // współczynniki ka =[kar,kag,kab] dla wiatła otoczenia

    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // współczynniki kd =[kdr,kdg,kdb] wiatła rozproszonego

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // współczynniki ks =[ksr,ksg,ksb] dla wiatła odbitego

    GLfloat mat_shininess = { 100.0 };
    // współczynnik n opisujšcy połysk powierzchni


    // Definicja ródła wiatła
    //-------------------------------------------------------
    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    // położenie ródła

    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    // składowe intensywnoci wiecenia ródła wiatła otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // składowe intensywnoci wiecenia ródła wiatła powodujšcego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // składowe intensywnoci wiecenia ródła wiatła powodujšcego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant = { 1.0f };
    // składowa stała ds dla modelu zmian owietlenia w funkcji
    // odległoci od ródła

    GLfloat att_linear = { 0.05f };
    // składowa liniowa dl dla modelu zmian owietlenia w funkcji
    // odległoci od ródła

    GLfloat att_quadratic = { 0.001f };
    // składowa kwadratowa dq dla modelu zmian owietlenia w funkcji
    // odległoci od ródła

    // Ustawienie patrametrów materiału
    //-------------------------------------------------------
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    // Ustawienie parametrów ródła wiatła
    //-------------------------------------------------------
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    // Ustawienie opcji systemu owietlania sceny
    //-------------------------------------------------------
    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING); // właczenie systemu owietlenia sceny
    glEnable(GL_LIGHT0); // włšczenie ródła o numerze 0
    glEnable(GL_DEPTH_TEST); // włšczenie mechanizmu z-bufora

    //  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
    pBytes = LoadTGAImage("N3_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

    // Zdefiniowanie tekstury 2-D
    glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

    // Zwolnienie pamięci
    free(pBytes);

    // Włšczenie mechanizmu teksturowania
    glEnable(GL_TEXTURE_2D);

    // Ustalenie trybu teksturowania
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Okrelenie sposobu nakładania tekstur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych  w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoć i szerokoć okna) sš przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical) {

    pix2angleX = 360.0 * 0.01 / (float)horizontal; // przeliczenie pikseli na stopnie
    pix2angleY = 360.0 * 0.01 / (float)vertical;

    GLfloat AspectRatio = (GLfloat)horizontal / (GLfloat)vertical; // Deklaracja zmiennej AspectRatio  okrelajšcej proporcję wymiarów okna

    if (vertical == 0) { vertical = 1; }  // Zabezpieczenie przed dzieleniem przez 0
    glViewport(0, 0, horizontal, vertical);


    glMatrixMode(GL_PROJECTION); // Przełšczenie macierzy bieżšcej na macierz projekcji
    glLoadIdentity(); // Czyszcznie macierzy bieżšcej

    gluPerspective(70, AspectRatio, 1.0, 40.0);  //(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
                                                 // Ustawienie parametrów dla rzutu perspektywicznego
    if (horizontal <= vertical)
        glViewport(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio);
    else
        glViewport(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5);
    // Ustawienie wielkoci okna okna widoku (viewport)

    glMatrixMode(GL_MODELVIEW);
    // Przełšczenie macierzy bieżšcej na macierz widoku modelu

    glLoadIdentity();
    // Czyszczenie macierzy bieżšcej

}

int main(int argc, char *argv[])
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(600, 600);

    glutCreateWindow("Teksturowanie piramidy");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    MyInit();
    glEnable(GL_DEPTH_TEST);
    // Włšczenie mechanizmu usuwania niewidocznych elementów sceny

    glutKeyboardFunc(Keys);

    glutMouseFunc(Mouse);
    // Ustala funkcję zwrotnš odpowiedzialnš za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcję zwrotnš odpowiedzialnš za badanie ruchu myszy

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}
