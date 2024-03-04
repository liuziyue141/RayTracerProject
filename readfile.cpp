/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h" 

using namespace std;
#include "variables.h" 
#include "readfile.h"

// You may not need to use the following two functions, but it is provided
// here for convenience
// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat* values) 
{
  mat4 transform = transfstack.top(); 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
  vec4 newval = transform * valvec; 
  for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, GLfloat* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

void readfilenew(const char* filename) {
  string str, cmd; 
  ifstream in;
  in.open(filename); 
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.  
    // This is done using standard STL Templates 
    stack <mat4> transfstack; 
    transfstack.push(mat4(1.0));  // identity

    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i; 
        GLfloat values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 
        if(cmd == "maxverts"){
          validinput = readvals(s,1,values);
          maxverts = values[0];
        }
        else if (cmd == "maxvertnorms"){
          validinput = readvals(s,1,values);
          maxvertnorms = values[0];
        }
        else if (cmd == "vertex"){
          validinput = readvals(s,3,values);
          vector<float> curvert = {values[0], values[1], values[2]};
          vertexs.push_back(curvert);
        }
        else if (cmd == "vertexnormal"){
          validinput = readvals(s,6,values);
          vector<float> curvertnorm = {values[0], values[1], values[2], values[3], values[4], values[5]};
          vertexNormals.push_back(curvertnorm);
        }
        else if (cmd == "size") {
                validinput = readvals(s,2,values); 
                if (validinput) { 
                  w = (int) values[0]; h = (int) values[1]; 
                } 
        } else if (cmd == "camera") { //camera parser 
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE
            // Use all of values[0...9]
            // You may need to use the upvector fn in Transform.cpp
            // to set up correctly. 
            // Set eyeinit upinit center fovy in variables.h 
            eyeinit[0]=values[0];
            eyeinit[1]=values[1];
            eyeinit[2]=values[2];
            center[0]=values[3];
            center[1]=values[4];
            center[2]=values[5];
            upinit[0]=values[6];
            upinit[1]=values[7];
            upinit[2]=values[8];
            fovy = values[9];
            fovx = 2 * atan(tan(fovy * 0.5) * w/h);
          }
        }else if (cmd == "sphere" || cmd == "tri" || cmd == "trinormal") {
          object * obj = &(objects[numobjects]); 
          validinput = readvals(s, 1, values); 
          if(validinput){
            if (cmd == "sphere") {
              obj -> sphere_rad = values[0];
            }else if (cmd == "tri"){
              obj -> tri_v1 = values[0];
              obj -> tri_v2 = values[1];
              obj -> tri_v3 = values[2];
            }else{
              obj -> tri_norm_v1 = values[0];
              obj -> tri_norm_v2 = values[1];
              obj -> tri_norm_v3 = values[2];
            }
            // Set the object's light properties
            for (i = 0; i < 4; i++) {
              (obj->ambient)[i] = ambient[i]; 
              (obj->diffuse)[i] = diffuse[i]; 
              (obj->specular)[i] = specular[i]; 
              (obj->emission)[i] = emission[i];
            }
            obj->shininess = shininess; 

              // Set the object's transform
            obj->transform = transfstack.top(); 
            ++numobjects; 
          }
        }

        else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file. 
            // Also keep in mind what order your matrix is!
            mat4 tMatrix(
              1.0f, 0, 0, values[0],
              0, 1.0f, 0, values[1],
              0, 0, 1.0f, values[2],
              0, 0, 0, 1.0f);
            tMatrix = glm::transpose(tMatrix);
            rightmultiply(tMatrix, transfstack);

          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.  
            // Also keep in mind what order your matrix is!
            mat4 sMatrix = Transform::scale(values[0], values[1], values[2]);
            rightmultiply(sMatrix, transfstack);

          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE. 
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.  
            // Note that rotate returns a mat3. 
            // Also keep in mind what order your matrix is!
            vec3 axis(values[0], values[1], values[2]);
            axis = glm::normalize(axis);
            float degree = values[3];
            mat4 rMatrix = glm::mat4(Transform::rotate(degree, axis));
            rMatrix[3][3] = 1;
            rightmultiply(rMatrix, transfstack);
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }
    eye = eyeinit; 
    up = upinit; 
  }else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}
