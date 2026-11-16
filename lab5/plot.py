import matplotlib.pyplot as plt
threads = [1, 2, 4, 8, 12]
times = [0.2278, 0.1212, 0.0706, 0.0751, 0.0517]
speedup = [times[0] / t for t in times]
plt.figure(figsize=(8, 5))
plt.plot(threads, speedup, marker='o', color='b', label='Intel Core i5-12450H')
plt.plot(threads, threads, 'k--', label='Идеальное ускорение')
plt.xlabel('Количество потоков')
plt.ylabel('Ускорение (Speedup)')
plt.title('Ускорение генерации фрактала Мандельброта')
plt.legend()
plt.grid(True, linestyle=':')
plt.xticks(threads)
plt.tight_layout()
plt.savefig('speedup.png', dpi=150)
