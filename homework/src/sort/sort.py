import math
import matplotlib.pyplot as plt

# 題目參數
ts = 0.08    # seek time (秒)
tl = 0.02    # latency time (秒)
tt = 0.001   # 每筆傳輸時間（秒）
n = 200000   # 資料筆數
m = 64       # 初始 run 數

# 測試範圍：k from 2 to m
k_values = list(range(2, m + 1))
t_input_values = []

# 計算每個 k 的 I/O 時間
for k in k_values:
    rounds = math.ceil(math.log(m, k))
    t_input_k = rounds * (m * (ts + tl) + n * tt)
    t_input_values.append(t_input_k)

# 掃描 CPU 值從 1 到 300 秒
cpu_test_values = [i / 10.0 for i in range(10, 3000)]  # 1.0 到 300.0 秒，每 0.1 秒
closest_cpu = None
closest_k = None
min_diff = float('inf')
closest_t_input = None

# 找出最接近的 CPU 時間
for cpu_val in cpu_test_values:
    for k, t_input_k in zip(k_values, t_input_values):
        diff = abs(t_input_k - cpu_val)
        if diff < min_diff:
            min_diff = diff
            closest_cpu = cpu_val
            closest_k = k
            closest_t_input = t_input_k

# 繪圖：顯示所有 t_input 曲線與找到的最佳點
plt.figure(figsize=(10, 6))
plt.plot(k_values, t_input_values, label='t_input (I/O time)', color='blue')
plt.axhline(y=closest_cpu, color='red', linestyle='--', label=f'Best t_CPU ≈ {closest_cpu:.2f}s')
plt.axvline(x=closest_k, color='green', linestyle=':', label=f'k = {closest_k}')
plt.scatter([closest_k], [closest_t_input], color='black', zorder=5)
plt.xlabel("k (ways merged)")
plt.ylabel("Time (seconds)")
plt.title("Match Between I/O Time and Estimated CPU Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# 輸出最佳配對資訊
{
    "最接近 t_input 的 t_CPU": round(closest_cpu, 3),
    "對應的 k 值": closest_k,
    "對應的 t_input": round(closest_t_input, 3),
    "差距": round(min_diff, 3)
}
