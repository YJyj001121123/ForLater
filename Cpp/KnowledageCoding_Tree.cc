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
        if (root == nullptr) return result;
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
        while (cur != nullptr || !st.empty()) {
            if (cur != nullptr) { // 指针来访问节点，访问到最底层
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
//二叉树的右视图
//输入: [1,2,3,null,5,null,4]
//输出: [1,3,4]
//思路：BFS
std::vector<int> rightSideView(TreeNode* root) {
        std::queue<TreeNode*> nodeQueue;
        if(root != nullptr) nodeQueue.push(root);
        std::vector<int> res;
        while (!nodeQueue.empty()) {
            for(int i = 0; i < nodeQueue.size(); i++){
                TreeNode* node = nodeQueue.front();
                nodeQueue.pop();
                if(i = nodeQueue.size()-1) res.push_back(node->val);
                if(node->left) nodeQueue.push(node->left);
                if(node->right) nodeQueue.push(node->right);
            }
        }
        return res;
}
//二叉树层平均值
//输入[3,9,20,null,null,15,7]
//输出[3,14.5,11]
std::vector<double> arergaeLevels(TreeNode* root){
    std::queue<TreeNode*> que;
    if(root != nullptr) que.push(root);
    std::vector<double> res;
    while(!que.empty()){
        int size = que.size();
        double sum = 0;
        for(int i = 0; i<size; i++){
            TreeNode* node = que.front();
            que.pop();
            sum += node->val;
            if(node->left) que.push(node->left);
            if(node->right) que.push(node->right);
        }
        res.push_back(sum/size);
    }
    return res;
}
//N叉树层序遍历
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::queue<TreeNode*> que;
    if(root != nullptr) que.push(root);
    std::vector<std::vector<int>> result;
    while(!que.empty())
    {
        int size = que.size();
        std::vector<int> vec;
        for(int i = 0; i < size; i++)
        {
            TreeNode* node = que.front();
            que.pop();
            vec.push_back(node->val);
            // 将节点孩子加入队列
            for(int j = 0; j < node->children.size(); j++)
            {
                if (node->children[j]) que.push(node->children[j]);
            }
        }
        result.push_back(vec);
    }
    return result;
}
//最大深度
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    int res = 0;
    std::queue<TreeNode*> q{{root}};
    while (!q.empty()) {
        ++res;
        for (int i = q.size(); i > 0; --i) {
            TreeNode *t = q.front(); q.pop();
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
    }
    return res;
}
//最小深度
int minDepth(TreeNode* root) {
    if (root == NULL) return 0;
    int m1 = minDepth(root->left);
    int m2 = minDepth(root->right);
    return root->left == NULL || root->right == NULL ? m1 + m2 + 1 : min(m1, m2) + 1;
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

//完全二叉树的节点个数
//输入：root = [1,2,3,4,5,6]
//输出：6
int getNodeSum(TreeNode* cur){
    if(cur == nullptr) return 0;
    int leftNum = getNodeSum(cur->left);
    int rightNum = getNodeSum(cur->right);
    int treeSum = leftNum+rightNum+1;
    return treeSum;
}
//平衡二叉树--一个二叉树每个节点 的左右两个子树的高度差的绝对值不超过1
//给定二叉树 [3,9,20,null,null,15,7] true
int getHeight(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    int leftHeight = getHeight(node->left);
    if (leftHeight == -1) return -1;
    int rightHeight = getHeight(node->right);
    if (rightHeight == -1) return -1;
    return abs(leftHeight - rightHeight) > 1 ? -1 : 1 + std::max(leftHeight, rightHeight);
}
bool isBalanced(TreeNode* root) {
    return getHeight(root) == -1 ? false : true;
}
//二叉树所有路径--根到叶子
// 版本一
class Solution {
private:
    void traversal(TreeNode* cur, std::vector<int>& path, std::vector<std::string>& result) {
        path.push_back(cur->val); // 中，中为什么写在这里，因为最后一个节点也要加入到path中 
        // 这才到了叶子节点
        if (cur->left == NULL && cur->right == NULL) {
            std::string sPath;
            for (int i = 0; i < path.size() - 1; i++) {
                sPath += std::to_string(path[i]);
                sPath += "->";
            }
            sPath += std::to_string(path[path.size() - 1]);
            result.push_back(sPath);
            return;
        }
        if (cur->left) { // 左 
            traversal(cur->left, path, result);
            path.pop_back(); // 回溯
        }
        if (cur->right) { // 右
            traversal(cur->right, path, result);
            path.pop_back(); // 回溯
        }
    }

public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) {
        std::vector<std::string> result;
        std::vector<int> path;
        if (root == NULL) return result;
        traversal(root, path, result);
        return result;
    }
};

//左叶子之和
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (root == NULL) return 0;
        if (root->left == NULL && root->right== NULL) return 0;

        int leftValue = sumOfLeftLeaves(root->left);    // 左
        if (root->left && !root->left->left && !root->left->right) { // 左子树就是一个左叶子的情况
            leftValue = root->left->val;
        }
        int rightValue = sumOfLeftLeaves(root->right);  // 右

        int sum = leftValue + rightValue;               // 中
        return sum;
    }
};

class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        stack<TreeNode*> st;
        if (root == NULL) return 0;
        st.push(root);
        int result = 0;
        while (!st.empty()) {
            TreeNode* node = st.top();
            st.pop();
            if (node->left != NULL && node->left->left == NULL && node->left->right == NULL) {
                result += node->left->val;
            }
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);
        }
        return result;
    }
};
//路径总和
class Solution {
private:
    bool traversal(TreeNode* cur, int count) {
        if (!cur->left && !cur->right && count == 0) return true; // 遇到叶子节点，并且计数为0
        if (!cur->left && !cur->right) return false; // 遇到叶子节点直接返回

        if (cur->left) { // 左
            count -= cur->left->val; // 递归，处理节点;
            if (traversal(cur->left, count)) return true;
            count += cur->left->val; // 回溯，撤销处理结果
        }
        if (cur->right) { // 右
            count -= cur->right->val; // 递归，处理节点;
            if (traversal(cur->right, count)) return true;
            count += cur->right->val; // 回溯，撤销处理结果
        }
        return false;
    }

public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (root == NULL) return false;
        return traversal(root, sum - root->val);
    }
};

//从中序与后序遍历序列构造二叉树
class Solution {
private:
    // 中序区间：[inorderBegin, inorderEnd)，后序区间[postorderBegin, postorderEnd)
    TreeNode* traversal (vector<int>& inorder, int inorderBegin, int inorderEnd, vector<int>& postorder, int postorderBegin, int postorderEnd) {
        if (postorderBegin == postorderEnd) return NULL;

        int rootValue = postorder[postorderEnd - 1];
        TreeNode* root = new TreeNode(rootValue);

        if (postorderEnd - postorderBegin == 1) return root;

        int delimiterIndex;
        for (delimiterIndex = inorderBegin; delimiterIndex < inorderEnd; delimiterIndex++) {
            if (inorder[delimiterIndex] == rootValue) break;
        }
        // 切割中序数组
        // 左中序区间，左闭右开[leftInorderBegin, leftInorderEnd)
        int leftInorderBegin = inorderBegin;
        int leftInorderEnd = delimiterIndex;
        // 右中序区间，左闭右开[rightInorderBegin, rightInorderEnd)
        int rightInorderBegin = delimiterIndex + 1;
        int rightInorderEnd = inorderEnd;

        // 切割后序数组
        // 左后序区间，左闭右开[leftPostorderBegin, leftPostorderEnd)
        int leftPostorderBegin =  postorderBegin;
        int leftPostorderEnd = postorderBegin + delimiterIndex - inorderBegin; // 终止位置是 需要加上 中序区间的大小size
        // 右后序区间，左闭右开[rightPostorderBegin, rightPostorderEnd)
        int rightPostorderBegin = postorderBegin + (delimiterIndex - inorderBegin);
        int rightPostorderEnd = postorderEnd - 1; // 排除最后一个元素，已经作为节点了

        root->left = traversal(inorder, leftInorderBegin, leftInorderEnd,  postorder, leftPostorderBegin, leftPostorderEnd);
        root->right = traversal(inorder, rightInorderBegin, rightInorderEnd, postorder, rightPostorderBegin, rightPostorderEnd);

        return root;
    }
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (inorder.size() == 0 || postorder.size() == 0) return NULL;
        // 左闭右开的原则
        return traversal(inorder, 0, inorder.size(), postorder, 0, postorder.size());
    }
};
//从前序与中序遍历序列构造二叉树
class Solution {
private:
        TreeNode* traversal (vector<int>& inorder, int inorderBegin, int inorderEnd, vector<int>& preorder, int preorderBegin, int preorderEnd) {
        if (preorderBegin == preorderEnd) return NULL;

        int rootValue = preorder[preorderBegin]; // 注意用preorderBegin 不要用0
        TreeNode* root = new TreeNode(rootValue);

        if (preorderEnd - preorderBegin == 1) return root;

        int delimiterIndex;
        for (delimiterIndex = inorderBegin; delimiterIndex < inorderEnd; delimiterIndex++) {
            if (inorder[delimiterIndex] == rootValue) break;
        }
        // 切割中序数组
        // 中序左区间，左闭右开[leftInorderBegin, leftInorderEnd)
        int leftInorderBegin = inorderBegin;
        int leftInorderEnd = delimiterIndex;
        // 中序右区间，左闭右开[rightInorderBegin, rightInorderEnd)
        int rightInorderBegin = delimiterIndex + 1;
        int rightInorderEnd = inorderEnd;

        // 切割前序数组
        // 前序左区间，左闭右开[leftPreorderBegin, leftPreorderEnd)
        int leftPreorderBegin =  preorderBegin + 1;
        int leftPreorderEnd = preorderBegin + 1 + delimiterIndex - inorderBegin; // 终止位置是起始位置加上中序左区间的大小size
        // 前序右区间, 左闭右开[rightPreorderBegin, rightPreorderEnd)
        int rightPreorderBegin = preorderBegin + 1 + (delimiterIndex - inorderBegin);
        int rightPreorderEnd = preorderEnd;

        root->left = traversal(inorder, leftInorderBegin, leftInorderEnd,  preorder, leftPreorderBegin, leftPreorderEnd);
        root->right = traversal(inorder, rightInorderBegin, rightInorderEnd, preorder, rightPreorderBegin, rightPreorderEnd);

        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (inorder.size() == 0 || preorder.size() == 0) return NULL;

        // 参数坚持左闭右开的原则
        return traversal(inorder, 0, inorder.size(), preorder, 0, preorder.size());
    }
};
//最大二叉树
//二叉树的根是数组中的最大元素。
//左子树是通过数组中最大值左边部分构造出的最大二叉树。
//右子树是通过数组中最大值右边部分构造出的最大二叉树。
class Solution {
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        TreeNode* node = new TreeNode(0);
        if (nums.size() == 1) {
            node->val = nums[0];
            return node;
        }
        // 找到数组中最大的值和对应的下标
        int maxValue = 0;
        int maxValueIndex = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] > maxValue) {
                maxValue = nums[i];
                maxValueIndex = i;
            }
        }
        node->val = maxValue;
        // 最大值所在的下标左区间 构造左子树
        if (maxValueIndex > 0) {
            vector<int> newVec(nums.begin(), nums.begin() + maxValueIndex);
            node->left = constructMaximumBinaryTree(newVec);
        }
        // 最大值所在的下标右区间 构造右子树
        if (maxValueIndex < (nums.size() - 1)) {
            vector<int> newVec(nums.begin() + maxValueIndex + 1, nums.end());
            node->right = constructMaximumBinaryTree(newVec);
        }
        return node;
    }
};
//合并二叉树
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        if (t1 == NULL) return t2; // 如果t1为空，合并之后就应该是t2
        if (t2 == NULL) return t1; // 如果t2为空，合并之后就应该是t1
        // 修改了t1的数值和结构
        t1->val += t2->val;                             // 中
        t1->left = mergeTrees(t1->left, t2->left);      // 左
        t1->right = mergeTrees(t1->right, t2->right);   // 右
        return t1;
    }
};