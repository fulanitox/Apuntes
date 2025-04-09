
/*
    Estructura de todos los nodos que se utilizan en el utility (basados en el bt)
    Para meter el valor a danerLevel, distanceToBomb y todo eso hay que hacer otros metodos. Seguramente en IaSystem
*/

#ifndef UTILITY_SYSTEM_H
#define UTILITY_SYSTEM_H

#include <vector>
#include <memory>
#include <limits>
//#include "astar.h"
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


// Funciones adicionales
bool crearCamino(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem);

// Clase para gestionar la evaluación de nodos basados en utilidad
class Root {
private:
    std::vector<std::shared_ptr<NodoPrimerNivel>> childs;
public:
    // Agregar una acción al selector
    void addChild(std::shared_ptr<NodoPrimerNivel> child) {
        childs.push_back(child);
    }
    // Evaluar y ejecutar la acción con la utilidad más alta
    void evaluateAndExecute(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end);
};

class Nodo{
private:
    TaskStatus status;
public:
    virtual ~Nodo() = default;

    // Método para calcular la utilidad del nodo
    TaskStatus getStatus() const { return status; }
    void setStatusRunning() { status = TaskStatus::RUNNING; }
    void setStatusSuccess() { status = TaskStatus::SUCCESS; }
    void setStatusWaiting() { status = TaskStatus::WAITING; }
    void setStatusFailure() { status = TaskStatus::FAILURE; }
};

/*
    SOLO TIENEN HIJOS NodoSEGUNDONIVEL
*/
class NodoPrimerNivel : Nodo{
private:
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    virtual ~NodoPrimerNivel() = default;

    // Método para calcular la utilidad del nodo
    virtual float calculateUtility(EntityManager& em, Entity& e) const = 0;
    void addChild(std::shared_ptr<NodoSegundoNivel> child){
        
    }

    // Método para ejecutar la acción del nodo
    virtual TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;
    virtual TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;
};

/*
    NO TIENEN HIJOS
*/
class NodoSegundoNivel : Nodo{
public:
    virtual ~NodoSegundoNivel() = default;
    // Método para ejecutar la acción del nodo

    virtual TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;
    virtual TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;

};

// Nodo: Crear Camino de Huida
class Huir : public NodoPrimerNivel {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    // Constructor por defecto
    Huir() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Crear Camino de Ida
class Camino : public NodoPrimerNivel {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    Camino() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Atacar
class Atacar : public NodoPrimerNivel {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    // Constructor por defecto
    Atacar() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Crear Camino de Huida
class RecogerPowerup : public NodoPrimerNivel {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    // Constructor por defecto
    RecogerPowerup() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    // Ejecuta la formula
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// Nodo: Wait
class Wait : public NodoPrimerNivel {
private:
    TaskStatus status;
    std::vector<std::shared_ptr<NodoSegundoNivel>> childs;
public:
    Wait() = default;


    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    float calculateUtility(EntityManager& em, Entity& e) const override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

// NODO BASICO MOVERNPC
class MoverNPC : public NodoSegundoNivel {
public:
    MoverNPC() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};
    
// NODO BASICO COLOCAR BOMBA
class ColocarBomba : public NodoSegundoNivel {
public:
    ColocarBomba() = default;

    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

class CrearCamino : public NodoSegundoNivel {

public:
    CrearCamino() = default;
    TrueFalse invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
    TrueFalse run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};



#endif // UTILITY_SYSTEM_H
