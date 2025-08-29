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