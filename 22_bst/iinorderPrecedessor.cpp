#include <iostream>
using namespace std;
class TreeNode{
    public:
    int val;
    TreeNode* left;
    TreeNode*right;
};
int pred(TreeNode* root){
    if (!root->left)return -1;
TreeNode* pred=root->left;
while(pred->right){
    pred=pred->right;
}
return pred->val;


}
int successor(TreeNode* root){
    if (!root->right)return -1;
TreeNode* succ=root->right;
while(succ->left){
    succ=succ->left;
}
return succ->val;


}

int main() {
    
    return 0;
}