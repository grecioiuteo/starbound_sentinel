#include "GameplayComponents.h"
#include <cstdlib>
#include <utility>

Pozitie::Pozitie(int val_x, int val_y) : x(val_x), y(val_y) {}
int Pozitie::getX() const { return x; }
int Pozitie::getY() const { return y; }

void Pozitie::setX(int val) { if (val >= 0 && val < 30) x = val; }
void Pozitie::setY(int val) { y = val; }
bool Pozitie::operator==(const Pozitie& alta) const { return (x == alta.x && y == alta.y); }
std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), armura(s == 'W' ? 10 : 0), loc(x, y), simbol(s), viataMax(v), s_a_transformat(false) {
    if (x < 0 || x >= 30 || y < 0 || y > 20) {
        throw PozitieInvalidaException("Inamicul a spawnat in afara ecranului la (" + std::to_string(x) + "," + std::to_string(y) + ")");
    }
}

void Inamic::scadeViata(int dmg) {
    if (this->armura > 0) {
        this->armura = 0;
        std::cout << "--- SCUT DISTRUS! ---\n";
    } else {
        this->viata -= dmg;
    }

    if (!s_a_transformat) {
        if (simbol == 'W' && armura == 0) {
            simbol = 'v';
            viata = 10;
            s_a_transformat = true;
        }
    }
}

void InamicMeteorit::miscareInamic() {
    static int tureBlocat = 0;
    static bool esteInColiziune = false;

    if (this->navaTinta != nullptr) {
        NavaJucator* navaModificabila = const_cast<NavaJucator*>(this->navaTinta);
        int xJucator = navaModificabila->x();
        int xMeteorit = loc.getX();

        if (esteInColiziune) {
            if (tureBlocat > 0) {
                tureBlocat--;
                if (xJucator != xMeteorit) {
                    if (xJucator > xMeteorit) {
                        navaModificabila->miscare('A', 30);
                    } else {
                        navaModificabila->miscare('D', 30);
                    }
                }
                return;
            } else {
                esteInColiziune = false;
                this->viata = 0;
                return;
            }
        }

        if ((loc.getY() == 18 || loc.getY() == 19) && xMeteorit == xJucator) {
            esteInColiziune = true;
            tureBlocat = 5;
            this->simbol = 'X';
            return;
        }
    }

    loc.setY(loc.getY() + 1);

    if (this->navaTinta != nullptr) {
        int xJucator = this->navaTinta->x();
        int xMeteorit = loc.getX();

        if (xMeteorit < xJucator) {
            loc.setX(xMeteorit + 3);
        }
        else if (xMeteorit > xJucator) {
            loc.setX(xMeteorit - 3);
        }
    }
}

Diamant::Diamant(int x, int y) : loc(x, y), simbol('*') {}
void Diamant::miscare() { loc.setY(loc.getY() + 1); }
int Diamant::getX() const { return loc.getX(); }
int Diamant::getY() const { return loc.getY(); }
char Diamant::getSimbol() const { return simbol; }

PowerUp::PowerUp(int x, int y) : loc(x, y), simbol('P') {}
void PowerUp::miscare() { loc.setY(loc.getY() + 1); }
int PowerUp::getX() const { return loc.getX(); }
int PowerUp::getY() const { return loc.getY(); }
char PowerUp::getSimbol() const { return simbol; }

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

Proiectil::Proiectil(int x, int y) : loc(x, y), activ(true) {}
void Proiectil::miscare() {
    loc.setY(loc.getY() - 1);
    if (loc.getY() < 0) activ = false;
}
int Proiectil::getX() const { return loc.getX(); }
int Proiectil::getY() const { return loc.getY(); }

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

MotorGrafic::MotorGrafic(int l, int h) : lungime(l), inaltime(h) {}
void MotorGrafic::scena(const NavaJucator& nava, const std::vector<Inamic*>& inamici, const std::vector<Diamant>& diamante, const std::vector<PowerUp>& powerups, const std::vector<Proiectil>& proiectile, int scorActual) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string CYAN    = "\033[36m";
    const std::string MAGENTA = "\033[35m";
    const std::string BOLD    = "\033[1m";

    std::cout << CYAN << "==============================================================\n" << RESET;
    std::cout << BOLD << GREEN << " " << nava << RESET << "\n";
    std::cout << BOLD << YELLOW << " >>> SCOR: " << scorActual
              << " | VIEȚI BAZĂ: " << (nava.getVieti() == 3 ? GREEN : (nava.getVieti() == 2 ? YELLOW : RED))
              << nava.getVieti() << " / 3" << YELLOW << " <<<\n" << RESET;
    std::cout << CYAN << "==============================================================\n" << RESET;

    for (int i = 0; i < (lungime + 2); i++) std::cout << CYAN << (i % 2 == 0 ? "=" : "-") << RESET;
    std::cout << "\n";

    for (int y = 0; y < inaltime; y++) {
        std::cout << CYAN << "|" << RESET;
        for (int x = 0; x < lungime; x++) {
            bool obiectDesenat = false;

            if (x == nava.x() && y == nava.y()) {
                std::cout << BOLD << GREEN << nava.getAspect() << RESET;
                obiectDesenat = true;
            }

            if (!obiectDesenat) {
                for (const Inamic* const & in : inamici) {
                    if (in->getX() == x && in->getY() == y) {
                        if (in->getSimbol() == 'W') {
                            std::cout << BOLD << RED << "W" << RESET;
                        } else if (in->getSimbol() == 'v') {
                            std::cout << RED << "v" << RESET;
                        } else {
                            std::cout << MAGENTA << in->getSimbol() << RESET;
                        }
                        obiectDesenat = true;
                        break;
                    }
                }
            }

            if (!obiectDesenat) {
                for (const auto& d : diamante) {
                    if (d.getX() == x && d.getY() == y) {
                        std::cout << BOLD << YELLOW << "*" << RESET;
                        obiectDesenat = true;
                        break;
                    }
                }
            }

            if (!obiectDesenat) {
                for (const auto& p : powerups) {
                    if (p.getX() == x && p.getY() == y) {
                        std::cout << BOLD << CYAN << "P" << RESET;
                        obiectDesenat = true;
                        break;
                    }
                }
            }

            if (!obiectDesenat) {
                for (const auto& p : proiectile) {
                    if (p.getX() == x && p.getY() == y) {
                        std::cout << YELLOW << "|" << RESET;
                        obiectDesenat = true;
                        break;
                    }
                }
            }

            if (!obiectDesenat) std::cout << " ";
        }
        std::cout << CYAN << "|\n" << RESET;
    }

    for (int i = 0; i < (lungime + 2); i++) std::cout << CYAN << (i % 2 == 0 ? "=" : "-") << RESET;
    std::cout << "\n";

    std::cout << BOLD << "\n ENTITĂȚI SPAȚIALE:\n" << RESET
              << "   " << GREEN << "N" << RESET << " = Nava Ta (Interceptor)\n"
              << "   " << RED << "v" << RESET << " = Inamic Scout (Rapid) | " << BOLD << RED << "W" << RESET << " = Cruiser (Scut Greu) | " << MAGENTA << "F" << RESET << " = Fregată\n"
              << "   " << YELLOW << "*" << RESET << " = Diamant (+150 Pcte)  | " << CYAN << "P" << RESET << " = Kit Reparații (+Scut/HP)\n"
              << "   " << YELLOW << "|" << RESET << " = Proiectil Laser\n";

    std::cout << CYAN << "--------------------------------------------------------------\n" << RESET;
    std::cout << BOLD << " PANOU CONTROL: " << RESET
              << BOLD << YELLOW << "[A]" << RESET << " Stânga  | "
              << BOLD << YELLOW << "[D]" << RESET << " Dreapta  | "
              << BOLD << YELLOW << "[F]" << RESET << " Foc!  | "
              << BOLD << YELLOW << "[SPACE]" << RESET << " Staționează  | "
              << BOLD << RED << "[Q]" << RESET << " Abandon\n";
}
int MotorGrafic::getL() const { return lungime; }

Statistici::Statistici() : scor(0) {}
void Statistici::adaugaMoarte() { scor += 100; }
void Statistici::adaugaPuncte(int puncte) { scor += puncte; }
int Statistici::getScor() const { return scor; }

void InterfataUtilizator::afiseazaBanner() {
    std::cout << "==============================\n" << "      STARBOUND SENTINEL      \n" << "==============================\n";
}
void InterfataUtilizator::afiseazaGameOver(int scor) {
    std::cout << "\n******************************\n" << "          GAME OVER          \n" << "      Scor Final: " << scor << "\n" << "******************************\n";
}

GameMaster::GameMaster() { level = 1; pragNivel = 500; }
void GameMaster::update(int scorActual) {
    if (scorActual >= level * pragNivel) {
        level++;
        log.push_back("--- NIVELUL " + std::to_string(level) + " A INCEPUT ---");
    }
}
void GameMaster::addEvent(const std::string& msg) {
    if (log.size() > 4) { log.erase(log.begin()); }
    log.push_back(msg);
}
void GameMaster::showStatus() {
    std::cout << "\n>>> LEVEL: " << level << "<<<\n";
    for (const auto& msg : log) { std::cout << "  - " << msg << "\n"; }
}
int GameMaster::getSpawnChance() {
    int chance = 8 + (level * 2);
    return (chance > 25) ? 25 : chance;
}

Realizari::Realizari() : inamiciDistrusi(0), asulZborului(false) {}
void Realizari::inamicDoborat() {
    inamiciDistrusi++;
    if (inamiciDistrusi >= 5) asulZborului = true;
}
std::ostream& operator<<(std::ostream& os, const Realizari& r) {
    os << "Statistici Pilot: " << r.inamiciDistrusi << " kill-uri | Rank: " << (r.asulZborului ? "ASUL SPATIULUI" : "RECRUT");
    return os;
}

FlotaManager::~FlotaManager() {
    curataFlota();
}

FlotaManager::FlotaManager(const FlotaManager& alta) {
    for (const Inamic* inamic : alta.naveFlota) {
        this->naveFlota.push_back(inamic->clone());
    }
}

FlotaManager& FlotaManager::operator=(FlotaManager alta) {
    std::swap(this->naveFlota, alta.naveFlota);
    return *this;
}

void FlotaManager::adaugaInamic(Inamic* inamicNou) {
    naveFlota.push_back(inamicNou);
}

std::vector<Inamic*>& FlotaManager::getInamici() {
    return naveFlota;
}

void FlotaManager::curataFlota() {
    for (Inamic* inamic : naveFlota) {
        delete inamic;
    }
    naveFlota.clear();
}