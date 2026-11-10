#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 0;
    vector<vector<double>> A, B, C;
    
    if (rank == 0) {
        ifstream fileA("input/matrix_a.txt");
        ifstream fileB("input/matrix_b.txt");

        if (!fileA.is_open() || !fileB.is_open()) {
            cerr << "Ошибка: не удалось открыть файлы в input/" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fileA >> n;
        fileB >> n;

        A.resize(n, vector<double>(n));
        B.resize(n, vector<double>(n));
        C.resize(n, vector<double>(n, 0.0));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) fileA >> A[i][j];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) fileB >> B[i][j];
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<double> flat_A, flat_B, flat_C;
    if (rank == 0) {
        flat_A.resize(n * n);
        flat_B.resize(n * n);
        flat_C.resize(n * n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                flat_A[i * n + j] = A[i][j];
                flat_B[i * n + j] = B[i][j];
            }
        }
    } else {
        flat_B.resize(n * n);
    }

    MPI_Bcast(flat_B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size;
    int remainder = n % size;
    int local_rows = rows_per_proc + (rank < remainder ? 1 : 0);
    int start_row = rank * rows_per_proc + min(rank, remainder);

    vector<int> counts(size), displs(size);
    for (int i = 0; i < size; i++) {
        int r = rows_per_proc + (i < remainder ? 1 : 0);
        int s = i * rows_per_proc + min(i, remainder);
        counts[i] = r * n;
        displs[i] = s * n;
    }

    vector<double> local_A(local_rows * n);
    vector<double> local_C(local_rows * n, 0.0);

    MPI_Barrier(MPI_COMM_WORLD);
    auto start = high_resolution_clock::now();

    MPI_Scatterv(flat_A.data(), counts.data(), displs.data(), MPI_DOUBLE,
                 local_A.data(), local_rows * n, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        for (int k = 0; k < n; k++) {
            double aik = local_A[i * n + k];
            for (int j = 0; j < n; j++) {
                local_C[i * n + j] += aik * flat_B[k * n + j];
            }
        }
    }

    MPI_Gatherv(local_C.data(), local_rows * n, MPI_DOUBLE,
                flat_C.data(), counts.data(), displs.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    auto end = high_resolution_clock::now();
    double time_sec = duration<double>(end - start).count();

    if (rank == 0) {
        cout << "====== MPI ПЕРЕМНОЖЕНИЕ МАТРИЦ ======" << endl;
        cout << "Размер матриц: " << n << " x " << n << endl;
        cout << "Процессов: " << size << endl;
        cout << "Время выполнения: " << time_sec << " сек" << endl;

        double max_error = 0.0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double expected = 0.0;
                for (int k = 0; k < n; k++) {
                    expected += A[i][k] * B[k][j];
                }
                double diff = fabs(flat_C[i * n + j] - expected);
                if (diff > max_error) max_error = diff;
            }
        }
        
        cout << "Максимальная ошибка: " << max_error << endl;
        if (max_error < 1e-9) cout << "✅ VERIFICATION PASSED!" << endl;
        else cout << "❌ VERIFICATION FAILED!" << endl;

        long long memory = 3LL * n * n * sizeof(double);
        long long operations = 2LL * n * n * n;
        double gflops = (time_sec > 0) ? (operations / 1e9) / time_sec : 0.0;

        cout << "\n====== МЕТРИКИ ======" << endl;
        cout << "Память: " << memory / 1024 << " КБ" << endl;
        cout << "Операций: " << operations << endl;
        if (time_sec > 0) cout << "Производительность: " << gflops << " GFLOPS" << endl;

        ofstream out("output/result.txt");
        out << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                out << fixed << setprecision(15) << flat_C[i * n + j] << " ";
            }
            out << endl;
        }
        out.close();

        ofstream report("output/matrix_report.txt");
        report << "size=" << n << "\n";
        report << "processors=" << size << "\n";
        report << "time=" << time_sec << "\n";
        report << "max_error=" << max_error << "\n";
        report << "gflops=" << gflops << "\n";
        report.close();
    }

    MPI_Finalize();
    return 0;
}