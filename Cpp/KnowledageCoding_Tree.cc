#include <vector>
#include <stack>
/***
 * 二叉树
 * 1、遍历方式
 * 2、属性
 * 3、修改构造
 * 4、二叉搜索树
 * 5、二叉搜索树修改改造
 * 6、公共祖先
 * 
 * 
 * 满二叉树
 * 如果一棵二叉树只有度为0的结点和度为2的结点，并且度为0的结点在同一层上，则这棵二叉树为满二叉树
 * 深度为k，有2^k-1个节点的二叉树
 * 
 * 完全二叉树
 * 除了最底层节点可能没填满外，其余每层节点数都达到最大值，并且最下面一层的节点都集中在该层最左边的若干位置。
 * 若最底层为第 h 层（h从1开始），则该层包含 1~ 2^(h-1) 个节点
 * 
 * 二叉搜索树
 * 二叉搜索树是一个有序树
 * 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
 * 若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
 * 它的左、右子树也分别为二叉排序树
 * 
 * 平衡二叉搜索树
 * 它是一棵空树或它的左右两个子树的高度差的绝对值不超过1，并且左右两个子树都是一棵平衡二叉树
 * 
 * 二叉树可以链式存储，也可以顺序存储
 * 链式存储方式就用指针， 顺序存储的方式就是用数组
 * 用数组来存储二叉树如何遍历的呢？
 * 如果父节点的数组下标是 i，那么它的左孩子就是 i * 2 + 1，右孩子就是 i * 2 + 2
 * 
 * 遍历
 * 深度优先遍历：
 * 先往深走，遇到叶子节点再往回走。
 * 前序遍历（递归法，迭代法）
 * 中序遍历（递归法，迭代法）
 * 后序遍历（递归法，迭代法）
 * 前序遍历--根左右 中序--左根右 后序--左右根
 * 广度优先遍历：
 * 一层一层的去遍历。
 * 层次遍历（迭代法）
 * 
*/
#include <queue>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
// 每次写递归，都按照这三要素来写，可以保证大家写出正确的递归算法！
// 确定递归函数的参数和返回值： 确定哪些参数是递归的过程中需要处理的，那么就在递归函数里加上这个参数， 并且还要明确每次递归的返回值是什么进而确定递归函数的返回类型。
void traversal(TreeNode* cur, std::vector<int>& vec)
// 确定终止条件： 写完了递归算法, 运行的时候，经常会遇到栈溢出的错误，就是没写终止条件或者终止条件写的不对，操作系统也是用一个栈的结构来保存每一层递归的信息，如果递归没有终止，操作系统的内存栈必然就会溢出。
if (cur == NULL) return;
// 确定单层递归的逻辑： 确定每一层递归需要处理的信息。在这里也就会重复调用自己来实现递归的过程。
vec.push_back(cur->val);    // 中
traversal(cur->left, vec);  // 左
traversal(cur->right, vec); // 右

//前序遍历
void traversal(TreeNode* cur, std::vector<int>& vec){
    if(cur == nullptr) return;
    vec.emplace_back(cur->val);
    traversal(cur->left, vec); 
    traversal(cur->right, vec); 
}
std::vector<int> preorderTraversal2(TreeNode* root) {
        std::stack<TreeNode*> st;
        std::vector<int> result;
        if (root == NULL) return result;
        st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top();                       // 中
            st.pop();
            result.push_back(node->val);
            if (node->right) st.push(node->right);           // 右（空节点不入栈）
            if (node->left) st.push(node->left);             // 左（空节点不入栈）
        }
        return result;
    }
//中序遍历
void traversal2(TreeNode* cur, std::vector<int>& vec){
    if(cur == nullptr) return;
    traversal(cur->left, vec); 
    vec.emplace_back(cur->val);
    traversal(cur->right, vec); 
}
std::vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> result;
        std::stack<TreeNode*> st;
        TreeNode* cur = root;
        while (cur != NULL || !st.empty()) {
            if (cur != NULL) { // 指针来访问节点，访问到最底层
                st.push(cur); // 将访问的节点放进栈
                cur = cur->left;                // 左
            } else {
                cur = st.top(); // 从栈里弹出的数据，就是要处理的数据（放进result数组里的数据）
                st.pop();
                result.push_back(cur->val);     // 中
                cur = cur->right;               // 右
            }
        }
        return result;
    }
//后序遍历
void traversal2(TreeNode* cur, std::vector<int>& vec){
    if(cur == nullptr) return;
    traversal(cur->left, vec); 
    traversal(cur->right, vec); 
    vec.emplace_back(cur->val);
}
std::vector<int> postorderTraversal(TreeNode* root) {
        std::stack<TreeNode*> st;
        std::vector<int> result;
        if (root == NULL) return result;
        st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top();
            st.pop();
            result.push_back(node->val);
            if (node->left) st.push(node->left); // 相对于前序遍历，这更改一下入栈顺序 （空节点不入栈）
            if (node->right) st.push(node->right); // 空节点不入栈
        }
        std::reverse(result.begin(), result.end()); // 将结果反转之后就是左右中的顺序了
        return result;
}


std::vector<int> preorderTraversal(TreeNode* root){
    std::vector<int> res;
    traversal(root, res); 
    return res;
}

//层序遍历
//输入：[3,9.20,null,null.,15,7]
//输出：[ [3], [9,20] [15,7]]
//思路：借用一个辅助数据结构即队列来实现，队列先进先出，符合一层一层遍历的逻辑，
//而用栈先进后出适合模拟深度优先遍历也就是递归的逻辑
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::queue<TreeNode*> que;
    if (root != nullptr) que.push(root);
    std::vector<std::vector<int>> result;
    while (!que.empty()) {
        int size = que.size();
        std::vector<int> vec;
        // 这里一定要使用固定大小size，不要使用que.size()，因为que.size是不断变化的
        for (int i = 0; i < size; i++) {
            TreeNode* node = que.front();
            que.pop();
            vec.push_back(node->val);
            if (node->left) que.push(node->left);
            if (node->right) que.push(node->right);
        }
        result.push_back(vec);
    }
    return result;
}
void order(TreeNode* cur, std::vector<std::vector<int>>& result, int depth){
    if (cur == nullptr) return;
    if (result.size() == depth) result.push_back(std::vector<int>());
    result[depth].push_back(cur->val);
    order(cur->left, result, depth + 1);
    order(cur->right, result, depth + 1);
}
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> result;
    int depth = 0;
    order(root, result, depth);
    return result;
}

//翻转二叉树
TreeNode* invertTree(TreeNode* root) {
    if (root == NULL) return root;
    std::swap(root->left, root->right);  // 中
    invertTree(root->left);         // 左
    invertTree(root->right);        // 右
    return root;
}
TreeNode* invertTree2(TreeNode* root) {
    std::stack<TreeNode*> st;
    if (root != NULL) st.push(root);
    while (!st.empty()) {
        TreeNode* node = st.top();
        if (node != NULL) {
            st.pop();
            if (node->right) st.push(node->right);  // 右
            if (node->left) st.push(node->left);    // 左
            st.push(node);                          // 中
            st.push(NULL);
        } else {
            st.pop();
            node = st.top();
            st.pop();
            std::swap(node->left, node->right);          // 节点处理逻辑
        }
    }
    return root;
}
TreeNode* invertTree3(TreeNode* root) {
    std::queue<TreeNode*> que;
    if (root != NULL) que.push(root);
    while (!que.empty()) {
        int size = que.size();
        for (int i = 0; i < size; i++) {
            TreeNode* node = que.front();
            que.pop();
            std::swap(node->left, node->right); // 节点处理
            if (node->left) que.push(node->left);
            if (node->right) que.push(node->right);
        }
    }
    return root;
}
//对称二叉树
//思路：判断对称二叉树要比较的是哪两个节点，要比较的可不是左右节点！
//要比较的是两个树（这两个树是根节点的左右子树）
bool compare(TreeNode* left, TreeNode* right) {
    // 首先排除空节点的情况
    if (left == NULL && right != NULL) return false;
    else if (left != NULL && right == NULL) return false;
    else if (left == NULL && right == NULL) return true;
    // 排除了空节点，再排除数值不相同的情况
    else if (left->val != right->val) return false;

    // 此时就是：左右节点都不为空，且数值相同的情况
    // 此时才做递归，做下一层的判断
    bool outside = compare(left->left, right->right);   // 左子树：左、 右子树：右
    bool inside = compare(left->right, right->left);    // 左子树：右、 右子树：左
    bool isSame = outside && inside;                    // 左子树：中、 右子树：中 （逻辑处理）
    return isSame;

}
bool isSymmetric(TreeNode* root) {
    if (root == NULL) return true;
    return compare(root->left, root->right);
}