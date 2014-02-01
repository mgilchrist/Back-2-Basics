//
//  OpenGL_Main.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 11/23/13.
//  Copyright (c) 2013 Marcus Gilchrist. All rights reserved.
//

#include "OpenGL_Main.h"

#include <stdio.h>
#include <OpenGL/glu.h>	   // Standard header for MS Windows applications
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

//#include <UIKit/UIKit.h>
//#include <GLKit/GLKit.h>

#define KEY_ESCAPE 27



typedef struct {
  int width;
	int height;
	char* title;
  
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

glutWindow win;


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glutSwapBuffers();
}


void initialize ()
{
  glMatrixMode(GL_PROJECTION);												// select projection matrix
  glViewport(0, 0, win.width, win.height);									// set the viewport
  glMatrixMode(GL_PROJECTION);												// set matrix mode
  glLoadIdentity();															// reset projection matrix
  GLfloat aspect = (GLfloat) win.width / win.height;
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		// set up a perspective projection matrix
  glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
  glShadeModel( GL_SMOOTH );
  glClearDepth( 1.0f );														// specify the clear value for the depth buffer
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints
	glClearColor(0.0, 0.0, 0.0, 1.0);											// specify clear values for the color buffers
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )
{
  switch ( key )
  {
    case KEY_ESCAPE:
      exit ( 0 );
      break;
      
    default:
      break;
  }
}

int OpenGL_Main(int argc, char **argv)
{
	// set window values
	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT Window.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;
  
	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
  glutKeyboardFunc( keyboard );								// register Keyboard Handler
	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}