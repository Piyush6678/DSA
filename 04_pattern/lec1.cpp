#include<iostream>
using namespace std;
int main(){
    //star pattern
    //1 4X5 rect
    for (int i =1;i<=4;i++){ // upto number of rows
        for (int j =1;j<=5;j++){ // upto number of columns
cout<<"X";
        }cout<<endl;
    }
    //2 Star triangle
    for (int i =1;i<=4;i++){ // upto number of rows
        for (int j =1;j<=i;j++){ // upto number of columns
cout<<"X";
        }cout<<endl;
    }
    //3 reversed triangle
    for (int i =1;i<=4;i++){ // upto number of rows
        for (int j =4;j>=i;j--){ // upto number of columns
cout<<"X";
        }cout<<endl;
    }
//plus pattern
int n =5;
for (int i =1;i<=5;i++){
    for (int j =1;j<=5;j++){
        if(j==((n+1)/2) or i==((n+1)/2)){
            cout<<"X";
        }else{
            cout<<" ";
        }
        cout<<endl;
    }
}

//Star Cross

for (int i =1;i<=5;i++){
    for (int j =1;j<=5;j++){
        if(j==i or ((i+j)==(n+1)) ){
            cout<<"X";
        }else{
            cout<<" ";
        }
        cout<<endl;
    }
}

//Stra triangle flipped
for(int i =4;i>=1;i--){
    for (int j=1;j<=4;j++){
        if(j>=i)cout<<"X";
        else cout<<" ";
    }cout<<endl;
}






    //number pattern 
    //1 print 1 2 3 4 in 4 rows 
    for (int i =1;i<=4;i++){ // upto number of rows
        for (int j =1;j<=4;j++){ // upto number of columns
cout<<j;
        }cout<<endl;
    }
    //Number Triangle
    for (int i =1;i<=4;i++){ // upto number of rows
        for (int j =1;j<=i;j++){ // upto number of columns
cout<<j;
        }cout<<endl;
    }
//Odd Number Triangle
for (int i =1;i<=4;i++){ // upto number of rows
    for (int j =1;j<=(2*i -1);j+=2){ // upto number of columns
cout<<j;
    }cout<<endl;
}
//Floyds Triangle
int cnt=1;
for (int i =1;i<=4;i++){
    for (int j =1;j<=i;j++,cnt++){
        cout<<cnt;
    }cout<<endl;
}

//Binary triangle
//if i+j is even print 1 else 0
for(int i =1;i<=5;i++){
    for (int j =1 ;j<=i;j++){
        if((i+j)%2==0)cout<<"1";
        else cout<<"0";
    }cout<<endl;
}
// number triangle flipped
for (int i =1;i<=4;i++){
    //spaces
    for(int j =1;j<=4-i;j++){
        cout<<" ";
    }
    //star
    for(int k =1;k<=i;k++){
        cout<<"X";
    }cout<<endl;
}


//Alphabet pattern
for (int i =1;i<=4;i++){ // upto number of rows
    for (int j =1;j<=4;j++){ // upto number of columns
cout<<char(j+64);
    }cout<<endl;
}
    return 0;
}