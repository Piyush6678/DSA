#include <iostream>
#include<set>
#include<map>
using namespace std;

int main() {
    set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3); // take logn time sorted order
    for(auto x:s){
        cout<<x<<" ";

    }
    //ordered map
    map<int,int> m;
   m[10]=1;
   m[30]=2;
   m[20]=3; // sorted by keys 
    for(auto x:m){
        cout<<x.first<<" ";

    }

    return 0;
}