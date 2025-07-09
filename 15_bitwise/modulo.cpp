#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;
//print factorail of first 25 natural numbers and modulo the result by 10^9+7
vector<long long int> fact(int n ){
vector<long long int>fact(n+1,1);
int mod=1000000007;
for(int i =2;i<=n;i++){
    fact[i]=((i%mod) *(fact[i-1]%mod))%mod;
}return fact;
}
int main() {
     //(a+b)%c==(a%c+b%c)%c
     //(a-b)%c==(a%c-b%c +c)%c
     //(a*b)%c==(a%c * b%c)%c
     return 0;
}