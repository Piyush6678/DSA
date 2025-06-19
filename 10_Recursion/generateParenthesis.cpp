#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
  
void generateParenthesis(vector<string>&ans ,string s ,int o ,int c  ){
    if (c==0){
        ans.push_back(s);
        return ;

    }
  else  if (o==0){ generateParenthesis(ans,s + ')',o,c-1);}
 else   if (o>c){return;}
    else {
        generateParenthesis(ans,s + '(',o-1,c);
     if (o<c)   generateParenthesis(ans,s + ')',o,c-1);
    }

}
int main (){
    vector<string> s;
    generateParenthesis(s,"",3,3);
    for(string s:s){
        cout<<s<<endl;
    } 
    return 0;
}