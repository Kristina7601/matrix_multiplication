#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    system("chcp 1251>nul");
    cout << "====== ПЕРЕМНОЖЕНИЕ МАТРИЦ ======" << endl;

    if (argc < 5) {
        cout << "Использование: lab1.exe <A> <B> <result> <info>" << endl;
        return 1;
    }

    ifstream fileA(argv[1]);
    ifstream fileB(argv[2]);

    if (!fileA.is_open() || !fileB.is_open()) {
        cout << "Ошибка: не удалось открыть файлы!" << endl;
        cout << "A: " << argv[1] << endl;
        cout << "B: " << argv[2] << endl;
        return 1;
    }

    int n;
    fileA >> n;
    fileB >> n;

    cout << "Размер матриц: " << n << " x " << n << endl;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> B(n, vector<double>(n));
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fileA >> A[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fileB >> B[i][j];
        }
    }

    fileA.close();
    fileB.close();

    auto start = high_resolution_clock::now();

    cout << "Выполняется перемножение..." << endl;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = A[i][k];
            for (int j = 0; j < n; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }

    auto end = high_resolution_clock::now();
    double time_sec = duration<double>(end - start).count();

    cout << "Время выполнения: " << time_sec << " сек" << endl;

    ofstream resultFile(argv[3]);
    resultFile << n << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            resultFile << fixed << setprecision(15) << C[i][j] << " ";
        }
        resultFile << endl;
    }
    resultFile.close();

    cout << "Результат сохранён: " << argv[3] << endl;

    long long memory = 3LL * n * n * sizeof(double);
    long long operations = 2LL * n * n * n;
    double gflops = (time_sec > 0) ? (operations / 1e9) / time_sec : 0.0;

    cout << "\n====== МЕТРИКИ ======" << endl;
    cout << "Память: " << memory / 1024 << " КБ" << endl;
    cout << "Операций: " << operations << endl;

    if (time_sec > 0) {
        cout << "Производительность: " << gflops << " GFLOPS" << endl;
    }
    else {
        cout << "Производительность: время слишком мало" << endl;
    }

    ofstream infoFile(argv[4]);
    infoFile << "size=" << n << "\n";
    infoFile << "time=" << time_sec << "\n";
    infoFile << "gflops=" << gflops << "\n";
    infoFile.close();

    cout << "Информация сохранена: " << argv[4] << endl;
    cout << "==================================" << endl;

    return 0;
}