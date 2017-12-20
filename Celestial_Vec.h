/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Celestial_Vec.h
 * Author: Michael DeMoor
 *
 * Created on September 2, 2017, 3:10 AM
 */

#ifndef CELESTIAL_VEC_H
#define CELESTIAL_VEC_H
#include <memory>
#include "Attributes.h"

class Celestial_Body;
typedef std::shared_ptr<Celestial_Body> CelestialPtr;

class Generator{
    public:
        Generator();
        void Set_Defaults();
        int Rand_Orbit_Gen(Attributes*, float&, float&, std::vector<CelestialPtr>&,const std::string);
   
    private:
        void Initialize();
        //Settings Support Functions//
        void print_settings();
        void print_User_Settings();
        void Set_Settings(bool Def);
        void Set_Body(bool Def);
        void Set_Axis(bool Def);
        void Set_Eccentric(bool Def);
        void Set_Radius(bool Def);
        void Set_Position(bool Def);
        void Set_Mass(bool Def);
        
        //Orbit Generator Support Functions//
        float Gen_Mass(float*);
        float Gen_Radius();
        float Gen_Eccent();
        int Gen_Position(std::vector<CelestialPtr>&, float, double*, double*);
        void Specify_Path(std::vector<double*>&, double*, float*, float, float&, double&);
        int Point_Sampler(std::vector<double*>&, float*, float*);
        float get_rotation(double*);
        void Rotate(double*, float);
        double get_Error(std::vector<double*>&, float, float, double);
        int LS_Calculations(std::vector<double*>&, double*, float);
        int Least_Squares(double*, double*, std::vector<double*>&, float);
        double SLRec_Calc(const float, const double*, const float);
        float get_theta(double, double, double);
        bool Verify_Mass(float*, float*);
        Options Default;//Default Settings
        Options Active;//Active Settings
};

class Celestial{
    friend void Init_Sim(Celestial*);   
    public:
        Celestial(const std::string);
        void Add_Object();
        void Remove_Object();
        void Alter_Object();
        void Change_Settings();
        void Simulate();        
        void Upload();
        void Download();
        int Set_Parameters();        
        void Final_Sim(const std::string);        
        void set_Rocket(const int, const bool);
        void View_Object() const;
        void View_Objects() const;
        void View_Start_Objects() const;
        bool get_Rocket(const int) const;
        Sim_Param get_Parameters() const;
        std::string get_Launch_Planet() const;
        std::string get_name() const;
        
        //Public Celestial Attributes//
        std::vector<CelestialPtr> Celestial_Bodies;
        std::vector<Attributes> ObjectTracker;     
        int SpaceCount; 
    private:
        int alter_Rocket();
        int add_Rocket(Attributes&, float&, float&);
        int get_position(const std::string);
        void Count_Decr(int);        
        Generator Orbit_Gen;
        std::string Sim_Name;
        //Default Simulation Parameters//
        Sim_Param Default;
        struct Rocket{
            bool Exist;//Rocket exists and has not been destroyed
            bool Valid;//Default Simulation Parameters are up to date
            bool Launched;//Rocket is Launched
            std::string Launch;
            Rocket() : Exist(false), Valid(true), Launched(false), Launch("Earth"){}
        };
        Rocket Ship;
};


#endif /* CELESTIAL_VEC_H */

