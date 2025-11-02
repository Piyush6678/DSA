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



int minCostClimbingStairshelper(vector<int> & cost,int i ,vector<int>&dp){
    //memorization
if(i==0 || i==1) return cost[i];


if(dp[i]!=-1)return dp[i];

return dp[i]= cost[i]+min(minCostClimbingStairshelper(cost,i-1,dp),minCostClimbingStairshelper(cost,i-2,dp));
}
int minCostClimbingStairs(vector<int> & cost,vector<int>&dp){
    int n =cost.size();
    // return min(minCostClimbingStairshelper(cost,n-1,dp),minCostClimbingStairshelper(cost,n-2,dp));
    //tabulation

for(int i=2;i<n;i++){
    cost[i]+=min(cost[i-1],cost[i-2]))
}
return min(cost[n-1],cost[n-2]);
}

int uniquePathHelper(int sr,int sc, int er, int ec){
if(sr==er && sc==ec)return 1;
if(sr>er || sc>ec)return 0;
int rw= uniquePathHelper(sr,sc+1,er,ec);
int dw= uniquePathHelper(sr+1,sc,er,ec);
return dw+rw;
}
int uniquePath(int m ,int n ){
// if(m==1&& n==1) return 1;
// if(m==0 || n==01) return 0;

// return uniquePath(m-1,n)+ uniquePath(m,n-1);

return uniquePathHelper(0,0,m-1,n-1);
}















int main() {
    vector<int>dp(100,-1); // array should be greater the n+1 size
    cout<<fib(6,dp);


    return 0;
}