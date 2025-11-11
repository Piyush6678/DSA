#include <iostream>
#include <vector>

using namespace std;
int find(vector<int> &par,int x){
// return the group/cluster x belongs to 
return par[x]=(par[x]==x)?x:find(par,par[x]);
}
void Union (vector<int>&par,vector<int>&rank,int a ,int b){
int para=find(par,a);
int parb=find(par,b);
if(rank[para]>=rank[parb]){
    rank[a]++;
    par[b]=para;
}else{
    rank[b]++;
    par[a]=parb;
}


}
int main() {
    
    return 0;
}