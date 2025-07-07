#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
//AND (&) OR(|) NOT(~) XOR(^)  
int binary_to_decimal(string &binary){
    int n =binary.size();
    int result=0;
    for(int i =n-1;i>=0;i--){
        char ch =binary[i];
        int num= ch- '0' ;
        result =result+ num*(1<<(n-i-1));
    }
    return result;
}

string decimalToBinary(int num ){
    string res="";
while(num>0){
if (num%2==0){
    //even
    res='0'+res;
}else{
    //odd
    res='1'+res;

}
num=num>>1;
}return res;
}

//Find the number of set bit for any integer 

int SetBits(int n ){
    // Build in function 
    __builtin_popcount(13);
    //our algo
    int count =0;
    // method 1 
    // while (n>0){
    //     if (n%2!=0)count++;
    //    n= n>>1;
    // }
    // method 2 
    while (n>0){
         count++;
       n= n & (n-1) ; // brian kernighan algo 
    }
    
    return count ;
}
//maximum power of 2 
int maxPower  (int n){
    // int x ; 
    // while (n>0){
    //    x=n;
    //   n= n & (n-1) ; 
    // }
    // return( x) ;
    //method 2 
    n=n|(n>>1);
    n=n|(n>>2);
    n=n|(n>>4);
    n=n|(n>>8);
    n=n|(n>>16);
    return ((n+1)>>1);
    
}
//flip the bits
int flip (int a ){
    int n =a;
    n=n|(n>>1);
    n=n|(n>>2);
    n=n|(n>>4);
    n=n|(n>>8);
    n=n|(n>>16);
    n=((n+1)>>1);
return n^a;
}

//Min number of bit flip to conver the number x to y 
int flipConvert(int x ,int y){

   return __builtin_popcount(x^y);

}
// given an integer aray where every element occurs twice except only once find that unique element
int unique1(int arr[] ,int n ){
    int num=0 ;
    for(int i=0;i<n;i++){
num =num^arr[i];
    }return num;
}
void unique2(int arr[] ,int n ){
    int num=0 ;
    for(int i=0;i<n;i++){
num =num^arr[i];
    }
int k =0;
    int temp=num;
    while(true){
        if(temp&1==1){break;}
        temp>>1; 
        k++;

    }
    int retval;
    for (int i =0;i<n;i++){
        int number =arr[i];
        
        if ((number>>k)&1 ==1)retval^=number;
    }cout<<retval<<endl;
    cout<<(retval^num);
}
int main() {
    string str="100";
cout<<    binary_to_decimal(str);
cout<<    endl<<decimalToBinary(5);
cout<<    endl<<maxPower(100); 
    return 0;
}