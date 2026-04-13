#include <iostream>
#include <string>
#include <limits>
#include <ostream>
#include <vector>


class Pozitie {
private:
    int x, y;
public:
    explicit Pozitie(int val_x = 0, int val_y = 0) : x(val_x), y(val_y) {}
    Pozitie (const Pozitie& alta) : x(alta.x), y(alta.y) {}
    Pozitie& operator=(const Pozitie& alta) {
        if (this != &alta) {
            x = alta.x;
            y = alta.y;
        }
        return *this;
    }
    ~Pozitie()=default;
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int val) { x = val; }

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
        os << "("<<p.x<<", "<<p.y<<")";
        return os;
    }
};

class Inamic {
private:
    int viata;
    int posX, posY;
    char simbol;
    int puncteRecompensa;
public:
    Inamic(int v, int x, int y, char s) : viata(v), posX(x), posY(y), simbol(s) {}

    void miscareInamic() { posY++; }

    int getX() const { return posX; }
    int getY() const { return posY; }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }
    int getPuncte() const { return puncteRecompensa; }

    void scadeViata(int dmg);
};
Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), posX(x), posY(y), simbol(s), puncteRecompensa(v * 2) {}
void Inamic::miscareInamic() {
    this->posY++;
}
void Inamic::scadeViata(int dmg) {
    this->viata = this->viata - dmg;
}

class Arsenal {
    private:
    int munitie;
    int putereAtac;
    std::string tipArma;
    int capacitateMaxima;
public:
    explicit Arsenal(const std::string& tip = "Laser", int mun = 40, int pwr = 10) : munitie(mun), putereAtac(pwr), tipArma(tip), capacitateMaxima(40) {}
    Arsenal(const Arsenal& altul) : munitie(altul.munitie), putereAtac(altul.putereAtac), tipArma(altul.tipArma), capacitateMaxima(altul.capacitateMaxima) {}
    Arsenal& operator= (const Arsenal& altul ) {
        if (this !=&altul) {
            munitie = altul.munitie;
            putereAtac= altul.putereAtac;
            tipArma = altul.tipArma;
            capacitateMaxima = altul.capacitateMaxima;
        }
        return *this;
    }
    ~Arsenal()= default;

    bool trage();
    void reincarca();
    int getDmg() const { return putereAtac; }
    int getMunitie() const { return munitie; }

};
bool Arsenal::trage() {
    if (munitie > 0) {
        munitie--;
        return true;
    }
    return false;
}
void Arsenal::reincarca() {
    this->munitie = this->capacitateMaxima;
    std::cout << "Munitie refacuta!\n";
}


class NavaJucator {
private:
    std::string numeNava;
    int integritate;
    Pozitie locatie;
    Arsenal armament;
    char aspect;
    int scorCurent;
public:
    explicit NavaJucator(const std::string& nume, int startX, int startY);
    NavaJucator(const NavaJucator& alta);
    NavaJucator& operator=(const NavaJucator& alta);

    ~NavaJucator()= default;
    void miscare(char tasta, int limitaX) {
        int nx = locatie.getX();

        if ((tasta == 'a' || tasta == 'A') && nx > 0) nx--;
        if ((tasta == 'd' || tasta == 'D') && nx < limitaX - 1) nx++;

        locatie.setX(nx);
    }
    int actiuneAtac() {
        if (armament.trage()) {
            return armament.getDmg();
        } else {
            std::cout << "Eroare: Munitie insuficienta!\n";
            return 0;
        }
    }
    int x()const { return locatie.getX();}
    int y() const { return locatie.getY(); }
    char getAspect()const {return aspect; }
    int getIntegritate() const { return integritate; }

    void setScor(int s) { scorCurent = s; }
    int getScor() const { return scorCurent; }

    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n) {
        os << "[" <<n.numeNava << "] HP: "<< n.integritate<< "% | AMMO: " << n.armament.getMunitie();
        return os;
    }
    void executaReincarcare() {
        armament.reincarca();
    }
};
NavaJucator::NavaJucator(const std::string& nume, int startX, int startY)
    : numeNava(nume), integritate(100), locatie(startX, startY),
      armament("Laser", 40, 10), aspect('N'), scorCurent(0) {}

NavaJucator::NavaJucator(const NavaJucator& alta)
    : numeNava(alta.numeNava + "_Backup"), integritate(alta.integritate),
      locatie(alta.locatie), armament(alta.armament), aspect(alta.aspect), scorCurent(alta.scorCurent) {}

NavaJucator& NavaJucator::operator=(const NavaJucator& alta) {
    if (this != &alta) {
        this->numeNava = alta.numeNava;
        this->integritate = alta.integritate;
        this->locatie = alta.locatie;
        this->armament = alta.armament;
        this->aspect = alta.aspect;
        this->scorCurent = alta.scorCurent;
    }
    return *this;
}

class MotorGrafic {
private:
    int lungime,inaltime;

public:
    explicit MotorGrafic(int l = 30, int h = 10) : lungime(l), inaltime(h) {}
    void scena(const NavaJucator& nava, const std::vector<Inamic>& inamici) {
        for(int i = 0;i < 10;++i) std::cout << "\n";
        std::cout << nava << "\n";
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
        std::cout << "\n";
        for (int y = 0;y < inaltime;y++) {
            std::cout << "|";
            for (int x = 0; x < lungime;x++) {
                bool obiectDesenat = false;
                if (x == nava.x() && y == nava.y()) {
                    std::cout << nava.getAspect();
                    obiectDesenat = true;
                } else {
                    for (const auto& in : inamici) {
                        if (in.getX() == x && in.getY() == y) {
                            std::cout << in.getSimbol();
                            obiectDesenat = true;
                            break;
                        }
                    }
                }
                if (!obiectDesenat) std::cout << " ";
            }
            std::cout << "|\n";
        }
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
        std::cout << "\nComenzi: A/D (Miscare), F (Atac), Q (Exit)\n";
    }
    int getL() const { return lungime; }
};



int main() {
    NavaJucator albuquerque("Interceptor", 15, 9);
    MotorGrafic motor(30, 10);
    std::vector<Inamic> listaInamici;

    listaInamici.push_back(Inamic(20, 5, 0, 'v'));
    listaInamici.push_back(Inamic(50, 20, 1, 'W'));
    listaInamici.push_back(Inamic(30, 10, 1, 'x'));
    listaInamici.push_back(Inamic(50, 20, 0, 'W'));
    listaInamici.push_back(Inamic(20, 15, 2, 'v'));
    listaInamici.push_back(Inamic(40, 25, 1, 'U'));
    char tastaApasata = ' ';

    while (std::cin >> tastaApasata) {
        if (tastaApasata == 'q' || tastaApasata == 'Q') break;
        if (tastaApasata == 'f' || tastaApasata == 'F') {
            int dmg = albuquerque.actiuneAtac();
            if (dmg == 0) {
                albuquerque.executaReincarcare();
            } else {
                for (size_t i = 0; i < listaInamici.size(); i++) {
                    if (listaInamici[i].getX() == albuquerque.x()) {
                        listaInamici[i].scadeViata(dmg);
                    }
                }
            }
        } else {
            albuquerque.miscare(tastaApasata, motor.getL());
        }
        for (auto& inamic : listaInamici) {
            inamic.miscareInamic();
        }
        for (size_t i = 0; i < listaInamici.size(); ) {
            if (listaInamici[i].getViata() <= 0 || listaInamici[i].getY() >= 10) {
                listaInamici.erase(listaInamici.begin() + i);
            } else {
                i++;
            }
        }
        motor.scena(albuquerque, listaInamici);
        std::cout << "Input (A/D/F/Q): ";
    }
    std::cout << "Inchidere...\n";
    NavaJucator backup =albuquerque;
    std::cout << "Backup creat:" <<backup <<"\n";
    return 0;
}