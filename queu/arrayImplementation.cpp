#include <iostream>
using namespace std;
class Queue{
private:
int arr[100];
int size;
int front;int back;
public:
Queue(){
    size=-1;
    back=size;
    front=0;
}
void push(int val){
    
if(size=100){
    cout<<"queue overflow";return;
}

size++;
arr[size]=val;
back=size;
}
void pop(){
    back--;size--;
}
int front(){
    if(size<0){cout<<"stack is empty";return ;}
    
    return arr[front];
}
int back(){
   if(size<0){cout<<"stack is empty";return ;}
return arr[back];
}
int size(){
    if(size<0){cout<<"stack is empty";return ;}
    return size;

}


};
int main() {
    
    return 0;
}