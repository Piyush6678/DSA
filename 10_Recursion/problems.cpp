#include<iostream>
#include<string>
using namespace std;
// Skip a character -- remove all occurence of a in a string 
// void removeChar(string ans ,string ori){
//     if (ori.length()==0){cout<<ans;return ;}
     
//     char ch =ori[0];
//     if (ch=='a')removeChar(ans,ori.substr(1));
//     else{
//         removeChar(ans+ch,ori.substr(1));
//     }
// }
void removeChar(string ans ,string ori,int i ){
  
    if (i==ori.length()){cout<<ans;return ;}

    char ch =ori[i];
    if (ch=='a')removeChar(ans,ori,i+1);
    else{
        removeChar(ans+ch,ori,i+1);
    }
}
//traverse through array
void display(int arr[],int n ,int i ){
    if(i==n)return ;

    cout<<arr[i];
    display(arr,n,i+1);


}
void maxele (int arr[],int n , int i ,int max){
    if(n==i){
        cout<<max;
        return ;
    }
    if (arr[i]>max) max=arr[i];
maxele(arr,n,i+1,max);

}

// TOWER OF HANOI
void toh(char s,char h ,char d, int n ){
  if(n==0 )return ;
    toh(s,d,h,n-1);
    cout<<s<<"-->"<<d<<endl;
    toh(h,s,d,n-1);



}
//Generate all the strings of length n drawn from 0... k – 1.
void kstrings(int n ,int k,int arr[] ,int s){
    if(n<1){
    
        for (int i =0;i<s;i++){
            cout<<arr[i];
        }cout<<endl;
    }else{
        for (int i=0;i<k;i++){
            arr[n-1]=i;
            kstrings(n-1,k,arr,s);
        }
    }
}

int main(){ 
    int arr[3];
    kstrings(3,3,arr,3);
return 0;
}