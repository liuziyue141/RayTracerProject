/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  
#pragma once
using namespace std; 
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <string>
#include <vector>
#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
float fovy = 90.0 ; // For field of view
float fovx = 90.0;
#else 
EXTERN vec3 eyeinit ; 
EXTERN vec3 upinit ; 
EXTERN vec3 center ; 
EXTERN int amountinit;
EXTERN int w, h ; 
EXTERN float fovy ; 
EXTERN float fovx ;
#endif 

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
EXTERN mat4 projection, modelview; // The mvp matrices
EXTERN GLuint projectionPos, modelviewPos; // Uniform locations of the above matrices
static enum {view, translate, scale} transop ; // which operation to transform 
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ; 
EXTERN GLfloat lightposn [4*numLights] ; // Light Positions
EXTERN GLfloat lightcolor[3*numLights] ; // Light Colors
EXTERN int numused ;                     // How many lights are used 

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN GLfloat ambient[4] ; 
EXTERN GLfloat diffuse[4] ; 
EXTERN GLfloat specular[4] ; 
EXTERN GLfloat emission[4] ; 
EXTERN GLfloat shininess ; 

// For multiple objects, read from a file.  
const int maxobjects = 100000 ; 
EXTERN int numobjects ; 
EXTERN int maxverts ;
EXTERN int maxvertnorms ;

EXTERN float constant_atten ;
EXTERN float linear_atten ;
EXTERN float quadratic_atten ;

EXTERN int maxdepth ;
EXTERN string filename ;


EXTERN struct object {
  int id ;
  string type ; 
  float sphere_rad ;
  vec3 sphere_loc ;
  vec3 tri_v1 ;
  vec3 tri_v2 ;
  vec3 tri_v3 ;
  vec3 tri_norm_v1 ;
  vec3 tri_norm_v2 ;
  vec3 tri_norm_v3 ;
  GLfloat ambient [3]; 
  GLfloat diffuse [3]; 
  GLfloat specular [3];
  GLfloat emission [3]; 
  GLfloat shininess ;
  mat4 transform ; 
  mat4 inverse_transform ;
}objects[maxobjects];
//if this is too slow, we might change this 
// EXTERN vector< object > objects;
EXTERN vector< vec3 > vertexs;
EXTERN vector< vec3 > vertexNormals;

// EXTERN struct vertex{
//   GLfloat x;
//   GLfloat y;
//   GLfloat z;
// }vertex;

