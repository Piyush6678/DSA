#include<iostream>
using namespace std;
int fact(int x ){
    //Base Case
    if (x==0 ||x==1){
        return 1;
    }
    //recursive call
    return x*fact(x-1);
}
 //print n to 1 recursively
 void backcount(int n ){
    //base case
   if (n==0 )return;
    cout <<n;
    backcount(n-1);
    
 }
 //print 1 to n 
 void countToN(int i ,int n ){
     cout<<i;
     if(i==n)return;
     countToN(i+1,n); 
 } 

 //print 1 to n without extra parameter 
 void countToN2(int n ){
if(n==0)return ;
    countToN2(n-1);
cout<<n;
     
 } 
//sum 1 to n 
void Sum (int sum ,int n )
{
    if (n==0){
       cout<<sum; return; }
    Sum(sum+n,n-1);
}
int Sum2 (int n )
{
    if (n==1 ||n==0)return n ;
    return n +Sum2(n-1);
}
int main(){
    
// backcount(5);
// countToN(1,5);
countToN2(5);

    return 0; 

}