#include <iostream>
#include<queue>
#include<stack>
using namespace std;
//first in first out
void display(queue<int>& q){
    int n =q.size();
    for(int i =1;i<=n;i++){
        int x =q.front();
        cout<<x<<" ";
        q.pop();q.push(x);
    }cout<<endl;
} 
void display_rev(queue<int>& q){
    int n =q.size();
    stack<int> st;
    for(int i =1;i<=n;i++){
       int x= q.front();
       st.push(x);
       q.pop();
       q.push(x); 
       
       
    }
    for(int i =1;i<=n;i++){
     cout<<  st.top()<<" ";
       st.pop();
       
    }cout<<endl;
} 
//remove ele  at even index consider 0 based idexing
void removeEven(queue<int>& q){
    int n=q.size();
    for(int i =0;i<n;i++){
        int x= q.front();
            q.pop();
            if(i%2!=0)q.push(x);
    }

}
int main() {
    queue<int> q;
    //push pop front size
        q.push(10);
        q.push(20);
        q.push(30);
        q.push(40);
        q.push(50);
        cout<<q.front();//10
        cout<<q.back();//50
        q.pop();
        cout<<q.front();//20
    return 0;
}