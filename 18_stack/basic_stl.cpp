#include <iostream>
#include<stack>
using namespace std;
void print(stack<int> st){
    while(st.size()>0){
        cout<<st.top()<<" ";
        st.pop();
    }
}
int main() {
    stack<int> st;
    cout<<st.size()<<endl;
    st.push(10);
    st.push(20);
    st.push(30);
    // cout<<st.size()<<endl;
    // st.pop(); 
    // cout<<st.top()<<endl;
print(st);
    return 0;
}