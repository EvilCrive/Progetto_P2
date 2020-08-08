#ifndef BIMODE_H
#define BIMODE_H

#include "electric.h"
#include "internal_combustion.h"

enum Tmotore {
    elettrico, combustione, fullhybrid
};

class Bimode: public Electric, Internal_Combustion {
private:
    Tmotore motorePrimario;

public:
    Bimode(const std::string& = "NoName", unsigned int =10, const std::string& ="NoBuilder", unsigned int =100, Trotaia =Trotaia::maglev, Ttreno =Ttreno::alta_velocita, TtrasmissioneElettrico =TtrasmissioneElettrico::overhead_lines, float =0.7f, float =0.4f, Tfuel =Tfuel::kerosene, TtrasmissioneFuel =TtrasmissioneFuel::electric, Tmotore =Tmotore::elettrico);
    std::string getMotorePrimario() const;

    void setMotorePrimario(std::string);

    float carburanteNecessario(unsigned int) const;  //input km da percorrere
    unsigned int kmPercorribili(unsigned int) const; //input kw o kg di carburante(in base al motore primario)(se hybrid e' fuel kg)
    std::string type() const;
    void print() const;
    //   Bimode* clone() const;

};

#endif // BIMODE_H
