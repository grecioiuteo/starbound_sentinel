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

    void reseteazaStare() {
        viata = viataMax;
        s_a_transformat = false;
    }
};
Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), armura(30), posX(x), posY(y), simbol(s), viataMax(v), s_a_transformat(false) {}
void Inamic::miscareInamic() {
    this->posY++;
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
class PowerUp {
private:
    int px, py;
    std::string tip;
    char simbol;
public:
    PowerUp(int x, int y, const std::string& t) : px(x), py(y), tip(t) {
        if (tip == "Scut") simbol = 'S';
        else if (tip == "FocRapid") simbol = 'R';
        else simbol = 'P';
    }
    void miscare() { py++; }
    int getX() const { return px; }
    int getY() const { return py; }
    char getSimbol() const { return simbol; }
    const std::string& getTip() const { return tip; }
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
    int px, py;
    bool activ;
public:
    Proiectil(int x, int y) : px(x), py(y), activ(true) {}
    void miscare() { py--;if (py < 0) activ = false; }
    int getX() const {return px; }
    int getY() const{return py; }
    bool esteActiv() const { return activ;}
    void dezactiveaza() { activ= false; }
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

        if ((tasta == 'a' || tasta == 'A') && nx > 0) nx--;
        if ((tasta == 'd' || tasta == 'D') && nx < limitaX - 1) nx++;

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
    void aplicaPowerUp(const std::string& tip) {
        if (tip == "Scut") {
            valoareScut = 50;
        } else if (tip == "FocRapid") {
            bonusDamage = 15;
        }
    }

    void primesteLovitura(int dmg) {
        if (valoareScut > 0) {
            valoareScut -= dmg;
            if (valoareScut < 0) valoareScut = 0;
        } else {
            integritate -= dmg;
        }
    }

    int getAtacTotal() {
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
    explicit MotorGrafic(int l = 30, int h = 10) : lungime(l), inaltime(h) {}

    void scena(const NavaJucator& nava, const std::vector<Inamic>& inamici, const std::vector<Diamant>& diamante,const std::vector<PowerUp>& powerups, const std::vector<Proiectil>& proiectile) {
        for(int i = 0; i < 10; ++i) std::cout << "\n";
        std::cout << nava << "\n";
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
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
        int chance = 10 + (level * 3);
        return (chance > 35) ? 35 : chance;
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
    NavaJucator albuquerque("Interceptor", 15, 9);
    MotorGrafic motor(30, 10);
    std::vector<Inamic> listaInamici;
    std::vector<Diamant> listaDiamante;
    std::vector<PowerUp> listaPowerUps;
    std::vector<Proiectil> listaProiectile;
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
                listaProiectile.push_back(Proiectil(albuquerque.x(), albuquerque.y() - 1));
                gm.addEvent("FOC: Racheta lansata!");
            }
        }
        for (auto& inamic : listaInamici) {
            inamic.miscareInamic();
        }
        for (auto it = listaInamici.begin(); it != listaInamici.end(); ) {
            if (it->getViata() <= 0) {
                stats.adaugaMoarte();
                progres.inamicDoborat();
                it = listaInamici.erase(it);
            }
            else if (it->getY() >= 10) {
                albuquerque.primesteLovitura(20);
                gm.addEvent("AVARIE: Inamicul a trecut de defensiva!");
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
        for (auto it = listaPowerUps.begin(); it != listaPowerUps.end(); ) {
            it->miscare();
            if (it->getX() == albuquerque.x() && it->getY() == albuquerque.y()) {
                albuquerque.aplicaPowerUp(it->getTip());
                gm.addEvent("BONUS ACTIVAT: " + it->getTip());
                it = listaPowerUps.erase(it);
            } else if (it->getY() >= 10) {
                it = listaPowerUps.erase(it);
            } else {
                ++it;
            }
        }
        for (auto& p : listaProiectile) p.miscare();
        for (auto& p : listaProiectile) {
            if (!p.esteActiv()) continue;
            for (auto& in : listaInamici) {
                if (p.getX() == in.getX() && p.getY() == in.getY()) {
                    in.scadeViata(albuquerque.getAtacTotal());
                    p.dezactiveaza();
                    gm.addEvent("Target Hit! Inamic lovit la " + std::to_string(p.getX()));
                }
            }
        }
        for (auto it = listaProiectile.begin(); it != listaProiectile.end(); ) {
            if (!it->esteActiv()) it = listaProiectile.erase(it);
            else ++it;
        }
        if (rand() % 100 < 5) {
            std::string t = (rand() % 2 == 0) ? "Scut" : "FocRapid";
            listaPowerUps.push_back(PowerUp(rand() % motor.getL(), 0, t));
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
        motor.scena(albuquerque, listaInamici, listaDiamante, listaPowerUps,listaProiectile);
        std::cout << "Input (A/D/F/Q): ";
    }
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