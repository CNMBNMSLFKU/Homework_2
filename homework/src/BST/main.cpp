#include <iostream>
#include <vector>
#include <algorithm> // sort, max
#include <cmath>     // log2
#include <cstdlib>   // rand
#include <ctime>     // time

using namespace std;

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
