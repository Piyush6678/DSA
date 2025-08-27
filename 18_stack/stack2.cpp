#include <iostream>
#include <stack>
#include <vector>
using namespace std;
void displayRev_rec(stack<int> st){
    if(st.size()==0)return;

    cout<<st.top();
    int x= st.top();
    st.pop();
    displayRev_rec(st);
    st.push(x);
    
}
void display_rec(stack<int> st){
    if(st.size()==0)return;
    int x= st.top();
    st.pop();
    display_rec(st);
    cout<<x;
    st.push(x);

}
void pushAtBottom_rec(stack<int> &st,int val){
    if(st.size()==0)
    {
        st.push(val);
        
        return;}
    int x= st.top();
    st.pop();
    pushAtBottom_rec(st,val);

    st.push(x);

}
void reverse_rec(stack<int>& st){
     if(st.size()==1)
    {
       
        return;}
    int x= st.top();
    st.pop();
   reverse_rec(st);

    pushAtBottom_rec(st,x);
}
int main() {
    stack<int> st;
    cout<<st.size()<<endl;
    st.push(10);
    st.push(20);
    st.push(30);
    st.push(40);
    return 0;
}