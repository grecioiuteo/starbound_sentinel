#include "../include/inamici.h"

int Inamic::totalInamiciDistrusi = 0;

Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), loc(x, y), simbol(s), viataMax(v), armura(0), s_a_transformat(false) {}

void Inamic::scadeViata(int dmg) {
    int dmgEfectiv = dmg - armura;
    if (dmgEfectiv < 1) dmgEfectiv = 1;
    viata -= dmgEfectiv;
    if (viata < 0) viata = 0;
}
void InamicFregata::miscareInamic() {
    pasMecanic++;
    if (pasMecanic % 2 == 0) {
        loc.setX(loc.getX() + directieX);
        if (loc.getX() <= 3 || loc.getX() >= 24) {
            directieX = -directieX;
        }
    }
    loc.setY(loc.getY() + 1);
}

void InamicMeteorit::miscareInamic() {
    loc.setY(loc.getY() + 1);
    if (navaTinta && loc.getY() < 15) {
        if (loc.getX() < navaTinta->x()) {
            loc.setX(loc.getX() + 3);
        } else if (loc.getX() > navaTinta->x()) {
            loc.setX(loc.getX() - 3);
        }
    }
}


FlotaManager::~FlotaManager() {
    curataFlota();
}

void FlotaManager::curataFlota() {
    for (auto* inamic : naveFlota) {
        delete inamic;
    }
    naveFlota.clear();
}

FlotaManager::FlotaManager(const FlotaManager& alta) {
    for (auto* inamic : alta.naveFlota) {
        naveFlota.push_back(inamic->clone());
    }
}

FlotaManager& FlotaManager::operator=(FlotaManager alta) {
    std::swap(this->naveFlota, alta.naveFlota);
    return *this;
}

void FlotaManager::adaugaInamic(Inamic* inamicNou) {
    if (inamicNou != nullptr) {
        naveFlota.push_back(inamicNou);
    }
}

std::vector<Inamic*>& FlotaManager::getInamici() {
    return naveFlota;
}