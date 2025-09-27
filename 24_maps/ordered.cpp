#include <iostream>
#include<set>
#include<map>
using namespace std;

int main() {
    set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3); // sorted order
    for(auto x:s){
        cout<<x<<" ";
        
    }
    return 0;
}