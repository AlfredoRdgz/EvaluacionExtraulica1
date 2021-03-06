/*
 * 	Ejercicio 2
 *
 *  Created on: 17/02/2020
 *  Authors: Alfredo Rodriguez & Gustavo Rueda
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "Utils.h"

// Global variables needed
static int PEAKS = 24;
static float INNER_RADIUS = 0.25, OUTER_RADIUS = 0.5;
// Necessary ID's
GLuint programId, winSizeLoc, vertexArrayId, bufferId[2], vertexPosLoc, vertexColorLoc;
// Array declarations
static float *positionArray, *colorArray;

// Absolute value function for float elements
float absolute(float value) {
	if(value < 0.0) {
		return value * -1;
	}
	return value;
}

// Array calculation function
void fillArrays(int peaks, float innerRadius, float outerRadius) {
	PEAKS = peaks;
	INNER_RADIUS = innerRadius;
	OUTER_RADIUS = outerRadius;
	positionArray = (float*) malloc((PEAKS * 2 * 2 + 4) * sizeof(float)); // PEAKS * 2 for internal & external points * 2 for dimensions + 4 due to triangle fan elements needed
	colorArray = (float*) malloc((PEAKS * 2 * 3 + 6) * sizeof(float)); // PEAKS * 2 for internal & external points * 3 for dimensions + 6 due to extra 2 elements with rgb each
	float step = (2 * M_PI) / (PEAKS * 2), angle = 0.0;
	int i = 0, j = 2, c = 3;
	positionArray[0] = 0.0;
	positionArray[1] = 0.0;
	colorArray[0] = 1.0;
	colorArray[1] = 1.0;
	colorArray[2] = 1.0;
	for(i = 0, j = 2, c = 3; i < (PEAKS * 2); i++, angle += step) {
		if((i % 2) == 0) {
			positionArray[j++] = OUTER_RADIUS * cos(angle);
			positionArray[j++] = OUTER_RADIUS * sin(angle);
			colorArray[c++] = absolute(sin(angle));
			colorArray[c++] = absolute(cos(angle));
			if(OUTER_RADIUS * sin(angle) > 0){
				colorArray[c++] = 0.0;
			} else {
				colorArray[c++] = 0.5;
			}
		} else {
			positionArray[j++] = INNER_RADIUS * cos(angle);
			positionArray[j++] = INNER_RADIUS * sin(angle);
			colorArray[c++] = absolute(sin(angle));
			colorArray[c++] = absolute(cos(angle));
			if(INNER_RADIUS * sin(angle) > 0){
				colorArray[c++] = 0.0;
			} else {
				colorArray[c++] = 0.5;
			}
		}
	}
	if((i % 2) == 0) {
		positionArray[j++] = OUTER_RADIUS * cos(angle);
		positionArray[j++] = OUTER_RADIUS * sin(angle);
		colorArray[c++] = absolute(sin(angle));
		colorArray[c++] = absolute(cos(angle));
		if(OUTER_RADIUS * sin(angle) > 0){
			colorArray[c++] = 0.0;
		} else {
			colorArray[c++] = 0.5;
		}
	} else {
		positionArray[j++] = INNER_RADIUS * cos(angle);
		positionArray[j++] = INNER_RADIUS * sin(angle);
		colorArray[c++] = absolute(sin(angle));
		colorArray[c++] = absolute(cos(angle));
		if(INNER_RADIUS * sin(angle) > 0){
			colorArray[c++] = 0.0;
		} else {
			colorArray[c++] = 0.5;
		}
	}
	for(int k = 0; k < (PEAKS * 2 * 2 + 4); k++) {
		printf("%f ",positionArray[k]);
	}
	printf("\n");
	for(int k = 0; k < (PEAKS * 3 * 2 + 6); k++) {
		printf("%f ",colorArray[k]);
	}
}

// Shaders initialization function
static void initShaders(){
	GLuint vShaderId = compileShader("Shaders/ejercicio2.vsh",GL_VERTEX_SHADER);
	if(!shaderCompiled(vShaderId)) return;
	GLuint fShaderId = compileShader("Shaders/ejercicio2.fsh",GL_FRAGMENT_SHADER);
	if(!shaderCompiled(fShaderId)) return;
	programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
	vertexColorLoc = glGetAttribLocation(programId, "color");
	winSizeLoc = glGetUniformLocation(programId, "windowSize");
}

// Program init function
static void init() {
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glGenBuffers(2, bufferId);
	// Vertex array
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, (PEAKS * 2 * 2 + 4) * sizeof(float), positionArray, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(vertexPosLoc);
    // Color array
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER,(PEAKS * 2 * 3 + 6) * sizeof(float), colorArray, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(vertexColorLoc);

}

// Display function
static void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(vertexArrayId);
	glUniform2f(winSizeLoc, (float) glutGet(GLUT_WINDOW_WIDTH), (float) glutGet(GLUT_WINDOW_HEIGHT));
	glDrawArrays(GL_TRIANGLE_FAN, 0, (PEAKS * 2 * 2 + 4));
	glutSwapBuffers();
}

// Main function
int main2(int argc, char** argv) {
	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Ejercicio 2");
	glutDisplayFunc(display);
	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	initShaders();
	// Arrays calculation function:
	fillArrays(PEAKS, INNER_RADIUS, OUTER_RADIUS);
	init();
	glutMainLoop();
	return 0;
}
