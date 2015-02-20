//
//  bbbIO.h
//  cppTest
//
//  Created by Riya.Liel on 2015/02/04.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __cppTest__bbbIO__
#define __cppTest__bbbIO__

#include<string>

#define BONE_CAPEMGR 8
#define HELPER 15
#define OCP 2

/* AD Convert Control functions*/
void validADConvert();
int inputAIN(int ainID);

/* USE TO VALID BBB PWM FUNCTION */
void validPWM();

class pwmIO{
public:
    ~pwmIO();
    void init(int pinNum,int pinID,int order);
    void validOut();
    void stop();
    void Run();
    void setPeriod(int period);
    void setDuty(int duty);
    void setPolarity(int polarity);
private:
    int _pinNum;
    int _pinID;
    int _order;
};

class gpIO{
public:
    gpIO(int _pinID,std::string direction);
    gpIO(){};
    ~gpIO();
    void init(int _pinID,std::string direction);
    void outVoltage();
    void stop();
    void setEdge(std::string edge);
    int inVoltage();
private:
    void valid(std::string direction);
    void invalid();
    int pinID;
};



#endif /* defined(__cppTest__bbbIO__) */
