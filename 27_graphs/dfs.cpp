#include<iostream>
# include<list>
#include<vector> 
#include<unordered_set> 
using namespace std;

vector <  list<int>>graph;
int v;
unordered_set<int> visited;
vector<vector<int>> allPathResult;
void add_edge(int src,int dest , bool bi_dir=true)
{
    graph[src].push_back(dest);
    if(bi_dir){
        graph[dest].push_back(src);

    }

}

void allPath(int src,int end ,vector<int>&path){
    path.push_back(src);
   if(src==end){
    
    allPathResult.push_back(path);

    path.pop_back();
    return ;
}
   
       visited.insert(src);
    for (auto nei : graph[src] ){
        if(not visited.count(src)){
           allPath(nei,end,path);
          
        }

    }
    path.pop_back();
    visited.erase(src);
    return ;

}
bool anyPath(int src, int dest){
    if(src==dest)return true;
       visited.insert(src);
    for (auto nei : graph[src] ){
        if(not visited.count(src)){
            bool result=anyPath(nei,dest);
            if(result )return true;

        }

    }
    return false;
}
void display()
{
    for (int i =0;i<graph.size();i++){
        cout<<i<<"->";
        for(auto ele:graph[i]){
            cout<<ele<<" ,";


        }
        cout<<endl;
    }
}
    int main(){
cin>>v;
graph.resize(v,list<int>());
int e;
cin>>e;
visited.clear();
while(e--){
    int s,d;
    cin>>s>>d;
    add_edge(s,d);

}
display();

    return 0;
}
