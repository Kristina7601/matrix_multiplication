import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


data = {}
with open("results.txt", encoding="utf-8") as f:
    next(f)
    for line in f:
        line = line.strip()
        if not line:
            continue
        parts = line.split()
        n, t, tv = int(parts[0]), int(parts[1]), float(parts[2])
        data[(n, t)] = tv

sizes = sorted({n for n, _ in data})
threads = sorted({t for _, t in data})

plt.figure(figsize=(9, 5))
for t in threads:
    xs = [n for n in sizes if (n, t) in data]
    ys = [data[(n, t)] for n in xs]
    plt.plot(xs, ys, marker="o", label=f"{t} поток(ов)")
plt.xlabel("Размер матрицы N")
plt.ylabel("Время, с")
plt.title("Время умножения матриц от размера (OpenMP)")
plt.grid(True, ls=":")
plt.legend()
plt.tight_layout()
plt.savefig("time.png", dpi=150)
plt.close()


plt.figure(figsize=(9, 5))
for t in threads:
    if t == 1:
        continue
    xs, ys = [], []
    for n in sizes:
        if (n, 1) in data and (n, t) in data:
            xs.append(n)
            ys.append(data[(n, 1)] / data[(n, t)])
    plt.plot(xs, ys, marker="o", label=f"{t} поток(ов)")
plt.axhline(y=1, color="gray", ls="--", linewidth=1)
plt.xlabel("Размер матрицы N")
plt.ylabel("Ускорение S = T(1) / T(p)")
plt.title("Ускорение OpenMP от размера матрицы")
plt.grid(True, ls=":")
plt.legend()
plt.tight_layout()
plt.savefig("speedup.png", dpi=150)
plt.close()


plt.figure(figsize=(9, 5))
for t in threads:
    xs = [n for n in sizes if (n, t) in data]
    ys = [2 * n**3 / data[(n, t)] / 1e9 for n in xs]
    plt.plot(xs, ys, marker="o", label=f"{t} поток(ов)")
plt.xlabel("Размер матрицы N")
plt.ylabel("Производительность, GFLOPS")
plt.title("Производительность OpenMP-умножения матриц")
plt.grid(True, ls=":")
plt.legend()
plt.tight_layout()
plt.savefig("gflops.png", dpi=150)
plt.close()

print("Готово: time.png, speedup.png, gflops.png")