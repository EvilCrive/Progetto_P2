#ifndef TRENO_H
#define TRENO_H
#include <string>
enum Trotaia {
    maglev, strap, plate, bridge, barlow, flat_bottomed, double_headed,
        bullhead
};
enum Ttreno {
    alta_velocita, inter_city, regionale, commuter
};


class Treno {
private:
    std::string nome;
    unsigned int id;
    std::string costruttore;
    unsigned int speed;
    Trotaia tipo_rotaia;
    Ttreno tipo_treno;

public:
    Treno(const std::string& = "NoName", unsigned int =10, const std::string& ="NoBuilder", unsigned int =100, Trotaia =Trotaia::maglev, Ttreno =Ttreno::alta_velocita );
    virtual ~Treno() = default;

    std::string getNome() const;
    unsigned int getId() const;
    std::string getCostruttore() const;
    unsigned int getSpeed() const;
    std::string getTipo_rotaia() const;
    std::string getTipo_treno() const;

    void setNome(std::string);
    void setId(unsigned int);
    void setCostruttore(std::string);
    void setSpeed(unsigned int);
    void setTipo_rotaia(std::string);
    void setTipo_treno(std::string);

    virtual std::string type() const;
    virtual float carburanteNecessario(unsigned int) const =0; //km in input
    virtual unsigned int kmPercorribili(unsigned int) const =0; //carburante in input
    virtual void print()const;
 // virtual int calcolaCosto() const;
 // virtual void serialize();
 // static Treno* unserialize();
 // virtual Treno* clone() const = 0;
};


#endif // TRENO_H
