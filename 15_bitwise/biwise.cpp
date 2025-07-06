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
num/=2;
}return res;
}

int main() {
    string str="100";
cout<<    binary_to_decimal(str);
cout<<    endl<<decimalToBinary(5);
    return 0;
}