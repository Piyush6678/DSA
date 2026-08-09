#include<iostream>
# include<list>
#include<vector> 
#include<unordered_set> 
using namespace std;
vector <  list<int>>graph;
int v;
void add_edge(int src,int dest , bool bi_dir=true)
{
    graph[src].push_back(dest);
    if(bi_dir){
        graph[dest].push_back(src);

    }

}

void dfs(int node,unordered_set<int>&visited){
    visited.insert(node);
    for(auto nei:graph[node]){
        if(!visited.count(nei)){
            dfs(nei,visited);
        }
    }

}

int connected_component(){
    int result=0;
unordered_set<int>visited;

    for (int i =0;i<v;i++){
        // go to every vertex 
if(!visited.count(i)){
    result++;
    dfs(i,visited);
}
    }
    return result;
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
