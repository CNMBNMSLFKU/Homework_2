# 41243206 41243209 

作業二

## 解題說明

本次作業主要包含三個部分：

- 最小堆積（MinHeap）：基於介面 ``MinPQ`` 設計並使用 C++ vector 作為內部容器，完成最小堆積的插入與刪除。

- 二元搜尋樹（BST）：針對隨機資料建立平衡 BST，並分析其高度與理論最小高度 log₂(n) 的比值。

- 外部排序：探討第二階段的輸入時間與 CPU 合併時間的關係。在記憶體容量遠小於資料筆數的情況下，必須使用多路合併（k-way merge）來進行外部排序。根據題意，我們計算不同 k 值下所需的合併輪數與總輸入時間，並評估是否存在某個 k 值，使得 CPU 處理時間與輸入時間相當。

### 解題策略

**1. MinHeap**

  - 使用 vector 動態陣列實作，並維護其最小堆積性質。

  - 實作 HeapifyUp（插入後）與 HeapifyDown（刪除後）兩個方法以保持堆的結構正確。

  - 提供介面如 ``Push``、``Pop``、``Top`` 與 ``IsEmpty``，並繼承自抽象基底類別 ``MinPQ``，符合物件導向原則。


**2. BST**

  - 使用亂數產生資料，先排序再遞迴以中間分割法建立平衡 BST。

  - 避免傳統一筆一筆插入造成偏斜樹，使得高度更接近理論值 log₂(n)。

  - 藉由計算實際高度與理論值的比值，分析平衡性與效率。


**3. 外部排序**

  - CPU 時間假設：

    題目給定 t_CPU 為常數（假設與 k 無關），所以假設 t_CPU 範圍為 1～300 秒，並找出 t_CPU ≈ t_input 的值與對應 k 值。
    
  - 使用公式：

    
    ***t <sub>input</sub>* = rounds × ( *m* × ( *t <sub>s</sub>* + *t <sub>l</sub>* ) + *n* × *t <sub>t</sub>*** )
  

    其中：
    
    ``rounds`` : 合併階段的輪數，計算方式為 ⌈ logₖ(m) ⌉。

    ``ts`` : seek time (磁碟讀寫頭移動到目標磁軌所需的時間)

    ``tl`` : latency time (磁碟旋轉至目標區塊所需的延遲時間)

    ``tt`` : 每筆資料傳輸時間

    ``m`` : run 數（磁碟存取次數）

    ``n`` : 總資料筆數


## 程式實作

### MinHeap
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

// 定義一個抽象類別 MinPQ，作為最小優先佇列的介面
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}  // 虛擬解構子，確保多型行為
    virtual bool IsEmpty() const = 0;           // 檢查佇列是否為空
    virtual const T& Top() const = 0;           // 回傳最小值（堆頂元素）
    virtual void Push(const T& item) = 0;       // 插入一個新元素
    virtual void Pop() = 0;                     // 移除最小值（堆頂元素）
};

// MinHeap 類別繼承 MinPQ，使用 vector 實作最小堆結構
template <class T>
class MinHeap : public MinPQ<T> {
private:
    std::vector<T> heap;  // 儲存堆的容器

    // 自底向上調整堆（插入時使用）
    void HeapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else break;
        }
    }

    // 自頂向下調整堆（刪除堆頂時使用）
    void HeapifyDown(int index) {
        int size = heap.size();
        while (index * 2 + 1 < size) {
            int child = index * 2 + 1;  // 左子節點
            // 若右子節點存在且比左子節點小，則選擇右子節點
            if (child + 1 < size && heap[child + 1] < heap[child])
                child++;
            // 若子節點比父節點小，交換兩者
            if (heap[child] < heap[index]) {
                std::swap(heap[child], heap[index]);
                index = child;
            } else break;
        }
    }

public:
    // 檢查堆是否為空
    bool IsEmpty() const override {
        return heap.empty();
    }

    // 取得堆頂元素（最小值）
    const T& Top() const override {
        if (IsEmpty())
            throw std::runtime_error("Heap is empty.");
        return heap[0];
    }

    // 插入元素並維持堆性質
    void Push(const T& item) override {
        heap.push_back(item);
        HeapifyUp(heap.size() - 1);
    }

    // 移除堆頂元素並重建堆
    void Pop() override {
        if (IsEmpty())
            throw std::runtime_error("Heap is empty.");
        heap[0] = heap.back();     // 將最後一個元素移至堆頂
        heap.pop_back();           // 移除最後一個元素
        if (!heap.empty()) HeapifyDown(0);  // 自頂向下重建堆
    }
};

// 主程式進行測試
int main() {
    MinHeap<int> h;
    h.Push(5);
    h.Push(2);
    h.Push(8);
    h.Push(1);
    h.Push(-10);
    h.Push(2);
    h.Push(INT_MAX);
    h.Push(INT_MIN);


    // 依序輸出最小元素，並移除
    while (!h.IsEmpty()) {
        std::cout << h.Top() << " ";
        h.Pop();
    }
    return 0;
}
```

### BST
```cpp
#include <iostream>
#include <vector>
#include <algorithm> // sort, max
#include <cmath>     // log2
#include <cstdlib>   // rand
#include <ctime>     // time

using namespace std;

// 定義 BST 節點結構
struct TreeNode {
    int val;              // 節點的值
    TreeNode* left;       // 左子樹指標
    TreeNode* right;      // 右子樹指標
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}  // 建構子
};

// BST 類別，提供平衡建樹與高度計算功能
class BST {
public:
    TreeNode* root;  // BST 的根節點

    BST() : root(nullptr) {}  // 建構子，初始化 root 為空

    // 使用排序後陣列遞迴建立平衡 BST
    TreeNode* buildBalancedBST(const vector<int>& nums, int left, int right) {
        if (left > right) return nullptr;  // 終止條件：區間無效
        int mid = (left + right) / 2;      // 取中間值作為根
        TreeNode* node = new TreeNode(nums[mid]);       // 建立節點
        node->left = buildBalancedBST(nums, left, mid - 1);   // 遞迴建左子樹
        node->right = buildBalancedBST(nums, mid + 1, right); // 遞迴建右子樹
        return node;
    }

    // 公開介面：從排序後的陣列建樹
    void buildFromSortedArray(const vector<int>& nums) {
        root = buildBalancedBST(nums, 0, nums.size() - 1);
    }

    // 遞迴計算 BST 的高度
    int getHeight(TreeNode* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // 公開介面：取得整棵樹的高度
    int getHeight() {
        return getHeight(root);
    }
};

int main() {
    srand(time(0));  // 設定隨機種子，確保每次執行資料不同
    vector<int> ns = { 100, 500, 1000, 2000, 3000, 5000, 10000 };  // 不同資料規模測試

    for (int n : ns) {
        vector<int> nums;
        for (int i = 0; i < n; ++i)
            nums.push_back(rand());  // 隨機產生 n 筆資料

        sort(nums.begin(), nums.end());  // ⭐️關鍵步驟：先排序，為了建出平衡樹

        BST tree;
        tree.buildFromSortedArray(nums);  // 用排序好的陣列建立平衡 BST

        int h = tree.getHeight();             // 計算樹高
        double ratio = h / log2(n);           // 與理想高度 log₂(n) 的比值

        // 輸出結果
        cout << "n = " << n
             << ", Height = " << h
             << ", Height/log2(n) = " << ratio << endl;
    }

    return 0;
}
```

### 外部排序
```python
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

```

## 效能分析

### MinHeap

- 時間複雜度：

  1. ``Push`` 與 ``Pop`` 為 O(log n)。

  2. ``Top`` 與 ``IsEmpty`` 為 O(1)。


### BST

- 理論最佳高度為 log₂(n)，從排序陣列建 Balanced BST 控制其為穩定狀態。

### 外部排序

- 輸入時間受 k 值影響，k 越大，輪數越少，t_input 越小，但受限於記憶體 S（實際實作中需考慮）。

- CPU 合併時間為常數（t_CPU ≈ 某固定值），可與不同的 t_input 作比較。

- 本次模擬，程式成功找出一組 k 值與 t_input，使其接近某個合理的 CPU 時間，證明 t_CPU ≈ t_input 是有可能達成的，但不是保證永遠成立。
  
## 測試與驗證

- **MinHeap：**
  
  包含正數、負數、重複值、邊界值（INT_MAX, INT_MIN），最終輸出排序為由小到大。
  
  ![MINHEAP](https://github.com/CNMBNMSLFKU/Homework_2/blob/main/MINHEAP.png)

- **BST：**

  Height/log₂(n) 約在 1.0 左右，顯示二元樹相當接近平衡。

  ![BST](https://github.com/CNMBNMSLFKU/Homework_2/blob/main/BST.png)


- **外部排序：**
  
  - 輸入參數：
    
    n = 200000，m = 64，ts = 0.08 秒，tl = 0.02 秒，tt = 0.001 秒/record，S = 2000（題目設定但未用於模擬）。

  - 測試範圍：k = 2 到 64；t_CPU 掃描 1.0 秒到 300 秒。

  - 圖表顯示 t_input 隨 k 值變化的趨勢，並標記出最佳的 t_CPU 與對應的 k。

  ![sort](https://github.com/CNMBNMSLFKU/Homework_2/blob/main/sort.png)

  P.S. MinHeap、BST 在 Visual Studio 2019 編譯，外部排序在 colab 編譯

## 申論與開發報告

這份作業的三個主題：MinHeap、平衡 BST（Binary Search Tree）以及外部排序的模擬，主要目的是幫助我們理解：

- 如何用程式模擬資料的排序與管理

- 不同資料結構在處理大量資料時的優劣

- 實體硬碟的存取特性對演算法設計的影響

雖然這三部分看起來各自獨立，但實際上都是在處理「怎麼有效率地整理資料」。

1. MinHeap 

   這部分主要是練習用 vector 實作最小堆積結構，重點在 ``push`` 和 ``pop`` 時維持堆的性質。寫的時候最常出問題的是 parent 和 child 的 index 計算，有時候會 swap 錯邊導致順序跑掉。調整好 HeapifyUp 和 HeapifyDown 後，結果就穩定了，最後測試幾組亂數和極端值（像 INT_MIN、INT_MAX）都能正確從小到大輸出，代表整體邏輯應該沒問題。

2. BST

   一開始想用隨機插入的方式建 BST，但發現高度會飆高導致結構不平衡，後來改用「排序後中間值當根」的方式來遞迴建樹，成功讓樹維持在接近 log₂(n) 的高度。這樣的平衡樹結構能有效避免退化成 linked list 的情況，測試多組 n 值後，發現高度都在 1 log₂(n) 左右，說明這種方法很穩定。

3. 外部排序

   磁碟操作的時間成本遠大於 CPU 處理時間，故磁碟 I/O 是外部排序效能的關鍵。而選擇合適的 k 值可以減少合併輪數，降低磁碟讀取次數，進而提升效能。雖然實務上還要考慮記憶體大小 S 對 k 的限制，但本次模擬先忽略此限制以簡化模型，仍有助於理解整體概念。
  
### ❗需要改進的地方

  - MinHeap 可進一步包裝為支援複雜資料型別（如 struct with key-value），適用於更進階的合併。

  - BST 之後可以試看看 AVL 或 Red-Black Tree，自動維持平衡性，而不是靠排序後建樹。

  - 外部排序目前只模擬輸入時間與 CPU 合併，忽略了多階段合併的複雜磁碟存取。後續應加入記憶體容量對 k 值的限制，並結合實際排序演算法，以更全面地評估效能。
