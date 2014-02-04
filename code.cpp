#include<windows.h>
#include<scrnsave.h>
#include<glut.h>
#include<math.h>
#include"resource.h"

//define windows timer
#define TIMER 1000 
#define szAppName TEXT("Sine screensaver-Jax")
void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC);
void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC);		
void SetupAnimation(int Width, int Height);
void CleanupAnimation();
void OnTimer(HDC hDC);

int Width, Height; //globals for size of screen

//  THE THREE FUNCTIONS 
// Screen Saver Procedure

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;// Handle to device context
  static HGLRC hRC;//Handle to OpenGL rendering context
  static RECT rect; //Instance of structure RECT which defines the coords of the 
					//upper-left and lower-right  corners of a rectangle
  switch(message)
  {
		case WM_CREATE: 
			// get window dimensions
			GetClientRect( hWnd, &rect );
			Width = rect.right;   //Store width		
			Height = rect.bottom;  // Store height
			// setup OpenGL, then animation
			InitGL( hWnd, hDC, hRC );
			SetupAnimation(Width, Height);

			//set timer to tick every 10 ms
			SetTimer( hWnd, TIMER, 10, NULL );
			return 0;
 
		case WM_DESTROY:
			KillTimer( hWnd, TIMER );
			CleanupAnimation();
			CloseGL( hWnd, hDC, hRC );
			return 0;

		case WM_TIMER:
			OnTimer(hDC);	//animate!	
			return 0;				

  }
	return DefScreenSaverProc(hWnd,message,wParam,lParam);
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
  //InitCommonControls();  
  //would need this for slider bars or other common controls
  switch ( message ) 
  {
	case WM_INITDIALOG:
		LoadString(hMainInstance, IDS_DESCRIPTION, szAppName, 40);
	    return TRUE;
	case WM_COMMAND:
		switch( LOWORD( wParam ) ) 
		{ 
			case IDOK:
				EndDialog( hDlg, LOWORD( wParam ) == IDOK ); 
				return TRUE; 
		}

  }	//end command switch

  return FALSE; 
}
// needed for SCRNSAVE.LIB
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}

// Initialize OpenGL
static void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC)
{
  
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory( &pfd, sizeof pfd );
  pfd.nSize = sizeof pfd;
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; //blaine's
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  
  hDC = GetDC( hWnd ); //Retrieves a handle to a display device context
  
  int i = ChoosePixelFormat( hDC, &pfd );  
  SetPixelFormat( hDC, i, &pfd );

  hRC = wglCreateContext( hDC );
  wglMakeCurrent( hDC, hRC );

}

// Shut down OpenGL
static void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( hRC );
  ReleaseDC( hWnd, hDC );
}
void SetupAnimation(int Width, int Height)
{
	
	glViewport(0, 0,Width,Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2,2,-2,2, -1, 1);	
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0,-1, 0, 1, 0);
		
	
	//background
	glClearColor(0,0,0,0); //0.0s is black

}   

float x, y;//point1(x,y)
float z,m;//point2(z,m)
float j,k;//point2(j,k)
float spin=0; 
int color_num=1;//color counter
float color[60][3]={
{0.0,0.0,0.1},
{0.0,0.0,0.2},
{0.0,0.0,0.3},
{0.0,0.0,0.4},
{0.0,0.0,0.5},
{0.0,0.0,0.6},
{0.0,0.0,0.7},
{0.0,0.0,0.8},
{0.0,0.0,0.9},
{0.0,0.0,1.0},//0,0,1

{0.0,0.1,0.0},
{0.0,0.2,0.0},
{0.0,0.3,0.0},
{0.0,0.4,0.0},
{0.0,0.5,0.0},
{0.0,0.6,0.0},
{0.0,0.7,0.0},
{0.0,0.8,0.0},
{0.0,0.9,0.0},
{0.0,1.0,0.0},//0,1,0

{0.0,1.0,0.1},
{0.0,1.0,0.2},
{0.0,1.0,0.3},
{0.0,1.0,0.4},
{0.0,1.0,0.5},
{0.0,1.0,0.6},
{0.0,1.0,0.7},
{0.0,1.0,0.8},
{0.0,1.0,0.9},
{0.0,1.0,1.0},//0,1,1

{0.1,0,0}, 
{0.2,0,0},
{0.4,0,0},
{0.5,0,0},
{0.6,0,0},
{0.7,0,0},
{0.8,0,0},
{0.9,0,0},
{1.0,0,0},//1,0,0

{1.0,0.0,0.1},
{1.0,0.0,0.2},
{1.0,0.0,0.3},
{1.0,0.0,0.4},
{1.0,0.0,0.5},
{1.0,0.0,0.6},
{1.0,0.0,0.7},
{1.0,0.0,0.8},
{1.0,0.0,0.9},
{1.0,0.0,1.0},//1,0,1

{1.0,1.0,0.1},
{1.0,1.0,0.2},
{1.0,1.0,0.3},
{1.0,1.0,0.4},
{1.0,1.0,0.5},
{1.0,1.0,0.6},
{1.0,1.0,0.7},
{1.0,1.0,0.8},
{1.0,1.0,0.9},
{1.0,1.0,1.0}//1,1,1
};
//for every 10 milli seconds Ontimer will get called 
void OnTimer(HDC hDC) //increment and display
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spin++; 

	glPushMatrix();
	glRotatef(spin, 1.0, 0.0, 0.0);


	for(x=-10;x<10;x+=0.01)
	{
		if(color_num==60)
			color_num=1;
		glColor3fv(color[color_num]);
		color_num++;
		y=sin(10*x)/(1+(x*x));
		glRasterPos3f(x,y,1.7);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'*');
	}
	glPushMatrix();
	glRotatef(spin*-3.0, 0.0, 0.0, 1.0);
	
	for(z=-10;z<10;z+=0.001)
	{
		if(color_num==60)
			color_num=1;
		glColor3fv(color[color_num]);
			color_num++;
	m=sin(10*z)/(1+(z*z));
	glBegin(GL_POINTS);
	glVertex2f(z,m);
	glEnd();
	}

	glPushMatrix();
	glRotatef(spin*-3.0 , 1.0, 0.0, 0.0); 
	
	for(j=-10;j<10;j+=0.001)
	{
		if(color_num==60)
			color_num=1;
		glColor3fv(color[color_num]);
		color_num++;
		k=sin(10*j)/(1+(j*j));
		glBegin(GL_POINTS);
		glVertex2f(j,k);
		glEnd();
	}	
	glPopMatrix();
	
	glPopMatrix();
	glFinish();
	SwapBuffers(hDC);
	glPopMatrix();
}

void CleanupAnimation()
{
	//didn't create any objects, so no need to clean them up
}
