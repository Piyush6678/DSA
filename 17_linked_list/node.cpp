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
    Node a(1);
    Node b(2);
    a.next=&b;
    Node c(3);
    b.next=&c;
cout<<a.next->val; //2
cout<<b.next->val; //3
    return 0;
}