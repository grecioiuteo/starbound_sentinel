#ifndef NAVA_JUCATOR_H
#define NAVA_JUCATOR_H

#include <string>
#include <iostream>
#include "entitatiBaza.h"
#include "exceptii.h"

class Arsenal {
private:
    int munitie;
    int putereAtac;
    std::string tipArma;
    int capacitateMaxima;
public:
    explicit Arsenal(const std::string& tip = "Laser", int mun = 40, int pwr = 10);
    Arsenal& operator+=(int cantitate);
    bool trage();
    int reincarca();
    int getDmg() const;
    friend std::ostream& operator<<(std::ostream& os, const Arsenal& a);
};

class NavaJucator {
private:
    std::string numeNava;
    int integritate;
    Pozitie locatie;
    Arsenal armament;
    char aspect;
    int valoareScut;
    int bonusDamage;
public:
    int getVieti() const;
    int getIntegritate() const { return integritate; }
    void setIntegritate(int val) { integritate = val; if(integritate > 100) integritate = 100; }
    bool esteOperationala() const;
    void autoDiagnostic() const;
    explicit NavaJucator(const std::string& nume, int startX, int startY);
    NavaJucator(const NavaJucator& alta);
    NavaJucator& operator=(const NavaJucator& alta);
    ~NavaJucator() = default;
    void miscare(char tasta, int limitaX);
    int actiuneAtac();
    int x() const;
    int y() const;
    char getAspect() const;
    void activeazaScut(int val);
    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n);
    int executaReincarcare();
    void primesteLovitura(int dmg);
    int getAtacTotal() const;
    Arsenal& getArmament();
};

#endif