#include <iostream>
#include<vector>
using namespace std;
class Stack{
    public:
vector<int> v ;
int idx;
Stack(){
    idx=-1;
}
void push (int val){
 v.push_back(val)  ;
idx++;
}
void pop(){
    if(idx=-1){
        cout<<"stack underflow";
        return;
    }
    v.pop_back();
idx--;
}
int size(){

    return idx+1;
}
int top(){
    return v[idx];
}
};
int main() {
      
    return 0;
}