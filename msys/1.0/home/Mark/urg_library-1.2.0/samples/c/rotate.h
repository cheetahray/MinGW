#ifndef ROTATE_H
#define ROTATE_H
// kmeans.h
// Ethan Brodsky
// October 2011

float inputMatrix[4][1];
float outputMatrix[4][1];

int compareXA (const void * a, const void * b);
int compareXD (const void * a, const void * b);
int compareYA (const void * a, const void * b);
int compareYD (const void * a, const void * b);
void showPoint();
void multiplyMatrix();
void setUpRotationMatrix(float angle, float u, float v, float w);

#endif
