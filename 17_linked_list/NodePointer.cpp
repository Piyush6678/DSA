#include <iostream>
using namespace std;
//node
class Node{

    public:
int val;
Node* next;
Node (int val){
    this->val=val;
    this->next=NULL;
}

};
void display(Node* head){
//printing linked list 
Node* temp =head;
while(temp!=NULL){
    cout<<temp->val<<"  ";
    temp=temp->next;

}
}
void recDisplay(Node* head){
//printing linked list recursively 
if (head==NULL)return ;
cout<<head->val<<" ";
recDisplay(head->next);
}
void revDisplay(Node* head){
//printing linked list reverse 
if (head==NULL)return ;
recDisplay(head->next);
cout<<head->val<<" ";
}

int size(Node* head){
    //size of a linked list
    Node* temp =head;
    
    int count=0;
    while(temp!=NULL){
        count++;
        temp=temp->next;
    
    }return count;

}
int main() {
Node* a=new Node(10);
Node* b=new Node(20);
Node* c=new Node(30);
Node* d=new Node(40);

a->next=b;
b->next=c;
c->next=d;
display(a); 
cout<<size(a);
recDisplay(a);
    return 0;
}