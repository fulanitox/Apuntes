/*
    Estructura de todos los nodos que se utilizan en el utility (basados en el bt)
    Para meter el valor a danerLevel, distanceToBomb y todo eso hay que hacer otros metodos. Seguramente en IaSystem
*/

#ifndef UTILITY_SYSTEM_H
#define UTILITY_SYSTEM_H

#include <vector>
#include <memory>
#include <limits>
#include "astar.h"
#include <chrono>
class EntityManager;
class Entity;
class IASystem;
class BombSystem;
class MapSystem;

enum class TaskStatus{
    RUNNING,
    SUCCESS,
    FAILURE,
    WAITING
};

enum class TrueFalse{
    TRUE,
    SEMITRUE,
    FALSE
};









// Clase base para los nodos del sistema de utilidad
class Action {
private:
    TaskStatus status;
public:
    virtual ~Action() = default;

    // Método para calcular la utilidad del nodo
    virtual float calculateUtility(EntityManager& em, Entity& e) const = 0;
    virtual void addAction(std::shared_ptr<Action> action) = 0;
    TaskStatus getStatus() const { return status; }
    void setStatusRunning() { status = TaskStatus::RUNNING; }
    void setStatusSuccess() { status = TaskStatus::SUCCESS; }
    void setStatusWaiting() { status = TaskStatus::WAITING; }
    void setStatusFailure() { status = TaskStatus::FAILURE; }


    // Método para ejecutar la acción del nodo
    virtual TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;
    virtual TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;
};

// Funciones adicionales
bool crearCamino(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem);

// Clase para gestionar la evaluación de nodos basados en utilidad
class UtilitySelector {
private:
    std::vector<std::shared_ptr<Action>> childs;

public:
    // Agregar una acción al selector
    void addAction(std::shared_ptr<Action> action) {
        childs.push_back(action);
    }
    // Evaluar y ejecutar la acción con la utilidad más alta
    void evaluateAndExecute(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end);
};

// Nodo: Crear Camino de Huida
class Huir : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    // Constructor por defecto
    Huir() = default;

    void addAction(std::shared_ptr<Action> action) override{
        childs.push_back(action);
    }


    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Crear Camino de Ida
class Camino : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    Camino() = default;

    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Atacar
class Atacar : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    // Constructor por defecto
    Atacar() = default;

    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Crear Camino de Huida
class RecogerPowerup : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    // Constructor por defecto
    RecogerPowerup() = default;

    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Wait
class Wait : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    Wait() = default;

    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// NODO BASICO MOVERNPC
class MoverNPC : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    MoverNPC() = default;
    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }
    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};
    
// NODO BASICO COLOCAR BOMBA
class ColocarBomba : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    ColocarBomba() = default;
    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }
    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

class CrearCamino : public Action {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<Action>> childs;
public:
    CrearCamino() = default;
    void addAction(std::shared_ptr<Action> action) override {
        childs.push_back(action);
    }
    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};



#endif // UTILITY_SYSTEM_H
