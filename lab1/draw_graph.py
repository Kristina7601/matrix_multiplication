import os
import csv
import matplotlib.pyplot as plt


def main():
    base = os.path.dirname(os.path.abspath(__file__))
    csv_path = os.path.join(base, "results", "lab1.csv")

    if not os.path.exists(csv_path):
        print(f"ОШИБКА: не найден {csv_path}")
        return

    sizes, times, gflops = [], [], []

    with open(csv_path, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            sizes.append(int(row["size"]))
            times.append(float(row["time"]))
            gflops.append(float(row["gflops"]))

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, marker="o", markersize=8, linewidth=2, color="blue")
    plt.xlabel("Размер матрицы N", fontsize=14)
    plt.ylabel("Время, сек", fontsize=14)
    plt.title("Время перемножения матриц", fontsize=15)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

    out = os.path.join(base, "time_vs_size.png")
    plt.savefig(out, dpi=150)
    plt.close()

    print(f"График сохранён: {out}\n")

    print("| N | Время, с | GFLOPS |")
    print("|---|---------:|-------:|")
    for s, t, g in zip(sizes, times, gflops):
        print(f"| {s} | {t:.4f} | {g:.2f} |")


if __name__ == "__main__":
    main()