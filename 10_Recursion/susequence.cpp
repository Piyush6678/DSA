#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
void subset1( string ans, string original,vector<string>& v, int k  )  {  if(!original.size()){
    if (ans.size()==k) v.push_back(ans);
    return; 
    }
    char s= original[0];


    subset1(ans,  original.substr(1),v   );
subset1(ans+s,  original.substr(1),v  );
}


int main(){
return 0;
}