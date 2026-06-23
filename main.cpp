#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "include/sistemJoc.h"
#include "include/inamici.h"
#include "include/navaJucator.h"
#include "include/entitatiBaza.h"
#include "include/exceptii.h"

#ifdef _WIN32
#include <windows.h>
#endif

void curataProiectileInactive(std::vector<Proiectil>& listaProiectile);
void proceseazaInputJucator(char tasta, NavaJucator& albuquerque, std::vector<Proiectil>& listaProiectile, FlotaManager& flota, GameMaster& gm, bool esteNavaUpgrade, const MotorGrafic& motor);
void calculeazaColiziuniProiectile(std::vector<Proiectil>& listaProiectile, FlotaManager& flota, const std::vector<int>& vechiulYProiectile, GameMaster& gm, const NavaJucator& albuquerque);
void curataInamiciDistrusi(FlotaManager& flota, Statistici& stats, Realizari& progres);
void updateObiecteSiMiscari(ResourceCollection<Diamant>& listaDiamante, ResourceCollection<PowerUp>& listaPowerUps, FlotaManager& flota, NavaJucator& albuquerque, Statistici& stats, GameMaster& gm);
void genereazaEntitatiNoi(FlotaManager& flota, GameMaster& gm, const NavaJucator& albuquerque, ResourceCollection<PowerUp>& listaPowerUps, ResourceCollection<Diamant>& listaDiamante, const Statistici& stats);
int main() {
    srand(static_cast<unsigned int>(time(0)));
    GameMaster& gm = GameMaster::getInstance();

#ifdef _WIN32
    system("chcp 65001 > nul");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    Realizari progres;
    InterfataUtilizator::afiseazaBanner();

    NavaJucator albuquerque("Interceptor", 15, 19);
    MotorGrafic motor(30, 20);

    FlotaManager flota;
    ResourceCollection<Diamant> listaDiamante;
    ResourceCollection<PowerUp> listaPowerUps;
    std::vector<Proiectil> listaProiectile;
    Statistici stats;

    std::cout << "\n>>> CONTROL: A = Stanga | D = Dreapta | F = Foc | SPACE = Pas <<<" << std::endl;
    std::cout << "\nApasa ENTER pentru a incepe jocul... ";
    std::string asteaptaStart;
    std::getline(std::cin, asteaptaStart);
    std::string inputComplex;

    while (albuquerque.esteOperationala()) {
        try {
            curataProiectileInactive(listaProiectile);

            motor.scena(albuquerque, flota.getInamici(), listaDiamante.getElemente(), listaPowerUps.getElemente(), listaProiectile, stats.getScor());
            gm.showStatus();

            std::cout << "\nCe faci? (A/D/F/Space/Q): ";
            std::getline(std::cin, inputComplex);
            if (inputComplex.empty()) inputComplex = " ";

            char tasta = inputComplex[0];
            if (tasta == 'q' || tasta == 'Q') break;

            bool esteNavaUpgrade = (stats.getScor() >= 1500);

            proceseazaInputJucator(tasta, albuquerque, listaProiectile, flota, gm, esteNavaUpgrade, motor);

            std::vector<int> vechiulYProiectile;
            for (size_t i = 0; i < listaProiectile.size(); ++i) {
                vechiulYProiectile.push_back(listaProiectile[i].getY());
                listaProiectile[i].miscare();
            }

            calculeazaColiziuniProiectile(listaProiectile, flota, vechiulYProiectile, gm, albuquerque);

            curataInamiciDistrusi(flota, stats, progres);

            updateObiecteSiMiscari(listaDiamante, listaPowerUps, flota, albuquerque, stats, gm);

            genereazaEntitatiNoi(flota, gm, albuquerque, listaPowerUps, listaDiamante, stats);

        }
        catch (const MunitieInsuficientaException& e) {
            gm.addEvent(e.what() + std::string(" Se executa reincarcarea automata..."));
            albuquerque.executaReincarcare();
        }
        catch (const PozitieInvalidaException& e) {
            gm.addEvent(e.what());
        }
        catch (const NavaDistrusaException& e) {
            std::cout << "\n[CRITICAL] " << e.what() << "\n";
            break;
        }
        catch (const GameException& e) {
            std::cout << "\n[EXCEPTIE JOC] " << e.what() << "\n";
            break;
        }
    }

    InterfataUtilizator::afiseazaGameOver(stats.getScor());
    std::cout << "\n--- STATISTICI FINALE MISIUNE ---\n";
    std::cout << "Raport nava jucator: " << albuquerque << "\n";
    std::cout << "Total inamici spulberati in misiune: " << Inamic::getTotalInamiciDistrusi() << "\n";
    return 0;
}

void curataProiectileInactive(std::vector<Proiectil>& listaProiectile) {
    for (auto itP = listaProiectile.begin(); itP != listaProiectile.end(); ) {
        if (itP->getY() == -1 || !esteInScena(itP->getY(), 20)) {
            itP = listaProiectile.erase(itP);
        } else {
            ++itP;
        }
    }
}

void proceseazaInputJucator(char tasta, NavaJucator& albuquerque, std::vector<Proiectil>& listaProiectile, FlotaManager& flota, GameMaster& gm, bool esteNavaUpgrade, const MotorGrafic& motor) {
    if (tasta == ' ') {
        gm.addEvent("Ai stat pe loc o tura.");
    }
    else if (tasta == 'a' || tasta == 'A' || tasta == 'd' || tasta == 'D') {
        albuquerque.miscare(tasta, motor.getL());
    }
    else if (tasta == 'f' || tasta == 'F') {
        if (albuquerque.actiuneAtac() > 0) {
            bool lovituraInstanta = false;

            for (size_t i = 0; i < flota.getInamici().size(); ++i) {
                Inamic* inamic = flota.getInamici()[i];
                bool peAxaNormala = (inamic->getX() == albuquerque.x());
                bool peAxaUpgrade = (esteNavaUpgrade && (inamic->getX() == albuquerque.x() + 2));

                if ((peAxaNormala || peAxaUpgrade) && (inamic->getY() == 18 || inamic->getY() == 19)) {
                    inamic->scadeViata(albuquerque.getAtacTotal());
                    gm.addEvent("Atac devastator de aproape!");
                    lovituraInstanta = true;
                    break;
                }
            }

            if (!lovituraInstanta) {
                if (esteNavaUpgrade) {
                    listaProiectile.push_back(Proiectil(albuquerque.x(), albuquerque.y()));
                    listaProiectile.push_back(Proiectil(albuquerque.x() + 2, albuquerque.y()));
                    gm.addEvent("Sistemul N-N activat! AMBELE NAVE TRAG SIMULTAN! 🔥");
                } else {
                    listaProiectile.push_back(Proiectil(albuquerque.x(), albuquerque.y()));
                }
            }
        }
    }
}

void calculeazaColiziuniProiectile(std::vector<Proiectil>& listaProiectile, FlotaManager& flota, const std::vector<int>& vechiulYProiectile, GameMaster& gm, const NavaJucator& albuquerque) {
    for (size_t i = 0; i < listaProiectile.size(); ++i) {
        if (listaProiectile[i].getY() <= 0) {
            listaProiectile[i] = Proiectil(listaProiectile[i].getX(), -1);
            continue;
        }

        if (listaProiectile[i].getY() == -1) continue;

        for (size_t k = 0; k < flota.getInamici().size(); ++k) {
            Inamic* inamic = flota.getInamici()[k];
            if (inamic->getViata() <= 0) continue;

            if (listaProiectile[i].getX() == inamic->getX()) {
                char simbolCurent = inamic->getSimbol();
                bool seLovescul = false;

                if (simbolCurent == 'v') {
                    int yAnteriorLaser = vechiulYProiectile[i];
                    int yCurentInamic = inamic->getY();
                    int yAnteriorInamic = yCurentInamic - 2;

                    if ((listaProiectile[i].getY() == yCurentInamic) ||
                        (yAnteriorLaser >= yAnteriorInamic && listaProiectile[i].getY() <= yCurentInamic)) {
                        seLovescul = true;
                    }
                } else if (simbolCurent == 'W') {
                    if (listaProiectile[i].getY() == inamic->getY() || listaProiectile[i].getY() == inamic->getY() + 1) {
                        seLovescul = true;
                    }
                } else {
                    if (listaProiectile[i].getY() >= inamic->getY() && listaProiectile[i].getY() <= inamic->getY() + 2) {
                        seLovescul = true;
                    }
                }

                if (seLovescul) {
                    if (inamic->getSimbol() == 'W') {
                        inamic->scadeViata(inamic->getViata());
                        int cx = inamic->getX();
                        int cy = inamic->getY();
                        if (cx - 3 >= 3) flota.adaugaInamic(InamicFactory::creeazaInamic("Scout", cx - 3, cy));
                        if (cx + 3 <= 24) flota.adaugaInamic(InamicFactory::creeazaInamic("Scout", cx + 3, cy));
                        gm.addEvent("💥 Cruiserul s-a divizat în două nave Scout!");
                    }
                    else {
                        inamic->scadeViata(albuquerque.getAtacTotal());
                        if (inamic->getViata() <= 0) {
                            gm.addEvent("Inamic spulberat!");
                        } else {
                            gm.addEvent("Inamic avariat!");
                        }
                    }

                    listaProiectile[i] = Proiectil(listaProiectile[i].getX(), -1);
                    break;
                }
            }
        }
    }
}

void curataInamiciDistrusi(FlotaManager& flota, Statistici& stats, Realizari& progres) {
    for (auto itIn = flota.getInamici().begin(); itIn != flota.getInamici().end(); ) {
        if ((*itIn)->getViata() <= 0) {
            stats.adaugaMoarte();
            progres.inamicDoborat();
            Inamic::incrementDistrusi();
            delete *itIn;
            itIn = flota.getInamici().erase(itIn);
        } else {
            ++itIn;
        }
    }
}

void updateObiecteSiMiscari(ResourceCollection<Diamant>& listaDiamante, ResourceCollection<PowerUp>& listaPowerUps, FlotaManager& flota, NavaJucator& albuquerque, Statistici& stats, GameMaster& gm) {
    listaDiamante.actualizeazaPozitii(20);
    listaPowerUps.actualizeazaPozitii(20);

    auto& vectorD = listaDiamante.getElemente();
    for (auto it = vectorD.begin(); it != vectorD.end(); ) {
        if (it->getX() == albuquerque.x() && it->getY() == albuquerque.y()) {
            stats.adaugaPuncte(150);
            gm.addEvent("Diamant colectat! +150 puncte.");
            it = vectorD.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = flota.getInamici().begin(); it != flota.getInamici().end(); ) {
        (*it)->miscareInamic();
        if ((*it)->getY() >= 20) {
            albuquerque.primesteLovitura(34);
            gm.addEvent("Atentie! Un inamic a trecut de defensiva si a lovit baza!");
            delete *it;
            it = flota.getInamici().erase(it);
        } else {
            ++it;
        }
    }

    auto& vectorP = listaPowerUps.getElemente();
    for (auto it = vectorP.begin(); it != vectorP.end(); ) {
        if (it->getX() == albuquerque.x() && it->getY() == albuquerque.y()) {
            if (albuquerque.getIntegritate() < 100) {
                albuquerque.setIntegritate(albuquerque.getIntegritate() + 20);
                gm.addEvent("Kit de reparatii activat! Integritate structurala refacuta.");
            } else {
                gm.addEvent("Nava este deja la capacitate maxima. Kitul a fost irosit.");
            }
            stats.adaugaPuncte(50);
            it = vectorP.erase(it);
        } else {
            ++it;
        }
    }
}

void genereazaEntitatiNoi(FlotaManager& flota, GameMaster& gm, const NavaJucator& albuquerque, ResourceCollection<PowerUp>& listaPowerUps, ResourceCollection<Diamant>& listaDiamante, const Statistici& stats)
{
    if (flota.getInamici().size() < 6 && (rand() % 100 < gm.getSpawnChance())) {
        int banda = 1 + (rand() % 8);
        int spawnX = banda * 3;
        int tipRandom = rand() % 3;

        if (tipRandom == 0) {
            flota.adaugaInamic(InamicFactory::creeazaInamic("Scout", spawnX));
        } else if (tipRandom == 1) {
            flota.adaugaInamic(InamicFactory::creeazaInamic("Cruiser", spawnX));
        } else {
            flota.adaugaInamic(InamicFactory::creeazaInamic("Fregata", spawnX));
        }

        if (flota.getInamici().size() < 6 && (rand() % 100 < 30)) {
            int bandaMeteorit = 1 + (rand() % 8);
            if (esteInScena(bandaMeteorit * 3, 30)) {
                flota.adaugaInamic(InamicFactory::creeazaInamic("Meteorit", bandaMeteorit * 3,0, &albuquerque));
            }
        }
    }

    if (rand() % 100 < 4) {
        int px = (1 + (rand() % 8)) * 3;
        listaPowerUps.adauga(PowerUp(px, 0));
    }
    if (rand() % 150 < 3) {
        int diamantX = (1 + (rand() % 8)) * 3;
        listaDiamante.adauga(Diamant(diamantX, 0));
    }

    if (stats.getScor() >= gm.getSpawnChance() * 30) {
        gm.update(stats.getScor());
    }
}