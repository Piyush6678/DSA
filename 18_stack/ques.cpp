#include<iostream>
#include<stack>
#include<vector>
#include<algorithm>
using namespace std;
bool balancedBrackets(string s){
    if(s.length() %2!=0) return false;
    stack<char> stk;
    for (int i =0;i<s.length();i++){
if(s[i]=='('){
    stk.push('(');
}
else{
    if(stk.size()==0)return false;
    else stk.pop();
}

    }
    if(stk.size()==0)return true;
    return false;

}
//Remove consectutive duplicates in a string
string removeConsecutiveDuplicate(string s ){
    stack<char> stk;
    for(int i=0;i<s.length();i++ ){
        if(stk.size()==0 || stk.top()!=s[i] )stk.push(s[i]);
      
    }
    s="";
    while(stk.size()){
s+=(stk.top());
stk.pop();
    }
 reverse(s.begin(),s.end());
     return s;
}
//next greater element
vector<int>  nxtgrtest(int arr[],int n){
vector <int> ans(n);
stack<int> help;
ans[n-1]=-1;
help.push(arr[n-1]);
for (int i =n-2;i>=0;i--){
while( help.size() &&  help.top()<arr[i]   )help.pop();
if(help.size()==0)ans[i]=-1;
else ans[i]=help.top();
help.push(arr[i]);
}return ans;
}
//prev greater element
vector<int>  prevgrtest(int arr[],int n){
vector <int> ans(n);
stack<int> help;
ans[0]=-1;
help.push(arr[0]);
for (int i =1;i<n;i++){
while( help.size() &&  help.top()<arr[i]   )help.pop();
if(help.size()==0)ans[i]=-1;
else ans[i]=help.top();
help.push(arr[i]);
}return ans;
}


int main(){
    string s="()(())()((())))";
    string s2="adsfaasafewffgggsae";
    cout<<balancedBrackets(s);
    return 0;
}