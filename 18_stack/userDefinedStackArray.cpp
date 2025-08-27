#include <iostream>
using namespace std;
class Stack{
    public:
int arr[100] ;
int idx;
Stack(){
    idx=-1;
}
void push (int val){
    if(idx=99){
        cout<<"stack underflow";
        return;
    }
    idx++;
    arr[idx]=val;
}
void pop(){
    if(idx=-1){
        cout<<"stack underflow";
        return;
    }
    idx--;

}
int size(){

    return idx+1;
}
int top(){
    return arr[idx];
}
};
int main() {
      
    return 0;
}