// heaps/priority Queues
#include <iostream>
#include <vector>
#include<queue>
using namespace std;

int main() {

priority_queue<int>pq;  // default max heap
pq.push(10);
pq.push(2);
pq.push(-5);
pq.push(15);
cout<<pq.top();

// 2types of heaps
// min heap 
priority_queue<int,vector<int> , greater<int>  >minH;

minH.push(10);
minH.push(2);
minH.push(-5);
minH.push(15);
cout<<minH.top();

//problem identification kth smallest /largest top k closest k

    return 0;
}