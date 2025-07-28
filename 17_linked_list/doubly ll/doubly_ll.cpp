#include <iostream>
using namespace std;
class DoubleNode{
    public:
    int val;
    DoubleNode* prev;
    DoubleNode* next;
    
    DoubleNode(int val){
        this->val=val;
        prev=NULL;
        prev=NULL;

    }
} ;




class doubly_ll{
    DoubleNode* head;
    DoubleNode* tail;
    int size;
    public:
    doubly_ll(){
        head=tail=NULL;
        size=0;
    }
    void insertAtHead(int val){
        DoubleNode* temp=new DoubleNode(val);
        if(size==0)head=tail=temp;
        else{
            temp->next=head;
            head->prev=temp;
            head=temp;
        }size++;
        
    }
    void insertAtTail(int val){
        DoubleNode* temp=new DoubleNode(val);
        if(size==0)head=tail=temp;
        else{
            tail->next=temp;
            temp->prev=tail;
            tail=temp;
        }
        size++;
    }
    void insertAtIdx(int idx ,int val){
       if(idx<0 ||idx>size ) cout<<"invalid Idx"<<endl;
       else if(idx==0)  insertAtHead(val);
       else if (idx==size) insertAtTail(val);
       else{
        DoubleNode* t =new DoubleNode(val);
        DoubleNode* temp =head;
        for (int i =1;i<=idx-1;i++){
            temp=temp->next;
        }
        t->next=temp->next;
        temp->next=t;
        t->prev=temp;
        t->next->prev=t;
        size++;
       }

    }
    void display(){
        DoubleNode *temp =head;
        while(temp){
            cout<<temp->val<< " ";
            temp=temp->next;
            
        }cout<<endl;
    }

    




};
int main() {
    doubly_ll first;
first.insertAtHead(10);
first.insertAtHead(5);
first.insertAtTail(30);
first.insertAtIdx(2,20);
first.display();
    return 0;
}