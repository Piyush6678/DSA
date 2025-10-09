#include <iostream>
#include<queue>
using namespace std;

void HeapifyminH(vector<int> & v){
    int n =v.size();
     for (int i =n/2;i>=1;i--){
 while(1){

    int l=2*i;
int r=2*i +1;
if( l>=n  )break; 
if(r>=n){
if(v[i]>v[l]){swap(v[i],v[l]);
i=l;} break;
}
if(v[l]<v[r] ){
if(v[i]>v[l]){swap(v[i],v[l]);
i=l;}else break;
}
else{
if(v[i]>v[r]){swap(v[i],v[r]);
i=r;}else break;
}


}
     }
}


int main() {
    
    return 0;
}