//generate binary strings of length n without consecutive 1 
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
void binString(string ans ,int n ){
    if (ans.size()==n){
        cout<<ans<<endl;
        return;
    }
    if(ans.size()==0 || ans[ans.size()-1] != '1'  ) binString(ans+"1",n);
    binString(ans+"0",n);


}



int main(){
    binString("",2);
    return 0;


}