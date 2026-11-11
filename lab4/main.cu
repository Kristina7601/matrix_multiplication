#include <iostream>
#include <vector>
#include <chrono>
#include <cuda_runtime.h>
#include <iomanip>

__global__ void matmul_kernel(const double* A, const double* B, double* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        double sum = 0.0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

void matmul_cpu(const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& C, int N) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            double aik = A[i * N + k];
            for (int j = 0; j < N; j++) {
                C[i * N + j] += aik * B[k * N + j];
            }
        }
    }
}

int main() {
    std::vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};
    std::vector<int> block_sizes = {16, 32}; 

    std::cout << "N,BlockSize,CPU_Time_s,GPU_Time_s,Speedup,GFLOPS" << std::endl;

    for (int N : sizes) {
        size_t bytes = N * N * sizeof(double);

        std::vector<double> h_A(N * N, 1.0);
        std::vector<double> h_B(N * N, 2.0);
        std::vector<double> h_C_cpu(N * N, 0.0);

        double cpu_time = 0;
        if (N <= 2000) {
            auto start = std::chrono::high_resolution_clock::now();
            matmul_cpu(h_A, h_B, h_C_cpu, N);
            auto end = std::chrono::high_resolution_clock::now();
            cpu_time = std::chrono::duration<double>(end - start).count();
        }

        double *d_A, *d_B, *d_C;
        cudaMalloc(&d_A, bytes);
        cudaMalloc(&d_B, bytes);
        cudaMalloc(&d_C, bytes);

        cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice);

        for (int bs : block_sizes) {
            dim3 blockSize(bs, bs);
            dim3 gridSize((N + bs - 1) / bs, (N + bs - 1) / bs);

            matmul_kernel<<<gridSize, blockSize>>>(d_A, d_B, d_C, N);
            cudaDeviceSynchronize();

            double gpu_time = 1e9;
            for (int i = 0; i < 3; i++) {
                auto start = std::chrono::high_resolution_clock::now();
                matmul_kernel<<<gridSize, blockSize>>>(d_A, d_B, d_C, N);
                cudaDeviceSynchronize(); 
                auto end = std::chrono::high_resolution_clock::now();
                double t = std::chrono::duration<double>(end - start).count();
                if (t < gpu_time) gpu_time = t;
            }

            double speedup = (cpu_time > 0) ? (cpu_time / gpu_time) : 0;
            double gflops = (2.0 * N * N * N) / (gpu_time * 1e9); 

            std::cout << N << "," << bs << "," 
                      << std::fixed << std::setprecision(6) << cpu_time << "," 
                      << gpu_time << "," 
                      << std::setprecision(2) << speedup << "," 
                      << gflops << std::endl;
        }

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }

    return 0;
}