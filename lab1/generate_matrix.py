import os
import random

SIZES = [200, 400, 800, 1200, 1600, 2000]


def make_matrix(n):
    return [[round(random.uniform(0, 10), 2) for _ in range(n)] for _ in range(n)]


def save_matrix(m, path):
    with open(path, "w", encoding="utf-8") as f:
        f.write(f"{len(m)}\n")
        for row in m:
            f.write(" ".join(map(str, row)) + "\n")


def main():
    base = os.path.dirname(os.path.abspath(__file__))

    for n in SIZES:
        a = make_matrix(n)
        b = make_matrix(n)

        inp = os.path.join(base, "lab1", "input")
        os.makedirs(inp, exist_ok=True)

        save_matrix(a, os.path.join(inp, f"input_matrix1_{n}.txt"))
        save_matrix(b, os.path.join(inp, f"input_matrix2_{n}.txt"))

        print(f"Сгенерированы матрицы {n}x{n}")

    print("\nГотово! Файлы в lab1/input/")


if __name__ == "__main__":
    main()