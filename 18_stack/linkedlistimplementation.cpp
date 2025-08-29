#include<iostream>
using namespace std;
class Node{
    public:
int val;
Node* next;
Node(int val ){
    this->val=val;this->next=NULL;
}
};
class Stack{
    Node* head;
    int size;
    Stack(){
        head=NULL;size=0;
    }
void push(int val){
    Node * temp=new Node(val);
    temp->next=head;
    head=temp;
    size++;

}
void pop(){
    if(head==NULL) {
        cout<<"stack under flow";return;
    }
head=head->next;
size--;
}
int top(){
    return head->val;
}
void display(){
     Node * temp=head;
     while(temp){
        cout<<temp->val<<" ";
        temp=temp->next;
     }
}
void display_rec(Node *head){
     if(head)return;
    display_rec(head->next);
    cout<<head->val<<" ";
     
}

};