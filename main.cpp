#include <iostream>
#include <string>
#include <limits>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <ctime>


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
    int viataMax;
    bool s_a_transformat;
public:
    Inamic(int v, int x, int y, char s);

    void miscareInamic();

    int getX() const { return posX; }
    int getY() const { return posY; }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }

    void scadeViata(int dmg);
};
Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), posX(x), posY(y), simbol(s), viataMax(v), s_a_transformat(false) {}
void Inamic::miscareInamic() {
    this->posY++;
}
void Inamic::scadeViata(int dmg) {
    this->viata = this->viata - dmg;
    if (!s_a_transformat && viata <= viataMax / 2) {
        if (simbol == 'W') simbol = 'w';
        else if (simbol == 'U') simbol = 'u';
        else if (simbol == 'X') simbol = 'x';
        s_a_transformat = true;
    }
}

class Diamant {
private:
    int posX, posY;
    char simbol;
    int valoare;
public:
    Diamant(int x, int y) : posX(x), posY(y), simbol('*'), valoare(500) {}
    void miscare() { posY++; }
    int getX()const {return posX; }
    int getY() const{ return posY; }
    char getSimbol() const {return simbol;}
    int getValoare() const {return valoare;}
};

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
      armament("Laser", 40, 10), aspect('N') {}

NavaJucator::NavaJucator(const NavaJucator& alta)
    : numeNava(alta.numeNava + "_Backup"), integritate(alta.integritate),
      locatie(alta.locatie), armament(alta.armament), aspect(alta.aspect) {}

NavaJucator& NavaJucator::operator=(const NavaJucator& alta) {
    if (this != &alta) {
        this->numeNava = alta.numeNava;
        this->integritate = alta.integritate;
        this->locatie = alta.locatie;
        this->armament = alta.armament;
        this->aspect = alta.aspect;
    }
    return *this;
}

class MotorGrafic {
private:
    int lungime, inaltime;

public:
    explicit MotorGrafic(int l = 30, int h = 10) : lungime(l), inaltime(h) {}

    void scena(const NavaJucator& nava, const std::vector<Inamic>& inamici, const std::vector<Diamant>& diamante) {
        for(int i = 0; i < 10; ++i) std::cout << "\n";
        std::cout << nava << "\n";
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
        std::cout << "\n";

        for (int y = 0; y < inaltime; y++) {
            std::cout << "|";
            for (int x = 0; x < lungime; x++) {
                bool obiectDesenat = false;

                // Prioritate 1: Nava
                if (x == nava.x() && y == nava.y()) {
                    std::cout << nava.getAspect();
                    obiectDesenat = true;
                }
                // Prioritate 2: Inamici
                if (!obiectDesenat) {
                    for (const auto& in : inamici) {
                        if (in.getX() == x && in.getY() == y) {
                            std::cout << in.getSimbol();
                            obiectDesenat = true;
                            break;
                        }
                    }
                }
                // Prioritate 3: Diamante
                if (!obiectDesenat) {
                    for (const auto& d : diamante) {
                        if (d.getX() == x && d.getY() == y) {
                            std::cout << d.getSimbol();
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

class Statistici {
private:
    int inamiciOmorati;
public:
    Statistici() : inamiciOmorati(0) {}
    void adaugaMoarte() { inamiciOmorati++; }
    int getScor() const { return inamiciOmorati * 100; }
};

class InterfataUtilizator {
public:
    static void afiseazaBanner() {
        std::cout << "==============================\n";
        std::cout << "   SPACE DEFENDER v1.0\n";
        std::cout << "==============================\n";
    }

    static void afiseazaGameOver(int scor) {
        std::cout << "\n******************************\n";
        std::cout << "          GAME OVER          \n";
        std::cout << "      Scor Final: " << scor << "\n";
        std::cout << "******************************\n";
    }
};
class GameMaster {
private:
    int level;
    int pragNivel;
    std::vector<std::string> log;

public:
    GameMaster() {
        level = 1;
        pragNivel = 500;
    }
    void update(int scorActual) {
        if (scorActual >= level * pragNivel) {
            level++;
            log.push_back("--- NIVELUL "+ std::to_string(level)+" A INCEPUT ---");
        }
    }
    void addEvent(std::string msg) {
        if (log.size() > 4) {
            log.erase(log.begin());
        }
        log.push_back(msg);
    }
    void showStatus() {
        std::cout << "\n>>> LEVEL: " << level << "<<<\n";
        for (const auto& msg : log) {
            std::cout << "  - " << msg << "\n";
        }
    }
    int getSpawnChance() {
        int chance = 10 + (level * 3);
        return (chance > 35) ? 35 : chance;
    }
};


int main() {
    srand(time(0));
    GameMaster gm;
    InterfataUtilizator::afiseazaBanner();
    NavaJucator albuquerque("Interceptor", 15, 9);
    MotorGrafic motor(30, 10);
    std::vector<Inamic> listaInamici;
    std::vector<Diamant> listaDiamante;
    Statistici stats;


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
        for (auto it = listaInamici.begin(); it != listaInamici.end(); ) {
            if (it->getViata() <= 0) {
                stats.adaugaMoarte();
                it =listaInamici.erase(it);
            }
            else if (it->getY() >= 10) {
                InterfataUtilizator::afiseazaGameOver(stats.getScor());
                return 0;
            }
            else {
                ++it;
            }
        }
        for (auto it = listaDiamante.begin(); it != listaDiamante.end(); ) {
            it->miscare();
            if (it->getX() == albuquerque.x() && it->getY() == albuquerque.y()) {
                for(int i = 0; i < it->getValoare()/100; ++i) stats.adaugaMoarte();
                std::cout << "DIAMANT COLECTAT! +500\n";
                it = listaDiamante.erase(it);
            }
            else if (it->getY() >= 10) {
                it = listaDiamante.erase(it);
            }
            else {
                ++it;
            }
        }
        if (rand() % 100 < gm.getSpawnChance()) {
            int rx = rand() % motor.getL();
            int rTip = rand() % 100;

            if (rTip < 75) {
                listaInamici.push_back(Inamic(20, rx, 0, 'v'));
            } else {
                listaInamici.push_back(Inamic(65, rx, 0, 'W'));
                gm.addEvent("Atentie: Radarul a detectat un inamic greu la X=" + std::to_string(rx));
            }
        }
        if (rand() % 100 < 5) {
            listaDiamante.push_back(Diamant(rand() % motor.getL(), 0));
        }
        gm.update(stats.getScor());
        gm.showStatus();
        motor.scena(albuquerque, listaInamici, listaDiamante);
        std::cout << "Input (A/D/F/Q): ";
    }
    std::cout << "Inchidere...\n";
    NavaJucator backup =albuquerque;
    std::cout << "Backup creat:" <<backup <<"\n";
    return 0;
}