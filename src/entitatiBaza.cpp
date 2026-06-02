#include "../include/entitatiBaza.h"

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

Proiectil::Proiectil(int x, int y) : loc(x, y), activ(true) {}
void Proiectil::miscare() {
    loc.setY(loc.getY() - 1);
    if (loc.getY() < 0) activ = false;
}
int Proiectil::getX() const { return loc.getX(); }
int Proiectil::getY() const { return loc.getY(); }