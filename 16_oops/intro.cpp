#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


class Gun{
public :
int ammo ;
int damage;
int scope;
};
class helmet{

};


class Player{
    //data members
public: 
//private: 
//protected:

 //Data members
    Gun gun ;

    string name ;
    
    int score ;
    
    int health;

    //members function

    Gun getgun (){
return gun ;
    }
    void setGun(Gun gun ){
this->gun=gun;
    }


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

string gettName(){      return name;}
int gettRollno(){    return rollno;}
int gettStandard(){  return standard;}





};


int addScore(Player a ,Player b){
return a.score+b.score;
}
Player maxScore(Player a ,Player b){
return (a.score>=b.score)?a:b;
}
void changeScore(Player a ){ ;
a.score+=10;
cout<<endl<<"changed score "<<a.score;
}
int main() {
//Player
//amit is a object of class player
    Player amit ;
    amit.name="amit";
    amit.health=100;
    amit.score=10;
    
    Player harsh;
    harsh.name="harsh";
    harsh.health=80;
    harsh.score=30;

amit.showHealth();
amit.showScore();

//Student
student raj;//object creation , statically

raj.setName("raj");
raj.setStandard(10);
raj.setRollno(37);
cout<<endl<<raj.gettName();
cout<<endl<<raj.gettStandard();
cout<<endl<<raj.gettRollno();

cout<<endl<<addScore(amit,harsh);

amit.showScore();
changeScore(amit);//pass by value 
amit.showScore();

//max score 

// cout<<endl<<maxScore(amit,harsh).name;
//          or 
Player winner =maxScore(amit,harsh);
cout<<endl<<winner.name;

//dynamic allocation
Player *urvi =new Player; //memory allocation as run time 
urvi->name="urvi";
urvi->score=75;
urvi->health=30;


//
Gun akm ;
akm.ammo=45;
akm.damage=79;
akm.scope=2;


urvi->setGun(akm);
harsh.setGun(akm);
amit.setGun(akm);



    return 0;
}