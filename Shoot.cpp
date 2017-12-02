/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shoot.cpp
 * Author: Michael DeMoor
 *
 * Created on November 24, 2017, 10:57 AM
 * Most of this code is directly copied from the original program and adjusted
 * to achieve its required purpose here.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <cfloat>
#include <string.h>
#include <string>
#include <sstream>
#include <math.h>
#define G  0.0000000000667
#define INVALID_ARG_NUM 1
#define INVALID_ARGS 2
#define FILE_ERROR 3
#define PARSE_ERROR 4

using namespace std;

struct Position{
    double Rx;
    double Ry;
    double Rz;
    /*Position operator+(Position &A, Position &B){
    	Position C;
    	C.Rx = A.Rx + B.Rx;
    	C.Ry = A.Ry + B.Ry;
    	C.Rz = A.Rz + B.Rz;
    	return C;
    }
    Position operator-(Position &A, Position &B){
    	Position C;
    	C.Rx = A.Rx - B.Rx;
    	C.Ry = A.Ry - B.Ry;
    	C.Rz = A.Rz - B.Rz;
    	return C;
    }*/ 
};

struct Attributes{
    int ID;
    double Vx;
    double Vy;
    double Vz;
    double Rx;
    double Ry;
    double Rz;
};

struct Celestial_Body{
    string Name;
    float Mass;
    float Radius;
    Attributes Values;   
};

//Forward Declarations//
double Get_R(const vector<Attributes>&, const double);
vector<Attributes>& RK_Functions(vector<Attributes>&, vector<Attributes>&);
Position Acceleration(vector<Attributes>&, int);
Position Calculations(Position&, Position&, const float);
double Distance_Calc(const double, const double, const double);
Attributes Momentum(Attributes, Attributes);
void collision(const double, vector<Attributes>&);

//Global Simulation Variables//
vector<Celestial_Body> Celestial_Bodies;
vector<Attributes> ObjectTracker;
double Distance = DBL_MAX;
string Status = "Failure";
double TrackTime;

void Initialize_Sim(vector<Attributes> &K1, vector<Attributes> &K1Copy, vector<Attributes> &K2, vector<Attributes> &K2Copy,
        vector<Attributes> &K3, vector<Attributes> &K3Copy, vector<Attributes> &K4, vector<Attributes> &K4Copy, vector<Attributes> &K5, 
        vector<Attributes> &K5Copy, vector<Attributes> &K6, vector<Attributes> &Input, vector<Attributes> &Result, vector<Attributes> &Correction, 
        vector<Attributes> &Tracker){
    //Prepare the simulation vectors by clearing them and deallocating all previously allocated memory//
    K1.clear();
    K1.shrink_to_fit();
    K1Copy.clear();
    K1Copy.shrink_to_fit();
    K2.clear();
    K2.shrink_to_fit();
    K2Copy.clear();
    K2Copy.shrink_to_fit();    
    K3.clear();
    K3.shrink_to_fit();
    K3Copy.clear();
    K3Copy.shrink_to_fit();    
    K4.clear();
    K4.shrink_to_fit();
    K4Copy.clear();
    K4Copy.shrink_to_fit();    
    K5.clear();
    K5.shrink_to_fit();
    K5Copy.clear();
    K5Copy.shrink_to_fit();    
    K6.clear();
    K6.shrink_to_fit();
    Input.clear();
    Input.shrink_to_fit();
    Result.clear();
    Result.shrink_to_fit();
    Correction.clear();
    Correction.shrink_to_fit();
    
    //Initialize each K vector 
    K1 = Tracker;
    K2 = Tracker;
    K3 = Tracker;
    K4 = Tracker;
    K5 = Tracker;
    K6 = Tracker;  
}

//Overload + operator to work across all the Attributes values in the vectors//
vector<Attributes>& operator+(vector<Attributes> &One, vector<Attributes> &Two){
    for(int i = 0; i<One.size(); i++){
        One[i].Rx = One[i].Rx + Two[i].Rx;
        One[i].Ry = One[i].Ry + Two[i].Ry;
        One[i].Rz = One[i].Rz + Two[i].Rz;
        One[i].Vx = One[i].Vx + Two[i].Vx;
        One[i].Vy = One[i].Vy + Two[i].Vy;
        One[i].Vz = One[i].Vz + Two[i].Vz;  
    }
    return One;
}
//Same with - operator//
vector<Attributes>& operator-(vector<Attributes> &One, vector<Attributes> &Two){
    for(int i = 0; i<One.size(); i++){
        One[i].Rx = One[i].Rx - Two[i].Rx;
        One[i].Ry = One[i].Ry - Two[i].Ry;
        One[i].Rz = One[i].Rz - Two[i].Rz;
        One[i].Vx = One[i].Vx - Two[i].Vx;
        One[i].Vy = One[i].Vy - Two[i].Vy;
        One[i].Vz = One[i].Vz - Two[i].Vz;  
    }
    return One;
}
//Same with * operator//
vector<Attributes>& operator*(const double C, vector<Attributes> &One){
    int num = 0;
    for(Attributes i : One){
        One[num].Rx = C*i.Rx;
        One[num].Ry = C*i.Ry;
        One[num].Rz = C*i.Rz;
        One[num].Vx = C*i.Vx;
        One[num].Vy = C*i.Vy;
        One[num].Vz = C*i.Vz;
        num++;
    }
    return One;
}

void Final_Sim(double Time, double h, double hmax, double hmin, double e, double Wait, string Target, string Launch){
    //Simulation parameter declarations and instantiations//
    vector<Attributes> K1, K1Copy, K2, K2Copy, K3, K3Copy, K4, K4Copy, K5, K5Copy, K6, Input, Result, Correction;
    vector<Attributes> Tracker = ObjectTracker;
    double c[24]; c[0] = 0.25; c[1] = 3.0/32.0; c[2] = 9.0/32.0; c[3] = 1932.0/2197.0;
    c[4]= 7200.0/2197.0; c[5] = 7296.0/2197.0; c[6] = 439.0/216.0; c[7] = 8.0;
    c[8] = 3680.0/513.0; c[9] = 845.0/4104.0; c[10] = 8.0/27.0; c[11] = 2.0;
    c[12] = 3544.0/2565.0; c[13] = 1859.0/4104.0; c[14] = 11.0/40.0; c[15] = 25.0/216.0;
    c[16] = 1408.0/2565.0; c[17] = 2197.0/4104.0; c[18] = 0.2; c[19] = 16.0/135.0;
    c[20] = 6656.0/12825.0; c[21] = 28561.0/56430.0; c[22] = 9.0/50.0; c[23] = 2.0/55.0; 
    double R, D, hnext, Rad, Dist; 
    bool Trigger, Last; bool Tracking = true;;
    int Control, i, j, Size, Rocket_ID, Target_ID;
    int Rocketplace = -1; int Launchplace = -1; int Targetplace = -1; 
    Attributes C, T, RocketShip, Start;
    double t = 0.0;
       
    collision(t, Tracker);//Check to see if any object starts inside another object//
    Initialize_Sim(K1, K1Copy, K2, K2Copy, K3, K3Copy, K4, K4Copy, K5, K5Copy, K6, Input, Result, Correction, Tracker);//Initialize Simulation Vectors//

    //Check to see if Rocket, Launch Planet, and Target still exist and are not destroyed
    int Count = 0;
    bool Fire = true;
    for(Attributes Track : Tracker){
        if(Celestial_Bodies[Track.ID].Name == "Rocket"){
            Rocketplace = Count;
            Rocket_ID = Track.ID;
        }
        if(Celestial_Bodies[Track.ID].Name == Launch) Launchplace = Count;
        if(Celestial_Bodies[Track.ID].Name == Target){
            Targetplace = Count;
            Target_ID = Track.ID;
        }
        Count++;
    }
    if(Launchplace == -1){
        Wait = 0.0;
        Fire = false;
        Tracking = false;
    }
    if(Rocketplace == -1){
        Wait = 0.0;
        Fire = false;
        Tracking = false;
        
    }        
    if(Fire){
        RocketShip = Tracker[Rocketplace];
        Start = Tracker[Launchplace];
        Celestial_Bodies[Tracker[Launchplace].ID].Mass += Celestial_Bodies[Tracker[Rocketplace].ID].Mass;
        Tracker.erase(Tracker.begin() + Rocketplace);
        K1.clear(); K1.shrink_to_fit(); K1 = Tracker;
        K2.clear(); K2.shrink_to_fit(); K2 = Tracker;
        K3.clear(); K3.shrink_to_fit(); K3 = Tracker;
        K4.clear(); K4.shrink_to_fit(); K4 = Tracker;
        K5.clear(); K5.shrink_to_fit(); K5 = Tracker;
        K6.clear(); K6.shrink_to_fit(); K6 = Tracker;        
    }

    //1st Simulation Loop//
    while(t<Wait){//This code cannot be parallelized//
        Trigger = false;
        Last = false;
        //Runge-Kutta-Fehlberg Method. This is where the simulation magic happens.//
        while(true){
            if(Trigger)
                h = hnext;
            if(h == hmin)
                Last = true;
            K1 = RK_Functions(K1, Tracker);////////////Calculate K1/////////////////// 
            K1Copy = K1;
            Input = (c[0]*h)*K1 + Tracker;
            K2 = RK_Functions(K2, Input);///////////////Calculate K2/////////////////// 
            K2Copy = K2; K1 = K1Copy;          
            Input = (c[1]*h)*K1 + (c[2]*h)*K2 + Tracker;           
            K3 = RK_Functions(K3,Input);///////////////Calculate K3////////////////////
            K3Copy = K3; K1 = K1Copy; K2 = K2Copy;
            Input = (c[3]*h)*K1 - (c[4]*h)*K2 + (c[5]*h)*K3 + Tracker;                   
            K4 = RK_Functions(K4,Input);///////////////Calculate K4////////////////////
            K4Copy = K4; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy;
            Input = (c[6]*h)*K1 - (c[7]*h)*K2 + (c[8]*h)*K3 - (c[9]*h)*K4 + Tracker;
            K5 = RK_Functions(K5,Input);//////////////Calculate K5/////////////////////
            K5Copy = K5; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy; K4 = K4Copy; Result = Tracker;
            Input = Result - (c[10]*h)*K1 + (c[11]*h)*K2 - (c[12]*h)*K3 + (c[13]*h)*K4 - (c[14]*h)*K5;
            K6 = RK_Functions(K6,Input);///////////////Calculate K6////////////////////
            K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
            Result = (c[15]*h)*K1 + (c[16]*h)*K3 + (c[17]*h)*K4 - (c[18]*h)*K5 + Tracker;
            K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
            Correction = (c[19]*h)*K1 + (c[20]*h)*K3 + (c[21]*h)*K4 - (c[22]*h)*K5 + (c[23]*h)*K6 + Tracker;
            Correction = Correction - Result;
            R = Get_R(Correction, h);
            D = pow((e/(2*R)),0.25);
            hnext = D*h;
            if(hnext < hmin)
                hnext = hmin;
            if(R<=e)            
                break;
            if(Last)
                break;
            Trigger = true;        
        }
        if(hmax < hnext)
            hnext = hmax;
        t = t + h;
        h = hnext;
        Tracker = Result;

        //Check for collisions//
        i = 0;
        Size = Tracker.size();
        while(i < Size){//This code can be parallelized//
            Control = 1;
            C = Tracker[i];
            j = i + 1;
            while(j<Size){
                T = Tracker[j];
                Rad = Celestial_Bodies[C.ID].Radius + Celestial_Bodies[T.ID].Radius;
                Dist = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
                if(Dist<=Rad){//Collision Handling//
                    if(Celestial_Bodies[T.ID].Mass <= Celestial_Bodies[C.ID].Mass){
                       //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                       C = Momentum(C,T);
                       Tracker[i] = C;
                       //Decrease Rocket and Planet Tracker position Counts if necessary
                       if(j < Rocketplace) Rocketplace--;
                       if(j < Targetplace) Targetplace--;
                       if(j == Targetplace) Targetplace = -1;
                       if(j < Launchplace) Launchplace--;
                       if(j == Launchplace) Launchplace = -1;

                       //Remove T and decrease size of vectors in algorithm by 1//
                       Tracker.erase(Tracker.begin() + j);
                       K1.erase(K1.begin() + j); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + j); K3.erase(K3.begin() + j); K3Copy.pop_back();K4.erase(K4.begin() + j);
                       K4Copy.pop_back(); K5.erase(K5.begin() + j); K5Copy.pop_back(); K6.erase(K6.begin() + j); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                       Size = Tracker.size(); 
                    }
                    else{
                       //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                       T = Momentum(T,C);
                       Tracker[j] = T;                
                       if(i < Rocketplace) Rocketplace--;
                       if(i < Targetplace) Targetplace--;
                       if(i == Targetplace) Targetplace = -1;                       
                       if(i < Launchplace) Launchplace--;
                       if(i == Launchplace) Launchplace = -1; 

                       //Remove C and decrease size of vectors in algorithm by 1//
                       Tracker.erase(Tracker.begin() + i);
                       K1.erase(K1.begin() + i); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + i); K3.erase(K3.begin() + i); K3Copy.pop_back();K4.erase(K4.begin() + i);
                       K4Copy.pop_back(); K5.erase(K5.begin() + i); K5Copy.pop_back(); K6.erase(K6.begin() + i); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                       Size = Tracker.size();
                       Control = 0;
                       break;
                    }                 
                }
                else
                   j++;
            }
            if(Control)
               i++;
        } 
    }

    if(0<Wait){
        //Check to see if Rocket/Launch Planet/Target is still intact//
        if(Launchplace != -1){
            Celestial_Bodies[Tracker[Launchplace].ID].Mass -= Celestial_Bodies[RocketShip.ID].Mass;
            //Update position and velocity of RocketShip//
            RocketShip.Rx = RocketShip.Rx - Start.Rx + Tracker[Launchplace].Rx;
            RocketShip.Ry = RocketShip.Ry - Start.Ry + Tracker[Launchplace].Ry;
            RocketShip.Rz = RocketShip.Rz - Start.Rz + Tracker[Launchplace].Rz;

            RocketShip.Vx = RocketShip.Vx - Start.Vx + Tracker[Launchplace].Vx;
            RocketShip.Vx = RocketShip.Vx - Start.Vx + Tracker[Launchplace].Vx;
            RocketShip.Vx = RocketShip.Vx - Start.Vx + Tracker[Launchplace].Vx;
            //Reinsert RocketShip into ObjectTracker//
            Tracker.insert(Tracker.begin() + Rocketplace, RocketShip);//Consider replacing with push_back//
            //Update other vectors...
            K1.clear(); K1.shrink_to_fit(); K1 = Tracker;
            K2.clear(); K2.shrink_to_fit(); K2 = Tracker;
            K3.clear(); K3.shrink_to_fit(); K3 = Tracker;
            K4.clear(); K4.shrink_to_fit(); K4 = Tracker;
            K5.clear(); K5.shrink_to_fit(); K5 = Tracker;
            K6.clear(); K6.shrink_to_fit(); K6 = Tracker;
            
            if(Targetplace == -1) Tracking = false;
        }
        else Tracking = false;
    }

    //2nd Simulation Loop Post Rocket Launch//
    while(t<Time){
        Trigger = false;
        Last = false;
        //Runge-Kutta-Fehlberg Method//
        while(true){
            if(Trigger)
                h = hnext;
            if(h == hmin)
                Last = true;
            K1 = RK_Functions(K1, Tracker);////////////Calculate K1/////////////////// 
            K1Copy = K1;
            Input = (c[0]*h)*K1 + Tracker;
            K2 = RK_Functions(K2, Input);///////////////Calculate K2/////////////////// 
            K2Copy = K2; K1 = K1Copy;          
            Input = (c[1]*h)*K1 + (c[2]*h)*K2 + Tracker;           
            K3 = RK_Functions(K3,Input);///////////////Calculate K3////////////////////
            K3Copy = K3; K1 = K1Copy; K2 = K2Copy;
            Input = (c[3]*h)*K1 - (c[4]*h)*K2 + (c[5]*h)*K3 + Tracker;                   
            K4 = RK_Functions(K4,Input);///////////////Calculate K4////////////////////
            K4Copy = K4; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy;
            Input = (c[6]*h)*K1 - (c[7]*h)*K2 + (c[8]*h)*K3 - (c[9]*h)*K4 + Tracker;
            K5 = RK_Functions(K5,Input);//////////////Calculate K5/////////////////////
            K5Copy = K5; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy; K4 = K4Copy; Result = Tracker;
            Input = Result - (c[10]*h)*K1 + (c[11]*h)*K2 - (c[12]*h)*K3 + (c[13]*h)*K4 - (c[14]*h)*K5;
            K6 = RK_Functions(K6,Input);///////////////Calculate K6////////////////////
            K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
            Result = (c[15]*h)*K1 + (c[16]*h)*K3 + (c[17]*h)*K4 - (c[18]*h)*K5 + Tracker;
            K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
            Correction = (c[19]*h)*K1 + (c[20]*h)*K3 + (c[21]*h)*K4 - (c[22]*h)*K5 + (c[23]*h)*K6 + Tracker;
            Correction = Correction - Result;
            R = Get_R(Correction, h);
            D = pow((e/(2*R)),0.25);
            hnext = D*h;
            if(hnext < hmin)
                hnext = hmin;
            if(R<=e)            
                break;
            if(Last)
                break;
            Trigger = true;        
        }
        if(hmax < hnext)
            hnext = hmax;
        t = t + h;
        h = hnext;
        Tracker = Result;
        
        //Update Smallest distance between Rocket and Target if necessary//
        if(Tracking){
            C = Tracker[Rocketplace];
            T = Tracker[Targetplace];
            Dist = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
            if(Dist < Distance){
                Distance = Dist;
                TrackTime = t;
            }
        }
        
        //Check for collisions//
        i = 0;
        Size = Tracker.size();
        while(i < Size){
            Control = 1;
            C = Tracker[i];
            j = i + 1;          
            while(j<Size){
                T = Tracker[j];
                Rad = Celestial_Bodies[C.ID].Radius + Celestial_Bodies[T.ID].Radius;
                Dist = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
                if(Dist<=Rad){//Collision Handling//
                    if(Celestial_Bodies[T.ID].Mass <= Celestial_Bodies[C.ID].Mass){
                       //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                       C = Momentum(C,T);
                       Tracker[i] = C;
                       if(j < Rocketplace) Rocketplace--;
                       if(j == Rocketplace){
                           Rocketplace = -1;
                           Tracking = false;
                       }
                       if(j < Targetplace) Targetplace--;
                       if(j == Targetplace){
                           Targetplace = -1;
                           Tracking = false;
                       }
                       //Remove T and decrease size of vectors in algorithm by 1//
                       Tracker.erase(Tracker.begin() + j);
                       K1.erase(K1.begin() + j); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + j); K3.erase(K3.begin() + j); K3Copy.pop_back();K4.erase(K4.begin() + j);
                       K4Copy.pop_back(); K5.erase(K5.begin() + j); K5Copy.pop_back(); K6.erase(K6.begin() + j); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                       Size = Tracker.size(); 
                    }
                    else{
                       //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                       T = Momentum(T,C);
                       Tracker[j] = T;                  
                       if(i < Rocketplace) Rocketplace--;
                       if(i == Rocketplace){
                           Rocketplace = -1;
                           Tracking = false;
                       }
                       if(i < Targetplace) Targetplace--;
                       if(i == Targetplace){
                           Targetplace = -1;
                           Tracking = false;
                       }
                       
                       //Remove C and decrease size of vectors in algorithm by 1//
                       Tracker.erase(Tracker.begin() + i);
                       K1.erase(K1.begin() + i); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + i); K3.erase(K3.begin() + i); K3Copy.pop_back();K4.erase(K4.begin() + i);
                       K4Copy.pop_back(); K5.erase(K5.begin() + i); K5Copy.pop_back(); K6.erase(K6.begin() + i); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                       Size = Tracker.size();
                       Control = 0;
                       break;
                    }                 
                }
                else
                   j++;
            }
            if(Control)
               i++;
        }                  
    }
    
    if(Distance < Celestial_Bodies[Rocket_ID].Radius + Celestial_Bodies[Target_ID].Radius) Status = "Success";  
}

double Get_R(const vector<Attributes>& Correction, const double h){
    double R = 0.0;
    double M1,M2;
    //Find and obtain the largest magnitude out of every position and velocity vector in the entire list//Max Norm 
    for(Attributes V : Correction){
        M1 = sqrt(pow(V.Rx,2) + pow(V.Ry,2) + pow(V.Rz,2));
        M2 = sqrt(pow(V.Vx,2) + pow(V.Vy,2) + pow(V.Vz,2));
        if(R < M1 || R < M2)
            R = (M1<M2)? M2:M1;
    }
    R = R/h;
    return R;//Return the result divided by the timestep//
}

vector<Attributes>& RK_Functions(vector<Attributes> &Temp, vector<Attributes> &Alt){//This code can be parallelized// 
    Position Result;
    //Implementing the functions calculating the components of the K values in the RKF45 algorithm// 
    for(int Num = 0; Num<Alt.size(); Num++){        
        Result = Acceleration(Alt, Num);
        Temp[Num].Vx = G*Result.Rx;
        Temp[Num].Vy = G*Result.Ry;
        Temp[Num].Vz = G*Result.Rz;
        Temp[Num].Rx = Alt[Num].Vx;
        Temp[Num].Ry = Alt[Num].Vy;
        Temp[Num].Rz = Alt[Num].Vz;
    }
    return Temp;
}


Position operator+(const Position &A, const Position &B){
    Position C;
    C.Rx = A.Rx + B.Rx;
    C.Ry = A.Ry + B.Ry;
    C.Rz = A.Rz + B.Rz;
    return C;
}

Position operator-(const Position &A, const Position &B){
    Position C;
    C.Rx = A.Rx - B.Rx;
    C.Ry = A.Ry - B.Ry;
    C.Rz = A.Rz - B.Rz;
    return C;
}


Position Acceleration(vector<Attributes>& Alt, int Num){
    int Curr = 0;
    Position Rnum, Rcurr, Result;
    //Initialize Position Data Structures: Result and Rnum//
    Result.Rx = 0.0;
    Result.Ry = 0.0;
    Result.Rz = 0.0;
    Rnum.Rx = Alt[Num].Rx;
    Rnum.Ry = Alt[Num].Ry;
    Rnum.Rz = Alt[Num].Rz;
    //Calculate the acceleration of Object Num due to the gravitational forces of all other objects// 
    while(Curr<Alt.size()){
        if(Curr != Num){
            Rcurr.Rx = Alt[Curr].Rx;
            Rcurr.Ry = Alt[Curr].Ry;
            Rcurr.Rz = Alt[Curr].Rz;
            Result = Result + Calculations(Rnum, Rcurr, Celestial_Bodies[Alt[Curr].ID].Mass);
            }
        Curr++;
    }       
    return Result;
}


Position Calculations(Position &Num,Position &Curr,const float Mass){
    Position R;
    double Distance;
    //Calculating the acceleration of Object Num due to the gravitational force from Object Curr on Object Num//
    R = Curr - Num;
    Distance = Distance_Calc(R.Rx,R.Ry,R.Rz);
    Distance = pow(Distance,3);
    R.Rx = (Mass/Distance)*R.Rx;
    R.Ry = (Mass/Distance)*R.Ry;
    R.Rz = (Mass/Distance)*R.Rz;
    return R;
}

double Distance_Calc(const double X, const double Y,const double Z){
   return sqrt(X*X + Y*Y + Z*Z);//Return the magnitude of a vector//    
}

Attributes Momentum(Attributes One, Attributes Two){
    double M1,M2;
    float Mass1 = Celestial_Bodies[One.ID].Mass;
    float Mass2 = Celestial_Bodies[Two.ID].Mass;
    //Calculating the resulting velocity of Object One due to the inelastic collision of Object Two by using conservation of momentum//
    M1 = Mass1/(Mass1 + Mass2);
    M2 = Mass2/(Mass1 + Mass2);
    One.Vx = M1*One.Vx + M2*Two.Vx;
    One.Vy = M1*One.Vy + M2*Two.Vy;
    One.Vz = M1*One.Vz + M2*Two.Vz;
    Celestial_Bodies[One.ID].Mass = Mass1 + Mass2;
    return One;    
}

void collision(const double t, vector<Attributes> &Tracker){
     int Control;
     double R,D;
     Attributes C, T;
     int i = 0;
     int j;
     int Size = Tracker.size();
     while(i < Size){
         Control = 0;
         C = Tracker[i];
         j = i + 1;
         while(j<Size){
             T = Tracker[j];
             R = Celestial_Bodies[C.ID].Radius + Celestial_Bodies[T.ID].Radius;
             D = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
             if(D<=R){
                 if(Celestial_Bodies[T.ID].Mass <= Celestial_Bodies[C.ID].Mass){
                    //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                    C = Momentum(C,T);
                    Tracker[i] = C;
                    //Remove T//
                    Tracker.erase(Tracker.begin() + j);
                    Size = Tracker.size(); 
                 }
                 else{
                    //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                    T = Momentum(T,C);
                    Tracker[j] = T;
                    //Remove C//
                    Tracker.erase(Tracker.begin() + i);
                    Size = Tracker.size();
                    Control = 1;
                    break;
                 }                 
             }
             else
                j++;
         }
         if(Control == 0)
            i++;
     }        
}

bool verify_input(string Input){
    int DotCount = 0;
    int eCount = 0;
    int PlusCount = 0;
    int MinusCount = 0;
    int Begin = 0;
    locale loc;
    if(Input[0] == '-')
        Begin = 1;
    for(int i = Begin; i < Input.length(); i++){
        char C = Input[i];
        if(!isdigit(C,loc) && C != '.' && C != 'e' && C != '+' && C != '-')
            return false;
        if(C == '.')
            DotCount++;        
        if(C == 'e')
            eCount++;
        if(C == '+')
            PlusCount++;
        if(C == '-')
            MinusCount++;
    }
    if(1 < DotCount || 1 < eCount || 1 < PlusCount || 1 < MinusCount)
        return false;
    return true;  
}

double convert(string x){
    double num = 0.0;
    stringstream convert(x);
    convert >> num;
    return num;
}

int Parser(ifstream &Config){
    //Throw objects onto stack
    string IN, Arg;
    int Pos, Old_Pos;
    int Count = 0;
    
    Celestial_Body Celestial;
    while(Config >> IN){
        Pos = IN.find("Mass", 0);
        Arg = IN.substr(7, Pos-7);
        Celestial.Name = Arg;
        
        Old_Pos = Pos;
        Pos = IN.find("Radius", Pos);
        Arg = IN.substr(Old_Pos+5, Pos - (Old_Pos+5));
        if(verify_input(Arg)) Celestial.Mass = convert(Arg);
        else return -1;
        
        Old_Pos = Pos;
        Pos = IN.find("Rx", Pos);
        Arg = IN.substr(Old_Pos+7, Pos - (Old_Pos+7));
        if(verify_input(Arg)) Celestial.Radius = convert(Arg);
        else return -1;

        Old_Pos = Pos;
        Pos = IN.find("Ry", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Rx = convert(Arg);
        else return -1;        
        
        Old_Pos = Pos;
        Pos = IN.find("Rz", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Ry = convert(Arg);
        else return -1;

        Old_Pos = Pos;
        Pos = IN.find("Vx", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Rz = convert(Arg);
        else return -1;        

        Old_Pos = Pos;
        Pos = IN.find("Vy", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Vx = convert(Arg);
        else return -1;        
        
        Old_Pos = Pos;
        Pos = IN.find("Vz", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Vy = convert(Arg);
        else return -1;  
        
        Old_Pos = Pos;
        Pos = IN.find("//", Pos);
        Arg = IN.substr(Old_Pos+3, Pos - (Old_Pos+3));
        if(verify_input(Arg)) Celestial.Values.Vz = convert(Arg);
        else return -1;

        Celestial.Values.ID = Count;
        Celestial_Bodies.push_back(Celestial);
        ObjectTracker.push_back(Celestial.Values);       
        Count++;
    }
    return 0;
}

int main(int argc, char** argv) {
    double Time, h, hmin, hmax, e, Wait, Theta, Phi, RMass, RRadius, RSpeed;
    string Launch, Target;
    string IN;
    
    if(argc != 5)
        return INVALID_ARG_NUM;
    for(int i=1; i<5; i++){//Needs verification//
        if(!verify_input(argv[i])) return INVALID_ARGS;
    }

    ifstream Config("Simulation.conf", ios::in);
    if(!Config)
        return FILE_ERROR;
    
    //Sort Command Line Inputs
    Wait = convert(argv[2]);
    Time = convert(argv[1]) + Wait;
    Theta = convert(argv[3]);
    Phi = convert(argv[4]);
        
    //Parse Simulation.conf file and create simulation profile//
    string Arg;
    int Pos, Old_Pos;
    Config >> IN;
    
    //Get h//
    Pos = IN.find("hmin", 6);
    Arg = IN.substr(5, Pos-5);
    if(verify_input(Arg)) h = convert(Arg);
    else return PARSE_ERROR;
    
    //Get hmin//
    Old_Pos = Pos;
    Pos = IN.find("hmax", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) hmin = convert(Arg);
    else return PARSE_ERROR;
            
    //Get hmax//
    Old_Pos = Pos;
    Pos = IN.find("etol", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) hmax = convert(Arg);
    else return PARSE_ERROR;
    
    //Get e//
    Old_Pos = Pos;
    Pos = IN.find("Mass", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) e = convert(Arg);
    else return PARSE_ERROR;
    
    //Get Rocket Mass//
    Old_Pos = Pos;
    Pos = IN.find("Radi", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) RMass = convert(Arg);
    else return PARSE_ERROR;
    
    //Get Rocket Radius//
    Old_Pos = Pos;
    Pos = IN.find("Sped", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) RRadius = convert(Arg);
    else return PARSE_ERROR;
    
    //Get Rocket Speed//
    Old_Pos = Pos;
    Pos = IN.find("Lnch", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    if(verify_input(Arg)) RSpeed = convert(Arg);
    else return PARSE_ERROR;    
    
    //Get Launch Planet//
    Old_Pos = Pos;
    Pos = IN.find("Trgt", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    Launch = Arg;
    
    //Get Target Planet//
    Old_Pos = Pos;
    Pos = IN.find("###", Pos);
    Arg = IN.substr(Old_Pos+5, Pos-(Old_Pos+5));
    Target = Arg;    
    
    if(Parser(Config) < 0) return PARSE_ERROR;

    //Add Rocket//
    Celestial_Body Celestial, Launch_Planet;
    for(Celestial_Body Object : Celestial_Bodies){
        if(Object.Name == Launch){
            Launch_Planet = Object;
            break;
        }        
    }
        
    Celestial.Name = "Rocket";
    Celestial.Mass = RMass;
    Celestial.Radius = RRadius;
    
    //Translate spherical angles into a launch position on launch planet//
    double r = Launch_Planet.Radius + Celestial.Radius + 1;//Plus Radius of Rocket to avoid colliding with launch planet at start of simulation//Needs verification
    double x = r*cos(Phi)*cos(Theta);
    double z = -r*cos(Phi)*sin(Theta);
    double y = r*sin(Phi);
    
    Celestial.Values.Rx = Launch_Planet.Values.Rx + x;
    Celestial.Values.Ry = Launch_Planet.Values.Ry + y;
    Celestial.Values.Rz = Launch_Planet.Values.Rz + z;
    
    //Translate Launch Speed into launch velocity//
    double vx = RSpeed*cos(Phi)*cos(Theta);
    double vy = RSpeed*sin(Phi);
    double vz = -RSpeed*cos(Phi)*sin(Theta);
    
    Celestial.Values.Vx = Launch_Planet.Values.Vx + vx;
    Celestial.Values.Vy = Launch_Planet.Values.Vy + vy;
    Celestial.Values.Vz = Launch_Planet.Values.Vz + vz;    
    
    Celestial.Values.ID = Celestial_Bodies.size();//Needs Verification
    Celestial_Bodies.push_back(Celestial);
    ObjectTracker.push_back(Celestial.Values);
       
    //Run simulation
    Final_Sim(Time, h, hmax, hmin, e, Wait, Target, Launch);
    
    //Upload Result to file//
    ofstream Result("Result.txt", ios::out);
    if(!Result) return FILE_ERROR;
    if(Distance < DBL_MAX)
        Result << Distance << " " << TrackTime << " " << Status << endl;
    else Result << "These input paramters suck!" << endl;
    
    Config.close();
    Result.close();    
    return 0;
}

