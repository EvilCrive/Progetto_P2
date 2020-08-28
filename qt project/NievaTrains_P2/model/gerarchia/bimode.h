#ifndef BIMODE_H
#define BIMODE_H
#include "model/gerarchia/electric.h"
#include "model/gerarchia/internal_combustion.h"

class Bimode: public Electric, public Internal_Combustion {
private:
    bool motorePrimario;
public:

    Bimode(const std::string& = "NoName", const std::string& ="NoBuilder", unsigned int =100,  unsigned int =100,bool =false, double =0.7, double =0.4, std::string ="diesel", bool =false);

    std::string getMotorePrimario() const;

    void setMotorePrimario(std::string);
    void setCarburanteIC(std::string);
    void setTrasmissioneIC(std::string);
    void setEfficenzaIC(double);
    void setTrasmissioneElettrico(std::string);
    void setEfficenzaElettrico(double);

    double carburanteNecessario(unsigned int) const;  //input km da percorrere
    unsigned int kmPercorribili(unsigned int) const; //input kw o kg di carburante(in base al motore primario)(se hybrid e' fuel kg)
    std::string type() const;
    void print() const;
    std::string treno2string() const;
    virtual void serialize(QJsonObject&);
};

#endif // BIMODE_H
