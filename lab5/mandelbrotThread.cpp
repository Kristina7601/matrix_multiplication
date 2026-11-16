#include "mandelbrotThread.h"
#include <thread>
#include <vector>

void workerThreadStart(WorkerArgs * const args) {
    int threadId = args->threadId;
    int numThreads = args->numThreads;
    
    for (int i = threadId; i < args->height; i += numThreads) {
        mandelbrotSerial(args->x0, args->y0, args->x1, args->y1,
                         args->width, args->height, i, i + 1, 
                         args->maxIterations, args->output);
    }
}

double mandelbrotThread(int numThreads, float x0, float y0, float x1, float y1, int width, int height, int maxIterations, unsigned char * output) {
    std::vector<std::thread> threads;
    std::vector<WorkerArgs*> argsList;

    for (int i = 0; i < numThreads; i++) {
        WorkerArgs* args = new WorkerArgs();
        args->threadId = i;
        args->numThreads = numThreads;
        args->x0 = x0; args->y0 = y0; args->x1 = x1; args->y1 = y1;
        args->width = width; args->height = height;
        args->maxIterations = maxIterations; args->output = output;
        
        argsList.push_back(args);
        threads.push_back(std::thread(workerThreadStart, args));
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
        delete argsList[i];
    }
    return 0.0;
}