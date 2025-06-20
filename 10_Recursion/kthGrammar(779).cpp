#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std ;
int kthGrammar(int n ,int k ){
if(n==1)return 0;

    if(k%2==0){ int ans= kthGrammar(n-1,k/2);
        if(ans){
            return 0;
        }
        return 1;
    }
    else{
return kthGrammar(n-1,k/2+1);

}

}


int main(){

    return 0;
}