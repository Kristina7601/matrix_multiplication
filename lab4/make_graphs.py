import matplotlib.pyplot as plt
import csv

data_16 = {'N': [], 'GPU_Time': [], 'GFLOPS': [], 'Speedup': []}
data_32 = {'N': [], 'GPU_Time': [], 'GFLOPS': [], 'Speedup': []}
cpu_n = []
cpu_time = []

with open('results.txt', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row['N'])
        bs = int(row['BlockSize'])
        cpu_t = float(row['CPU_Time_s'])
        gpu_t = float(row['GPU_Time_s'])
        sp = float(row['Speedup'])
        gf = float(row['GFLOPS'])

        if bs == 16:
            data_16['N'].append(n)
            data_16['GPU_Time'].append(gpu_t)
            data_16['GFLOPS'].append(gf)
            data_16['Speedup'].append(sp)
        else:
            data_32['N'].append(n)
            data_32['GPU_Time'].append(gpu_t)
            data_32['GFLOPS'].append(gf)
            data_32['Speedup'].append(sp)
        
        if cpu_t > 0 and n not in cpu_n:
            cpu_n.append(n)
            cpu_time.append(cpu_t)

# 1. График: Время CPU vs GPU
plt.figure(figsize=(8, 5))
plt.plot(cpu_n, cpu_time, marker='o', label='CPU (i5-12450H)')
plt.plot(data_16['N'], data_16['GPU_Time'], marker='s', label='GPU (Tesla T4, 16x16)')
plt.yscale('log')
plt.xlabel('Размер матрицы N')
plt.ylabel('Время, с (лог. шкала)')
plt.title('Сравнение времени CPU и GPU')
plt.grid(True, ls=':')
plt.legend()
plt.tight_layout()
plt.savefig('time_cpu_vs_gpu.png', dpi=150)
plt.close()

# 2. График: Ускорение
plt.figure(figsize=(8, 5))
plt.plot(data_16['N'], data_16['Speedup'], marker='o', label='Блок 16x16')
plt.plot(data_32['N'], data_32['Speedup'], marker='s', label='Блок 32x32')
plt.xlabel('Размер матрицы N')
plt.ylabel('Ускорение (CPU / GPU)')
plt.title('Ускорение от размера матрицы')
plt.grid(True, ls=':')
plt.legend()
plt.tight_layout()
plt.savefig('speedup.png', dpi=150)
plt.close()

# 3. График: GFLOPS
plt.figure(figsize=(8, 5))
plt.plot(data_16['N'], data_16['GFLOPS'], marker='o', label='Блок 16x16')
plt.plot(data_32['N'], data_32['GFLOPS'], marker='s', label='Блок 32x32')
plt.xlabel('Размер матрицы N')
plt.ylabel('Производительность, GFLOPS')
plt.title('Производительность CUDA-ядра')
plt.grid(True, ls=':')
plt.legend()
plt.tight_layout()
plt.savefig('gflops.png', dpi=150)
plt.close()

print("Графики готовы: time_cpu_vs_gpu.png, speedup.png, gflops.png")