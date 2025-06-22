#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

bool check (int speed ,vector<int>piles,int hours){
    int n =piles.size();
    int s =speed;
    for (int i=0;i<n;i++){
        if (hours<0)break;    }

    if(hours<0)return false;
    return true;
}

int koko(vector<int>piles,int hours){
    int n =piles.size();
    int sum=0;
    for(int i =0;i<n;i++){
        
        sum+=piles[i];
    }
    int lo =sum/hours;
    int hi =sum;
    int ans ;
    int speed;
    while(lo<=hi){
speed=lo+(hi-lo)/2;
if(check(speed ,piles,hours)){
ans=speed;
hi=speed-1;
}
else{
    lo=speed+1;
}
    } 
    return ans ;


}


int main(){


    return 0; 
}