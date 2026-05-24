#ifndef GAMEPLAY_COMPONENTS_H
#define GAMEPLAY_COMPONENTS_H

#include <iostream>
#include <string>
#include <vector>
#include <exception>

class GameException : public std::exception {
protected:
    std::string mesajEroare;
public:
    explicit GameException(const std::string& mesaj) : mesajEroare(mesaj) {}
    virtual ~GameException() = default;

    virtual const char* what() const noexcept override { return mesajEroare.c_str(); }
};

class MunitieInsuficientaException : public GameException {
public:
    MunitieInsuficientaException() : GameException("⚠️ Camere goale! Se inițiază reîncărcarea automată a sistemului...") {}
};

class PozitieInvalidaException : public GameException {
public:
    explicit PozitieInvalidaException(const std::string& detalii)
        : GameException("EROARE SISTEM: Coordonate de spawn corupte! " + detalii) {}
};

class NavaDistrusaException : public GameException {
public:
    NavaDistrusaException() : GameException("GAME OVER: Integritatea structurală a navei a ajuns la 0%.") {}
};


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

class Inamic {
protected:
    int viata;
    int armura;
    Pozitie loc;
    char simbol;
    int viataMax;
    bool s_a_transformat;
public:
    Inamic(int v, int x, int y, char s);
    virtual ~Inamic() = default;
    virtual void miscareInamic() = 0;
    virtual Inamic* clone() const = 0;

    int getX() const { return loc.getX(); }
    int getY() const { return loc.getY(); }
    char getSimbol() const { return simbol; }
    int getViata() const { return viata; }

    void scadeViata(int dmg);

    void afiseaza(std::ostream& os) const {
        afisareVirtuala(os);
    }

    friend std::ostream& operator<<(std::ostream& os, const Inamic& inamic) {
        inamic.afiseaza(os);
        return os;
    }

protected:
    virtual void afisareVirtuala(std::ostream& os) const {
        os << simbol << " [HP: " << viata << "]";
    }
};

class InamicScout : public Inamic {
public:
    InamicScout(int x, int y) : Inamic(5, x, y, 'v') {}

    void miscareInamic() override {
        loc.setY(loc.getY() + 2);
    }

    Inamic* clone() const override { return new InamicScout(*this); }
};

class InamicCruiser : public Inamic {
public:
    InamicCruiser(int x, int y) : Inamic(20, x, y, 'W') {
        armura = 10;
    }

    void miscareInamic() override {
        loc.setY(loc.getY() + 1);
    }

    Inamic* clone() const override { return new InamicCruiser(*this); }
};

class InamicFregata : public Inamic {
private:
    int directieX;
    int pasMecanic;
public:
    InamicFregata(int x, int y) : Inamic(15, x, y, 'F'), directieX(3), pasMecanic(0) {}

    void miscareInamic() override {
        pasMecanic++;
        loc.setY(loc.getY() + 1);

        if (pasMecanic % 3 == 0) {
            int urmatorulX = loc.getX() + directieX;
            if (urmatorulX > 24 || urmatorulX < 3) {
                directieX = -directieX;
            }
            loc.setX(loc.getX() + directieX);
        }
    }

    Inamic* clone() const override { return new InamicFregata(*this); }
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

class Arsenal {
private:
    int munitie;
    int putereAtac;
    std::string tipArma;
    int capacitateMaxima;
public:
    explicit Arsenal(const std::string& tip = "Laser", int mun = 40, int pwr = 10);
    Arsenal& operator+=(int cantitate);
    bool trage();
    int reincarca();
    int getDmg() const;
    friend std::ostream& operator<<(std::ostream& os, const Arsenal& a);
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
    int getVieti() const;
    int getIntegritate() const { return integritate; }
    void setIntegritate(int val) { integritate = val; if(integritate > 100) integritate = 100; }
    bool esteOperationala() const;
    void autoDiagnostic() const;
    explicit NavaJucator(const std::string& nume, int startX, int startY);
    NavaJucator(const NavaJucator& alta);
    NavaJucator& operator=(const NavaJucator& alta);
    ~NavaJucator() = default;
    void miscare(char tasta, int limitaX);
    int actiuneAtac();
    int x() const;
    int y() const;
    char getAspect() const;
    void activeazaScut(int val);
    friend std::ostream& operator<<(std::ostream& os, const NavaJucator& n);
    int executaReincarcare();
    void primesteLovitura(int dmg);
    int getAtacTotal() const;
    Arsenal& getArmament();
};

class InamicMeteorit : public Inamic {
private:
    const NavaJucator* navaTinta;
public:
    InamicMeteorit(int x, int y, const NavaJucator* nava)
        : Inamic(80, x, y, 'M'), navaTinta(nava) {}

    void miscareInamic() override;

    Inamic* clone() const override { return new InamicMeteorit(*this); }
};

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

class ManagerResurse {
public:
    static std::string obtineMesajInfrangere(int scor);
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

class FlotaManager {
private:
    std::vector<Inamic*> naveFlota;
public:
    FlotaManager() = default;

    ~FlotaManager();
    FlotaManager(const FlotaManager& alta);
    FlotaManager& operator=(FlotaManager alta);

    void adaugaInamic(Inamic* inamicNou);
    std::vector<Inamic*>& getInamici();
    void curataFlota();
    void activeazaScuturiCruisere();
};

#endif