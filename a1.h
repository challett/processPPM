#ifndef INCLUDED_A1_H
#define INCLUDED_A1_H

typedef struct {
  unsigned char r,g,b;
} RGB;

RGB * readPPM(char* file, int* width, int* height, int* max);
void writePPM(char* file, int width, int height, int max, const RGB *image);

RGB * processImage(int width, int height, RGB *image, int N, int offset, char *type);
int select(int *a, int s, int e, int k);
int cmpfunc (const void * a, const void * b);
void swap(int *i, int *j);

#endif
