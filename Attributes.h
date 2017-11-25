/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Attributes.h
 * Author: Owner
 *
 * Created on November 3, 2017, 12:10 AM
 */

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <stdlib.h>

#define pi 3.141592653589793
#define G  0.0000000000667 //Universal Gravitation Constant//
#define POINT_FAIL 0
#define POINT_SUCCESS 1
#define POINT_EMPTY 2

struct Attributes{//Might consider adding mass and radius back into this structure//
    int ID;
    double Vx;
    double Vy;
    double Vz;
    double Rx;
    double Ry;
    double Rz;
};

struct Position{
    double Rx;
    double Ry;
    double Rz;
};
//To do list: Get rid of User type and replace with int codes//
typedef enum {Zero, Partial, Full}User;
typedef struct{//Define random settings//
    User IMass;
    User IPosition;
    User IRadius;
    User IEccentric;
    bool Axis;    
    bool Perihelion;
    bool Body;
    bool Sampler;
}Settings;

struct iMass{
    int Min;
    int Max;
    int Decimal;
    int Powermax;
    int Powermin;
};
//Same with Sign type//
typedef enum {Yes, No, Maybe}Sign;
struct iPosition{
    int Minx;
    int Miny;
    int Minz;
    int Maxx;
    int Maxy;
    int Maxz;
    int Powerxmax;
    int Powerxmin;
    int Powerymax;
    int Powerymin;
    int Powerzmax;
    int Powerzmin;
    int Decimalx;
    int Decimaly;
    int Decimalz;
    Sign X;
    Sign Y;
    Sign Z;
};

struct iRadius{
    int Min;
    int Max;
    int Powermax;
    int Powermin;
    int Decimal;
};

//Settings//
struct Options{
    Settings Set;
    iMass Mass_Val;
    iPosition Pos_Val;
    iRadius Rad_Val;
    int Ecc_Dec;
    int Ecc_Range;
    std::string Orbit;
    char Axis;   
};

struct Sim_Param{
    double Time;
    double h;
    double hmin;
    double hmax;
    double e;
    double Wait;
};

//Input verification functions//
std::string str_lower(const std::string);
std::string yes_no();
double convert(std::string);
bool verify_input(std::string);
std::string verify_double();
bool verify_siminput(std::string);
std::string verify_simdouble();
bool verify_altinput(std::string);
std::string verify_altdouble();
bool verify_pointinput(std::string Input);
std::string verify_pointdouble();

#endif /* ATTRIBUTES_H */

