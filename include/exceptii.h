#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <exception>
#include <string>

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
    MunitieInsuficientaException() : GameException("⚠️ Camere goale! Se inițiază reîncărcarea automată...") {}
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

#endif