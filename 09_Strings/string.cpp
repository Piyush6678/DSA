#include<iostream>
#include<sstream>
#include<algorithm>
#include<string>
using namespace std;
//funtion to return the number of digits of a string without using a loop
int digits(int x){
    string str= to_string(x);
    return str.length();
}
//return the number of times the neighbouring character are different from each other 
int diff(string str){
   int cnt=0;
   for (int i =0;i<str.length();i++){
    if (i==0 && str[i]!=str[i+1] ) cnt++;
    else if (i==(str.length()-1) && str[i]!=str[i-1] ) cnt++;
    else if (str[i]!=str[i-1] && str[i]!=str[i+1])cnt++;
   }
    return cnt;
}
//if anagram
bool anagram(string a,string b ){
    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
if (a==b) return true;
return false;
}
//return the character occuring most of the timne 
string mostOccuringCharacter(string str){
    int i =0,max=0;
    int arr[125]={};
    while (str[i]!='\0'){
arr[int(str[i])]++;
    }
    for (int i =63;i<124;i++){
        if (arr[i]>max) {max=arr[i];
        }
    }
    string freq;
    //if multiple character have same frequency 
    for (int i =63;i<124;i++){
        if (arr[i]==max) {freq.push_back(char(i));
        }
    }

    return freq ;
}

int main(){

    string s ="Piyush";
s[1]='o';
cout<<s; // strings are mutable
//count vowels
int i =0; int cnt=0;
while(s[i]!='\0'){
    if (s[i]=='a' || s[i]=='e' ||s[i]=='i' ||s[i]=='o' || s[i]=='u'){
        cnt++;
    }
    }

//reverse the string
string str;
getline(cin,str);
int len=str.size();
reverse(str.begin(),str.begin()+len/2);  
 //to string Converts integer to string
 to_string(5);
 //sort function modify  the initial string sort on the basis of ascii value
sort(str.begin(),str.end());
// String Stream                                                                                                                
                 stringstream ss(str)  ;
                 string temp;
                 while(ss>>temp){cout<<temp;}                                      

    return 0;
}