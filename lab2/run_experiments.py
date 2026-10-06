import subprocess
import os
import sys


SIZES = [200, 400, 800, 1200, 1600, 2000]
THREADS = [1, 2, 4, 8]


env = os.environ.copy()
env["PATH"] = r"C:\msys64\ucrt64\bin;" + env.get("PATH", "")

exe = "matmul_omp.exe"

missing = []
for n in SIZES:
    for name in ("A", "B"):
        if not os.path.exists(f"{name}_{n}.txt"):
            missing.append(f"{name}_{n}.txt")
if missing:
    print("НЕТ ФАЙЛОВ МАТРИЦ:")
    for m in missing:
        print("  ", m)
    print("\nСгенерируйте матрицы командой generate_matrix.py (см. инструкцию).")
    sys.exit(1)

rows = []
for n in SIZES:
    for t in THREADS:
        env_run = env.copy()
        env_run["OMP_NUM_THREADS"] = str(t)

        cmd = [exe, str(n), f"A_{n}.txt", f"B_{n}.txt", f"C_{n}_{t}.txt"]
        result = subprocess.run(cmd, capture_output=True, text=True, env=env_run)

        if result.returncode != 0:
            print(f"ОШИБКА при N={n}, threads={t}")
            print(result.stderr)
            sys.exit(1)

        # вытаскиваем TIME из строки вида "N=... THREADS=... TIME=0.005234 GFLOPS=..."
        line = result.stdout.strip()
        time_val = None
        for token in line.split():
            if token.startswith("TIME="):
                time_val = float(token.split("=")[1])
        if time_val is None:
            print(f"Не удалось распарсить вывод: {line}")
            sys.exit(1)

        rows.append((n, t, time_val))
        print(f"N={n:<5} threads={t}  time={time_val:.6f} s")

with open("results.txt", "w", encoding="utf-8") as f:
    f.write("N\tThreads\tTime\n")
    for n, t, tv in rows:
        f.write(f"{n}\t{t}\t{tv:.6f}\n")

print("\nГотово. Результаты сохранены в results.txt")