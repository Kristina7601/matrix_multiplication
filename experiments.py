import os
import subprocess
import csv

SIZES = [200, 400, 800, 1200, 1600, 2000]


def run_experiment(exe_path, n, base):
    inp = os.path.join(base, "lab1", "input")
    out = os.path.join(base, "lab1", "output")
    os.makedirs(out, exist_ok=True)

    file_a = os.path.join(inp, f"input_matrix1_{n}.txt")
    file_b = os.path.join(inp, f"input_matrix2_{n}.txt")
    result = os.path.join(out, f"result_{n}.txt")
    info = os.path.join(out, f"info_{n}.txt")

    res = subprocess.run(
        [exe_path, file_a, file_b, result, info],
        capture_output=True,
        text=True,
    )

    if res.returncode != 0:
        print(f"ошибка при N={n}: {res.stderr}")
        return None

    time_sec = 0.0
    gflops = 0.0
    if os.path.exists(info):
        with open(info, "r", encoding="utf-8") as f:
            for line in f:
                if line.startswith("time="):
                    time_sec = float(line.split("=")[1])
                elif line.startswith("gflops="):
                    gflops = float(line.split("=")[1])

    return time_sec, gflops


def main():
    base = os.path.dirname(os.path.abspath(__file__))
    exe_path = os.path.join(base, "lab1", "x64", "Release", "lab1.exe")

    if not os.path.exists(exe_path):
        print(f"ОШИБКА: не найден {exe_path}")
        return

    results_dir = os.path.join(base, "results")
    os.makedirs(results_dir, exist_ok=True)
    csv_path = os.path.join(results_dir, "lab1.csv")

    print("====== ЭКСПЕРИМЕНТЫ ======\n")

    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["size", "time", "gflops"])

        for n in SIZES:
            print(f"  N = {n:5d} ...", end=" ", flush=True)
            result = run_experiment(exe_path, n, base)

            if result is None:
                print("ошибка")
                continue

            time_sec, gflops = result
            print(f"время = {time_sec:.4f} сек, GFLOPS = {gflops:.2f}")
            writer.writerow([n, time_sec, gflops])

    print(f"\nГотово! Результаты в {csv_path}")


if __name__ == "__main__":
    main()