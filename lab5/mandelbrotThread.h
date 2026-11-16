#ifndef MANDELBROT_THREAD_H
#define MANDELBROT_THREAD_H

struct WorkerArgs {
    int threadId;
    int numThreads;
    float x0, y0, x1, y1;
    int width, height;
    int maxIterations;
    unsigned char* output;
};

void mandelbrotSerial(float x0, float y0, float x1, float y1, int width, int height, int startRow, int endRow, int maxIterations, unsigned char* output);

double mandelbrotThread(int numThreads, float x0, float y0, float x1, float y1, int width, int height, int maxIterations, unsigned char* output);

#endif