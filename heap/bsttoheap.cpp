#include <iostream>
#include <vector>
using namespace std;

class Node{
public:
    int val;
Node* r;
Node* l;
Node(int val){
    this->val=val;
}
};
void inOrder(Node* root,vector<int>&dec){
if(!root)return ;

inOrder(root->r,dec);
dec.push_back(root->val);
inOrder(root->l,dec);
}
void preOrder(Node* root,int& i ,vector<int>&arr){
if(!root)return ;
root->val=arr[i++];
preOrder(root->r,i,arr);

preOrder(root->l,i,arr);
}
void bst2maxheap(Node* root){
vector<int>ans;
inOrder(root,ans);
int i =0;
preOrder(root,i,ans);
}
int main() {
    
    return 0;
}