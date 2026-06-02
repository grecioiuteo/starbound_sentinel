#include "../include/sistemJoc.h"
#include <cstdlib>

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