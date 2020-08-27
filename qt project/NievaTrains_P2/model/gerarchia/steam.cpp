#include "model/gerarchia/steam.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <math.h>

Steam::Steam(const std::string & n, const std::string & c, unsigned int s, unsigned int p, double e, TfuelSteam tfs): Treno(n,c,s,p),efficenzaSteam(e),carburanteSteam(tfs){}

double Steam::getEfficenzaSteam() const
{
    return efficenzaSteam;
}

std::string Steam::getCarburanteSteam() const
{
    if(carburanteSteam==TfuelSteam::oil)     return "Oil";
    if(carburanteSteam==TfuelSteam::coal)    return "Coal";
    if(carburanteSteam==TfuelSteam::wood)    return "Wood";
    return "NoType";
}

void Steam::setEfficenzaSteam(double e)
{
    efficenzaSteam=e;
}

void Steam::setCarburanteSteam(std::string tr)
{
    std::transform(tr.begin(), tr.end(), tr.begin(),
        [](unsigned char c){ return std::tolower(c); });
    //porta la stringa tutta lowercase
    if(tr=="coal"){
        carburanteSteam=TfuelSteam::coal;
    }else if(tr=="wood"){
        carburanteSteam=TfuelSteam::wood;
    }else if(tr=="oil"){
        carburanteSteam=TfuelSteam::oil;
    }
}

double Steam::carburanteNecessario(unsigned int km) const
{
    return km/efficenzaSteam;
}

unsigned int Steam::kmPercorribili(unsigned int kg) const
{
    return static_cast<unsigned int>(efficenzaSteam*kg);
}

std::string Steam::type() const
{
    return "Steam";
}
/*
Steam *Steam::clone() const
{
    return new Steam(*this);
}
*/

void Steam::print() const
{
    Treno::print();
    std::cout<<"\nEfficenza: "<<getEfficenzaSteam()*100<<"%"<<"\nCarburante: "<<getCarburanteSteam();
}
std::string Steam::treno2string() const{
    std::string s=Treno::treno2string();
    std::string efficenza=std::to_string(getEfficenzaSteam()*100);
    efficenza.erase ( efficenza.find(".")+3, std::string::npos );
    s.append("\nEfficenza: "+efficenza+"%"+"\nCarburante: "+getCarburanteSteam());
    return s;
}

void Steam::serialize(QJsonObject & json)
{
    json["type"]="Steam";
    json["nome"]=QString::fromStdString(getNome());
    json["builder"]=QString::fromStdString(getCostruttore());
    json["speed"]=static_cast<int>(getSpeed());
    json["peso"]=static_cast<int>(getPeso());

    json["efficenzaSteam"]=getEfficenzaSteam();
    json["tipo_carburanteSteam"]=QString::fromStdString(getCarburanteSteam());
}
