#include <iostream>
#include<queue>
#include<vector>

using namespace std;
// sort a k/nearly sorted array
void sortKnearest(vector<int> & v,int k ){
priority_queue<int,vector<int> , greater<int>  >minH;
int idx=0;
for (int i =0;i<v.size();i++){
    if(minH.size()<k+1){
        minH.push(v[i]);
    }
    else{

        v[idx++]==(minH.top());
        
        minH.pop();
    }
}
while(minH.size()>0){
            v[idx++]==(minH.top());
        minH.pop();
}

}

int  MinimumcostToconnectAllRopes(vector<int> & nums){
    priority_queue<int>pq;
   int length=0;
    for(int  x:nums){
    length+=x;
        pq.push(x);
    }
    while(pq.size()>1 ){
        int a =pq.top();
        pq.pop();
        int b =pq.top();
        pq.pop();
        pq.push(a+b);
    }
    if(!pq.size())return 0;
    return pq.top();

} 

void bstToMaxHeap(){
    
}

int main() {
   vector <int>v={10,20,-4,6,18,24,105,118};
    // heap method t.c. (n logk)
    // k smallest
    priority_queue<int> pq; int k =3;
    for(int i =0;i<v.size();i++){
        if(pq.size()<k)pq.push(v[i]);
        else if(v[i]<pq.top()){
            pq.push(v[i]);
            pq.pop();
        }
        
    }
    
    cout<<pq.top(); //10
    //k largest
priority_queue<int,vector<int> , greater<int>  >minH; 
for(int i =0;i<v.size();i++){
    if(minH.size()<k)minH.push(v[i]);
    else if(v[i]>minH.top()){
        minH.push(v[i]);
        minH.pop();
    }
    
}
cout<<minH.top(); //24


vector<int> v2 ={6,5,3,2,8,10,9};

sortKnearest(v2,3);
for (int i =0;i<v2.size();i++){
    cout<<endl<<v2[i];
}

    return 0;
}