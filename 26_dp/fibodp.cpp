#include <iostream>
#include <vector>
using namespace std;

int fib(int n,vector<int>&dp ){
    if(n==1||n==0) return n;
    int ans2,ans1;
if(dp[n-1]!=-1) ans1=dp[n-1];
else { ans1 =fib(n-1,dp);
dp[n-1]=ans1;}

if(dp[n-2]!=-1) ans2=dp[n-2];   
else { ans2=fib(n-2,dp);
    dp[n-2]=ans2;
}
return  ans1+ans2;
}
int main() {
    vector<int>dp(100,-1); // array should be greater the n+1 size
    cout<<fib(6,dp);
    return 0;
}