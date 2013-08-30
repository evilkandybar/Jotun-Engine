	// JotunEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "JotunEngine.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
void EnableOpenGL( HWND hWnd, HDC *hDC, HGLRC *hRC );
void DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC );
void				makeFullscreen();
void				initOpenGL();
void				genShadowMap();
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void				init();
void				update();
void				draw();
void				prepareForShadows();
void				genTextureMatrix();
void				drawAxis();

HGLRC hRC;
HWND hWnd;
HDC hDC;
HINSTANCE hInstance;

GLuint shadowMapGLID;
GLuint shadowFBOGLID;

int width, height;
std::vector<RenderMesh*> objects;
std::ofstream out( "debug.txt" );
Camera mainCamera;
Light mainLight;

void init() {
	mainLight.setPosition( 3, 25, 3 );
	mainLight.setColor( 1, 1, 1, 1 );
	makeFullscreen();
	initOpenGL();
	genShadowMap();
	mainCamera.setActive();
	mainCamera.setPosition( 0, 0, -10 );
	mainLight.enable();
	mainLight.getCamera().genMatrixes();
	Time::init();
	RenderMesh *rm = new RenderMesh( "HumanTransport.obj", "Diffuse" );
	objects.push_back( rm );
	out<<"Initialized stuff\n";
}

void initOpenGL() {
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glClearDepth( 1.0 );
	glClearColor( 0, 0, 0, 0 );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glEnable( GL_LIGHTING );
	glEnable( GL_NORMALIZE );
}

void genShadowMap() {
	GLenum fboStatus;

	//Use a texture depth component
	glGenTextures( 1, &shadowMapGLID );
	glBindTexture( GL_TEXTURE_2D, shadowMapGLID );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glGenFramebuffers( 1, &shadowFBOGLID );
	glBindFramebuffer( GL_FRAMEBUFFER, shadowFBOGLID );

	//Tell OpenGL that we won't care about color
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );

	//attach the depth texture to the framebuffer
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapGLID, 0 );

	fboStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( fboStatus != GL_FRAMEBUFFER_COMPLETE ) {
		out<<"ERROR: Framebuffer generation failed. Why?\n";
	}

	//switch to the default framebuffer
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void update() {
	Time::update();
	//Eventually, find a way to determine the correct camera between multiple cameras in the scene
	mainCamera.update();
}

void prepareForShadows() {
}

void genTextureMatrix() {
	static double modelView[16];
	static double projection[16];
	
	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5 
	// y = y* 0.5 + 0.5 
	// z = z* 0.5 + 0.5 
	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {	
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0};
	
	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	glLoadIdentity();	
	glLoadMatrixd(bias);
	
	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxis() {
	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );

	glColor3f( 1, 0, 0 );
	glVertex3f( -100, 0, 0 );
	glVertex3f( 100, 0, 0 );
	for( int i = 0; i < 100; i++ ) {
		glVertex3f( -i, 1, 0 );
		glVertex3f( -i, -1, 0 );
		glVertex3f( i, 1, 0 );
		glVertex3f( i, -1, 0 );
	}

	glColor3f( 0, 1, 0 );
	glVertex3f( 0, -100, 0 );
	glVertex3f( 0, 100, 0 );
	for( int i = 0; i < 100; i++ ) {
		glVertex3f( -1, -i, 0 );
		glVertex3f( 1, -i, 0 );
		glVertex3f( 1, i, 0 );
		glVertex3f( -1, i, 0 );
	}

	glColor3f( 0, 0, 1 );
	glVertex3f( 0, 0, -100 );
	glVertex3f( 0, 0, 100 );
	for( int i = 0; i < 100; i++ ) {
		glVertex3f( 1, 0, -i );
		glVertex3f( -1, 0, -i );
		glVertex3f( 1, 0, i );
		glVertex3f( -1, 0, i );
	}
	glEnd();
	glEnable( GL_LIGHTING );
}

void draw() {
	glBindFramebuffer( GL_FRAMEBUFFER, shadowFBOGLID );
	glClear( GL_DEPTH_BUFFER_BIT );
	glUseProgram( 0 );
	//call update on all lights
	//for each light:
	//	Move to the light's position
	//	render the scene (with simplest settings) to an off-screen depth buffer
	//	do the rest of the fun shadow map stuff
	static aiMatrix4x4 biasMatrix( 0.5f, 0.0f, 0.0f, 0.0f, 
								   0.0f, 0.5f, 0.0f, 0.0f,
								   0.0f, 0.0f, 0.5f, 0.0f,
								   0.5f, 0.5f, 0.5f, 1.0f );
	mainLight.update();
	mainLight.prepareShadowCam();
	mainCamera.genMatrixes();
	glCullFace( GL_FRONT );
	glShadeModel( GL_FLAT );
	glColorMask( 0, 0, 0, 0 );
	for( RenderMesh *rm : objects ) {
		rm->setShader( 0 );
		rm->draw();
	}
	glBegin( GL_QUADS );
	glVertex3f( -100, 0, -100 );
	glVertex3f( -100, 0, 100 );
	glVertex3f( 100, 0, 100 );
	glVertex3f( 100, 0, -100 );
	glEnd();

	//save texture matrix, for GPU transformation
	//Always make the GPU work?
	genTextureMatrix();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, width, height );
	glColorMask( 1, 1, 1, 1 );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	
	int id = Shaders::getShaderByName( "Diffuse" )->id();
	glUseProgram( id );
	glUniform1i( glGetUniformLocation( id, "shadowMap" ), 7 );
	glActiveTexture( GL_TEXTURE7 );
	glBindTexture( GL_TEXTURE_2D, shadowMapGLID );

	mainCamera.setActive();
	mainCamera.prepare();

	mainLight.update();

	glCullFace( GL_BACK );
	glShadeModel( GL_SMOOTH );


	//Find a way to concatenate shadow vaules for multiple lights
	//For now, just render the scene

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glScaled( 0.1, 0.1, 0.1 );
	glColor3f( 0.8, 0.8, 0.8 );
	mainCamera.prepare();
	for( RenderMesh *rm : objects ) {
		rm->setShader();
		rm->draw();
	}
	glBegin( GL_QUADS );
	glVertex3f( -100, 0, -100 );
	glVertex3f( -100, 0, 100 );
	glVertex3f( 100, 0, 100 );
	glVertex3f( 100, 0, -100 );
	glEnd();

	mainLight.drawGizmo();
	drawAxis();
	glPopMatrix();
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
    WNDCLASS wc;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW ); 
    wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Jotun Engine";
    RegisterClass( &wc );
	makeFullscreen();
    hWnd = CreateWindow( L"Jotun Engine", L"Jotun Engine",
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      -8, -32, width+16, height+40,
      NULL, NULL, hInstance, NULL );
    EnableOpenGL( hWnd, &hDC, &hRC );
	init();
	ShowCursor( false );
    while( !bQuit )
    {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
            if( msg.message == WM_QUIT ) {
                bQuit = TRUE;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
            }
		} else {
			update();
			draw();
			Sleep( 1 );
		}
		SwapBuffers( hDC );
	}
	DisableOpenGL( hWnd, hDC, hRC );
	DestroyWindow( hWnd );
	return (int) msg.wParam;
}

DEVMODE getCurDeviceMode() {
	DEVMODE deviceMode;
	int maxWidth = -1;
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &deviceMode );
	return deviceMode;
}

void makeFullscreen() {
	DEVMODE dmScreenSettings = getCurDeviceMode();
	width = dmScreenSettings.dmPelsWidth;
	height = dmScreenSettings.dmPelsHeight;
	mainCamera.setScreenX( width );
	mainCamera.setScreenY( height );
	out<<"Set screen width and height to " <<width <<", " <<height <<"\n";
	ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );
	ShowCursor( false );
}

void EnableOpenGL( HWND hWnd, HDC *hDC, HGLRC *hRC )
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    *hDC = GetDC( hWnd );
    ZeroMemory( &pfd, sizeof(pfd) );
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat( *hDC, &pfd );
    SetPixelFormat( *hDC, iFormat, &pfd );
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
	glewInit();
}

void DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC )
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

void clearKeys() {
	Input::a = false;
	Input::b = false;
	Input::c = false;
	Input::d = false;
	Input::e = false;
	Input::f = false;
	Input::g = false;
	Input::h = false;
	Input::i = false;
	Input::j = false;
	Input::k = false;
	Input::l = false;
	Input::m = false;
	Input::n = false;
	Input::o = false;
	Input::p = false;
	Input::q = false;
	Input::r = false;
	Input::s = false;
	Input::t = false;
	Input::u = false;
	Input::v = false;
	Input::w = false;
	Input::x = false;
	Input::y = false;
	Input::z = false;
	Input::enter = false;
	Input::mouse0 = false;
	Input::mouse1 = false;
	Input::mouse2 = false;
	Input::mouseWheelIn = false;
	Input::mouseWheelOut = false;
}

void onKeyDown( int key ) {
	clearKeys();
	out<<"Key " <<key <<" pressed\n";
	switch( key ) {
	case 'A':
		Input::a = true;
		break;
	case 'B':
		Input::b = true;
		break;
	case 'C':
		Input::c = true;
		break;
	case 'D':
		Input::d = true;
		break;
	case 'E':
		Input::e = true;
		break;
	case 'F':
		Input::f = true;
		break;
	case 'G':
		Input::g = true;
		break;
	case 'H':
		Input::h = true;
		break;
	case 'I':
		Input::i = true;
		break;
	case 'J':
		Input::j = true;
		break;
	case 'K':
		Input::k = true;
		break;
	case 'L':
		Input::l = true;
		break;
	case 'M':
		Input::m = true;
		break;
	case 'N':
		Input::n = true;
		break;
	case 'O':
		Input::o = true;
		break;
	case 'P':
		Input::p = true;
		break;
	case 'Q':
		Input::q = true;
		break;
	case 'R':
		Input::r = true;
		break;
	case 'S':
		Input::s = true;
		break;
	case 'T':
		Input::t = true;
		break;
	case 'U':
		Input::u = true;
		break;
	case 'V':
		Input::v = true;
		break;
	case 'W':
		Input::w = true;
		break;
	case 'X':
		Input::x = true;
		break;
	case 'Y':
		Input::y = true;
		break;
	case 'Z':
		Input::z = true;
		break;
	case VK_RETURN:
		Input::enter = true;
		break;
	}
}

void onKeyUp( int key ) {
	switch( key ) {
	case 'A':
		Input::a = false;
		break;
	case 'B':
		Input::b = false;
		break;
	case 'C':
		Input::c = false;
		break;
	case 'D':
		Input::d = false;
		break;
	case 'E':
		Input::e = false;
		break;
	case 'F':
		Input::f = false;
		break;
	case 'G':
		Input::g = false;
		break;
	case 'H':
		Input::h = false;
		break;
	case 'I':
		Input::i = false;
		break;
	case 'J':
		Input::j = false;
		break;
	case 'K':
		Input::k = false;
		break;
	case 'L':
		Input::l = false;
		break;
	case 'M':
		Input::m = false;
		break;
	case 'N':
		Input::n = false;
		break;
	case 'O':
		Input::o = false;
		break;
	case 'P':
		Input::p = false;
		break;
	case 'Q':
		Input::q = false;
		break;
	case 'R':
		Input::r = false;
		break;
	case 'S':
		Input::s = false;
		break;
	case 'T':
		Input::t = false;
		break;
	case 'U':
		Input::u = false;
		break;
	case 'V':
		Input::v = false;
		break;
	case 'W':
		Input::w = false;
		break;
	case 'X':
		Input::x = false;
		break;
	case 'Y':
		Input::y = false;
		break;
	case 'Z':
		Input::z = false;
		break;
	case VK_RETURN:
		Input::enter = false;
		break;
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	PVOID pulScrollLines = NULL;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
			break;
		case IDM_EXIT:
			DestroyWindow( hWnd );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
		break;
	case WM_KEYDOWN:
		out<<"Registering a key press\n";
		if( wParam ==  VK_ESCAPE ) {
			PostQuitMessage( 0 );
			break;
		} else {
			onKeyDown( wParam );
			break;
		}
	case WM_KEYUP:
		onKeyUp( wParam );
		break;
	case WM_LBUTTONDOWN:
		Input::mouse0 = true;
		break;
	case WM_LBUTTONUP:
		Input::mouse0 = false;
		break;
	case WM_MBUTTONDOWN:
		Input::mouse2 = true;
		break;
	case WM_MBUTTONUP:
		Input::mouse2 = true;
		break;
	case WM_RBUTTONDOWN:
		Input::mouse1 = true;
		break;
	case WM_RBUTTONUP:
		Input::mouse1 = false;
		break;
	Input::mouseWheelIn = false;
	Input::mouseWheelOut = false;
	case WM_MOUSEWHEEL:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, pulScrollLines, 0);
		if( &pulScrollLines > 0 ) {
			Input::mouseWheelIn = true;
		} else if( &pulScrollLines < 0 ) {
			Input::mouseWheelOut = true;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}