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
