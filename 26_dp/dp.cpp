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
void fiboTabulation(int n ){
int arr[n+1]={0};
arr[0]=0;
arr[1]=1;
for(int i =2;i<=n;i++){
    arr[i]=arr[i-1]+arr[i+2];
}
}
int minCostClimbingStairshelper(vector<int> & cost,int i ,int sum){
if(i>=cost.size()) return sum;
return min(minCostClimbingStairshelper(cost,i+1,sum+cost[i]),minCostClimbingStairshelper(cost,i+2,sum+cost[i]));
}
int minCostClimbingStairs(vector<int> & cost,vector<int>?&dp){
return min(minCostClimbingStairshelper(cost,0,0),minCostClimbingStairshelper(cost,1,0));
}


int main() {
    vector<int>dp(100,-1); // array should be greater the n+1 size
    cout<<fib(6,dp);


    return 0;
}