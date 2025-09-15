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
        if(size<0){cout<<"stack is empty";return ;}
    for (int i =0;i<size-1;i++){
        arr[i]=arr[i+1];
    }
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
void display(){
    for (int i =0;i<size;i++){
        cout<<arr[i]<<" ";
    }cout<<"\n";
}


};
int main() {
    
    return 0;
}