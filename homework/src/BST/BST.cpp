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
