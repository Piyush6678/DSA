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

//A^B
int power(int a ,int b )
{   
    if (b==0 )return 1 ;
    if (a==0 ||a==1)return a;

    return a *power(a,b-1);
}

//FIBONACCI
int fibo (int n ){
    if (n==2|| n==1) return 1;
    return fibo(n-1)+fibo(n-2);
}

//power function logarithmic 
int power2 (int a ,int b ){
    if (b==0)return 1 ;
    if (b==1)return a ;
    int ans =power(a,b/2);
    if (b%2==0) {
        return ans*ans;
    }
    else{
        return ans*ans*a;

    }
}
//Stair Path
     // max 2 jump
    int stair(int n ){
     if (n==1 || n==2 ) return n;
     return stair(n-1)+stair(n-2);
       }

//max 3 jump
       int stair2(int n ){
        if (n>3)return n;
        if(n==3)return 4;
        return stair2(n-1)+stair2(n-2)+stair(n-3);
       }

       
int main(){
    
// backcount(5);
// countToN(1,5);
countToN2(5);

    return 0; 

}