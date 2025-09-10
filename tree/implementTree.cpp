#include<iostream>
using namespace std;
class Node{
public:
int val;
Node* left;
Node* right;
Node(int val) 
{
this->val=val;
left=NULL;
right=NULL;
}
};

// dfs depth first search 
void displayTree_Pre(Node *root){
  if(root==NULL)return ;
cout<<root->val<<"->";
displayTree_Pre(root->left);
displayTree_Pre(root->right);  
}
void displayTree_In(Node *root){
  if(root==NULL)return ;
  displayTree_In(root->left);
  cout<<root->val<<"->";
displayTree_In(root->right);  
}
void displayTree_Post(Node *root){
  if(root==NULL)return ;
  displayTree_Post(root->left);
  displayTree_Post(root->right);  
  cout<<root->val<<"->";
}

//bfs 
// print all the node  of level 3 
void display_level(Node* root, int level=0,int n =3){
    if(root==NULL)return ;

    if(level==n){cout<<root->val<<"->";return;}
    display_level(root->left,level+1,n);
  display_level(root->right,level+1,n);  
}
int levelTree(Node *root ){
    if(root==NULL)return INT16_MIN ;
    int levels ;
    levels= 1+ max(levelTree(root->left),levelTree(root->right)) ;
    
    return levels;
}

void display_level_R2L(Node* root, int level=0,int n =3){
    if(root==NULL)return ;

    if(level==n){cout<<root->val<<"->";return;}
    display_level_R2L(root->right,level+1,n);  
    display_level_R2L(root->left,level+1,n);
}
//bfs printing 
void bfs(Node* root){
    for(int i =1;i<=levelTree(root);i++){
          display_level(root->left,1,i);
          cout<<endl;
    }
}

void bfsR2L(Node* root){

}

int main(){

Node* A=new Node(1);
Node* B=new Node(2);
Node* C=new Node(3);
Node* D=new Node(4);
Node* E=new Node(5);
Node* F=new Node(6);
Node* G=new Node(7);


A->left=B;
A->right=C;
B->left=D;
B->right=E;
C->left=F;
C->right=G;





}