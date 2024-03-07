#pragma once
// Readfile definitions 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
// #ifdef __APPLE__
// #include <OpenGL/gl3.h>
// #include <OpenGL/glext.h>
// #include <GLUT/glut.h>
// #else
// #include <GL/glew.h>
// #include <GL/glut.h>
// #endif
#include "Transform.h" 
#include "variables.h" 
void matransform (stack<mat4> &transfstack, GLfloat * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, GLfloat * values) ;
void readfile (const char * filename) ;
