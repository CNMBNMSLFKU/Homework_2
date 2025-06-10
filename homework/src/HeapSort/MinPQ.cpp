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
