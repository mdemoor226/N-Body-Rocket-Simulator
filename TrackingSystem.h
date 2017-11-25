/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrackingSystem.h
 * Author: Michael DeMoor
 *
 * Created on March 28, 2017, 6:35 PM
 */

#ifndef TRACKINGSYSTEM_H
#define TRACKINGSYSTEM_H
#include "Celestial_Vec.h"

class Celestial_Body{friend class Celestial;
    friend std::string Get_Status(CelestialPtr); 
    public:
        Celestial_Body(const std::string, const float, const Attributes, const float, Celestial*);             
        void Display_Objects() const;
        void display_Object() const;
        void Final_Sim(double,double,double,double,double,double,std::string);
        void Simulate_Motion(double,double,double,double,double,double);

        //Some Gets//
        Attributes get_attributes() const;                       
        std::string get_Name() const;
        float get_Radius() const;
        float get_Mass() const;
        
    private:      
        void update_attributes();
        void Remove();        
        void print_attributes(Attributes, std::string, float, float) const;              
        //Simulation Support Functions//
        void Initialize_Sim(
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, 
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&,
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&,
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&
        );
        std::vector<Attributes>& RK_Functions(std::vector<Attributes>&,std::vector<Attributes>&);
        Position Acceleration(std::vector<Attributes>&, int);
        Position Calculations(Position&, Position&, const float);        
        double Distance_Calc(const double,const double,const double);
        double Get_R(const std::vector<Attributes>&, const double);
        void collision(const double, std::vector<Attributes>&);
        Attributes Momentum(Attributes, Attributes);

        //Data Members//
        Celestial *Celestial_Vec;        
        std::string Name;
        Attributes Values;
        std::string Status;//Change to bool//
        float Mass;
        float Radius; 
        //Initial Data Members//
        Attributes Start;
        float S_Mass;
        float S_Radius;
};


#endif /* TRACKINGSYSTEM_H */
