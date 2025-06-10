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
