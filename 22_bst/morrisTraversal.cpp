#include <iostream>
using namespace std;
class TreeNode {
public:
    int val ;
TreeNode* left;
TreeNode* right;

};
void Morris(TreeNode* root){
TreeNode* curr=root;
while(!curr){
if(!curr->left){
    TreeNode* pre=curr->left;
    while(pre->right && pre->right!=curr){
        pre=pre->right;
        if(!pre->right){
            pre->right=curr;
            curr=curr->left;
        }
        if(pre->right==curr){
            pre->right=NULL;
 cout<<curr->val<<" ";
    curr=curr->right;
        }
    }

}
else{
    cout<<curr->val<<" ";
    curr=curr->right;

}



}
}
int main() {
    
    return 0;
}
