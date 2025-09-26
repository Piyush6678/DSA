#include <iostream>
#include<unordered_map>
using namespace std;

int main() {
    // pair<int,int>p;
    // p.first=1;
    // p.second=2;

    unordered_map<string,int> student;
    pair<string,int> p1;
    p1.first="Piyush";
    p1.second=23;
    student.insert(p1); 
    student["Raghav"]=56;
    student["Mridul"]=6;

// for(pair<string, int> p : student)
// {
//     cout<<"roll no. of "<<p.first<<" is " << p.second;

// }

for(auto p : student)
{
    cout<<"roll no. of "<<p.first<<" is " << p.second;

}

student.erase("Raghav"); // take keys
if(student.find("Piyush")!=student.end()){//code}

    return 0;
}