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

    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int val) {
        if (val >= 0 && val < 100) x = val;
    }

    void setY(int val) { y = val; }
    bool operator==(const Pozitie& alta) const {
        return (x == alta.x && y == alta.y);
    }

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
        os << "("<<p.x<<", "<<p.y<<")";
        return os;
    }
};

class Inamic {
private:
    int viata;
    int armura;
    Pozitie loc;
    char simbol;
    int viataMax;
    bool s_a_transformat;
public:
    Inamic(int v, int x, int y, char s);

    void miscareInamic();

    int getX() const { return loc.getX(); }
    int getY() const { return loc.getY(); }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }

    void scadeViata(int dmg);

    void reseteazaStare() {
        viata = viataMax;
        s_a_transformat = false;
    }
};
Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), armura(0), loc(x, y), simbol(s), viataMax(v), s_a_transformat(false) {}
void Inamic::miscareInamic() {
    loc.setY(loc.getY() + 1);
}
void Inamic::scadeViata(int dmg) {
    if (this->armura > 0) {
        std::cout << "--- IMPACT IN ARMURA! ---\n";
        int dmgBlocat = dmg / 2;
        this->armura -= dmgBlocat;

        if (this->armura < 0) {
            int rest = -this->armura;
            this->armura = 0;
            this->viata -= (dmgBlocat + rest);
        } else {
            this->viata -= (dmg - dmgBlocat);
        }
    } else {
        this->viata -= dmg;
    }
    if (!s_a_transformat && viata <= viataMax / 2) {
        if (simbol == 'W') simbol = 'w';
        else if (simbol == 'U') simbol = 'u';
        else if (simbol == 'X') simbol = 'x';
        s_a_transformat = true;
    }
}

class Diamant {
private:
    Pozitie loc;
    char simbol;
    int valoare;
public:
    Diamant(int x, int y) : loc(x, y), simbol('*'), valoare(500) {}
    void miscare() {loc.setY(loc.getY() + 1); }
    int getX()const {return loc.getX(); }
    int getY() const{ return loc.getY(); }
    char getSimbol() const {return simbol;}
};
class PowerUp {
private:
    Pozitie loc;
    std::string tip;
    char simbol;
public:
    PowerUp(int x, int y, const std::string& t) : loc(x, y), tip(t) {
        if (tip == "Scut") simbol = 'S';
        else if (tip == "FocRapid") simbol = 'R';
        else simbol = 'P';
    }
    void miscare() {loc.setY(loc.getY() + 1); }
    int getX() const { return loc.getX(); }
    int getY() const {return loc.getY(); }
    char getSimbol() const { return simbol; }
};
class Arsenal {
    private:
    int munitie;
    int putereAtac;
    std::string tipArma;
    int capacitateMaxima;
public:
    explicit Arsenal(const std::string& tip = "Laser", int mun = 40, int pwr = 10) : munitie(mun), putereAtac(pwr), tipArma(tip), capacitateMaxima(40) {}
    Arsenal& operator+=(int cantitate) {
        this->munitie += cantitate;
        if (this->munitie > capacitateMaxima) this->munitie = capacitateMaxima;
        return *this;
    }

    bool trage();
    int reincarca();
    int getDmg() const { return putereAtac; }

    friend std::ostream& operator<<(std::ostream& os, const Arsenal& a) {
        os << "Mun: " << a.munitie << "/" << a.capacitateMaxima << " [" << a.tipArma << "]";
        return os;
    }

};
bool Arsenal::trage() {
    if (munitie > 0) {
        munitie--;
        return true;
    }
    return false;
}
int Arsenal::reincarca() {
    this->munitie = this->capacitateMaxima;
    std::cout << "Munitie refacuta!\n";
    return munitie;
}

class Proiectil {
private:
    Pozitie loc;
    bool activ;
public:
    Proiectil(int x, int y) : loc(x, y), activ(true) {}
    void miscare() {
        loc.setY(loc.getY() - 1);
        if (loc.getY() < 0) activ = false;
    }
    int getX() const {return loc.getX(); }
    int getY() const{return loc.getY(); }
    bool esteActiv() const { return activ;}
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
    int getVieti() const {
        if (integritate > 66) return 3;
        if (integritate > 33) return 2;
        if (integritate > 0) return 1;
        return 0;
    }
    bool esteOperationala() const { return integritate > 0; }
    void autoDiagnostic() const {
        std::cout << "Diagnostic: ";
        if (integritate > 50) std::cout << "Sisteme Nominale.\n";
        else std::cout << "Sisteme Avariate - Necesita Reparatii!\n";
    }
    explicit NavaJucator(const std::string& nume, int startX, int startY);
    NavaJucator(const NavaJucator& alta);
    NavaJucator& operator=(const NavaJucator& alta);

    ~NavaJucator()= default;
    void miscare(char tasta, int limitaX) {
        int nx = locatie.getX();
        int viteza = 2;

        if ((tasta == 'a' || tasta == 'A') && nx >= viteza) nx -= viteza;
        if ((tasta == 'd' || tasta == 'D') && nx <= limitaX - viteza - 1) nx += viteza;

        locatie.setX(nx);
    }
    int actiuneAtac() {
        if (armament.trage()) {
            return armament.getDmg() + bonusDamage;
        } else {
            std::cout << "Eroare: Munitie insuficienta!\n";
            return 0;
        }
    }
    int x()const { return locatie.getX();}
    int y() const { return locatie.getY(); }
    char getAspect()const {return aspect; }

    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n) {
        os << "[" << n.numeNava << "] HP: " << n.integritate << "% | Pozitie: " << n.locatie << " | Armament: " << n.armament;
        return os;
    }
    int executaReincarcare() {
        return armament.reincarca();
    }

    void primesteLovitura(int dmg) {
        if (valoareScut > 0) {
            valoareScut -= dmg;
            if (valoareScut < 0) valoareScut = 0;
        } else {
            integritate -= dmg;
        }
    }

    int getAtacTotal() const {
        return armament.getDmg() + bonusDamage;
    }
    Arsenal& getArmament() { return armament; }
};
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

class MotorGrafic {
private:
    int lungime, inaltime;

public:
    explicit MotorGrafic(int l = 60, int h = 20) : lungime(l), inaltime(h) {}

    void scena(const NavaJucator& nava, const std::vector<Inamic>& inamici, const std::vector<Diamant>& diamante, const std::vector<PowerUp>& powerups, const std::vector<Proiectil>& proiectile, int scorActual) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
        std::cout << nava << "\n";
        std::cout << ">>> SCOR: " << scorActual << " | VIEȚI BAZĂ: " << nava.getVieti() << " / 3 <<<\n";

    for (int i = 0; i < (lungime + 2); i++) std::cout << (i % 2 == 0 ? "=" : " ");
    std::cout << "\n";

    for (int y = 0; y < inaltime; y++) {
        std::cout << "|";
        for (int x = 0; x < lungime; x++) {
            bool obiectDesenat = false;
            if (x == nava.x() && y == nava.y()) {
                std::cout << nava.getAspect();
                obiectDesenat = true;
            }
            if (!obiectDesenat) {
                for (const auto& in : inamici) {
                    if (in.getX() == x && in.getY() == y) {
                        std::cout << in.getSimbol();
                        obiectDesenat = true;
                        break;
                    }
                }
            }
            if (!obiectDesenat) {
                for (const auto& d : diamante) {
                    if (d.getX() == x && d.getY() == y) {
                        std::cout << d.getSimbol();
                        obiectDesenat = true;
                        break;
                    }
                }
            }
            if (!obiectDesenat) {
                for (const auto& p : powerups) {
                    if (p.getX() == x && p.getY() == y) {
                        std::cout << p.getSimbol();
                        obiectDesenat = true;
                        break;
                    }
                }
            }
            if (!obiectDesenat) {
                for (const auto& p : proiectile) {
                    if (p.getX() == x && p.getY() == y) {
                        std::cout << "|";
                        obiectDesenat = true;
                        break;
                    }
                }
            }
            if (!obiectDesenat) std::cout << " ";
        }
        std::cout << "|\n";
    }
    for (int i = 0; i < (lungime + 2); i++) std::cout << (i % 2 == 0 ? "=" : " ");
    std::cout << "\n";
    std::cout << "\nLEGENDA: N=Nava, v/W=Inamic, *=Diamant, S/R=PowerUp, |=Glonat\n";
    std::cout << "Comenzi: A/D (Miscare), F (Atac), Space (Asteapta), Q (Iesire)\n";
}
    int getL() const { return lungime; }
};

class Statistici {
private:
    int scor;
public:
    Statistici() : scor(0) {}
    void adaugaMoarte() { scor += 100; }
    void adaugaPuncte(int puncte) { scor += puncte; }
    int getScor() const { return scor; }
};

class InterfataUtilizator {
public:
    static void afiseazaBanner() {
        std::cout << "==============================\n";
        std::cout << "      STARBOUND SENTINEL      \n";
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
    void addEvent(const std::string& msg) {
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
        int chance = 8 + (level * 2);
        return (chance > 25) ? 25 : chance;
    }
};
class ManagerResurse {
public:
    static std::string obtineMesajInfrangere(int scor) {
        if (scor < 1000) return "Mai incearca! Data viitoare va fi mai bine.";
        if (scor < 5000) return "O performanta bunaa!";
        return "Esti o legenda a spatiului!";
    }
};
class Realizari {
private:
    int inamiciDistrusi;
    bool asulZborului;
public:
    Realizari() : inamiciDistrusi(0), asulZborului(false) {}

    void inamicDoborat() {
        inamiciDistrusi++;
        if (inamiciDistrusi >= 5) asulZborului = true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Realizari& r) {
        os<< "Statistici Pilot: " << r.inamiciDistrusi << " kill-uri | Rank: "
           << (r.asulZborului ? "ASUL SPATIULUI":"RECRUT");
        return os;
    }
};


int main() {
    srand(time(0));
    GameMaster gm;
    Realizari progres;
    InterfataUtilizator::afiseazaBanner();
    NavaJucator albuquerque("Interceptor", 30, 19);
    MotorGrafic motor(60, 20);
    std::vector<Inamic> listaInamici;
    std::vector<Diamant> listaDiamante;
    std::vector<PowerUp> listaPowerUps;
    std::vector<Proiectil> listaProiectile;
    Statistici stats;

    std::cout << "\n>>> INSTRUCTIUNI: Introdu comenzi si apasa ENTER<<<\n";
    std::cout << "\n>>> ATENTIE: Pe masura ce avansezi in nivele, apar din ce in ce mai multi inamici.<<<\n";
    std::cout << "\n>>> Prin urmare, nu te speria ca la primul nivel apar inamicii mai greu la inceput<<<\n";
    std::cout << ">>> (A=Stanga, D=Dreapta, F=Foc, SPACE=Asteapta, Q=Iesire) <<<\n";
    std::cout << "\nApasa o tasta si ENTER pentru start: ";
    std::string asteaptaStart;
    std::getline(std::cin, asteaptaStart);
    std::string inputComplex;

    while (albuquerque.esteOperationala()) {
        motor.scena(albuquerque, listaInamici, listaDiamante, listaPowerUps, listaProiectile, stats.getScor());
        gm.showStatus();

        std::cout << "\nComanda ta (A/D/F/Space/Q): ";
        std::getline(std::cin, inputComplex);
        if (inputComplex.empty()) inputComplex = " ";

        char tasta = inputComplex[0];
        if (tasta == 'q' || tasta == 'Q') goto finalJoc;
        if (tasta == ' ') {
            gm.addEvent("Nava stationeaza...");
        }
        else if (tasta == 'a' || tasta == 'A' || tasta == 'd' || tasta == 'D') {
            albuquerque.miscare(tasta, motor.getL());
        }
        else if (tasta == 'f' || tasta == 'F') {
            if (albuquerque.actiuneAtac() > 0) {
                listaProiectile.push_back(Proiectil(albuquerque.x(), albuquerque.y() - 1));
            } else {
                albuquerque.executaReincarcare();
            }
        }
        for (auto& p : listaProiectile) p.miscare();
        std::vector<int> coloaneDistruse;

        for (auto itP = listaProiectile.begin(); itP != listaProiectile.end(); ) {
            bool glontConsumat = false;
            for (auto itIn = listaInamici.begin(); itIn != listaInamici.end(); ) {
                if (itP->getX() == itIn->getX() && (itP->getY() == itIn->getY() || itP->getY() == itIn->getY() + 1)) {

                    itIn->scadeViata(albuquerque.getAtacTotal());
                    glontConsumat = true;

                    if (itIn->getViata() <= 0) {
                        stats.adaugaMoarte();
                        progres.inamicDoborat();
                        gm.addEvent("Target Distrus!");
                        coloaneDistruse.push_back(itIn->getX());
                        itIn = listaInamici.erase(itIn);
                    } else {
                        gm.addEvent("Hit!");
                        ++itIn;
                    }
                    break;
                } else ++itIn;
            }

            if (glontConsumat || !itP->esteActiv()) itP = listaProiectile.erase(itP);
            else ++itP;
        }

        for (auto itP = listaProiectile.begin(); itP != listaProiectile.end(); ) {
            bool peColoanaDistrusa = false;
            for (int col : coloaneDistruse) {
                if (itP->getX() == col) {
                    peColoanaDistrusa = true;
                    break;
                }
            }

            if (peColoanaDistrusa) itP = listaProiectile.erase(itP);
            else ++itP;
        }

        for (auto it = listaDiamante.begin(); it != listaDiamante.end(); ) {
            it->miscare();
            if (it->getX() == albuquerque.x() && it->getY() == albuquerque.y()) {
                stats.adaugaPuncte(150);
                gm.addEvent("Diamant colectat! +150");
                it = listaDiamante.erase(it);
            } else if (it->getY() >= 20) {
                it = listaDiamante.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = listaInamici.begin(); it != listaInamici.end(); ) {
            it->miscareInamic();
            if (it->getY() >= 20) {
                albuquerque.primesteLovitura(34);
                gm.addEvent("Baza a fost lovita!");
                it = listaInamici.erase(it);
            } else { ++it; }
        }

        if (rand() % 100 < gm.getSpawnChance()) {
            int spawnX = (rand() % (motor.getL() / 2)) * 2;
            listaInamici.push_back(Inamic(10, spawnX, 1, 'v'));
        }
        if (rand() % 100 < 4) {
            int diamantX = (rand() % (motor.getL() / 2)) * 2;
            listaDiamante.push_back(Diamant(diamantX, 0));
        }
        gm.update(stats.getScor());
    }
finalJoc:
    InterfataUtilizator::afiseazaGameOver(stats.getScor());
    std::cout << "\n--- RAPORT FINAL ---\n";
    std::cout << "Nava la finalul misiunii: " << albuquerque << "\n";
    std::string mesajFinal = ManagerResurse::obtineMesajInfrangere(stats.getScor());
    std::cout << "Comandament: " << mesajFinal << "\n";
    NavaJucator backup = albuquerque;
    int munitieFinala = backup.getArmament().reincarca();
    std::cout << "Munitie dupa reincarcare: " << munitieFinala << std::endl;
    if (albuquerque.getAtacTotal() > 20) {
        std::cout << "Nava a terminat cu upgrade-uri de atac active.\n";
    }
    std::cout << progres << "\n";

    backup.getArmament() += 10;
    std::cout << "--- SISTEM BACKUP ACTIVAT ---\n";
    std::cout << backup << "\n";
    if (!listaInamici.empty()) {
        listaInamici[0].reseteazaStare();
    }
    std::cout << "Inchidere...\n";
    albuquerque.autoDiagnostic();
    return 0;
}