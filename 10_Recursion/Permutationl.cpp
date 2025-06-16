#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

void permutation (string ans,string original ){
    if (!original.length()){
        cout<<ans<< " ";
    }
for(int i =0;i<original.length();i++){
    // if (i==0){
    //     string org = original.substr(1);
    // }
    string org = original.substr(0,i)+original.substr(i+1); 


    permutation(ans+original[i],org);
}


}

int main(){
string s = "abc";
permutation("",s);

    return 0;
}