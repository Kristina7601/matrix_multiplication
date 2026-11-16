#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "CycleTimer.h"
#include "mandelbrotThread.h"

int main(int argc, char** argv) {
    int width = 800;
    int height = 600;
    int maxIterations = 256;
    float x0 = -2.0f, y0 = -1.0f, x1 = 1.0f, y1 = 1.0f;
    unsigned char* output = new unsigned char[width * height];
    int numThreads = 4;
    
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--threads" && i + 1 < argc) {
            numThreads = atoi(argv[i+1]);
        }
    }

    printf("Running with %d threads...\n", numThreads);
    mandelbrotThread(numThreads, x0, y0, x1, y1, width, height, maxIterations, output);
    printf("Done.\n");
    
    delete[] output;
    return 0;
}