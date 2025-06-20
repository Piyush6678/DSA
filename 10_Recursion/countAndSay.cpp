#include<iostream>

#include<string>


using namespace std ;
string countAndSay(int n  ){
if(n==1)return "1";
string str=countAndSay(n-1);
string ans="";
int count=1;
for (int i =1;i<str.length();i++){
    if(str[i]!=str[i-1] ){
      ans+to_string(count);
    ans+=str[i-1];
        count=1;
}else{count++;
}
}
ans+=to_string(count);
ans+=str.back();
return ans;

}


int main(){
string ans=countAndSay(3);
for(char s:ans){
  cout<<s;

}
    return 0;
}