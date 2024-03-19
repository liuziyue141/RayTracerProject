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
#include "readfile.h"
using namespace std;

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

void readfile(const char* filename) {
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
          //vertexs = new vector<vec3>(maxverts);
        }
        else if (cmd == "maxvertnorms"){
          validinput = readvals(s,1,values);
          maxvertnorms = values[0];
          //vertexNormals = new vector<vec3>(2*maxvertnorms);
        }
        else if (cmd == "vertex"){
          validinput = readvals(s,3,values);
          vec3 curvert = vec3(values[0], values[1], values[2]);
          vertexs.push_back(curvert);
        }
        else if (cmd == "vertexnormal"){
          validinput = readvals(s,6,values);
          vec3 curvertvert = vec3 (values[0], values[1], values[2]);
          vertexNormals.push_back(curvertvert);
          vec3 curvertnorm = vec3 (values[3], values[4], values[5]);
          vertexNormals.push_back(curvertnorm);
        }
         else if (cmd == "ambient") {
          validinput = readvals(s, 3, values); // colors 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              ambient[i] = values[i]; 
            }
          }
        } 
        else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              diffuse[i] = values[i]; 
            }
          }
        } 
        else if (cmd == "specular") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              specular[i] = values[i]; 
            }
          }
        } 
        else if (cmd == "emission") {
          validinput = readvals(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              emission[i] = values[i]; 
            }
          }
        } 
        else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
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
            fovy = values[9]/180*pi; // here change made in radians
            fovx = 2 * atan(tan(fovy * 0.5) * w/h); // in radians 
          }
        }else if (cmd == "sphere" || cmd == "tri" || cmd == "trinormal") {
          if (numobjects == maxobjects) { // No more objects 
            cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
          } 
          else {
            object * obj = &(objects[numobjects]);  
            if(validinput){
              obj -> id = numobjects;
              obj -> type = cmd;
              obj->transform = transfstack.top(); 
              obj->inverse_transform = glm::inverse(obj->transform);
              if (cmd == "sphere") {
                validinput = readvals(s, 4, values);
                obj -> sphere_loc = vec3(values[0], values[1], values[2]);
                obj -> sphere_rad = values[3];
              }else if (cmd == "tri"){
                validinput = readvals(s, 3, values);
                obj -> tri_v1 = vec3(obj->transform * vec4(vertexs.at(values[0]), 1.0f));
                obj -> tri_v2 = vec3(obj->transform * vec4( vertexs.at(values[1]), 1.0f));
                obj -> tri_v3 = vec3(obj->transform * vec4( vertexs.at(values[2]), 1.0f));
              }else{
                validinput = readvals(s, 6, values);
                obj -> tri_v1 = vertexNormals.at(2*values[0]);
                obj -> tri_norm_v1 = vertexNormals.at(2*values[0]+1);
                obj -> tri_v2 = vertexNormals.at(2*values[1]);
                obj -> tri_norm_v2 = vertexNormals.at(2*values[1]+1);
                obj -> tri_v3 = vertexNormals.at(2*values[2]);
                obj -> tri_norm_v3 = vertexNormals.at(2*values[2]+1);
              }
              // Set the object's light properties
              for (i = 0; i < 3; i++) {
                (obj->ambient)[i] = ambient[i]; 
                (obj->diffuse)[i] = diffuse[i]; 
                (obj->specular)[i] = specular[i]; 
                (obj->emission)[i] = emission[i];
              }
              obj->shininess = shininess; 

                // Set the object's transform
              ++numobjects; 
            }
          }
        }

        else if (cmd == "point"){
          if (numused == numLights) { // No more Lights 
            cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 6, values); // Position/color for lts.
            if (validinput) {
              // YOUR CODE FOR HW 2 HERE. 
              // Note that values[0...7] shows the read in values 
              // Make use of lightposn[] and lightcolor[] arrays in variables.h
              // Those arrays can then be used in display too.  

              lightposn[4*numused]=values[0];
              lightposn[4*numused+1]=values[1];
              lightposn[4*numused+2]=values[2];
              lightposn[4*numused+3]=0;
              lightcolor[3*numused]=values[3];
              lightcolor[3*numused+1]=values[4];
              lightcolor[3*numused+2]=values[5];
              ++numused; 
            }
          }
        }

        else if (cmd == "attenuation"){
          validinput = readvals(s, 3, values); // constant linear quadraticf
          if (validinput) {
            constant_atten = values[0];
            linear_atten = values[1];
            quadratic_atten = values[2];
          }
        }

        else if (cmd == "maxdepth"){
          validinput = readvals(s, 1, values);
          if (validinput) {
            maxdepth = values[0];
          }
        }

        // else if (cmd == "output"){
        //   validinput = readvals(s, 1, values);
        //   if (validinput) {
        //     filename = values[0];
        //   }
        // }
        else if (cmd == "directional"){
          if (numused == numLights) { // No more Lights 
            cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 6, values); // Position/color for lts.
            if (validinput) {
              // YOUR CODE FOR HW 2 HERE. 
              // Note that values[0...7] shows the read in values 
              // Make use of lightposn[] and lightcolor[] arrays in variables.h
              // Those arrays can then be used in display too.  

              lightposn[4*numused]=values[0];
              lightposn[4*numused+1]=values[1];
              lightposn[4*numused+2]=values[2];
              lightposn[4*numused+3]=1;
              lightcolor[3*numused]=values[3];
              lightcolor[3*numused+1]=values[4];
              lightcolor[3*numused+2]=values[5];
              ++numused; 
            }
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
