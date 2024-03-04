
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
    for (int i = 0; i < maxobjects; ++i) {
        cout << "Object " << i << ":\n";
        cout << "  Sphere Radius: " << objects[i].sphere_rad << "\n";
        cout << "  Triangle Vertices: " << objects[i].tri_v1 << ", " << objects[i].tri_v2 << ", " << objects[i].tri_v3 << "\n";
        // ... Print other members of the object struct

        // Print vertex and vertex normal data if needed
        cout << "  Vertex Data:\n";
        for (const auto& vertex : vertexs[i]) {
            cout << "   " << vertex << " ";
        }
        cout << "\n";

        cout << "  Vertex Normal Data:\n";
        for (const auto& normal : vertexNormals[i]) {
            cout << "    " << normal << " ";
        }
        cout << "\n";

        cout << "\n";
    }

}