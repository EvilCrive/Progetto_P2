#include "bimode.h"
#include <algorithm>
#include <cctype>
#include <iostream>

Bimode::Bimode(const std::string & n, unsigned int i, const std::string & c, unsigned int s, Trotaia tr, Ttreno tt,TtrasmissioneElettrico ttr1, float e1,float e2, Tfuel tf, TtrasmissioneFuel ttr2,Tmotore tm1)
    : Electric(n,i,c,s,tr,tt,ttr1,e1),Internal_Combustion(n,i,c,s,tr,tt,e2,tf,ttr2), motorePrimario(tm1){}

std::string Bimode::getMotorePrimario() const
{
    if(motorePrimario==Tmotore::elettrico)      return "Elettrico";
    if(motorePrimario==Tmotore::fullhybrid)     return "FullHybrid";
    if(motorePrimario==Tmotore::combustione)    return "Diesel";
    return "NoType";
}

void Bimode::setMotorePrimario(std::string tr)
{
    std::transform(tr.begin(), tr.end(), tr.begin(),
        [](unsigned char c){ return std::tolower(c); });
    //porta la stringa tutta lowercase
    if(tr=="elettrico"){
        motorePrimario=Tmotore::elettrico;
    }else if(tr=="combustione"){
        motorePrimario=Tmotore::combustione;
    }else if(tr=="fullhybrid"){
        motorePrimario=Tmotore::fullhybrid;
    }
}

float Bimode::carburanteNecessario(unsigned int km) const
{
    float result=0;
    if(motorePrimario==Tmotore::elettrico){
        result= (((km/efficenzaElettrico)*0.75f)+((km/efficenzaFuel)*0.25f));
    }else if(motorePrimario==Tmotore::combustione){
        result= (((km/efficenzaElettrico)*0.25f)+((km/efficenzaFuel)*0.75f));
    }else{
        result= (((km/efficenzaElettrico)*0.5f)+((km/efficenzaFuel)*0.5f));
    }
    return result;
}

unsigned int Bimode::kmPercorribili(unsigned int f) const
{
    unsigned int result=0;
    if(motorePrimario==Tmotore::elettrico){
        result= static_cast<unsigned int>(efficenzaElettrico*f);
    }else if(motorePrimario==Tmotore::combustione){
        result= static_cast<unsigned int>(efficenzaFuel*f);
    }else{
        result= static_cast<unsigned int>(efficenzaFuel*f);
    }
    return result;
}

std::string Bimode::type() const
{
    return "Bimode";
}

void Bimode::print() const
{
    Treno::print();
    std::cout<<"\nMotore Primario: "<<getMotorePrimario()<<"\nTrasmissione Motore Elettrico: "<<Electric::getTrasmissione()<<"\nEfficenza Motore Elettrico: "<<Electric::getEfficenza()*100<<"%"<<"\nTrasmissione Motore a Combustione Interna: "<<Internal_Combustion::getTrasmissione()<<"\nEfficenza Motore a Combustione Interna: "<<Internal_Combustion::getEfficenza()*100<<"%"<<"\nCarburante Motore a Combustione Interna: "<<getCarburante();

}
/*
Bimode *Bimode::clone() const
{
    return new Bimode(*this);
}
*/
