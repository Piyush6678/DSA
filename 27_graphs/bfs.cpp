#include<iostream>
# include<list>
#include<vector> 
#include<queue> 
#include<unordered_set>
using namespace std;
unordered_set<int> visited;

vector <  list<int>>graph;
int v;
void add_edge(int src,int dest , bool bi_dir=true)
{
    graph[src].push_back(dest);
    if(bi_dir){
        graph[dest].push_back(src);

    }

}
void bfs(int src,int dest,vector<int>&dist){
queue<int> qu;
visited.clear();
dist.resize(v,INT8_MAX);
dist[src]=0;
visited.insert(src);
qu.push(src);
while(qu.size()){
int curr=qu.front();
qu.pop();
for(auto nei :graph[curr]){
    if(! visited.count(nei)){
qu.push(nei);
visited.insert(nei);
dist[nei]=dist[curr]+1;

    }
}
}




}



    int main(){
cin>>v;
graph.resize(v,list<int>());
int e;
cin>>e;
while(e--){
    int s,d;
    cin>>s>>d;
    add_edge(s,d);

}


    return 0;
}
