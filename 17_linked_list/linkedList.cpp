#include <iostream>
using namespace std;
class Node { //User defined Data Type 
public:
int val ;
Node * next;
Node(int val ){
    this->val=val;
    this->next=NULL;
}
};
class LinkedList{  //User defined Data Structure
    protected:
Node* head;
Node* tail;

public:
int size;
LinkedList(){
    head=tail=NULL;
    size=0;
}

void display(){
    Node* temp =head;
    while(temp!=NULL){
        cout<<temp->val<<" ";
        temp=temp->next;
    }
}
    // insert at the end 
void insertAtEnd(int val ){
    Node *temp =new Node(val);
    if(size==0){
        head=tail=temp;
        
    }
    else{
        tail->next=temp;
        tail=temp;
        
    }
    size++;
}

void deleteAtENd(){
    if(size==0)return ;
Node* temp =head;
while(temp!=NULL){
    if(temp->next==tail){
        temp->next=NULL;
        tail=temp;
        size--;
        break;
    }
    temp=temp->next;
}    
}

    







};
int main() {
    LinkedList ll;
    ll.insertAtEnd(10);
    ll.insertAtEnd(20); 
    ll.insertAtEnd(30); 
    ll.insertAtEnd(40); 
    ll.display(); //10 20 30 40 
    ll.deleteAtENd();
    ll.display(); //10 20 30  
    return 0;
}