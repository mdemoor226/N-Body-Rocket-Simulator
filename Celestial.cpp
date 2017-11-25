/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <time.h>
#include <fstream>
#include "TrackingSystem.h"

using namespace std;

//Temporarily save currently loaded simulation profile//
void Save_Sim(vector<Celestial> &Sims, Celestial &Active){
    int Count = 0;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == Active.get_name()){
            Sims[Count] = Active;
            break;
        }
        Count++;
    }
    cout << "Saved" << endl;
}

void Load_Sim(vector<Celestial> &Sims, Celestial &Active){
    cout << "Warning, loading in a new simulation will overwrite the current one. Would you like to save the current one first?\n";
    string response = yes_no();
    if(response == "yes")Save_Sim(Sims, Active);
    
    cout << "Enter the name of the simulation you would like to load.\n";
    cin >> response;
    bool fake = true;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == response){
            Active = Sim;
            fake = false;
            break;
        }
    }
    if(fake) cout << "Error, simulation profile does not exist." << endl;
    else cout << "Done." << endl;
}

void Add_Sim(vector<Celestial> &Sims){
    cout << "What is the name of the new simulation.\n";
    string Name;
    cin >> Name;
    Sims.push_back(Celestial(Name));
    cout << "Done." << endl;
}

void Remove_Sim(vector<Celestial> &Sims){
    cout << "Enter the name of the simulation you would like to remove.\n";
    string Name;
    cin >> Name;
    bool fake = true;
    int Count = 0;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == Name){
            Sims.erase(Sims.begin() + Count);
            fake = false;
            break;
        }
        Count++;
    }
    if(fake) cout << "Error, simulation profile does not exist." << endl;
    else cout << "Done." << endl;
}

void View_Sims(vector<Celestial> &Sims){
    cout << "Current Simulations:\n";
    int Count = 1;
    for(Celestial Sim : Sims){
        cout << "Simulation #" << Count << ": " << Sim.get_name() << "\n";
        Count++;
    }
}

bool exit(Celestial &Active){
    //Check if Rocket exists, sim parameters are valid, and has not been launched//
    if(!Active.get_Rocket(0)){
        cout << "Error, Rocket either doesn't exist or was destroyed in a simulation. Please make sure Rocket is valid and up to date." << endl;
        return false;
    }
    if(Active.get_Rocket(1)){
        cout << "Error, Rocket has already been launched. Make sure Rocket is ready to launch and try again." << endl;
        return false;
    }
    if(!Active.get_Rocket(2)){
        cout << "Error, Default simulation parameters are invalid. Set proper Default simulation parameters to use for each simulation and try again." << endl;
        return false;
    }
    
    //Make sure there are no destroyed objects in Celestial_Vec and calculate the Launch Speed of the Rocket//   
    double Speed, Mass, Radius;
    bool Destroyed = false;
    for(CelestialPtr Object : Active.Celestial_Bodies){
        if(Object->get_Name() == "Rocket"){
            Attributes Ship = Object->get_attributes();
            Speed = sqrt(Ship.Vx*Ship.Vx + Ship.Vy*Ship.Vy + Ship.Vz*Ship.Vz);
            Mass = Object->get_Mass();
            Radius = Object->get_Radius();
        }
        if(Get_Status(Object) == "Destroyed"){
            Destroyed = true;
            break;
        }
    }
    if(Destroyed){
        cout << "Error, there are one or more destroyed objects in the simulation profile. Please remove them and try again." << endl;
        return false;
    }
    
    //Display final sim profile and rocket parameters to user//
    cout << "Final Simulation Profile:\n";
    Active.View_Objects();
    cout << "Launch Planet: " << Active.get_Launch_Planet() << endl;
    cout << "Rocket Launch Speed: " << Speed << endl;
    
    //Ask if they would like to continue//
    cout << "Are these values good?\n";
    if(yes_no() == "no"){
        cout << "Canceling" << endl;
        return false;
    }
    
    //Ask user to specify a target planet//
    string Target;
    cout << "Please specify a Target Planet/Object to shoot for.\n";
    while(true){
        cin >> Target;
        bool Exist = false;
        for(CelestialPtr Object : Active.Celestial_Bodies){
            if(Object->get_Name() == Target){
                Exist = true;
                break;
            }
        }
        if(Exist)break;
        cout << "Error, Object does not exist. Please specify another one." << endl;
    }
      
    //Ask user if they would like to run simulation one last time to verify their profile
    cout << "Would you like to run this simulation profile one last time to test it out?\n";
    if(yes_no() == "yes"){
        Active.Final_Sim(Target);
        cout << "Are you fine with these results?\n";
        if(yes_no() == "no"){
            cout << "Canceling." << endl;
            return false;
        }
    }

    //Upload Simulation information to a configuration file//
    Sim_Param Final = Active.get_Parameters();
    ofstream Config("Simulation.conf", ios::out);   
    if(!Config){
        cerr << "Error, could not open file to upload simulation profile to. Exiting program." << endl;
        return false;
    }
    
    Config.precision(25);
    Config << "###h:" << Final.h << "hmin:" << Final.hmin << "hmax:" << Final.hmax << "etol:" << Final.e << "Mass:" << Mass << "Radi:" << Radius
            << "Sped:" << Speed << "Lnch:" << Active.get_Launch_Planet() << "Trgt:" << Target << "###";
    
    for(CelestialPtr Object : Active.Celestial_Bodies){
        if(Object->get_Name() != "Rocket"){
            double mass = Object->get_Mass();
            double radius = Object->get_Radius();
            Attributes V = Object->get_attributes();
            Config << " //Name:" << Object->get_Name() << "Mass:" << mass << "Radius:" << radius << "Rx:" << V.Rx << "Ry:" << V.Ry << "Rz:" << V.Rz
                    << "Vx:" << V.Vx << "Vy:" << V.Vy << "Vz:" << V.Vz << "//";
        }       
    }
    Config.close();
    
    cout << "Upload Successful. Goodbye." << endl;
    return true;
}

//User needs to specify launch planet and rocket/simulation parameters in this program
int main(){
    srand(static_cast<unsigned int>(time(0)));  
    Celestial Sim_Init("Default");
    Celestial Active_Sim = Sim_Init; 
    Init_Sim(&Active_Sim);
    vector<Celestial> Sims = {Active_Sim};  
    
    string In;
    cout << "Build your simulation." << endl;  
    while(true){
        cin >> In;
        In = str_lower(In);
             
        if(In == "set-o")
            Active_Sim.Change_Settings();
        else if(In == "set-s")
            Active_Sim.Set_Parameters();
        else if(In == "add")
            Active_Sim.Add_Object(&Active_Sim);  
        else if(In == "remove")
            Active_Sim.Remove_Object();
        else if(In == "display")
            Active_Sim.View_Object();
        else if(In == "display-a")
            Active_Sim.View_Objects();
        else if(In == "display-s")
            Active_Sim.View_Start_Objects();
        else if(In == "alter")
            Active_Sim.Alter_Object();
        else if(In == "simulate")
            Active_Sim.Simulate();
        else if(In == "load")
            Load_Sim(Sims, Active_Sim);
        else if(In == "save")
            Save_Sim(Sims, Active_Sim);
        else if(In == "new")
            Add_Sim(Sims);
        else if(In == "delete")
            Remove_Sim(Sims);
        else if(In == "view")
            View_Sims(Sims);
        else if(In == "help")
            cout << "View      : Display a list of current simulation profiles.\nNew       : Create a new simulation profile.\n"
                    "Delete    : Delete a simulation profile.\nSave      : Save the current simulation profile.\nLoad      : Load in a different simulation profile.\n"
                    "Add       : Add objects to the system.\nRemove    : Remove an object from the system.\n"
                    "Alter     : Change the values/attributes of an object.\nDisplay    : Display the attributes of a specific object.\n"
                    "Display-a : Display the attributes of every object.\nDisplay-s : Display the original attributes each object was created with.\nSimulate  : Run a simulation."
                    "\nSet-s     : Set the Default Simulation Parameters.\nSet-o     : View and alter the Orbit Generator Settings.\nHelp      : List the commands.\n"
                    "Exit      : Exit the program and upload your chosen simulation profile into a configuration file.\nQuit       : Directly quit the program" << endl;                 
        else if(In == "exit"){
            if(exit(Active_Sim)) break;
        }
        else if(In == "quit")
            break;
        else
            cout << "Error, Invalid input. Type 'Help' for a list of commands." << endl;
        cout << endl;
    }
}


//terrific pear 941 !