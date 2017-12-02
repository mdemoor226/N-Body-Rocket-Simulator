/* final, goto, restrict
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TrackingSystem.h"

using namespace std;

Celestial::Celestial(const string Name) : Sim_Name(Name){
    SpaceCount = 0;
    Default.Time = -1;
}

void Celestial::Change_Settings(){
    Orbit_Gen.Set_Defaults();
}

int Celestial::Set_Parameters(){
    string IN;
    double Time, h, hmax, hmin, e, Wait;
    cout << "Specify the default time length for simulations\n";
    IN = str_lower(verify_simdouble());
    if(IN == "cancel"){
        cout << "Canceled" << endl;
        return -1;
    }
    Time = convert(IN);
    
    cout << "Specify the default initial time step for simulations\n";
    IN = str_lower(verify_simdouble());
    if(IN == "cancel"){
        cout << "Canceled" << endl;
        return -1;
    }    
    h = convert(IN);
    
    cout << "Specify the default max time step for simulations\n";
    IN = str_lower(verify_simdouble());
    if(IN == "cancel"){
        cout << "Canceled" << endl;
        return -1;
    }    
    hmax = convert(IN);
    
    cout << "Specify the default min time step for simulations\n";
    IN = str_lower(verify_simdouble());
    if(IN == "cancel"){
        cout << "Canceled" << endl;
        return -1;
    }
    hmin = convert(IN);
    
    cout << "Specify the default error tolerance for simulations\n";
    IN = str_lower(verify_simdouble());
    if(IN == "cancel"){
        cout << "Canceled" << endl;
        return -1;
    }    
    e = convert(IN);
 
    //Check to see if rocket launch conditions are valid//
    if(Ship.Exist && !Ship.Launched){
        cout << "Please specify the length in time to wait for the rocket launch.\n";
        while(true){
            IN = verify_simdouble();
            if(str_lower(IN) == "cancel"){
                cout << "Canceled" << endl;
                return -1;
            }
            if(convert(IN) <= Time/* && convert(IN) != 0.0*/)break;//Needs verification
            cout << "Error, wait time must be less than simulation time. Sim time: " << Time << endl;
        }
        Wait = convert(IN);
    }
    else Wait = 0.0;
    
    Default.Time = Time;
    Default.h = h;
    Default.hmax = hmax;
    Default.hmin = hmin;
    Default.e = e;
    Default.Wait = Wait;
    
    //Update Rocket Paramters//
    Ship.Valid = true;
    
    cout << "Done" << endl;
    return 0;
}

int Celestial::add_Rocket(Attributes &Celestial, float &Mass, float &Radius){
    CelestialPtr Launch;
    string IN;
    cout << "Default Launch Planet is " << Ship.Launch << ". Is this ok?\n";
    if(yes_no() == "no"){
    	cout << "Enter Launch Planet Name\n";
    	cin >> Ship.Launch;
    }

    //Check to see if launch planet currently exists and is not destroyed
    while(true){
        bool exist = false;
        for(CelestialPtr Planet : Celestial_Bodies){
            if(Planet->Name == Ship.Launch && Planet->Status == "Intact"){
                Launch = Planet;
                exist = true;
                break;
            }
        }
        if(exist)break;
        cout << "Error, Launch planet either doesn't exist or was destroyed in a simulation. Please specify a nem one." << endl;
        cin >> IN;
	if(str_lower(IN) == "cancel") return 1;
	Ship.Launch = IN;
    }
    
    //Receive Mass and Radius of Rocket from user
    cout << "\nPlease enter Mass value for the Rocket.\n";
    while(true){
        IN = verify_double();
        if(str_lower(IN) == "done" || 0 < convert(IN))break;
        cout << "Invalid value. Enter a positive mass value" << endl;
    }
    if(str_lower(IN) == "done")return 1;
    Mass = convert(IN);
    
    cout << "\nPlease enter Radius value for the Rocket.\n";
    while(true){
        IN = verify_double();
        if(str_lower(IN) == "done" || 0 < convert(IN))break;
        cout << "Invalid value. Enter a positive radius value\n";
    }
    if(str_lower(IN) == "done")return 1; 
    Radius = convert(IN);   
    
    float phi, theta;
    //Receive input spherical angles from user//This might have to change depending on how Scott receives launch angles from user//
    cout << "Please enter the launch angles that will determine the Rocket's position on the Launch Planet\nLaunch angles consist of the two spherical angles "
            "phi(from the x-z plane to the y-axis) and theta(the x-z plane).\n";
    cout << "Please enter an angle for theta.\n";    
    IN = verify_pointdouble();
    if(str_lower(IN) == "done")return 1; 
    theta = convert(IN);
    if(0 <= theta) theta = ((int)theta % 360) + (theta - (int)theta);
    else theta = ((int)theta % 360) - (theta - (int)theta);
    theta = 2*pi*(theta/360.0);     
    
    cout << "Please enter an angle for phi.\n";
    IN = verify_pointdouble();
    if(str_lower(IN) == "done")return 1; 
    phi = convert(IN);
    if(0 <= phi) phi = ((int)phi % 360) + (phi - (int)phi);
    else phi = ((int)phi % 360) - (phi - (int)phi);
    phi = 2*pi*(phi/360.0);  
    
    //Receive Launch Speed from user
    double Speed;
    cout << "Please enter the launch speed of the rocket. (m/s)\n";
    while(true){
        IN = verify_double();
        if(str_lower(IN) == "done" || 0 <= convert(IN))break;
        cout << "Invalid value. Enter a nonnegative speed value\n";
    }
    if(str_lower(IN) == "done")return 1; 
    Speed = convert(IN);
    
    //Translate spherical angles into a launch position on launch planet//
    double r = Launch->Radius + Radius + 1;//Plus Radius of Rocket to avoid colliding with launch planet at start of simulation//Needs verification
    double x = r*cos(phi)*cos(theta);
    double z = -r*cos(phi)*sin(theta);
    double y = r*sin(phi);
    
    Celestial.Rx = Launch->Values.Rx + x;
    Celestial.Ry = Launch->Values.Ry + y;
    Celestial.Rz = Launch->Values.Rz + z;
    
    //Translate Launch Speed into launch velocity//
    double vx = Speed*cos(phi)*cos(theta);
    double vy = Speed*sin(phi);
    double vz = -Speed*cos(phi)*sin(theta);
    
    Celestial.Vx = Launch->Values.Vx + vx;
    Celestial.Vy = Launch->Values.Vy + vy;
    Celestial.Vz = Launch->Values.Vz + vz;

    //Set Rocket Statuses
    Ship.Exist = true;
    Ship.Launched = false;
    if(Ship.Valid)Ship.Valid = false;
    else Ship.Valid = true;
    
    return 0;
}

//This code still needs to be commented out
void Celestial::Add_Object(Celestial* Sim){
    Attributes Celestial;
    string IN, Name;
    int Count = 0;
    float Mass, Radius;
    cout << "\nEnter 'Done' when you're done entering objects. Any object that isn't completely specified will not be added." << endl;
    while(true){
        cout << "Objects Added: " << Count << "\n";         
        while(true){       
            cout << "Please enter object name.\n";
            cin >> Name;
            if(str_lower(Name) == "done")break;            
            if(str_lower(Name) == "rocket"){
                Name = str_lower(Name);
                Name[0] = 'R';
            }
            
            //Test whether object currently exists already or not//
            bool exist = false;
            for(CelestialPtr Object : Celestial_Bodies){
                if(Object->get_Name() == Name){
                    exist = true;
                    break;
                }                
            }
            if(!exist)break;
            cout << "Error, object already exists. If it was destroyed in a simulation remove it first and try again." << endl;
        }
        if(str_lower(Name) == "done")break;
        
        IN = "no";
        if(!Celestial_Bodies.empty() && Name != "Rocket"){
            cout << "Would you like to launch the Orbit Customizer? (Yes/No)\n";
            IN = yes_no();
        }
        if(IN == "yes" || Name == "Rocket"){//This code needs future updating
            if(Name == "Rocket"){if(add_Rocket(Celestial, Mass, Radius))break;}
            else if(Orbit_Gen.Rand_Orbit_Gen(&Celestial, Mass, Radius, Celestial_Bodies, Name))break;//Count--;
            else cout << endl;
        }
        else{
            cout << "\nPlease enter Mass value.\n";
            while(true){
                IN = verify_double();
                if(str_lower(IN) == "done" || 0 < convert(IN))break;
                cout << "Invalid value. Enter a positive mass value" << endl;
            }
            if(str_lower(IN) == "done")break;
            Mass = convert(IN);

            cout << "\nPlease enter Rx value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Rx = convert(IN);

            cout << "\nPlease enter Ry value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Ry = convert(IN);

            cout << "\nPlease enter Rz value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Rz = convert(IN);

            cout << "\nPlease enter Vx value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Vx = convert(IN);        

            cout << "\nPlease enter Vy value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Vy = convert(IN); 

            cout << "\nPlease enter Vz value.\n";
            IN = verify_double();
            if(str_lower(IN) == "done")break; 
            Celestial.Vz = convert(IN);          

            cout << "\nPlease enter Radius value.\n";
            while(true){
                IN = verify_double();
                if(str_lower(IN) == "done" || 0 < convert(IN))break;
                cout << "Invalid value. Enter a positive radius value\n";
            }
            if(str_lower(IN) == "done")break; 
            Radius = convert(IN);
        }
        Celestial_Bodies.emplace_back(new Celestial_Body(Name, Mass, Celestial, Radius, Sim));
        Count++;
    }  
    cout << "Ok, what now?" << endl;
}

void Celestial::Final_Sim(const string Target){
    Celestial_Bodies[0]->Final_Sim(Default.Time, Default.h, Default.hmax, Default.hmin, Default.e, Default.Wait, Target);
}

void Celestial::Simulate(){
    if(ObjectTracker.empty())
        cout << "There's nothing to simulate. Add some objects first.\n";
    else{
        if(Default.Time != -1){//Default simulation parameters have been specified//
            cout << "Would you like to use the default simulation parameters?\n";
            if(yes_no() == "yes"){
                if(Ship.Valid){//Default parameters are valid//
                    Celestial_Bodies[0]->Simulate_Motion(Default.Time, Default.h, Default.hmax, Default.hmin, Default.e, Default.Wait);
                    return;
                }
                else{//Default parameters are not valid//
                    cout << "Default parameters are out of date. Would you like to specify new ones?\n";//Might change later on//
                    if(yes_no() == "yes"){
                        if(Set_Parameters() == 0){
                            Celestial_Bodies[0]->Simulate_Motion(Default.Time, Default.h, Default.hmax, Default.hmin, Default.e, Default.Wait);
                            return;
                        }
                    }
                }
            }
        }       
        double Time, h, hmax, hmin, e, Wait;
        string IN;

        cout << "Enter 'Cancel' if you would like to cancel the simulation setup.\n";
        cout << "Please enter the time length (in seconds) of the simulation.\n";;
        IN = verify_simdouble();
        if(str_lower(IN) == "cancel"){
            cout << "Canceled, what now?" << endl;  
            return;
        }
        Time = convert(IN);

        cout << "Please enter the initial time step (in seconds) for the simulation.\n";
        IN = verify_simdouble();
        if(str_lower(IN) == "cancel"){
            cout << "Canceled, what now?" << endl;  
            return;
        }
        h = convert(IN);        

        cout << "Please enter the max time step (in seconds) for the simulation.\n";
        IN = verify_simdouble();
        if(str_lower(IN) == "cancel"){
            cout << "Canceled, what now?" << endl;  
            return;
        }
        hmax = convert(IN);

        cout << "Please enter the min time step (in seconds) for the simulation.\n";
        IN = verify_simdouble();
        if(str_lower(IN) == "cancel"){
            cout << "Canceled, what now?" << endl;  
            return;
        }
        hmin = convert(IN);

        cout << "Please enter the error tolerance for the simulation.\n";
        IN = verify_simdouble();
        if(str_lower(IN) == "cancel"){
            cout << "Canceled, what now?" << endl;  
            return;
        }
        e = convert(IN);
        
        //Check to see if a rocket has been specified//
        if(Ship.Exist && !Ship.Launched){
            cout << "Please specify the length in time to wait for the rocket launch.\n";
            while(true){
                IN = verify_simdouble();
                if(str_lower(IN) == "cancel"){
                    cout << "Canceled" << endl;
                    return;
                }
                if(convert(IN) <= Time /*&& convert(IN) != 0.0*/)break;//Needs verification
                cout << "Error, wait time must be less than simulation time. Sim time: " << Time << endl;
            }
            Wait = convert(IN);
        }
        else Wait = 0.0;
        
        Celestial_Bodies[0]->Simulate_Motion(Time,h,hmax,hmin,e,Wait);         
    }    
}

void Celestial::View_Objects() const{
    if(!ObjectTracker.empty()){
        cout << "Here is the current status of all celestial objects." << endl;
        Celestial_Bodies[0]->Display_Objects();
    }
    else
        cout << "There are no objects to display. Add some objects first." << endl;
}

void Celestial::View_Object() const{
    string Name;
    cout << "Enter the name of the Celestial Body you would like to display." << endl;
    cin >> Name;
    if(str_lower(Name) == "rocket") Name[0] = 'R';
    bool Control = true;
    for(CelestialPtr I : Celestial_Bodies){
        if(Name == I->get_Name()){
            I->display_Object();
            Control = false;            
            break;
        }
    }
    if(Control)
        cout << "Sorry, Celestial Body not found" << endl;
}

void Celestial::View_Start_Objects() const{
    if(!Celestial_Bodies.empty()){
        cout << "Initial start values for each object. (Including the destroyed ones)\n";
        for(CelestialPtr Object : Celestial_Bodies){
            cout << "////////////////////////////////////////////////////////" << endl;
            cout << "Currently " << Object->Status << endl;
            Object->print_attributes(Object->Start, Object->Name, Object->S_Mass, Object->S_Radius);
        }
        cout << "////////////////////////////////////////////////////////" << endl << endl; 
    }
}

string Celestial::get_name() const{ return Sim_Name;}

string Celestial::get_Launch_Planet() const{ return Ship.Launch;}

Sim_Param Celestial::get_Parameters() const{ return Default;}

bool Celestial::get_Rocket(const int Code) const{
    switch(Code){
        case 0: return Ship.Exist;
        case 1: return Ship.Launched;
        case 2: return Ship.Valid;
        default: cout << "Error, invalid return code for get_Rocket function." << endl;
    }
}

void Celestial::set_Rocket(const int Code, const bool Status){
    switch(Code){
        case 0: 
            Ship.Exist = Status;
            break;
        case 1: 
            Ship.Launched = Status;
            break;
        case 2: 
            Ship.Valid = Status;
            break;
        default: cout << "Error, invalid set code for Set_Rocket function." << endl;
    }    
}

void Celestial::Remove_Object(){
    if(Celestial_Bodies.empty())
        cout << "There is nothing to remove. Add some objects first." << endl;
    else{
        string Name;
        cout << "Enter the name of the object you would like to remove." << endl;
        cin >> Name;      
        bool Fake = true;
        int Count = 0;
        for(CelestialPtr I : Celestial_Bodies){
            if(Name == I->get_Name()){
                if(Name == "Rocket"){
                    Ship.Exist = false;
                    Ship.Launched = false;
                    Ship.Valid = false;//Needs verification
                }                  
                I->Remove();              
                Celestial_Bodies.erase(Celestial_Bodies.begin() + Count);
                Fake = false;
                break;
            }
            Count++;          
        }
        if(Fake)
            cout << "Object does not exist." << endl;
        else{
            //Decrement Celestial IDs of each object after 'Count' and decrease SpaceCount by 1//
            if(!Celestial_Bodies.empty())
                Count_Decr(Count);
            cout << "Done. What now?" << endl;
        }
    }
}

int Celestial::alter_Rocket(){
    cout << "Would you like to reposition the Rocket on its launch planet?\n"; 
    if(yes_no() == "yes"){
        CelestialPtr Launch;
        CelestialPtr Rocket;
        string IN;
        
        cout << "Would you like to change the Launch Planet?\n";
        if(yes_no() == "yes"){
        	cout << "Enter new Launch Planet Name\n";
        	cin >> Ship.Launch;
        } 
        
        //Check to see if launch planet currently exists and is not destroyed
        while(true){
            bool exist = false;
            for(CelestialPtr Planet : Celestial_Bodies){
                if(Planet->Name == Ship.Launch && Planet->Status == "Intact"){
                    Launch = Planet;
                    exist = true;
                    break;
                }
            }
            if(exist)break;
            cout << "Error, Launch planet either doesn't exist or was destroyed in a simulation. Please specify a new one." << endl;
            cin >> IN;
            if(str_lower(IN) == "cancel") return 1;
            Ship.Launch = IN;
        }
        
        //Get pointer to Rocket from Celestial_Bodies vector//
        for(CelestialPtr ship : Celestial_Bodies){
            if(ship->Name == "Rocket"){
                Rocket = ship;
                break;
            }
        }
        
        //Receive Mass and Radius of Rocket from user
        cout << "\nPlease enter Mass value for the Rocket.\n";
        while(true){
            IN = verify_double();
            if(str_lower(IN) == "cancel" || 0 < convert(IN))break;
            cout << "Invalid value. Enter a positive mass value" << endl;
        }
        if(str_lower(IN) == "cancel")return 1;
        Rocket->Mass = convert(IN);

        cout << "\nPlease enter Radius value for the Rocket.\n";
        while(true){
            IN = verify_double();
            if(str_lower(IN) == "cancel" || 0 < convert(IN))break;
            cout << "Invalid value. Enter a positive radius value\n";
        }
        if(str_lower(IN) == "cancel")return 1; 
        Rocket->Radius = convert(IN);   

        float phi, theta;
        //Receive input spherical angles from user//This might have to change depending on how Scott receives launch angles from user//
        cout << "Please enter the launch angles that will determine the Rocket's position on the Launch Planet\nLaunch angles consist of the two spherical angles "
                "phi(from the x-z plane to the y-axis) and theta(the x-z plane).\n";
        cout << "Please enter an angle for theta.\n";
        IN = verify_altdouble();
        if(str_lower(IN) == "cancel")return 1;
        theta = convert(IN);
        if(0 <= theta) theta = ((int)theta % 360) + (theta - (int)theta);
        else theta = ((int)theta % 360) - (theta - (int)theta);
        theta = 2*pi*(theta/360.0);     

        cout << "Please enter an angle for phi.\n";
        IN = verify_altdouble();
        if(str_lower(IN) == "cancel")return 1;
        phi = convert(IN);
        if(0 <= phi) phi = ((int)phi % 360) + (phi - (int)phi);
        else phi = ((int)phi % 360) - (phi - (int)phi);
        phi = 2*pi*(phi/360.0);  

        //Receive Launch Speed from user
        double Speed;
        cout << "Please enter the launch speed of the rocket. (m/s)\n";
        while(true){
            IN = verify_double();
            if(str_lower(IN) == "cancel" || 0 <= convert(IN))break;
            cout << "Invalid value. Enter a nonnegative speed value\n";
        }
        if(str_lower(IN) == "cancel")return 1; 
        Speed = convert(IN);

        //Translate spherical angles into a launch position on launch planet
        double r = Launch->Radius + Rocket->Radius + 1;
        double x = r*cos(phi)*cos(theta);
        double z = -r*cos(phi)*sin(theta);
        double y = r*sin(phi);

        Rocket->Values.Rx = Launch->Values.Rx + x;
        Rocket->Values.Ry = Launch->Values.Ry + y;
        Rocket->Values.Rz = Launch->Values.Rz + z;

        //Translate Launch Speed into launch velocity//
        double vx = Speed*cos(phi)*cos(theta);
        double vy = Speed*sin(phi);
        double vz = -Speed*cos(phi)*sin(theta);

        Rocket->Values.Vx = Launch->Values.Vx + vx;
        Rocket->Values.Vy = Launch->Values.Vy + vy;
        Rocket->Values.Vz = Launch->Values.Vz + vz;

        //Update ObjectTracker
        int Count = 0;
        for(Attributes Tracker : ObjectTracker){
            if(Tracker.ID == Rocket->Values.ID){
                ObjectTracker[Count] = Rocket->Values;
                break;                
            }
            Count++;
        }   

        //Make necessary changes to Rocket Launch Parameters
        if(Ship.Valid){
            if(Ship.Launched){
                Ship.Valid = false;
                Ship.Launched = false;
            }
        }
        else{
            if(Ship.Launched){
                Ship.Valid = true;
                Ship.Launched = false;
            }
        }
        
        cout << "Done" << endl;
        return 1;
    }
    return 0;
}

void Celestial::Alter_Object(){
    if(Celestial_Bodies.empty())
        cout << "There is nothing to alter. Add some objects first." << endl;
    else{
        string Name, newName;                
        cout << "Please enter the name of the object you would like to alter." << endl;
        cin >> Name;
        if(Name == "Rocket" && Ship.Exist){
            if(alter_Rocket())
                return;
        }
        int Pos = get_position(Name);
        if(0 <= Pos){
            string Entry;
            double Rad, Mss;
            bool name = false;
            bool mass = false;
            bool radius = false;
            Attributes New = ObjectTracker[Pos];
            cout << "If you would like to cancel any changes enter 'cancel'. If you are done making changes enter 'done'\n";
            while(true){
                cout << "Which attributes from " << Name << " would you like to change? (Rx, Ry, Rz, Vx, Vy, Vz, Name, Mass, Radius)\n";
                while(true){
                    cin >> Entry;
                    Entry = str_lower(Entry);
                    if(Entry == "name" || Entry == "rx" || Entry == "ry" || Entry == "rz" || Entry == "vx" || Entry == "vy" || Entry == "vz")break;
                    if(Entry == "mass" || Entry == "radius" || Entry == "cancel" || Entry == "done")break;
                    else cout << "Invalid entry. Try again." << endl;
                }    
                if(Entry == "done")break;
                else if(Entry == "cancel"){
                    cout << "Canceled. Ok what now?" << endl; 
                    return;
                }
                else if(Entry == "name"){
                    if(Name == "Rocket") cout << "Forbidden. Cannot change name of Rocket.\n";
                    else{
                        cout << "Please enter a new name\n";
                        cin >> newName;
                        name = true;
                    }
                }
                else{
                    string newValue;
                    cout << "Please enter a new double value\n";
                    if(Entry == "mass" || Entry == "radius"){
                        while(true){
                            newValue = verify_altdouble();
                            if(str_lower(newValue) == "cancel" || 0 < convert(newValue))break;
                            cout << "Invalid entry. Both the Radius and Mass must be strictly positive." << endl;
                        }                           
                    }
                    else
                        newValue = verify_altdouble();
                    if(str_lower(newValue) == "cancel"){
                        cout << "Canceled. Ok what now?" << endl; 
                        return;
                    }
                    else{
                        if(Entry == "rx")
                            New.Rx = convert(newValue);
                        if(Entry == "ry")
                            New.Ry = convert(newValue);
                        if(Entry == "rz")
                            New.Rz = convert(newValue);
                        if(Entry == "vx")
                            New.Vx = convert(newValue);
                        if(Entry == "vy")
                            New.Vy = convert(newValue);
                        if(Entry == "vz")
                            New.Vz = convert(newValue);                        
                        if(Entry == "mass"){
                            mass = true;
                            Mss = convert(newValue);
                        }
                        if(Entry == "radius"){
                            radius = true;
                            Rad = convert(newValue);
                        }
                    }                  
                }                         
            }            
            ObjectTracker[Pos] = New;
            if(name)//Change name if necessary//
                Celestial_Bodies[New.ID]->Name = newName;
            if(mass)
                Celestial_Bodies[New.ID]->Mass = Mss;
            if(radius)
                Celestial_Bodies[New.ID]->Radius = Rad;
            Celestial_Bodies[New.ID]->Values = New;
            cout << "Done, what now?" << endl;
        }
        else
            cout << "Sorry. Object either doesn't exist or was destroyed in a simulation. Add a new one." << endl;
    }
}

int Celestial::get_position(const string name){
    int Counter = 0;
    for(Attributes i : ObjectTracker){
        if(Celestial_Bodies[i.ID]->Name == name)
            return Counter;
        Counter++;
    }
    return -1;
}

void Celestial::Count_Decr(int Decrement){
    int Element = 0;
    for(int i=Decrement; i<Celestial_Bodies.size(); i++){
        Element = 0;
        for(Attributes Tracker : ObjectTracker){
            if(Tracker.ID == Celestial_Bodies[i]->Values.ID){
                Tracker.ID--;
                ObjectTracker[Element] = Tracker;                
                break;
            }
            Element++;
        }
        Celestial_Bodies[i]->Values.ID--;
    }
}

void Init_Sim(Celestial *Sim){    
    Attributes Celestial;
    Celestial.Rx = 0.0; Celestial.Ry = 0.0; Celestial.Rz = 0.0; Celestial.Vx = 0.0; Celestial.Vy = 0.0; Celestial.Vz = 0.0;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Sun",597220000000000000000.0,Celestial,987654321, Sim));
    Celestial.Rx = 70000000000.0; Celestial.Ry = 0.0; Celestial.Rz = 0.0; Celestial.Vx = 0.0; Celestial.Vy = 2385509.050196923; Celestial.Vz = 0.0;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Jupiter", 9237000000000000.0, Celestial, 10000000, Sim));
    Celestial.Rx = 5000000000.0; Celestial.Ry = -50000000000.0; Celestial.Rz = 0.0; Celestial.Vx = 0.0; Celestial.Vy = 50000.0; Celestial.Vz = 0.0;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Earth",500000000000000000000000000000000.0,Celestial,987321, Sim));
    Celestial.Rx = 5007910000; Celestial.Ry = -49956500000; Celestial.Rz = 444010000; Celestial.Vx = 1317430; Celestial.Vy = 7290370; Celestial.Vz = 2172960;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Daniel", 500000, Celestial, 462300, Sim));
    Celestial.Rx = 4235670000; Celestial.Ry = -49653000000; Celestial.Rz = 823960000; Celestial.Vx = 1770000; Celestial.Vy = -753565; Celestial.Vz = -958138;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Michael", 200000, Celestial, 492200, Sim));
    Celestial.Rx = 452450000; Celestial.Ry = 999380000; Celestial.Rz = 944590000; Celestial.Vx = 2.1058; Celestial.Vy = 465134; Celestial.Vz = -4.05;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Thomas", 21600000, Celestial, 1060800, Sim));    
    Celestial.Rx = 700000070.0; Celestial.Ry = 0.0; Celestial.Rz = 800200000; Celestial.Vx = 0.0; Celestial.Vy = 0.0; Celestial.Vz = -50000;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Mars", 600000000.0, Celestial,43553, Sim));    
    Celestial.Rx = 5000000000.0; Celestial.Ry = -50000000000.0; Celestial.Rz = 2000000.0; Celestial.Vx = 0.0; Celestial.Vy = 50000.0; Celestial.Vz = -5000.0;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Moon", 300000.0, Celestial, 10000, Sim));
    Celestial.Rx = 0.0; Celestial.Ry = 0.0; Celestial.Rz = 4248934090.0; Celestial.Vx = 55632.0; Celestial.Vy = 77797.0; Celestial.Vz = -83932;
    Sim->Celestial_Bodies.emplace_back(new Celestial_Body("Venus", 9500000000000.0, Celestial, 800892, Sim));
}
