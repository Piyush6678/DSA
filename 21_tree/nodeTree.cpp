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


int sumOfTree(Node *root ){
  if(root==NULL)return 0 ;
  int sum=root->val ;
sum+=  sumOfTree(root->left);
  sum+=sumOfTree(root->right);  
 return sum;
}
// number of nodes
int sizeOfTree(Node *root ){
  if(root==NULL)return 0 ;
  int size=1 ;
size+=  sizeOfTree(root->left);
  size+=sizeOfTree(root->right);  
 return size;
}

//find the maximum value node
int max_value_node(Node *root ){
  if(root==NULL)return INT16_MIN ;
  int max_val=root->val ;
max_val= max(max_value_node(root->left),max_val) ;
  max_val=max(max_value_node(root->right),max_val);  
 return max_val;
}
//find the levels of binary tree 
int levelTree(Node *root ){
  if(root==NULL)return INT16_MIN ;
  int levels ;
levels= 1+ max(levelTree(root->left),levelTree(root->right)) ;
   
 return levels;
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
//pre order

cout<<endl<<"sum of tree"<<endl;
cout<<sumOfTree(A);
cout<<endl<<"size of tree"<<endl;
cout<<sizeOfTree(A);
cout<<endl<<"max value of node"<<endl;
cout<<max_value_node(A);



}