#include <iostream>
#include <vector>
using namespace std;

int fib(int n,vector<int>&dp ){
    if(n==1||n==0) return n;
    int ans2,ans1;
if(dp[n]!=-1) {
    return dp[n];
}
return dp[n]=fib(n-1,dp)+fib(n-2,dp);
}
// iterative dp bootp dp tabulation
void fibo(int n ){
int arr[n+1]={0};
arr[0]=0;
arr[1]=1;
for(int i =2;i<=n;i++){
    arr[i]=arr[i-1]+arr[i+2];
}
}
int main() {
    vector<int>dp(100,-1); // array should be greater the n+1 size
    cout<<fib(6,dp);


    return 0;
}