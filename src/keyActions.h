void input(unsigned char, int, int);

/*
 *  Define global variables to be used by the program
 *
 *  scale:  a factor to scale the viewed cube by
 *  alpha:  number of degrees by which the cube is rotated
 *  beta:   number of degrees by which the cube is tilted
 *  inc:    amount to increase alpha for every frame of animation
 */
float scale, alpha, beta, zoom, protAlpha;
int rotX, rotY;
int rotInc;
int animate;
int pause;
int missileInc;
