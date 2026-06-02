#include "../include/inamici.h"

// Inițializarea membrului static obligatoriu
int Inamic::totalInamiciDistrusi = 0;

// Constructorul clasei de bază Inamic
Inamic::Inamic(int v, int x, int y, char s)
    : viata(v), loc(x, y), simbol(s), viataMax(v), armura(0), s_a_transformat(false) {}

// Scădere viață inamic general
void Inamic::scadeViata(int dmg) {
    int dmgEfectiv = dmg - armura;
    if (dmgEfectiv < 1) dmgEfectiv = 1; // Minim 1 damage încasat
    viata -= dmgEfectiv;
    if (viata < 0) viata = 0;
}

// ============================================================================
// METODELE DE MIȘCARE ȘI DEFINIȚIILE LIPSĂ CARE CAUZAU EROAREA DE LINKER
// ============================================================================

// Mișcare Inamic Fregată (Are logică de zig-zag pe axa X)
void InamicFregata::miscareInamic() {
    pasMecanic++;
    if (pasMecanic % 2 == 0) {
        loc.setX(loc.getX() + directieX);
        if (loc.getX() <= 3 || loc.getX() >= 24) {
            directieX = -directieX; // Schimbă direcția la margini
        }
    }
    loc.setY(loc.getY() + 1); // Coboară spre bază
}

// Mișcare Inamic Meteorit (Urmărește nava jucătorului pe axa X)
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

// ============================================================================
// IMPLEMENTARE FLOTA MANAGER (Regula celor 3: Destructor, Copy, Assignment)
// ============================================================================

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