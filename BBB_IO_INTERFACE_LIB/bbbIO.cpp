//
//  bbbIO.cpp
//  cppTest
//
//  Created by Riya.Liel on 2015/02/04.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "bbbIO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void fatalReadError(std::string message);
void fatalWriteError(std::string message);

pwmIO::~pwmIO(){
    stop();
}

void pwmIO::init(int pinNum,int pinID,int order){
    _pinNum = pinNum;
    _pinID = pinID;
    _order = order;
	validOut();
}

void pwmIO::validOut(){
    std::ostringstream ss;
    ofstream ofs;
    
    ss <<  "/sys/devices/bone_capemgr." <<  BONE_CAPEMGR <<  "/slots";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    
	ss.str("");
    ss.clear(stringstream::goodbit);
    ss << "bone_pwm_P" << _pinNum << "_" << _pinID;
    ofs << ss.str();
    ofs.close();
	std::cout << "wrote " << ss.str() << std::endl;
}

void pwmIO::stop(){
    std::ostringstream ss;
    ofstream ofs;
    
    ss <<  "/sys/devices/ocp." << OCP  <<  "/pwm_test_P" << _pinNum << "_" << _pinID << "." << _order <<  "/run";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << 0;
    ofs.close();
}

void pwmIO::Run(){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/devices/ocp." << OCP << "/pwm_test_P" << _pinNum << "_" << _pinID << "." << _order << "/run";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << 1;
    ofs.close();

}

void pwmIO::setPeriod(int period){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/devices/ocp." << OCP << "/pwm_test_P" << _pinNum << "_" << _pinID << "." << _order <<  "/period";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << period;
    ofs.close();

}
void pwmIO::setDuty(int duty){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/devices/ocp." << OCP << "/pwm_test_P" << _pinNum << "_" << _pinID << "." << _order <<  "/duty";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << duty;
    ofs.close();

}

void pwmIO::setPolarity(int polarity){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/devices/ocp." << OCP << "/pwm_test_P" << _pinNum << "_" << _pinID << "." << _order <<  "/polarity";
    
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << polarity;
    ofs.close();

}

gpIO::gpIO(int _pinID,std::string direction){
    pinID = _pinID;
    valid(direction);
}

gpIO::~gpIO(){
	stop();
    invalid();
}

void gpIO::init(int _pinID,std::string direction){
    pinID = _pinID;
    valid(direction);
}

void gpIO::valid(std::string direction){
    std::ostringstream ss;
    ofstream ofs("/sys/class/gpio/export");
    if(!ofs.is_open())fatalWriteError("/sys/class/gpio/export write error");
    ofs << pinID;
    ofs.close();
    
    if(direction == "in"){
    }
    else if(direction == "out"){
    }
    else{
        printf("direction parameter wrong\n");
        exit(1);
    }
    
    ss << "/sys/class/gpio/gpio" << pinID << "/direction";
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << direction;
    ofs.close();
}

void gpIO::outVoltage(){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/class/gpio/gpio" << pinID << "/value";
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << 1;
    ofs.close();
}

void gpIO::stop(){
    std::ostringstream ss;
    ofstream ofs;
    
    ss << "/sys/class/gpio/gpio" << pinID << "/value";
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << 0;
    ofs.close();
}


int gpIO::inVoltage(){
    std::ostringstream ss;
    ifstream ifs;
    int value;
    
    ss << "/sys/class/gpio/gpio" << pinID << "/value";
    ifs.open(ss.str());
    if(!ifs.is_open())fatalReadError(ss.str());
    ifs >> value;
    ifs.close();
    return value;
}

void gpIO::invalid(){
    ofstream ofs("/sys/class/gpio/unexport");
    ofs << pinID;
    ofs.close();
}

void gpIO::setEdge(std::string edge){
    std::ostringstream ss;
    std::string path;
    ofstream ofs;
    
    ss << "/sys/class/gpio/gpio" << pinID << "/edge";
    ofs.open(ss.str());
    if(!ofs.is_open())fatalWriteError(ss.str());
    ofs << edge;
    ofs.close();
}

void validADConvert(){
    char path[50];
    FILE* fp;
    sprintf(path,"/sys/devices/bone_capemgr.%d/slots",BONE_CAPEMGR);
    fp = fopen(path,"w");
    if(fp==NULL)fatalWriteError(path);
    fprintf(fp,"cape-bone-iio");
    fclose(fp);
}


int inputAIN(int ainID){
    char path[50];
    FILE* fp;
    int value;
    
    sprintf(path,"/sys/devices/ocp.%d/helper.%d/AIN%d",OCP,HELPER,BONE_CAPEMGR);
    fp = fopen(path,"rb");
    if(fp==NULL)fatalWriteError(path);
    fscanf(fp,"%d",&value);
    fscanf(fp,"%d",&value);
    fclose(fp);
    
    return value;
}

void fatalReadError(std::string message){
    std::cout << message << " read error" << std::endl;
    throw;
}

void fatalWriteError(std::string message){
    std::cout << message << " write error" << std::endl;
    throw;
}

