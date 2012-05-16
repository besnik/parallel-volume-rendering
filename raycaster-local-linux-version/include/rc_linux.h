// rc_linux.h
// by skip
// dalsie datove struktury
// version 1.0
// 3.1.2006

#ifndef __RC_LINUX_H
#define __RC_LINUX_H


#include <GL/glut.h>

#include "matica.h"
#include "raycaster.h"
#include "timer.h"



// TEXTURE DATA

GLuint texture[1]; // pointer na texturu, ktoru budeme vykreslovat




#endif


/*

Version History
6.1.2006
- RGB, not Intensity input to the texture
- camera rotation and distance change via mouse added
- 2 critical bugs fixed

5.1.2006
- timer added
- FPS counting added
	
3.1.2006
- function renderuj() was coded
- OpenGL suppor via GLUT library was added


*/
