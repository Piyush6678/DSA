#include<iostream>
#include<string> 
#include<vector>
#include<algorithm>
using namespace std;
void subset1( string ans, string original,vector<string>& v ){


    if(!original.size()){
v.push_back(ans);
    return; 
    }
    char s= original[0];


    subset1(ans,  original.substr(1),v   );
subset1(ans+s,  original.substr(1),v  );


}
void subsetNum1( int arr[] ,int n,int i ,vector<int>& v){
if(i==n){
    for (int i :v ){cout<<i<<" ";}
}

    
subsetNum1(arr,n,i+1,v);
v.push_back(arr[i]);
subsetNum1(arr,n,i+1,v);


}
//having duplicate character
void subset2(string ans, string original ,vector<string > v,bool flag){
    if(original.length()==0){
        v.push_back(ans);
    }
 sort(original.begin(),original.end());
    char s= original[0] ;

if (original.length()==1){
  if(flag==true)  subset2(ans+s,original.substr(1),v,true);
    subset2(ans,original.substr(1),v,true);
return ;
}

if(s==original[1]){
  if(flag==true)       subset2(ans+s,original.substr(1),v,true);
    subset2(ans,original.substr(1),v,false);
}
else{
  if(flag==true)       subset2(ans+s,original.substr(1),v,true);
    subset2(ans,original.substr(1),v,false);
}


}
void subsetNum2(){

}

int main(){

 string str ="asdf";
vector<string> v;
subset1("",str,v);

int arr[]={1,2,3};
int n = sizeof(arr)/sizeof(arr[0]);
vector<int> v2;

subsetNum1(arr,n,0,v2);

    return 0;

}