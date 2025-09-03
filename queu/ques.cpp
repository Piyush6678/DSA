#include <iostream>
#include<queue>
#include<vector>
#include<stack>
using namespace std;
//reverse First k elements of a queue

void reverse_k_ele(queue<int> &q,int k ){
    int n =q.size();
    stack<int> st;
    for(int i =1;i<=k;i++){
        st.push(q.front());
        q.pop();
    }
    for(int i =1;i<n;i++){
        if(i<=k){
            q.push(st.top());
            st.pop();
            
        }
        else{q.push(q.front());
            q.pop();}
            
        }
        
    }
    // first negative in each window of size k
void firstnegative(int arr[],int k){
    int n =sizeof(arr)/sizeof(arr[0]);
    queue<int> help;
    vector<int> ans;
    for (int i =0;i<n;i++){
        if(arr[i]<0)help.push(i);
    }
    for(int i=0;i<=n-k;i++){
        while( help.size() &&  i+k<n){
            help.pop();
        }
        if(!help.size() ||   help.front()>i+k )ans.push_back(0);
       else ans.push_back(arr[help.front()]);
    }

}
//

int main() {
    
    return 0;
}