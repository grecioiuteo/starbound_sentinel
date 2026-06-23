#ifndef ENTITATI_BAZA_H
#define ENTITATI_BAZA_H

#include <iostream>
#include <vector>

class Pozitie {
private:
    int x, y;
public:
    explicit Pozitie(int val_x = 0, int val_y = 0);
    int getX() const;
    int getY() const;
    void setX(int val);
    void setY(int val);
    bool operator==(const Pozitie& alta) const;
    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
};

class Diamant {
private:
    Pozitie loc;
    char simbol;
public:
    Diamant(int x, int y);
    void miscare();
    int getX() const;
    int getY() const;
    char getSimbol() const;
};

class PowerUp {
private:
    Pozitie loc;
    char simbol;
public:
    PowerUp(int x, int y);
    void miscare();
    int getX() const;
    int getY() const;
    char getSimbol() const;
};

class Proiectil {
private:
    Pozitie loc;
    bool activ;
public:
    Proiectil(int x, int y);
    void miscare();
    int getX() const;
    int getY() const;
    bool esteActiv() const;
};

template <typename T>
bool esteInScena(T coordonata, T limitaMaxima) {
    return (coordonata >= 0 && coordonata < limitaMaxima);
}
template <typename T>
class ResourceCollection {
private:
    std::vector<T> elemente;
public:
    void adauga(const T& element) {
        elemente.push_back(element);
    }

    std::vector<T>& getElemente() {
        return elemente;
    }

    void actualizeazaPozitii(int limitaY) {
        for (auto it = elemente.begin(); it != elemente.end(); ) {
            it->miscare();
            if (it->getY() >= limitaY) {
                it = elemente.erase(it);
            } else {
                ++it;
            }
        }
    }
};

#endif