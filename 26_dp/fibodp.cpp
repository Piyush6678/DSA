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
int main() {
    vector<int>dp(100,-1); // array should be greater the n+1 size
    cout<<fib(6,dp);
    return 0;
}