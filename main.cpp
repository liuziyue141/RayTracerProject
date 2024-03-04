#include <FreeImage.h>
#include "readfile.h" // prototypes for readfile.cpp  

int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    readfile(argv[1]) ; 

}