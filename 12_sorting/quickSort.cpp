#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// TC => O(nloogn)  worse O(n^@)

int partition(vector<int> &v ,int si,int ei ){
    int n =v.size();
    int count=0;
    for (int i =si+1;i<=ei;i++){
        if (v[i]<v[si]){
            count++;
            
        }
    }
int pivotidx = count+si;
    swap(v[pivotidx],v[si]);
int i= si,j=ei;
while( i<(pivotidx) || j>(pivotidx)  ){
     if(v[i]>v[pivotidx] && v[j]<v[pivotidx]){
        swap(v[i],v[j]);
        i++;j--;
    }
    if(v[i]> v[pivotidx] ){
        swap(v[i],v[j]);
        j--;
        
    }
    if(v[j]< v[pivotidx] ){
        swap(v[i],v[j]);
        i++;
        
    }

}



    return pivotidx;
    
    
}
void QuickSort (vector<int>& v ,int si,int ei ){

if(si>=ei){
    return ;
}

    int n =v.size();
    
int idx=partition(v,si,ei); 
 QuickSort(v,si,idx-1); 
 QuickSort(v,idx+1,ei); 






}
int main() {
    
    return 0;
}