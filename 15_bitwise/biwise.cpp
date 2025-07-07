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
    int count =0;
    while (n>0){
        if (n%2!=0)count++;
       n= n>>1;
    }
    return count ;
}




int main() {
    string str="100";
cout<<    binary_to_decimal(str);
cout<<    endl<<decimalToBinary(5);
cout<<    endl<<SetBits(7);
    return 0;
}