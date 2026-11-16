#include <stdio.h>
#include <stdlib.h>

void mandelbrotSerial(float x0, float y0, float x1, float y1, int width, int height, int startRow, int endRow, int maxIterations, unsigned char* output) {
    float dx = (x1 - x0) / width;
    float dy = (y1 - y0) / height;
    for (int j = startRow; j < endRow; j++) {
        for (int i = 0; i < width; i++) {
            float x = x0 + i * dx;
            float y = y0 + j * dy;
            int index = (j * width + i);
            float z_re = x, z_im = y;
            int iter = 0;
            while (z_re * z_re + z_im * z_im <= 4.0f && iter < maxIterations) {
                float new_re = z_re * z_re - z_im * z_im;
                float new_im = 2 * z_re * z_im;
                z_re = new_re + x;
                z_im = new_im + y;
                iter++;
            }
            output[index] = (unsigned char)(255 * iter / maxIterations);
        }
    }
}