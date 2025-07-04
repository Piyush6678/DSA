#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

void merge( vector<int>& a,vector<int>&  b ,vector<int>& c ){
int i=0;
int j =0;
int k=0;
while (i<a.size() && j<b.size()){
    if (a[i]<=b[j]){c[k++]=a[i++];}
    else{c[k++]=b[j++]; }

}
if(i==a.size())
{
    while(j<b.size()){
        c[k++]=b[j++];

    }
}
if(i==b.size())
{
    while(j<a.size()){
        c[k++]=a[i++];

    }
}

}

void mergeSort(vector<int> & v){
    //TC O(nlogn)   
    //yes
    int n =v.size();
    if(n==1)return;
    vector<int> a ;
for (int i =0;i<n/2;i++){
    a.push_back(v[i]);
}
vector<int>b;
for (int i =n/2;i<n-(n/2);i++){
    b.push_back(v[i]);
}
mergeSort(a);
mergeSort(b);
vector<int> c;
merge(a,b,v);
}

// count inversion 
int main(){
                                    
                              
          
            
    
     
   
  }