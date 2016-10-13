/***
 * This is the under the sea scene. It is based off of the sample file.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <OPENGL/gl.h>
#include "GLUT/GLUT.h"

#include "underthe.h"
#include "keyActions.h"
#include "bmploader.h"


#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

GLUquadric* qobjGeneric;
GLUquadric* qobjSub;
GLfloat light_position1[4];

GLuint sandTexture;
GLuint missileTexture;

/**
 * Inits variables and lighting.
 */
void init()
{
    // Load bitmap textures.
    sandTexture = LoadGLTextures("bitmaps/sand.bmp");
    missileTexture = LoadGLTextures("bitmaps/missile.bmp");

    // Set lighting.
    static GLfloat  LightAmb[] = {0.1f, 0.1f, 0.1f, 1.0f};
    light_position1[0] = 1.0;
    light_position1[1] = 20.0;
    light_position1[2] = 1.0;
    light_position1[3] = 1.0;

    // Set bg colour.
    glClearColor( 0.2, 0.2, 0.4, 0.9);
 
    // Allows transperancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable lights, materials and depth test.
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);  
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    qobjGeneric = gluNewQuadric();
    qobjSub = gluNewQuadric();
    gluQuadricNormals(qobjGeneric, GLU_SMOOTH);

    /* set initial rotation and scale values  */
    alpha = -20.0;
    beta  = 20.0;
    scale = 1.0;
    zoom = 1.0;
    
    rotX = 0;
    rotY = 0;
}

/**
 * Draws objects.
 */
void draw()
{
    drawFog();
    drawSkyDome();
    drawSub(0.0, 0.0 ,0.0 );
    drawBomb(-3.0, -4.0, 3.0);
    drawBomb(-10.0, -5.0, -1.0);
    drawBomb(3.0, 4.0, -10.0);
    drawSub(5.0, 3.0 ,-15.0 );
    drawFloor();
}

/**
 * Draw skydome to limit horizon.
 */
void drawSkyDome()
{
    glPushMatrix();

        glColor4f(0.1, 0.2, 0.2, 0.9);
        glScalef(200.0, 200.0, 200.0);
        glutSolidSphere(1, getStacks(200), getStacks(200));

    glPopMatrix();
}

/**
 * Draw fog.
 */
void drawFog()
{
    GLfloat density = 0.05;

    GLfloat fogColor[4] = {0.1, 0.1, 0.1, 1.0};
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_EXP2);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, density);
}

/**
 * Draw ocean floor.
 */
void drawFloor()
{
    static GLfloat floorVertices[4][3] = {
        { -200.0, 0.0, 200.0 },
        { 200.0, 0.0, 200.0 },
        { 200.0, 0.0, -200.0 },
        { -200.0, 0.0, -200.0 },
    };

    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sandTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glColor3f(1.0f, 1.0f, 1.0f);

        glDisable(GL_LIGHTING);
        glTranslatef(0.0, -15.0, 0.0);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3fv(floorVertices[0]);
            glTexCoord2f(0.0, 160.0);
            glVertex3fv(floorVertices[1]);
            glTexCoord2f(160.0, 160.0);
            glVertex3fv(floorVertices[2]);
            glTexCoord2f(160.0, 0.0);
            glVertex3fv(floorVertices[3]);
        glEnd();
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/**
 * Called each iteration, sets some position variables and draws the scene.
 */
void display(void)
{
    // Clear the buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    printMessage();

    glPushMatrix();
        // Scene rotation/animation.
        if(rotX)
        {
            alpha-= 0.5;
        }
        if(rotY)
        {
            beta-= 0.5;
        }
        if(animate && !pause )
        {
            protAlpha+= rotInc;
            missileInc+= rotInc;
        }

        // Reset the rotation angle at 360 degrees.
        if(alpha > 360.0)
        {
            alpha -= 360.0;
        }
        
        // Sets scene at -20 in z direction. Also sets zoom.
        glTranslatef( 0.0,  0.0, - ((20.0 + zoom)));

        // do any rotation.
        glRotatef(beta ,  1.0,  0.0,  0.0);
        glRotatef(alpha,  0.0,  1.0,  0.0);

        // Draw scene.
        draw();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

/**
 * Draw characters of a string.
 */
void printString(const char* text, float x, float y)
{
    int length = strlen(text);

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(x, y, -15.0f);
    
    glPushMatrix();
        for(int i = 0; i < length; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
        }
    glPopMatrix();
}

/**
 * Print controls on screen.
 */
void printMessage()
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
        // Corrective translate/scale.
        glTranslatef(-4.0, 3.0, 0.0);
        glScalef(1.0, 0.5, 1.0);

        printString("Commands:",-5.0,12.0);
        printString("    [z/Z]  Zoom in/out",-5.0,11.0);
        printString("     [x]   Toggle X-axis rotation",-5.0,10.0);
        printString("     [y]   Toggle Y-axis rotation",-5.0,9.0);
        printString("    [p/P]  Toggle smooth/flat shading",-5.0,8.0);
        printString("     [a]   Start animation",-5.0,7.0);
        printString("     [t]   Pause animation",-5.0,6.0);
        printString("     [c]   Resume animation",-5.0,5.0);
        printString("     [q]   Quit",-5.0,4.0);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

/**
 * Used to get number of stacks to draw spheres ect with.
 * More zoom = lower number.
 */
int getStacks(int stacks)
{
    if(stacks - zoom > 10)
    {
        stacks-= zoom;
    }
    else
    {
        stacks = 10;
    }
    return stacks;
}

/**
 * Draw a bomb.
 */
void drawBomb(float x, float y, float z)
{
    glPushMatrix();
        GLfloat mat_specular[] = { 1, 1, 0.5, 0.1 };
        GLfloat mat_shininess[] = { 12.0 };

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

        glTranslatef(x, y, z);

        // Flashing lights.
        GLfloat lightColor0[4];
        lightColor0[1] = 0.0;
        lightColor0[2] = 0.0;
        lightColor0[3] = 0.1;

        if(missileInc % 60 < 5)
        {
            lightColor0[0] = 1.0;
        }
        else
        {
            lightColor0[0] = 0.0;
        }

        GLfloat lightPos0[] = {x, y ,z, 1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

        glColor4f(0.4, 0.0, 0.0, 0.4);
        glScalef(2.0, 2.0, 2.0);
        glutSolidSphere(1, getStacks(20), getStacks(20));

        // Spikes
        glPushMatrix();
            glScalef(0.7, 0.7, 0.7);
            for(int j = 0; j < 2; j ++)
            {
                glRotatef(90.0, 0.0, 1.0, 0.0);                
                for(int i = 0; i < 6; i++ )
                {
                    glRotatef(60.0, 1.0, 0.0, 0.0);  
                    glColor3f(0.0, 0.0, 0.0);
                    gluCylinder(qobjGeneric, 0.6, 0.2, 2.0, getStacks(16), getStacks(16));
                    
                    glPushMatrix();
                        if(missileInc % 60 < 5)
                        {
                            glColor3f(1.0, 0.0, 0.0);
                        }
                        glTranslatef(0.0, 0.0, 2.0);
                        gluDisk(qobjGeneric, 0.0, 0.2, 10, 1);
                    glPopMatrix();
                }
            }
        glPopMatrix();
    glPopMatrix();
}

/**
 * Draw a submarine.
 */
void drawSub(float x, float y, float z)
{
    glPushMatrix();
   
    glTranslatef(x, y, z);

    GLfloat mat_specular[] = { 0.1, 0.1, 0.5, 0.1 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

    // Main fusilage.
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);

        glTranslatef(0.0, 0.0, 0.0);
        glScalef(8.0, 1.5, 2.0);
        gluSphere(qobjSub, 1.0, getStacks(30), getStacks(20));

    glPopMatrix();

    // Bullnose.
    for(int i = 0; i < 70; i++ )
    {
        glPushMatrix();
            glTranslatef(7.0 - (float)i/10.0, 0.0, 0.0);
            glScalef(1.8, 1.5, 2.0);
            gluSphere(qobjSub, 1.0, getStacks(30), getStacks(20));
        glPopMatrix();
    }

    // Crest.
    glPushMatrix();
        glTranslatef(-0.5, 1.0, 0.0);
        glScalef(3.0, 1.0, 1.2);
        glColor3f(0.2, 0.2, 0.2);
        glutSolidSphere(1,getStacks(30), getStacks(20));
    glPopMatrix();

    for(int i = 0; i < 25; i++ )
    {
        glPushMatrix();
            glTranslatef(((float)i/10.0) - 0.5, 1.0, 0.0);
            glScalef(1.0, 1.0, 1.2);
            glColor3f(0.2, 0.2, 0.2);
            glutSolidSphere(1,getStacks(30), getStacks(20));
        glPopMatrix();
    }

    // Chimney.
    glPushMatrix();
        glTranslatef(0.2, 3.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 0.0);
        glScalef(3.8, 0.7, 1.5);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.5, 0.5, 1.0, getStacks(16), getStacks(16));
        gluDisk(qobjGeneric, 0.0, 0.5, 30, 1 );
    glPopMatrix();

    for(int i = 0; i < 12; i++ )
    {
        glPushMatrix();
            glTranslatef(0.5 + (float)i/10.0, 3.0, 0.0);
            glRotatef(90.0, 0.0, 0.0, 0.0);
            glScalef(1.0 + (float)i/90.0, 0.7+ (float)i/90.0, 1.5);
            glColor3f(0.2, 0.2, 0.2);
            gluCylinder(qobjGeneric, 0.5, 0.5, 1.0, getStacks(16), getStacks(16));
            gluDisk(qobjGeneric, 0.0, 0.5, 30, 1 );

        glPopMatrix();
    }

    // Fins. (Verical)
    glPushMatrix();
        glTranslatef(-6.8, 0.5, 0.0);
        glRotatef(90.0, 90.0, 0.0, -10.0);
        glScalef(3.5, 0.5, 3.0);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.2, 0.08, 1.0, getStacks(16), getStacks(16));
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            gluDisk(qobjGeneric, 0.0, 0.08, 30, 1 );
        glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glTranslatef(-6.8, -0.5, 0.0);
        glRotatef(-90.0, 90.0, 0.0, -10.0);
        glScalef(3.5, 0.5, 3.0);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.2, 0.08, 1.0, getStacks(16), getStacks(16));
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            gluDisk(qobjGeneric, 0.0, 0.08, 30, 1 );
        glPopMatrix();

    glPopMatrix();

    // (Horizontal)
    glPushMatrix();
        glTranslatef(-6.8, 0.0, 0.0);
        glScalef(4.0, 1.0, 2.5);
        glRotatef(-6.0, 0.0, 1.0, 0.0);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.2, 0.08, 1.0, getStacks(16), getStacks(16));
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            gluDisk(qobjGeneric, 0.0, 0.08, 30, 1 );
        glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glTranslatef(-6.8, 0.0, 0.0);
        glScalef(4.0, 1.0, 2.5);
        glRotatef(6.0, 0.0, 1.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.2, 0.08, 1.0, getStacks(16), getStacks(16));
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            gluDisk(qobjGeneric, 0.0, 0.08, 30, 1 );
        glPopMatrix();

    glPopMatrix();

    // Front fins.
    glPushMatrix();
        glTranslatef(6.0, 1.2, -2);
        glScalef(3.0, 1.0, 4.0);
        glColor3f(0.2, 0.2, 0.2);
        gluCylinder(qobjGeneric, 0.1, 0.1, 1.0, getStacks(16), getStacks(16));
        gluDisk(qobjGeneric, 0.0, 0.1, 30, 1 );
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            gluDisk(qobjGeneric, 0.0, 0.1, 30, 1 );
        glPopMatrix();

    glPopMatrix();

    // Propeller.
    drawProp(1.3);
    drawProp(-1.3);
    
    // Missile.
    glPushMatrix();
    
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(qobjGeneric,missileTexture);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, missileTexture);

        if(animate)
        {
            glTranslatef(missileInc/10.0, 0.0, 0.0);
            if(missileInc > 500)
            {
                missileInc = 1;
            }
        }
        glPushMatrix();
            glTranslatef(5.0, -1.0, 0.0);
            glScalef(2.0, 0.3, 0.3);
            glRotatef(90.0, 0.0, 1.0, 0.0);

            gluCylinder(qobjGeneric, 1.0, 1.0, 1.0, getStacks(16), getStacks(16));
            
            glPushMatrix();
                glTranslatef(0.0, 0.0, 1.0);
                glScalef(1.0, 1.0, 0.5);
                glutSolidSphere(1,getStacks(30), getStacks(20));

            glPopMatrix();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //---------------------------------------------------
glPopMatrix();
}

/**
 * Draws a propeller.
 */
void drawProp(float z)
{
    glPushMatrix();

        GLfloat mat_specular[] = { 1.0, 1.0, 0, 0.1 };
        GLfloat mat_shininess[] = { 1.0 };

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

        glTranslatef(0.3, 0.0, z);
        glScalef(1.0, 0.6, 0.6);

        // Blades.
        glPushMatrix();

            if(animate)
            {
                glRotatef(protAlpha, 0.0, 0.0, 0.0);
            }

            glPushMatrix();
                glTranslatef(-8.0, -0.5, 0.0);
                glRotatef(0.0, 80.0, 5.0, 10.0);
                glScalef(0.1, 0.4, 0.4);

                drawTriangle();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-8.0, 0.5, 0.0);
                glRotatef(180.0, 80.0, 5.0, 10.0);
                glScalef(0.1, 0.4, 0.4);

                drawTriangle();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-8.0, 0.0, -0.5);
                glRotatef(90.0, 80.0, 5.0, 10.0);
                glScalef(0.1, 0.4, 0.4);

                drawTriangle();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-8.0, 0.0, 0.5);
                glRotatef(270.0, 80.0, 5.0, 10.0);
                glScalef(0.1, 0.4, 0.4);

                drawTriangle();
            glPopMatrix();
            
            // Centre
            glPushMatrix();
                glTranslatef(-8.0, 0.0, 0.0);
                glScalef(0.1, 0.2, 0.2);
                glColor3f(1.0, 1.0, 0.0);
                glutSolidSphere(1, 30,20);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

/**
 * Draws a triangle.
 */
void drawTriangle()
{
    glBegin(GL_TRIANGLES);          
        // Front
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(1.0f, -1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(1.0f, 1.0f, 0.0f);   
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(1.0f, -1.0f, -1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);    
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(1.0f, 1.0f, 0.0f);      
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);     
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);      
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();  
}

/**
 * Does the perspective transform.
 */
void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(- 2.0,  2.0, - 2.0, 
                2.0,  3.0,  300.0);

    glMatrixMode(GL_MODELVIEW);
}

/**
 * Draw faster if spare resources.
 */
void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Init window and glut.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Typhoon Class Submarine");

    init();

    // Set glut callbacks.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(input);
    glutIdleFunc(idle);
    
    // Loop forever.
    glutMainLoop();

    return 0;
}
