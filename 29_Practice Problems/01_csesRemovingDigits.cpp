#include <iostream>
#include <vector>
using namespace std;
int noOfDigits(int n){
int cnt=0;
while(n>0){
    cnt++;
    n/=10;
}return n;
}
vector<int> getDigits(int n ){
    vector<int> res;
    while(n>0){
        if(n%10!=0){
            res.push_back(n%10);
        }
        n/=10;
    }return res;
}

//memoization
vector<int>dp;

int RemovingDigits(int n){
if(n==0)return 0;
if(n<10)return 1;
if(dp[n]!=-1)return dp[n];


int dig=noOfDigits(n);
vector<int>d=getDigits(n);
int res=INT16_MAX;
for(int i =0;i<d.size();i++){
res=min(res,RemovingDigits(n-d[i]));

}return dp[n]= res+1;

}


int btmup(int n ){
    dp[0]=0;
    for(int i= 1;i<=9;i++){
        dp[i]=1;
    }
    for(int i =10;i<=n;i++)
    {
      vector<int>d=getDigits(i);
  int res=INT16_MAX;
      for(int j =0;i<d.size();j++){
res=min(res,dp[(n-d[i])]);

}dp[i]=res+1;
    }
return dp[n];
}



int main() {
    dp.resize(1000005,-1);
    return 0;
}