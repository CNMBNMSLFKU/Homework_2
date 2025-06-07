# 41243206 廖珮岑 41243209 謝詠晴

作業二

## 解題說明

本次作業主要包含三個部分：

- 實作最小堆積（MinHeap）：基於介面 MinPQ 設計並使用 C++ vector 作為內部容器，完成最小堆的插入與刪除操作。

- 實作並測試二元搜尋樹（BST）：支援插入與刪除節點，並能量測其樹高與理論高度比值，進行效能分析。

- 計算外部排序的 I/O 時間估算：根據磁碟存取模型推導總讀取時間。

### 解題策略

- **MinHeap**

  使用陣列實作堆，並維護其最小堆性質。

  實作 HeapifyUp（插入後）與 HeapifyDown（刪除後）兩個方法以保持堆的結構正確。

  提供介面如 Push、Pop、Top 與 IsEmpty，並繼承自抽象基底類別 MinPQ，符合物件導向原則。


- **BST**

  使用遞迴實作插入與刪除節點。

  插入時比較值以決定左右子樹。

  刪除節點時，依據左右子樹存在情況採用對應策略，若兩者皆存在，使用中序繼承者（右子樹最小值）取代。

  為分析其效能，實作 getHeight 函式遞迴計算樹高。

- **外部排序**

  依據磁碟存取模型計算：

  t_input = m × (ts + tl) + n × tt

  其中：

  ts 為 seek time

  tl 為 latency time

  tt 為單筆資料傳輸時間

  m 為 run 數

  n 為資料筆數


## 程式實作

### MinHeap
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& item) = 0;
    virtual void Pop() = 0;
};

// MinHeap implementation using vector
template <class T>
class MinHeap : public MinPQ<T> {
private:
    std::vector<T> heap;

    void HeapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    void HeapifyDown(int index) {
        int size = heap.size();
        while (index * 2 + 1 < size) {
            int child = index * 2 + 1; // left child
            if (child + 1 < size && heap[child + 1] < heap[child])
                child++; // right child is smaller
            if (heap[child] < heap[index]) {
                std::swap(heap[child], heap[index]);
                index = child;
            }
            else break;
        }
    }

public:
    bool IsEmpty() const override {
        return heap.empty();
    }

    const T& Top() const override {
        if (IsEmpty())
            throw std::runtime_error("Heap is empty.");
        return heap[0];
    }

    void Push(const T& item) override {
        heap.push_back(item);
        HeapifyUp(heap.size() - 1);
    }

    void Pop() override {
        if (IsEmpty())
            throw std::runtime_error("Heap is empty.");
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) HeapifyDown(0);
    }
};

// Test
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


    while (!h.IsEmpty()) {
        std::cout << h.Top() << " ";
        h.Pop();
    }
    return 0;
}
```

## BST
```cpp

```

