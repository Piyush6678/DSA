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





    return 0;
}