//
// Created by W on 2019/2/19 0019.
//
// 面试题7：重建二叉树
// 题目：输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输
// 入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,
// 2, 4, 7, 3, 5, 6, 8}和中序遍历序列{4, 7, 2, 1, 5, 3, 8, 6}，则重建出
// 图2.6所示的二叉树并输出它的头结点。

#include <iostream>
#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>
#include "Utilities\BinaryTree.h"
#include <exception>
using namespace std;

// 超简洁写法，置顶========================================推荐，推荐==============================
BinaryTreeNode* Construct(int* preorder, int startPre, int endPre, int* inorder, int startIn, int endIn) {
    if(startPre > endPre || startIn > endIn)
        return nullptr;
    BinaryTreeNode* root = new BinaryTreeNode(preorder[startPre]);
    for(int i = startIn; i <= endIn; i++){
        if(inorder[i] == preorder[startPre]) {
            root->m_pLeft = Construct(preorder, startPre + 1, startPre + i - startIn, inorder, startIn, i - 1);
            root->m_pRight = Construct(preorder, startPre + i - startIn + 1, endPre, inorder, i + 1, endIn);
            break;
        }
    }
    return root;
}

BinaryTreeNode* Construct(int* preorder, int* inorder, int length) {
    BinaryTreeNode* root = Construct(preorder, 0, length - 1, inorder, 0, length - 1);
    return root;
}

class Solution {
public:
    BinaryTreeNode* reConstructBinaryTree(vector<int> pre, vector<int> vin){
        if(pre.empty() || vin.empty() || pre.size() != vin.size())
            return nullptr;
        int length = pre.size();
        if(length <= 0)
            return nullptr;
        return ConstructCore(pre.begin(), --pre.end(), vin.begin(), --vin.end());
    }

    BinaryTreeNode* ConstructCore(vector<int>::iterator startPreorder, vector<int>::iterator endPreorder,
                                  vector<int>::iterator startInorder, vector<int>::iterator endInorder){
        int rootValue = *startPreorder;
        auto *root = new BinaryTreeNode(rootValue);

        if(startPreorder == endPreorder) {
            if(startInorder == endInorder && *startPreorder == *startInorder)
                return root;
            else
                return nullptr;
        }

        // 在中序遍历中找到根结点的值
        auto rootInorder = startInorder;
        while(rootInorder <= endInorder && *rootInorder != rootValue)
            ++ rootInorder;

        if(rootInorder == endInorder && *rootInorder != rootValue)
            return nullptr;

        int leftLength = int(rootInorder - startInorder);
        auto leftPreorderEnd = startPreorder + leftLength;
        if(leftLength > 0) {
            // 构建左子树
            root->m_pLeft = ConstructCore(startPreorder + 1, leftPreorderEnd,
                                          startInorder, rootInorder - 1);
        }
        if(leftLength < endPreorder - startPreorder) {
            // 构建右子树
            root->m_pRight = ConstructCore(leftPreorderEnd + 1, endPreorder,
                                           rootInorder + 1, endInorder);
        }

        return root;

    }

};

BinaryTreeNode* ConstructCore(int* startPreorder, int* endPreorder, int* startInorder, int* endInorder);

BinaryTreeNode* Construct(int* preorder, int* inorder, int length) {
    if(preorder == nullptr || inorder == nullptr || length <= 0)
        return nullptr;

    return ConstructCore(preorder, preorder + length - 1,
                         inorder, inorder + length - 1);
}

BinaryTreeNode* ConstructCore(int* startPreorder, int* endPreorder, int* startInorder, int* endInorder) {
    // 前序遍历序列的第一个数字是根结点的值
    int rootValue = startPreorder[0];
    BinaryTreeNode* root = new BinaryTreeNode();
    root->m_nValue = rootValue;
    root->m_pLeft = root->m_pRight = nullptr;

    if(startPreorder == endPreorder) {
        if(startInorder == endInorder && *startPreorder == *startInorder)
            return root;
        else
            throw string("Invalid input.");
    }

    // 在中序遍历中找到根结点的值
    int* rootInorder = startInorder;
    while(rootInorder <= endInorder && *rootInorder != rootValue)
        ++ rootInorder;

    if(rootInorder == endInorder && *rootInorder != rootValue)
        throw string("Invalid input.");

    int leftLength = int(rootInorder - startInorder);
    int* leftPreorderEnd = startPreorder + leftLength;
    if(leftLength > 0) {
        // 构建左子树
        root->m_pLeft = ConstructCore(startPreorder + 1, leftPreorderEnd,
                                      startInorder, rootInorder - 1);
    }
    if(leftLength < endPreorder - startPreorder) {
        // 构建右子树
        root->m_pRight = ConstructCore(leftPreorderEnd + 1, endPreorder,
                                       rootInorder + 1, endInorder);
    }

    return root;
}

// ====================测试代码====================
void Test(char* testName, int* preorder, int* inorder, int length) {
    if(testName != nullptr)
        printf("%s begins:\n", testName);

    printf("The preorder sequence is: ");
    for(int i = 0; i < length; ++ i)
        printf("%d ", preorder[i]);
    printf("\n");

    printf("The inorder sequence is: ");
    for(int i = 0; i < length; ++ i)
        printf("%d ", inorder[i]);
    printf("\n");

    try {
        BinaryTreeNode* root = Construct(preorder, inorder, length);
        PrintTree(root);

        DestroyTree(root);
    }
    catch(std::exception& exception) {
        printf("Invalid Input.\n");
    }
}

// 普通二叉树
//              1
//           /     \
//          2       3
//         /       / \
//        4       5   6
//         \         /
//          7       8
void Test1()
{
    const int length = 8;
    int preorder[length] = {1, 2, 4, 7, 3, 5, 6, 8};
    int inorder[length] = {4, 7, 2, 1, 5, 3, 8, 6};

    Test("Test1", preorder, inorder, length);
}

// 所有结点都没有右子结点
//            1
//           /
//          2
//         /
//        3
//       /
//      4
//     /
//    5
void Test2()
{
    const int length = 5;
    int preorder[length] = {1, 2, 3, 4, 5};
    int inorder[length] = {5, 4, 3, 2, 1};

    Test("Test2", preorder, inorder, length);
}

// 所有结点都没有左子结点
//            1
//             \
//              2
//               \
//                3
//                 \
//                  4
//                   \
//                    5
void Test3()
{
    const int length = 5;
    int preorder[length] = {1, 2, 3, 4, 5};
    int inorder[length] = {1, 2, 3, 4, 5};

    Test("Test3", preorder, inorder, length);
}

// 树中只有一个结点
void Test4()
{
    const int length = 1;
    int preorder[length] = {1};
    int inorder[length] = {1};

    Test("Test4", preorder, inorder, length);
}

// 完全二叉树
//              1
//           /     \
//          2       3
//         / \     / \
//        4   5   6   7
void Test5()
{
    const int length = 7;
    int preorder[length] = {1, 2, 4, 5, 3, 6, 7};
    int inorder[length] = {4, 2, 5, 1, 6, 3, 7};

    Test("Test5", preorder, inorder, length);
}

// 输入空指针
void Test6()
{
    Test("Test6", nullptr, nullptr, 0);
}

// 输入的两个序列不匹配
void Test7()
{
    const int length = 7;
    int preorder[length] = {1, 2, 4, 5, 3, 6, 7};
    int inorder[length] = {4, 2, 8, 1, 6, 3, 7};

    Test("Test7: for unmatched input", preorder, inorder, length);
}


int main(int argc, char* argv[])
{
    //Test1();
/*    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7();*/

    Solution solver;
    vector<int> preorder = {1, 2, 4, 7, 3, 5, 6, 8};
    vector<int> inorder = {4, 7, 2, 1, 5, 3, 8, 6};

    BinaryTreeNode* root = solver.reConstructBinaryTree(preorder, inorder);
    PrintTree(root);

    return 0;
}

