#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
bool isPalindrome(string s ){
    int i =0,j=s.size();
    while(i<j){
        if (s[i]!=s[j]) return false;
        i++;j++;
}
    return true;
}
bool isPlaindromeRec(string s , int i ,int j ){
   if(i==j)return true;
    
        if (s[i]!=s[j]) return false;
   

    return isPlaindromeRec(s,i+1,j-1);
}

 int main(){
string s ="piyush";

    return 0;
 }