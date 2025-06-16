#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
//iterativeSolution
void subarray(int arr[],int   n){
//for arr =1234 1 12 123 1234 2 23 234 3 34 4

for (int i =0;i<n;i++){
    for (int k =i;k<n;k++){
        for (int j =i;j<k;j++){
            cout <<arr[j];
        }
        cout<<endl;
    }
}

}
void print (vector <int> v){
    for (int i=0 ;i<v.size();i++){
        cout<<v[i];
    }
}
void subArrayRec(vector<int> ans ,vector<int> &v,int idx){
    if (idx == v.size()){
        print(ans);
        cout<<endl;
        return ;
    }
        if(ans.size()==0){
            subArrayRec(ans,v,idx+1 );
            ans.push_back(v[idx]);
            subArrayRec(ans,v,idx+1 );
        }
        else{
            subArrayRec(ans,v,idx+1 );
            if(ans[ans.size()-1]==v[idx-1]) {
                
                ans.push_back(v[idx]);
                subArrayRec(ans,v,idx+1 );
            }

        }


}
int main(){
vector<int>v ={1,2,3
};
vector <int> v2;
subArrayRec(v2,v,0);
    return 0;
}