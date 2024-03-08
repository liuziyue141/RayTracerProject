#pragma once
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

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "readfile.h" // prototypes for readfile.cpp  
#include "raytracer.h"
using namespace std; 

int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    cout << "FreeIage " << FreeImage_GetVersion() << "\n";
    readfile(argv[1]) ; 
    traceRay();
    cout << "fovx " << fovx << ":\n";
    cout << "fovy " << fovy << ":\n";
    cout << "eye " << eye[0] << ", " << eye[1] << ", " << eye[2] << ":\n";
    cout << "up " << up[0] << ", " << up[1] << ", " << up[2] << ":\n";
    for (int i = 0; i < numobjects; ++i) {
        cout << "type" << objects[i].type << endl;
        cout << "Object " << i << ":\n";
        cout << "diffuse color1 " << objects[i].diffuse[0]  << " 2 " << objects[i].diffuse[1] << " 3 " << objects[i].diffuse[2]<< endl;
        if(objects[i].type == "sphere"){
            cout << "  Sphere Radius: " << objects[i].sphere_rad << "\n";
        }else if (objects[i].type == "tri"){
            cout << "  Triangle Vertices: " << objects[i].tri_v1[0] << ", " << objects[i].tri_v1[1] << ", " << objects[i].tri_v1[2] << "\n";
            cout << "  Triangle Vertices: " << objects[i].tri_v2[0] << ", " << objects[i].tri_v2[1] << ", " << objects[i].tri_v2[2] << "\n";
            cout << "  Triangle Vertices: " << objects[i].tri_v3[0] << ", " << objects[i].tri_v3[1] << ", " << objects[i].tri_v3[2] << "\n";
        // ... Print other members of the object struct
        }else{
            cout << "  Triangle Vertices: " << objects[i].tri_norm_v1[0] << ", " << objects[i].tri_norm_v1[1] << ", " << objects[i].tri_norm_v1[2] << "\n";
            cout << "  Triangle Vertices: " << objects[i].tri_norm_v2[0] << ", " << objects[i].tri_norm_v2[1] << ", " << objects[i].tri_norm_v2[2] << "\n";
            cout << "  Triangle Vertices: " << objects[i].tri_norm_v3[0] << ", " << objects[i].tri_norm_v3[1] << ", " << objects[i].tri_norm_v3[2] << "\n";
        } 
        mat4 lookat = glm::lookAt(eye, vec3(1,0,0), up); 
        cout << "\n";
        cout << "\n";
    }
    FreeImage_DeInitialise();
    return 0;

}