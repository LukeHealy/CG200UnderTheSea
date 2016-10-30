# CG200 Assignment 2 - OpenGL
--
## Introduction
The scene is of two typhoon class submarines under the sea near some flashing mines.

The animation in this scene is made up from both movement and lighting. Each submarine fires a missile periodically, which shoots off into the distance.
The two propellers also rotate on each submarine.
In addition, each mine contains a light source, which flashes red, illuminating most of the scene. 

## Scene

### Modelling/Rendering

The bulk of the modelling was done using OpenGL and GLUT functions to draw basic shapes, then modify them using scaling, translation and rotation, in conjuntion with the OpenGL matrix stack. This allowed me to easily draw shapes and manipulate them to make a fairly accurate submarine.

### Animation
The missile animation is created using translations according to a global value which continually increments, then resets at a maximum. The propeller rotation animation is acheives by rotating relative to the same global value.
The periodic flashing is acheived by changing the colour of the light source inside each missile according to the value of the global variable. It is modded by 60, and if that is less than 5, it changes to red, otherwise it changes to black.
## Objects
### Missile

 * The missiles are a combination of a scaled sphere, and a cylinder.
 * They also have a missile texture wrapped around them to make them look like a torpedo.

### Propeller

 * The propellers are made of a centeral scaled sphere, with four equilateral trianglular prisms whose base is paralell to the line accross the point at which the tip touches the sphere. Each triangle is rotated slightly about z to give the effect of a propeller.
 * The finish on the propellers is highly reflective, so that when they rotate during the animation, there is an obvious change in colour on the side facing the light source.

### Submarine

 * The submarine is made from a series of cylinders, spheres and disks, all scaled an rotated to look like a classic submarine. The nose is a series of spheres, each one slightly offset from the last, followed by a large scales sphere which makes up the tail end of the fusilage. The front and rear fins are scaled and rotated cylinders, each with disks to cover the open ends. Two propellers are attached to each submarine on the horizontal rear fin. The crest of the submarine was made similar to the fusilage, in leading staggered spheres followed by a scaled sphere. The top part is the same, except instead of spheres, cylinders were used, again with disks to cover the end.
 * The finish on the submarines is like a dull metalic finish, which tries to emulate an actual submarine. This means that it blends well with the dark background, but when the mines flash, it is illuminated.

### Mines

* The mines are comprised of a sphere, with scaled and rotated cylinders placed around it. They are placed using a loop to get them evenly spaced. Each "spike" also has a disk on the end which changed colour when the light flashes to enhance the flashing effect.
* The mines are transparent, with the spikes being solid.

### Floor/water
 * The sea floor is a large flat quad, and the "sea" is a large sphere that creates the horizon.
 * The floor has a sand texture.
 * There is also heavy fog to give the effect of poor vision deep under the sea.

## Referneces
The following resources were used to aid the learning required to complete this assignment.

 * http://nehe.gamedev.net/
 * http://www.videotutorialsrock.com/
 * https://www.opengl.org/documentation/
 * http://stackoverflow.com/