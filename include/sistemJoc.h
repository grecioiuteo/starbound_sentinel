#ifndef SISTEM_JOC_H
#define SISTEM_JOC_H

#include <vector>
#include <string>
#include "navaJucator.h"
#include "inamici.h"
#include "entitatiBaza.h"

class MotorGrafic {
private:
    int lungime, inaltime;
public:
    explicit MotorGrafic(int l = 30, int h = 20);
    void scena(const NavaJucator& nava, const std::vector<Inamic*>& inamici, const std::vector<Diamant>& diamante, const std::vector<PowerUp>& powerups, const std::vector<Proiectil>& proiectile, int scorActual);
    int getL() const;
};

class Statistici {
private:
    int scor;
public:
    Statistici();
    void adaugaMoarte();
    void adaugaPuncte(int puncte);
    int getScor() const;
};

class InterfataUtilizator {
public:
    static void afiseazaBanner();
    static void afiseazaGameOver(int scor);
};

class GameMaster {
private:
    int level;
    int pragNivel;
    std::vector<std::string> log;
public:
    GameMaster();
    void update(int scorActual);
    void addEvent(const std::string& msg);
    void showStatus();
    int getSpawnChance();
};

class Realizari {
private:
    int inamiciDistrusi;
    bool asulZborului;
public:
    Realizari();
    void inamicDoborat();
    friend std::ostream& operator<<(std::ostream& os, const Realizari& r);
};

#endif