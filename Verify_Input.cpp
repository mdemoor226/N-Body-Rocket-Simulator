/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <locale>
#include <sstream>
#include "TrackingSystem.h"

using namespace std;

string str_lower(const string Input){
    string Out;
    char v[Input.length()+1];
    strcpy(&v[0], Input.c_str());
    char *c = &v[0];
    for (int i = 0;c[i] != '\0';i++)
        c[i] = tolower(c[i]);
    Out = c;
    return Out;    
}

bool verify_input(string Input){
    if(str_lower(Input) == "done")
        return true;
    int DotCount = 0;
    int Begin = 0;
    locale loc;
    if(Input[0] == '-')
        Begin = 1;
    for(int i = Begin; i < Input.length(); i++){
        if(!isdigit(Input[i],loc) && Input[i] != '.')
            return false;
        if(Input[i] == '.')
            DotCount++;        
    }
    if(1 < DotCount)
        return false;
    return true;    
}

string verify_double(){
    string IN;
    while(true){
        cin >> IN;
        if(verify_input(IN))
            break;
        else
            cout << "Invalid Input. Please enter a floating point number" << endl;
    }
    //The rest of this function is just more or less me satisfying my OCD//
    int DotCount = 0;
    for(int i=0; i<IN.length(); i++)
        if(IN[i] == '.')
            DotCount++;
    if(IN[0] == '.'){
        string zero = "0";
        IN = zero.append(IN);
    }
    if(IN[0] == '-' && IN[1] == '.')
        IN = IN.insert(1, "0");    
    if(DotCount == 0 && str_lower(IN) != "done")
        IN.append(".0");
    return IN;
}

double convert(string x){
    double num = 0.0;
    stringstream convert(x);
    convert >> num;
    return num;
}

bool verify_siminput(string Input){
    if(str_lower(Input) == "cancel")
        return true;
    int DotCount = 0;
    locale loc;
    for(int i = 0; i < Input.length(); i++){
        if(!isdigit(Input[i],loc) && Input[i] != '.')
            return false;
        if(Input[i] == '.')
            DotCount++;        
    }
    if(1 < DotCount)
        return false;
    int nonzeroCnt = 0;
    for(int i = 0; i < Input.length(); i++){
        if(Input[i] != '0' && Input[i] != '.')
            nonzeroCnt++;
    }
    if(nonzeroCnt == 0)
        return false;
    
    return true;    
}

string verify_simdouble(){
    string IN;
    while(true){
        cin >> IN;
        if(verify_siminput(IN))
            break;
        else
            cout << "Invalid Input. Please enter a positive floating point number" << endl;
    }
    //The rest of this function is just more or less me satisfying my OCD//
    int DotCount= 0;
    for(int i=0; i<IN.length(); i++)
        if(IN[i] == '.')
            DotCount++;
    if(IN[0] == '.'){
        string zero = "0";
        IN = zero.append(IN);
    }
    if(DotCount == 0 && str_lower(IN) != "cancel")
        IN.append(".0");
    return IN;
}

bool verify_altinput(string Input){
    if(str_lower(Input) == "cancel")
        return true;
    int DotCount = 0;
    int Begin = 0;
    locale loc;
    if(Input[0] == '-')
        Begin = 1;
    for(int i = Begin; i < Input.length(); i++){
        if(!isdigit(Input[i],loc) && Input[i] != '.')
            return false;
        if(Input[i] == '.')
            DotCount++;        
    }
    if(1 < DotCount)
        return false;
    return true;    
}

string verify_altdouble(){
    string IN;
    while(true){
        cin >> IN;
        if(verify_altinput(IN))
            break;
        else
            cout << "Invalid Input. Please enter a floating point number" << endl;
    }
    //The rest of this function is just more or less me satisfying my OCD//
    int DotCount= 0;
    for(int i=0; i<IN.length(); i++)
        if(IN[i] == '.')
            DotCount++;
    if(IN[0] == '.'){
        string zero = "0";
        IN = zero.append(IN);
    }
    if(IN[0] == '-' && IN[1] == '.')
        IN = IN.insert(1, "0");    
    if(DotCount == 0 && str_lower(IN) != "cancel")
        IN.append(".0");
    return IN;
}

string yes_no(){
    string Ans;
    while(true){
        cin >> Ans;//Yes/No Verification//
        Ans = str_lower(Ans);
        if(Ans == "no" || Ans == "yes")
            break;
        else
            cout << "Invalid response. Please enter yes or no." << endl;
    }
    return Ans;
}

bool verify_pointinput(string Input){
    if(str_lower(Input) == "done")
        return true;
    int DotCount = 0;
    int Begin = 0;
    locale loc;
    if(Input[0] == '-')
        Begin = 1;
    for(int i = Begin; i < Input.length(); i++){
        if(!isdigit(Input[i],loc) && Input[i] != '.')
            return false;
        if(Input[i] == '.')
            DotCount++;        
    }
    if(1 < DotCount)
        return false;
    return true;    
}

string verify_pointdouble(){
    string IN;
    while(true){
        cin >> IN;
        if(verify_pointinput(IN))
            break;
        else
            cout << "Invalid Input. Please enter a number" << endl;
    }
    //The rest of this function is just more or less me satisfying my OCD//
    int DotCount = 0;
    for(int i=0; i<IN.length(); i++)
        if(IN[i] == '.')
            DotCount++;
    if(IN[0] == '.'){
        string zero = "0";
        IN = zero.append(IN);
    }
    if(IN[0] == '-' && IN[1] == '.')
        IN = IN.insert(1, "0");    
    if(DotCount == 0 && str_lower(IN) != "done")
        IN.append(".0");
    return IN;
}

bool verify_fileinput(string Input){
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
