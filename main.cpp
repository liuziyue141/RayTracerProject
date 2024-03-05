
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
#include <FreeImage.h>
using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
using namespace std; 

int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    readfile(argv[1]) ; 
    for (int i = 0; i < numobjects; ++i) {
        cout << "Object " << i << ":\n";
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
        
        cout << "\n";

        cout << "\n";
    }
    FreeImage_DeInitialise();
    return 0;

}