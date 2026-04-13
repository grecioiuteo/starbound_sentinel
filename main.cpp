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
public:
    Inamic(int v, int x, int y, char s) : viata(v), posX(x), posY(y), simbol(s) {}

    void miscareInamic() { posY++; }

    int getX() const { return posX; }
    int getY() const { return posY; }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }

    void scadeViata(int dmg) { viata = viata - dmg; }
};

class Arsenal {
    private:
    int munitie;
    int putereAtac;
    std::string tipArma;
public:
    explicit Arsenal(const std::string& tip = "Laser", int mun = 40, int pwr = 10) : munitie(mun), putereAtac(pwr), tipArma(tip) {}
    Arsenal(const Arsenal& altul) : munitie(altul.munitie), putereAtac(altul.putereAtac), tipArma(altul.tipArma) {}
    Arsenal& operator= (const Arsenal& altul ) {
        if (this !=&altul) {
            munitie = altul.munitie;
            putereAtac= altul.putereAtac;
            tipArma = altul.tipArma;
        }
        return *this;
    }
    ~Arsenal()= default;

    bool trage() {
        if (munitie > 0) {
            munitie--;
            return true;
        }
        return false;
    }
    int getDmg() const { return putereAtac; }
    int getMunitie() const { return munitie; }
    void reincarca() {
        munitie = 40;
        std::cout << "Munitie refacuta!\n";
    }

};


class NavaJucator {
private:
    std::string numeNava;
    int integritate;
    Pozitie locatie;
    Arsenal armament;
    char aspect;
public:
    explicit NavaJucator(const std::string& nume, int startX, int startY)
        : numeNava(nume), integritate(100), locatie(startX, startY), armament("Laser",40, 10), aspect('N') {}
    NavaJucator(const NavaJucator& alta)
        : numeNava(alta.numeNava + "_Backup"), integritate(alta.integritate),
          locatie(alta.locatie), armament(alta.armament), aspect(alta.aspect) {}

    NavaJucator& operator=(const NavaJucator& alta) {
        if (this != &alta) {
            numeNava =alta.numeNava ;
            integritate=alta.integritate;
            locatie =alta.locatie;
            armament=alta.armament ;
            aspect=alta.aspect;
        }
        return *this;
    }
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

    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n) {
        os << "[" <<n.numeNava << "] HP: "<< n.integritate<< "% | AMMO: " << n.armament.getMunitie();
        return os;
    }
    void executaReincarcare() {
        armament.reincarca();
    }
};
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
                    for (size_t i = 0; i < inamici.size(); i++) {
                        if (inamici[i].getX() == x && inamici[i].getY() == y) {
                            std::cout << inamici[i].getSimbol();
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