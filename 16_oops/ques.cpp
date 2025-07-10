#include <iostream>
using namespace std;
class Cricketer{
    public:
string name;
    int age;
    int noTestMatches;
    int avgScore;

};
int main() {
    Cricketer virat;
    Cricketer rohit;
    Cricketer cricketers[2]={virat,rohit};
cricketers[0].name="virat";
cricketers[0].noTestMatches  =63;
cricketers[0].avgScore=53;
cricketers[1].name="rohit";
cricketers[1].noTestMatches  =73;
cricketers[1].avgScore=63;

    return 0;
}