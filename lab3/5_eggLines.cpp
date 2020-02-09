/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych

/*************************************************************************************/

//#include <windows.h>
#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

typedef float point3[3];

struct point{
	float x;
	float y;
	float z;
};

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

void drawEgg(){
	int N=20;
	float baseVar=1.0/N;
	float u,v;
	float lastU,lastV;
	//point tab[10];

	glBegin(GL_LINES);

	// for(int i=0;i<=N;i++){
	// 	for(int i2=0;i2<=N;i2++){
	for(int i2=0;i2<=N;i2++){
		for(int i=0;i<=N;i++){
			u=baseVar*i;
			v=baseVar*i2;
			if((i == 0 && i2 == 0)||(i == 1 && i2 == 0)) continue;
			glVertex3f(calcX(u,v),calcY(u,v)-5,calcZ(u,v));
			glVertex3f(calcX(lastU,lastV),calcY(lastU,lastV)-5,calcZ(lastU,lastV));
			lastV=v;
			lastU=u;
		}
	}

	glEnd();
}

// void drawEggV2(){
// 	int N=100;
// 	float baseVar=1.0/N;
// 	float u,v;
// 	point **tab[];
// 	tab=new *int[N];
// 	for(int i=0;i<N;i++)
// 		tab[i]=new int[N];

// 	glBegin(GL_POINTS);

// 	for(int i=1;i<N;i++){
// 		for(int i2=1;i2<N;i2++){
// 			cout << i << " " << i2 << endl;
// 			u=baseVar*i;
// 			v=baseVar*i2;
// 			glVertex3f(calcX(u,v),calcY(u,v),calcZ(u,v));
// 		}
// 	}
// }

// 	glEnd();

// 	tab=new *int[N];
// 	for(int i=0;i<N;i++)
// 		tab[i]=new int[N];
// }



void RenderScene(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały

	glRotated(60.0, 1.0, 1.0, 1.0 );  // Obrót o 60 stopni

	drawEgg();


	glFlush();
	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

 

void MyInit(void)
{

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszcący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

 

void ChangeSize(GLsizei horizontal, GLsizei vertical )
{

GLfloat AspectRatio;
// Deklaracja zmiennej AspectRatio  określającej proporcję
// wymiarów okna 
if(vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
vertical = 1;  
glViewport(0, 0, horizontal, vertical);
// Ustawienie wielkościokna okna widoku (viewport)
// W tym przypadku od (0,0) do (horizontal, vertical)  
glMatrixMode(GL_PROJECTION);
// Przełączenie macierzy bieżącej na macierz projekcji 
glLoadIdentity();
// Czyszcznie macierzy bieżącej            
AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
// Wyznaczenie współczynnika  proporcji okna
// Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
// przestrzeni ograniczającej pozwalającej zachować właściwe
// proporcje rysowanego obiektu.
// Do okreslenia przestrzeni ograniczjącej służy funkcja
// glOrtho(...)            
if(horizontal <= vertical)

glOrtho(-7.5,7.5,-7.5/AspectRatio,7.5/AspectRatio,10.0, -10.0); 
else

glOrtho(-7.5*AspectRatio,7.5*AspectRatio,-7.5,7.5,10.0,-10.0);                     
glMatrixMode(GL_MODELVIEW);
// Przełączenie macierzy bieżącej na macierz widoku modelu                                   

glLoadIdentity();
// Czyszcenie macierzy bieżącej
}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

 

int main(int argc, char **argv){

glutInit( & argc, argv );

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

glutInitWindowSize(300, 300);

glutCreateWindow("Jajo");
               
glutDisplayFunc(RenderScene);
// Określenie, że funkcja RenderScene będzie funkcją zwrotną
// (callback function).  Bedzie ona wywoływana za każdym razem 
// gdy zajdzie potrzba przeryswania okna 
glutReshapeFunc(ChangeSize);
// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
// zazmiany rozmiaru okna      
MyInit();
// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
// inicjalizacje konieczne  przed przystąpieniem do renderowania
glEnable(GL_DEPTH_TEST);
// Włączenie mechanizmu usuwania powierzchni niewidocznych

glutMainLoop();
// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/