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