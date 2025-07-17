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
int main() {

    //10 20 30 40 
    Node a(10);
    Node b(20);
    Node c(30);
    Node d(40);
    //linking list list a->b->c->d->NUL
    a.next=&b;
    b.next=&c;
    c.next=&d;
cout<<a.next->val<<endl; //20
cout<<b.next->val<<endl; //30
cout<<a.next->next->next->val<<endl; //d ki value

//printing linked list using while loop 
//using pointer
Node  *temp=&a;
while(temp->next!=NULL){ //10 20 30 
    cout<<temp->val<<"  ";
    temp=temp->next;
}
//or
// Node temp=a;
// while(1){ // 10 20 30 40 
//     cout<<temp.val<<"  ";
//if(temp.next==NULL=) break;
//     temp=*(temp.next);
// }
    return 0;
}