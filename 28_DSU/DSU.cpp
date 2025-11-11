#include <iostream>
#include <vector>

using namespace std;
void Union (){

}
int find(vector<int> &par,int x){
// return the group/cluster x belongs to 
return (par[x]==x)?x:find(par,par[x]);
}
int main() {
    
    return 0;
}