#include "keyActions.h"
#include <stdlib.h>
#include <stdio.h>
#include <OPENGL/gl.h>
#include "GLUT/GLUT.h"
/**
 * Used as the function provided to glutKeyboardFunc(). Takes the users
 * key stroke and mutates some globals etc.
 */
void input(unsigned char key, int x, int y)
{
    switch(key){
        case 't': case 'T':
            pause = 1;
            break;
        case 'c': case 'C':
            pause = 0;
            break;
        case 'f': case 'F':
            if(rotInc < 20)
            {
                rotInc++;                
            }
            break;
        case 's': case 'S':
            if(rotInc > 1)
            {
                rotInc--;
            }
            break;
        case 'x': case 'X':
            rotX = !rotX;
            break;
        case 'y': case 'Y':
            rotY = !rotY;
            break;
        case 'z': 
            zoom -= 0.1;
            break;
        case 'Z':
            zoom += 0.1;
            break;
        case 'q':
            exit(0);
            break;
        case 'a': case 'A':
            animate = 1;
            rotInc = 1;
            missileInc = 1;
            break;
        case 'p':
            glShadeModel(GL_FLAT);
            break;

        case 'P':
            glShadeModel(GL_SMOOTH);

            break;
    }
}
