#include <iostream>
#include <string>
/*#include <array>
#include "include/Example.h"
// This also works if you do not want `include/`, but some editors might not like it
// #include "Example.h"

int main() {
    std::cout << "Hello, world!\n";
    Example e1;
    e1.g();
    std::array<int, 100> v{};
    int nr;
    std::cout << "Introduceți nr: ";
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////
    std::cin >> nr;
    /////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < nr; ++i) {
        std::cout << "v[" << i << "] = ";
        std::cin >> v[i];
    }
    std::cout << "\n\n";
    std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
    for(int i = 0; i < nr; ++i) {
        std::cout << "- " << v[i] << "\n";
    }
    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////
    return 0;
}*/

class Pozitie {
private:
    int x, y;

public:
    explicit Pozitie(int val_x = 0, int val_y = 0) : x(val_x), y(val_y) {}
    Pozitie (const Pozitie& alta) : x(alta.x), y(alta.y) {}
    Pozitie& operator=(const Pozitie& alta) { // Assignment operator
        if (this != &alta) {
            x = alta.x;
            y = alta.y;
        }
        return *this;
    }
    ~Pozitie() {}
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int val) { x = val; }
    void setY(int val) { y = val; }

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
        os << "("<<p.x<<", "<<p.y<<")";
        return os;
    }
};

class Arsenal {
    private:
    int munitie;
    int putereAtac;
    std::string tipArma;
public:
    explicit Arsenal(std::string tip = "Laser", int mun = 40, int pwr = 10) : munitie(mun), tipArma(tip), putereAtac(pwr) {}
    Arsenal(const Arsenal& altul) : munitie(altul.munitie), putereAtac(altul.putereAtac), tipArma(altul.tipArma) {}
    Arsenal& operator= (const Arsenal& altul ) {
        if (this !=&altul) {
            munitie = altul.munitie;
            putereAtac= altul.putereAtac;
            tipArma = altul.tipArma;
        }
        return *this;
    }
    ~Arsenal() {}

    bool trage() {
        if (munitie > 0) {
            munitie--;
            return true;
        }
        return false;
    }
    int getMunitie() const { return munitie; }
   /* void reincarca(int cantitate) { munitie += cantitate; }
    friend std::ostream& operator<<(std::ostream& os, const Arsenal& a) {
        os << a.tipArma << " [Mun: " << a.munitie << ", Pwr: " << a.putereFoc << "]";
        return os;
    }*/
};


class NavaJucator {
private:
    std::string numeNava;
    int integritate;
    Pozitie locatie;
    Arsenal armament;
    char aspect;
public:
    explicit NavaJucator(std::string nume, int startX, int startY)
        : numeNava(nume), integritate(100), locatie(startX, startY), armament("Laser",40, 10), aspect('N') {}
    NavaJucator(const NavaJucator& alta)
        : numeNava(alta.numeNava + "_Backup"), integritate(alta.integritate),
          locatie(alta.locatie), armament(alta.armament), aspect(alta.aspect) {}

    NavaJucator& operator=(const NavaJucator& alta) {
        if (this != &alta) {
            numeNava =alta.numeNava ;
            integritate=alta.integritate;
            locatie =alta.locatie;
            armament=alta.armament ;
            aspect=alta.aspect;
        }
        return *this;
    }
    ~NavaJucator() {}
    void miscare(char tasta, int limitaX, int limitaY) {
        int nx = locatie.getX();
        int ny = locatie.getY();

        if (tasta == 'a' && nx > 0) nx--;
        else if (tasta == 'd' &&nx < limitaX - 1) nx++;
        else if (tasta =='w' && ny > 0) ny--;
        else if (tasta == 's'&& ny <limitaY - 1) ny++;

        locatie.setX(nx);
        locatie.setY(ny);
    }
    void actiuneAtac() {
        if (armament.trage()) {
            std::cout << "Sistemul de arme a confirmat tragerea!\n";
        } else {
            std::cout << "Eroare: Munitie insuficienta!\n";
        }
    }
    int x()const { return locatie.getX();}
    int y() const { return locatie.getY(); }
    char getAspect()const {return aspect; }
    int getStatusMunitie() const {return armament.getMunitie();}

    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n) {
        os << "[" <<n.numeNava << "] HP: "<< n.integritate<< "% | AMMO: " << n.getStatusMunitie();
        return os;
    }
};
class MotorGrafic {
private:
    int lungime,inaltime;

public:
    explicit MotorGrafic(int l = 20, int h = 8) : lungime(l), inaltime(h) {}
    void scena(const NavaJucator& nava) {
        for(int i = 0;i < 10;++i) std::cout << "\n";
        std::cout << nava << "\n";
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
        std::cout << "\n";
        for (int y = 0;y < inaltime;y++) {
            std::cout << "|";
            for (int x = 0; x < lungime;x++) {
                if (x == nava.x() && y == nava.y()) {
                    std::cout << nava.getAspect();
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "|\n";
        }
        for (int i = 0; i < lungime + 2; i++) std::cout << "=";
        std::cout << "\nComenzi: WASD+Enter (Q pt exit si F pt atac)\n";
    }
    int getL() const { return lungime; }
    int getH() const { return inaltime; }
};



int main() {
    NavaJucator albuquerque("Interceptor", 5, 3);
    MotorGrafic motor(30, 10);
    char tastaApasata = ' ';

    while (tastaApasata != 'q'&& tastaApasata!='Q') {
        motor.scena(albuquerque);
        std::cout << "Input: ";
        std::cin >> tastaApasata;

        if (tastaApasata =='f'|| tastaApasata== 'F') {
            albuquerque.actiuneAtac();
        } else {
            albuquerque.miscare(tastaApasata, motor.getL(), motor.getH());
        }
    }
    std::cout << "Inchidere...\n";
    NavaJucator backup =albuquerque;
    std::cout << "Backup creat:" <<backup <<"\n";
    return 0;
}