// Question: Given the stock prices for consecutive days, find the stock span
// for each day. The span is the number of consecutive days, including the
// current day, for which the stock price was less than or equal to today's
// price. Solve the problem using a stack.
                  
# include<iostream>
# include<stack>
using namespace std;
int main(){
int arr[]={100,80,60,70,60,75,85};
int n=sizeof(arr)/sizeof(arr[0]);
int pgi[n];
stack<int>st;
pgi[0]=1;
st.push(0);
for (int i =1 ;i<n;i++){
while(st.size()&&arr[st.top()]<=arr[i]){
    st.pop();
}
if(st.size()==0) st.push(-1);
else pgi[i]=st.top();
pgi[i]=i-pgi[i];
st.push(i);
}



return 0;

}