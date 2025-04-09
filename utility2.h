
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

        TaskStatus invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end);
        virtual TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;

};

/*
    SOLO TIENEN HIJOS Nodo
*/
class NodoPrimerNivel : public Nodo {
    private:
        std::vector<std::shared_ptr<Nodo>> childs;
    public:
        ~NodoPrimerNivel() = default;

        // Método para calcular la utilidad del nodo
        void addChild(std::shared_ptr<Nodo> child){
            childs.push_back(child);
        }

        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;

};




// Nodo: Crear Camino de Huida
class Huir : public NodoPrimerNivel {
    public:
        // Constructor por defecto
        Huir() = default;

        // Ejecuta la formula
        float calculateUtility(EntityManager& em, Entity& e) const;
};

// Nodo: Crear Camino de Ida
class Camino : public NodoPrimerNivel {
    public:
        Camino() = default;
        float calculateUtility(EntityManager& em, Entity& e) const;
};

// Nodo: Atacar
class Atacar : public NodoPrimerNivel {
    public:
        // Constructor por defecto
        Atacar() = default;

        // Ejecuta la formula
        float calculateUtility(EntityManager& em, Entity& e) const;
};

// Nodo: Crear Camino de Huida
class RecogerPowerup : public NodoPrimerNivel {
    public:
        // Constructor por defecto
        RecogerPowerup() = default;

        // Ejecuta la formula
        float calculateUtility(EntityManager& em, Entity& e) const;
};

// Nodo: Wait
class Wait : public NodoPrimerNivel {
    public:
        Wait() = default;
        float calculateUtility(EntityManager& em, Entity& e) const;
};


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// NODO BASICO MOVERNPC
class MoverNPC : public Nodo {
    public:
        MoverNPC() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};
    
// NODO BASICO COLOCAR BOMBA
class ColocarBomba : public Nodo {
    public:
        ColocarBomba() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

class CrearCamino : public Nodo {
    public:
        CrearCamino() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};



#endif // UTILITY_SYSTEM_H
