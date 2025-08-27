#include <iostream>
#include<stack>
#include<vector>
using namespace std;
void pushAtBottom(stack<int>&st,int val){
    stack<int> temp;
  while(st.size()>0){
        
             temp.push(st.top());
             st.pop();
    
         }
         st.push(val);
  while(temp.size()>0){
        
             st.push(temp.top());
             temp.pop();
    
         }

        }
void pushAtIdx(stack<int>&st,int idx,int val){
    stack<int> temp;
  while(st.size()>idx){
        
             temp.push(st.top());
             st.pop();
    
         }
         st.push(val);
  while(temp.size()>0){
        
             st.push(temp.top());
             temp.pop();
    
         }

        }



stack<int> reverse(stack<int> st){
    //reverse the stack
    // stack<int>temp;
    
    // stack<int>temp2;
    //   while(st.size()>0){
        
    //         temp.push(st.top());
    //         st.pop();
    
    //     }
    //   while(temp.size()>0){
        
    //         temp2.push(temp.top());
    //         temp.pop();
    
    //     }
    //   while(st.size()>0){
        
    //         st.push(temp2.top());
    //         temp2.pop();
    
    //     }
    vector<int> v;
   while(st.size()>0){
    
         v.push_back(st.top());

    st.pop();
     }
     for(int i =0; i<v.size();i++){
        st.push(v[i]);
     }
return st;

}
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
// print(st);
stack<int> temp;
    while(st.size()>0){
        cout<<st.top()<<" ";
        temp.push(st.top());
        st.pop();
    }
    while(temp.size()>0){
       
        st.push(temp.top());
        temp.pop();
    }

    return 0;
}
