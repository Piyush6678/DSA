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

void display(DoubleNode* head ){
    while(head){cout<<head->val<<" ";
    head=head->next;}
    cout<<endl;
}
void displayrev(DoubleNode* tail ){
    while(tail){cout<<tail->val<<" ";
    tail=tail->prev;}
}



int main() {
    DoubleNode* a =new DoubleNode(10);
    DoubleNode* b =new DoubleNode(20);
    DoubleNode* c =new DoubleNode(30);
    a->next=b;
    b->prev=a;
    b->next=c;
    c->prev=b;
    display(a);//10 20 30 
    displayrev(c); // 30 20 10 
    return 0;
}