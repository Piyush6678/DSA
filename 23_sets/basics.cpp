#include <iostream>
#include <unordered_set>
using namespace std;

int main() { // stores unique elements
    unordered_set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.insert(5);
    cout<<s.size();

    s.erase(2);

    for (int x : s){
cout<<x;
    }
    int target=4 ;
    if(s.find(target)!=s.end() ){
        cout<<"target exist";
    }else{
        cout<<"not exist";
    }
    return 0;
}