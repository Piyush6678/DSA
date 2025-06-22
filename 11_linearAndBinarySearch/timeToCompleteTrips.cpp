#include<iostream>
#include<vector>

using namespace std;
bool check(vector<int>time,int trips,int hours){
for (int i =0;i<time.size();i++){
    trips-=(time[i]/hours);
}
if (trips<=0)return true;
return false;

}
long long minTime(vector<int>time,int trips ){
int n =time.size();
long long max=0;
for (int i =0;i<n ;i++){
if ((long long )time[i]>max)max=(long long )time[i];
}
long long ans; 
long long  lo =1;
long long  hi =max*trips;
long long mid;
while(lo<=hi){
mid=lo+(hi-lo)/2;
if(check(time, trips,mid)){
    ans=mid;hi=mid-1;
}
else{
    lo=mid+1;
}
}
return ans;


    
}
int main() {
    
    return 0;
}