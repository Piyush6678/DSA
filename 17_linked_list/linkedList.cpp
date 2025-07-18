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
    }cout<<endl; 
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
//Insert at head 
void insertAtHead(int val ){
    Node *temp =new Node(val);
    if(size==0){
        head=tail=temp;
        
    }
    else{
        temp->next=head;
        head=temp;
        
    }
    size++;
}

void insertAtIdx(int idx,int val ){
if(idx==0)   insertAtHead(val);

else if(idx==size)insertAtEnd(val);
else if(idx<0 or idx>size)    cout<<"invalid Idx";
else{   
     int i =1;
    Node *temp =new Node(val);
Node* h=head;
while(i<idx){
    h=h->next;
    i++;
}
temp->next=h->next;
h->next=temp;
size++;
}
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

int getElementIdx(int idx){
    Node* temp =head;
if(idx==0)return temp->val;
if(idx==size-1)return tail->val;
if(idx<0 || idx>=size){
    cout<<"invalid IDX";
    return -1;
}
    for (int i=0;i<idx;i++){
        temp=temp->next;

    }return temp->val;

}

    







};
int main() {
    LinkedList ll;
    int i =0;
  
    ll.insertAtEnd(10);
    ll.insertAtEnd(20); 
    ll.insertAtEnd(30); 
    ll.insertAtEnd(40); 
    ll.display(); //10 20 30 40 
    ll.deleteAtENd();
    ll.display(); //10 20 30  
    ll.insertAtHead(5); 
    ll.display(); //5 10 20 30  
    ll.insertAtIdx(2,15); 
    ll.display(); //5 10 15 20 30  

    cout<<ll.getElementIdx(2);
    return 0;
}