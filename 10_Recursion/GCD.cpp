#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
//Euclids Division Algorithm 
int GCD(int a,int b ){
     
    if(b%a==0) return a;
    else return GCD(b%a,a);
}

using namespace std;
int main(){
    
   cout<< GCD(4,8);
    return 0;
}