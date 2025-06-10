#include <iostream>
#include <vector>
#include <stdexcept>

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
