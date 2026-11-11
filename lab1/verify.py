import os
import numpy as np

SIZES = [200, 400, 800, 1200, 1600, 2000]


def load_matrix(path):
    with open(path, "r", encoding="utf-8") as f:
        n = int(f.readline().strip())
        data = []
        for _ in range(n):
            row = list(map(float, f.readline().split()))
            data.append(row)
    return np.array(data)


def main():
    base = os.path.dirname(os.path.abspath(__file__))
    inp = os.path.join(base, "lab1", "input")
    out = os.path.join(base, "lab1", "output")

    print("====== ВЕРИФИКАЦИЯ ======\n")

    all_ok = True
    for n in SIZES:
        try:
            a = load_matrix(os.path.join(inp, f"input_matrix1_{n}.txt"))
            b = load_matrix(os.path.join(inp, f"input_matrix2_{n}.txt"))
            c_cpp = load_matrix(os.path.join(out, f"result_{n}.txt"))

            c_python = a @ b
            diff = np.max(np.abs(c_cpp - c_python))

            status = "OK" if diff < 1e-6 else "FAIL"
            if status == "FAIL":
                all_ok = False

            print(f"  N = {n:5d}: {status}, разница = {diff:.2e}")
        except FileNotFoundError:
            print(f"  N = {n:5d}: файл не найден")
            all_ok = False

    print()
    if all_ok:
        print("Все результаты совпали с NumPy!")
    else:
        print("Есть расхождения.")


if __name__ == "__main__":
    main()