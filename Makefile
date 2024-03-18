CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep macOS | awk '{ print $$2}'),macOS)
CFLAGS = -g -O3 -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
#LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage
LDFLAGS = -framework OpenGL -L/opt/homebrew/lib \
		-lGLEW -lfreeimage -L/opt/homebrew/Cellar/freeglut/3.4.0/lib -framework GLUT
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L./lib/nix/ \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: transforms
transforms: main.o Transform.o readfile.o raytracer.o variables.h readfile.h Transform.h raytracer.h
	$(CC) $(CFLAGS) -o tranforms main.o Transform.o readfile.o raytracer.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
readfile.o: readfile.cpp readfile.h variables.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp 
raytracer.o: raytracer.cpp raytracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c raytracer.cpp
clean: 
	$(RM) *.o transforms *.png
