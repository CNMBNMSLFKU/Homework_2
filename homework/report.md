# 41243206 41243209 

作業二

## 解題說明

本次作業主要包含三個部分：

- 最小堆積（MinHeap）：基於介面 ``MinPQ`` 設計並使用 C++ vector 作為內部容器，完成最小堆積的插入與刪除。

- 二元搜尋樹（BST）：針對隨機資料建立平衡 BST，並分析其高度與理論最小高度 log₂(n) 的比值。

- 外部排序時間估算：根據磁碟 I/O 模型計算大規模資料輸入所需時間。

### 解題策略

**1. MinHeap**

  - 使用 vector 動態陣列實作，並維護其最小堆積性質。

  - 實作 HeapifyUp（插入後）與 HeapifyDown（刪除後）兩個方法以保持堆的結構正確。

  - 提供介面如 Push、Pop、Top 與 IsEmpty，並繼承自抽象基底類別 MinPQ，符合物件導向原則。


**2. BST**

  - 使用亂數產生資料，先排序再遞迴以中間分割法建立 平衡 BST。

  - 避免傳統一筆一筆插入造成偏斜樹，使得高度更接近理論值 log₂(n)。

  - 藉由計算實際高度與理論值的比值，分析平衡性與效率。


**3. 外部排序**

  - 模擬外部排序第一階段（Initial Run 輸入階段）的 I/O 耗時情況。
    
  - 採用靜態參數模擬真實磁碟特性。
    
  - 依據磁碟存取模型計算：

    ``t_input = m × (ts + tl) + n × tt``

    其中：

    ``ts`` : seek time

    ``tl`` : latency time

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
```cpp
#include <iostream>
using namespace std;

int main() {
    // 題目給定參數設定（可依需求調整）
    double ts = 0.08;   // 磁碟 seek time（秒）
    double tl = 0.02;   // 磁碟 latency time（秒）
    double tt = 0.001;  // 每筆資料的傳輸時間（秒）

    int n = 200000;     // 總資料筆數
    int m = 64;         // 初始的 run 數（磁碟區塊數）
    int S = 2000;       // 可容納筆數的記憶體大小（這邊沒用到，但可擴充使用）

    /*
        外部排序輸入階段的 I/O 模型公式：
        t_input = m × (ts + tl) + n × tt

        - m × (ts + tl)：模擬每個 run 做一次磁碟 seek 和 latency
        - n × tt：所有資料傳輸到記憶體所花的時間
    */
    double t_input = m * (ts + tl) + n * tt;

    // 輸出總 I/O 時間
    cout << "Total input time (t_input): " << t_input << " seconds" << endl;

    return 0;
}
```

## 效能分析

### MinHeap

- 時間複雜度：

  1. ``Push`` 與 ``Pop`` 為 O(log n)。

  2. ``Top`` 與 ``IsEmpty`` 為 O(1)。

- 實作符合最小堆邏輯，測試輸出由小至大。

  ![MINHEAP]()

### BST

- 理論最佳高度為 log₂(n)，透過排序+中序建樹方式控制其為平衡狀態。

- 實驗輸出：

  Height/log₂(n) 約在 1.0 ~ 1.2 間，顯示樹相當接近平衡。

  ![BST]()

### 外部排序

- 輸入階段耗時主要來自 seek+latency 成本，尤其當 run 數（m）變多時。

- 模型可進一步擴充至計算多階段 merge 時間，用於分析多次外部排序。

- 輸出結果
  ![外部排序]()
