#include "../include/navaJucator.h"

Arsenal::Arsenal(const std::string& tip, int mun, int pwr) : munitie(mun), putereAtac(pwr), tipArma(tip), capacitateMaxima(40) {}
Arsenal& Arsenal::operator+=(int cantitate) {
    this->munitie += cantitate;
    if (this->munitie > capacitateMaxima) this->munitie = capacitateMaxima;
    return *this;
}
bool Arsenal::trage() {
    if (munitie > 0) { munitie--; return true; }
    return false;
}
int Arsenal::reincarca() {
    this->munitie = this->capacitateMaxima;
    std::cout << "Munitie refacuta!\n";
    return munitie;
}
int Arsenal::getDmg() const { return putereAtac; }
std::ostream& operator<<(std::ostream& os, const Arsenal& a) {
    os << "Mun: " << a.munitie << "/" << a.capacitateMaxima << " [" << a.tipArma << "]";
    return os;
}

NavaJucator::NavaJucator(const std::string& nume, int startX, int startY)
    : numeNava(nume), integritate(100), locatie(startX, startY),
      armament("Laser", 40, 10), aspect('N'), valoareScut(0), bonusDamage(0) {}
NavaJucator::NavaJucator(const NavaJucator& alta)
    : numeNava(alta.numeNava + "_Backup"), integritate(alta.integritate),
      locatie(alta.locatie), armament(alta.armament), aspect(alta.aspect),
      valoareScut(alta.valoareScut), bonusDamage(alta.bonusDamage) {}
NavaJucator& NavaJucator::operator=(const NavaJucator& alta) {
    if (this != &alta) {
        this->numeNava = alta.numeNava;
        this->integritate = alta.integritate;
        this->locatie = alta.locatie;
        this->armament = alta.armament;
        this->aspect = alta.aspect;
        this->valoareScut = alta.valoareScut;
        this->bonusDamage = alta.bonusDamage;
    }
    return *this;
}
int NavaJucator::getVieti() const {
    if (integritate > 66) return 3;
    if (integritate > 33) return 2;
    if (integritate > 0) return 1;
    return 0;
}
bool NavaJucator::esteOperationala() const { return integritate > 0; }
void NavaJucator::miscare(char tasta, int limitaX) {
    int nx = locatie.getX();
    int viteza = 3;
    if ((tasta == 'a' || tasta == 'A') && nx >= viteza) nx -= viteza;
    if ((tasta == 'd' || tasta == 'D') && nx <= limitaX - viteza - 1) nx += viteza;
    locatie.setX(nx);
}
int NavaJucator::actiuneAtac() {
    if (armament.trage()) {
        return armament.getDmg() + bonusDamage;
    } else {
        throw MunitieInsuficientaException();
    }
}
int NavaJucator::x() const { return locatie.getX(); }
int NavaJucator::y() const { return locatie.getY(); }
char NavaJucator::getAspect() const { return aspect; }
int NavaJucator::executaReincarcare() { return armament.reincarca(); }
void NavaJucator::primesteLovitura(int dmg) {
    if (valoareScut > 0 && dmg > 0) {
        valoareScut -= dmg;
        if (valoareScut < 0) valoareScut = 0;
    } else {
        integritate -= dmg;
    }

    if (integritate <= 0) {
        throw NavaDistrusaException();
    }
}
int NavaJucator::getAtacTotal() const { return armament.getDmg() + bonusDamage; }
std::ostream& operator<<(std::ostream& os, const NavaJucator& n) {
    os << "[" << n.numeNava << "] HP: " << n.integritate << "% | Pozitie: " << n.locatie << " | Armament: " << n.armament;
    return os;
}