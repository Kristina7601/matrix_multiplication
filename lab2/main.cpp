#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <omp.h>

using namespace std;
using Matrix = vector<vector<double>>;

Matrix readMatrix(const string& path, int n) {
    Matrix m(n, vector<double>(n));
    ifstream f(path.c_str());
    if (!f) { cerr << "cannot open " << path << "\n"; exit(1); }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            f >> m[i][j];
    return m;
}

void writeMatrix(const string& path, const Matrix& m, int n) {
    ofstream f(path.c_str());
    f.precision(10);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            f << m[i][j] << (j + 1 == n ? '\n' : ' ');
    }
}

void matmul_seq(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double aik = A[i][k];
            for (int j = 0; j < n; j++)
                C[i][j] += aik * B[k][j];
        }
}

void matmul_omp(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double aik = A[i][k];
            for (int j = 0; j < n; j++)
                C[i][j] += aik * B[k][j];
        }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "usage: " << argv[0]
             << " <N> <A.txt> <B.txt> [C_out.txt] [--seq]\n";
        return 1;
    }
    int n = atoi(argv[1]);
    string pa = argv[2], pb = argv[3];
    string pc = (argc >= 5 && argv[4][0] != '-') ? argv[4] : "C_out.txt";
    bool seq = false;
    for (int i = 1; i < argc; i++)
        if (string(argv[i]) == "--seq") seq = true;

    Matrix A = readMatrix(pa, n);
    Matrix B = readMatrix(pb, n);
    Matrix C(n, vector<double>(n, 0.0));

    int threads = omp_get_max_threads();
    double t0 = omp_get_wtime();
    if (seq) matmul_seq(A, B, C, n);
    else     matmul_omp(A, B, C, n);
    double t1 = omp_get_wtime();

    writeMatrix(pc, C, n);

    double dt = t1 - t0;
    double gflops = (2.0 * n * n * n) / dt / 1e9;
    printf("N=%d THREADS=%d MODE=%s TIME=%.6f GFLOPS=%.2f\n",
           n, seq ? 1 : threads, seq ? "seq" : "omp", dt, gflops);
    return 0;
}