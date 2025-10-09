#include <iostream>
# include<queue>
using namespace std;
void heapSort(vector<int>v){
    priority_queue<int>p;
    for(int i =0;i<v.size();i++){
        p.push(v[i]);
    }
    for(int i =v.size()-1;i>=0;i--){
v[i]=p.top();
p.pop();
    }
}
int main() {
    
    return 0;
}