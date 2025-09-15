#include <iostream>
using namespace std;
class node{
    public:
    int val ;
    node* next;
    node(int val ){
        this->val=val;
        this->next=NULL;
    }
};
class Queue{
    private:
    int size;
    node* front,*back;
    public:
    Queue(){
        size=0;
        front=back=NULL;
    }
    void push(int val){
        node* temp=new node(val);
        if(size==0) front=back=temp;
        else{

            back->next=temp;
            back=temp;

        }size++;
    }
    void pop(){
        if(size==0){cout<<"Queue is empty";return;}
        else{
            node*temp=front;

            front=front->next;
            delete(temp);
            size--;
        }
    }
int front(){
    if(size==0){cout<<"Queue is empty";return -1;}
    return front->val; 
}
int back(){
    if(size==0){cout<<"Queue is empty";return -1;}
    return back->val; 
}
void display(){
    node* temp=front;
    while(temp){
        cout<<temp->val<<" ";
        temp=temp->next;
    }
}
};
int main() {
    
    return 0;
}