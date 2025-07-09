#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
class Player{
    //data members
public: 
//private: 
//protected:

 //Data members
    string name ;
    
    int score ;
    
    int health;

    //members function
    void showHealth (){
        cout<<endl<<name<<"'s Health is "<< health;
    }
    void showScore (){
        cout<<endl<<name<<"'s Score is "<< score;
    }

};


class student{
private:
string name ;
int standard ;
int rollno;
public:
void setName(string name){this->name=name;}
void setRollno(int rollno){this->rollno=rollno;}
void setStandard(int standard){this->standard=standard;}

void gettName(){cout<<endl<<name;}
void gettRollno(){cout<<endl<<rollno;}
void gettStandard(){cout<<endl<<standard;}





};
int main() {
//Player
    Player amit ;
    amit.name="amit";
    amit.health=100;
    amit.score=0;
    
    Player harsh;
    harsh.name="harsh";
    harsh.health=80;
    harsh.score=30;

amit.showHealth();
amit.showScore();

//Student
student raj;
raj.setName("raj");
raj.setStandard(10);
raj.setRollno(37);
raj.gettName();
raj.gettStandard();
raj.gettRollno();


    return 0;
}