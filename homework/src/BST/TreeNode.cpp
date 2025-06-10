// 定義 BST 節點結構
struct TreeNode {
    int val;              // 節點的值
    TreeNode* left;       // 左子樹指標
    TreeNode* right;      // 右子樹指標
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}  // 建構子
};
