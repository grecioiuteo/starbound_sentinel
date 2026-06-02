#ifndef INAMICI_H
#define INAMICI_H

#include <vector>
#include <iostream>
#include "entitatiBaza.h"
#include "navaJucator.h"
#include "exceptii.h"

class Inamic {
protected:
    int viata;
    int armura;
    Pozitie loc;
    char simbol;
    int viataMax;
    bool s_a_transformat;
    
    static int totalInamiciDistrusi;
public:
    static int getTotalInamiciDistrusi() { return totalInamiciDistrusi; }
    static void incrementDistrusi() { totalInamiciDistrusi++; }

    Inamic(int v, int x, int y, char s);
    virtual ~Inamic() = default;
    virtual void miscareInamic() = 0;
    virtual Inamic* clone() const = 0;

    int getX() const { return loc.getX(); }
    int getY() const { return loc.getY(); }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }

    void scadeViata(int dmg);
    void afiseaza(std::ostream& os) const { afisareVirtuala(os); }
    friend std::ostream& operator<<(std::ostream& os, const Inamic& inamic) {
        inamic.afiseaza(os);
        return os;
    }
protected:
    virtual void afisareVirtuala(std::ostream& os) const {
        os << simbol << " [HP: " << viata << "]";
    }
};

class InamicScout : public Inamic {
public:
    InamicScout(int x, int y) : Inamic(5, x, y, 'v') {}
    virtual ~InamicScout() override = default;
    void miscareInamic() override { loc.setY(loc.getY() + 2); }
    Inamic* clone() const override { return new InamicScout(*this); }
};

class InamicCruiser : public Inamic {
public:
    InamicCruiser(int x, int y) : Inamic(1, x, y, 'W') { armura = 10; }
    virtual ~InamicCruiser() override = default;
    void miscareInamic() override { loc.setY(loc.getY() + 1); }
    Inamic* clone() const override { return new InamicCruiser(*this); }
};

class InamicFregata : public Inamic {
private:
    int directieX;
    int pasMecanic;
public:
    InamicFregata(int x, int y) : Inamic(15, x, y, 'F'), directieX(3), pasMecanic(0) {}
    virtual ~InamicFregata() override = default;
    void miscareInamic() override;
    Inamic* clone() const override { return new InamicFregata(*this); }
};

class InamicMeteorit : public Inamic {
private:
    const NavaJucator* navaTinta;
public:
    InamicMeteorit(int x, int y, const NavaJucator* nava)
        : Inamic(80, x, y, 'M'), navaTinta(nava) {}
    virtual ~InamicMeteorit() override = default;
    void miscareInamic() override;
    Inamic* clone() const override { return new InamicMeteorit(*this); }
};

class FlotaManager {
private:
    std::vector<Inamic*> naveFlota;
public:
    FlotaManager() = default;
    ~FlotaManager();
    FlotaManager(const FlotaManager& alta);
    FlotaManager& operator=(FlotaManager alta);

    void adaugaInamic(Inamic* inamicNou);
    std::vector<Inamic*>& getInamici();
    void curataFlota();
};

#endif