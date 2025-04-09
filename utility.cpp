#include "utility.h"
#include <iostream>
#include "../AlmondGE/EntityManager.h"   // y EntityManager
#include "../Sys/IAsystem.h"
#include "../Sys/BombSystem.h"
#include "../Sys/MapSystem.h"


/*

    En este archivo se deben ejecutar las siguientes funciones:
                - Los run de todos los nodos
                    CrearCaminoHuida
                    CrearCaminoIda
                    MoverNpc
                    ColocarBomba
                    Wait
                - Los evaluate de los utilitySelector
                    Calcula la utility de los hijos y run el que tiene mas utility

        - Los evaluateAndExecute de los nodos hijos del root
            Ejecuta nodos utilitySelector hijos del root (el arbol entero)

*/

/*
    ==============================================================
                        METODOS ADICIONALES
    ==============================================================
*/

int puntuarCasillaRodeada(int estoyx, int estoyz, std::vector<std::vector<int>> mapa, int ronda){
    int cantidadDeBloques = 0;
    for(int i=1; i<=3; i++){
        if(estoyx+i < mapa.size() && estoyx-i >= 0){
            if(mapa[estoyx+i][estoyz] == 2 || (mapa[estoyx+i][estoyz] > 109 && mapa[estoyx+i][estoyz] <118) ||
               mapa[estoyx-i][estoyz] == 2 || (mapa[estoyx-i][estoyz] > 109 && mapa[estoyx-i][estoyz] <118)){
                if(i == 1){
                    cantidadDeBloques += 3;
                }
                if(i == 2){
                    cantidadDeBloques += 2;
                }
                if(i == 3){
                    cantidadDeBloques +=1;
                }
            } 
            if(mapa[estoyx+i][estoyz] == 1 || mapa[estoyx-i][estoyz] == 1){
                cantidadDeBloques--;
            }
        }
        if(estoyz+i < mapa[0].size() && estoyz-i >= 0){
            
            if(mapa[estoyx][estoyz+i] == 2 || (mapa[estoyx][estoyz+i] > 109 && mapa[estoyx][estoyz+i] <118) ||
               mapa[estoyx][estoyz-i] == 2 || (mapa[estoyx][estoyz-i] > 109 && mapa[estoyx][estoyz-i] <118)){
                if(i == 1){
                    cantidadDeBloques += 3;
                }
                if(i == 2){
                    cantidadDeBloques += 2;
                }
                if(i == 3){
                    cantidadDeBloques +=1;
                }
            }
            if(mapa[estoyx][estoyz+i] == 1 || mapa[estoyx][estoyz-i] == 1){
                cantidadDeBloques-2;
            } 
        }
    }
    int aux = cantidadDeBloques + ronda;
    if(aux<0) aux = 0;
    return aux;
}



std::vector<Conexion*> buscarCaminoObjetivo(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iasystem){
    auto &fisicacomponent = em.fisicas.get(e.fisica_key);
    int estoyx = fisicacomponent.tileX;
    int estoyz = fisicacomponent.tileZ;

    SimpleHeuristic heuristic;

    std::vector<std::vector<int>> casillasDisponibles;
    std::vector<std::vector<Conexion*>> caminosPosibles;
    std::vector<Conexion*> camino;


    std::vector<std::vector<int>> posicionesPosibles;
    std::vector<int> puntosRonda;

    float rx = 0;
    float rz = 0;
    bool r = false;
    int rondo = mapa.size();

    for(int w=rondo;w>0;w--){
        for(int i=estoyz-w;i<=estoyz+w;i++){
            for(int j=estoyx-w;j<=estoyx+w;j++){
                if(i < mapa.size() && i >= 0 && j < mapa[0].size() && j >= 0 ){
                    if(i==estoyz-w || i==estoyz+w){
                        if(iasystem.casillaEsSalva(mapa, em, j,i)){
                            posicionesPosibles.push_back({i,j});
                            puntosRonda.push_back(w);
                        }
                    }else if(j==estoyx-w || j==estoyx+w){
                        if(iasystem.casillaEsSalva(mapa, em, j,i)){
                            posicionesPosibles.push_back({i,j});
                            puntosRonda.push_back(w);
                        }
                    }
                }
            }
        }
        // if(r == true){ break; }
    }
    std::vector<int> puntuacionesDeCasillas;
    //SIGUIENTE PASO
    for(int i=0; i<posicionesPosibles.size(); i++){
        puntuacionesDeCasillas.push_back(puntuarCasillaRodeada(posicionesPosibles[i][0], posicionesPosibles[i][1], mapa, puntosRonda[i]));        
    }
    
    int n = puntuacionesDeCasillas.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (puntuacionesDeCasillas[j] > puntuacionesDeCasillas[j + 1]) {
                // Intercambia puntuacionesDeCasillas[j] y puntuacionesDeCasillas[j+1]
                int temp = puntuacionesDeCasillas[j];
                puntuacionesDeCasillas[j] = puntuacionesDeCasillas[j + 1];
                puntuacionesDeCasillas[j + 1] = temp;

                std::vector<int> temp2 = posicionesPosibles[j];
                posicionesPosibles[j] = posicionesPosibles[j + 1];
                posicionesPosibles[j + 1] = temp2;
            }
        }
    }
    std::vector<Conexion*> elVeldaeroCamino;
    std::vector<Conexion*> elFarzoCamino;

    for(int i=puntuacionesDeCasillas.size()-1; i>=0; i--){
        elFarzoCamino = iasystem.encontrarCamino(estoyz, estoyx, posicionesPosibles[i][0], posicionesPosibles[i][1], heuristic);
        if(!elFarzoCamino.empty()){
            elVeldaeroCamino = elFarzoCamino;
            break;
        }
    }
    return elVeldaeroCamino;
}

/*
    ==============================================================
                            TODOS LOS INVOKE
    ==============================================================
*/

TrueFalse Wait::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        auto& iaComponent = em.ia.get(e.ia_key);
        iaComponent.nodoActual = NodoClass::WAIT;
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse Huir::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        auto& iaComponent = em.ia.get(e.ia_key);
        iaComponent.nodoActual = NodoClass::HUIR;
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}
 
TrueFalse Camino::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        auto& iaComponent = em.ia.get(e.ia_key);
        iaComponent.nodoActual = NodoClass::CAMINO;
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse Atacar::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        auto& iaComponent = em.ia.get(e.ia_key);
        iaComponent.nodoActual = NodoClass::ATACAR;
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse RecogerPowerup::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        auto& iaComponent = em.ia.get(e.ia_key);
        iaComponent.nodoActual = NodoClass::POWERUP;
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse MoverNPC::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse ColocarBomba::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}

TrueFalse CrearCamino::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);
    }else{
        return TrueFalse::FALSE;
    }
}
/* 
    ==============================================================
                            TODOS LOS RUN
    ==============================================================
*/

/*
    HACE FALTA QUE CUANDO UN NODO MOVERNPC O COLOCAR BOMBA DEVUELVA FALSE PQ NO SE HA PODIDO EJECUTAR QUE NO SE SIGA EJECUTANDO LA ACCION
    QUE DEVUELVA FALSE Y SE VUELVA AL ROOT
*/

/*
    Nodo de espera, no ejecuta nada, simplemente pasa la ejecucion
    INPUTS -> NO
    OUTPUT -> NO
*/
TrueFalse Wait::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"Ejecuto nodo Wait\n";
    auto &iaComponent = em.ia.get(e.ia_key);
}

/*
    Ejecuta Huir
    Pone como nodo en ejecucion Huir
    HAce el while raro
*/
TrueFalse Huir::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"Ejecuto nodo Huir\n";
    auto &iaComponent = em.ia.get(e.ia_key);

    int i = 0;
    TrueFalse r = TrueFalse::TRUE;

    while (i<childs.size() && (r == TrueFalse::TRUE || r == TrueFalse::SEMITRUE)) {         //Mientras no haya recorrido todos sus hijos

        const auto& action = childs[i]; //La accion actual es el hijo[i]

        if(action->getStatus() == TaskStatus::RUNNING){                    // Si aun esta en ejecucion
            r = action->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);  // Miras si te queda tiempo y ejecutas
            if(r == TrueFalse::TRUE){
                action->setStatusSuccess();
            }
            if(r == TrueFalse::FALSE){
                action->setStatusFailure();
            }
        } else if(action->getStatus() == TaskStatus::SUCCESS){                    // Si ya es success sumo uno a i y paso al siguiente hijo (las condiciones ya seran del siguiente hijo) 
            i++;
        } else if(action->getStatus() == TaskStatus::FAILURE){         // Si es failure return false y se para la ejecucion de la accion padre pq no puede continuar
            return TrueFalse::FALSE;
        } else if(action->getStatus() == TaskStatus::WAITING){
            action->setStatusRunning();
        }
    }

    for(auto& action : childs){
        action->setStatusWaiting();
    }

    return TrueFalse::TRUE;
}

/*
    Busca la casilla mas alejada accesible y guarda el camino con a* hacia ella
    INPUTS -> Casilla en la que esta el npc
    OUTPUT -> NO -> Guarda el camino en caminoNPC
*/
TrueFalse Camino::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"Ejecuto nodo Camino\n";
    auto &iaComponent = em.ia.get(e.ia_key);

    int i = 0;
    TrueFalse r = TrueFalse::TRUE;

    while (i<childs.size() && (r == TrueFalse::TRUE || r == TrueFalse::SEMITRUE)) {         //Mientras no haya recorrido todos sus hijos

        const auto& action = childs[i]; //La accion actual es el hijo[i]

        if(action->getStatus() == TaskStatus::RUNNING){                    // Si aun esta en ejecucion
            r = action->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);  // Miras si te queda tiempo y ejecutas
            if(r == TrueFalse::TRUE){
                action->setStatusSuccess();
            }
            if(r == TrueFalse::FALSE){
                action->setStatusFailure();
            }
        } else if(action->getStatus() == TaskStatus::SUCCESS){                    // Si ya es success sumo uno a i y paso al siguiente hijo (las condiciones ya seran del siguiente hijo) 
            i++;
        } else if(action->getStatus() == TaskStatus::FAILURE){         // Si es failure return false y se para la ejecucion de la accion padre pq no puede continuar
            return TrueFalse::FALSE;
        } else if(action->getStatus() == TaskStatus::WAITING){
            action->setStatusRunning();
        }
    }

    //for(auto& action : childs){
    //    action->setStatusWaiting();
    //}

    return TrueFalse::TRUE;
}

TrueFalse Atacar::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"Ejecuto nodo Atacar\n";
    auto &iaComponent = em.ia.get(e.ia_key);

    int i = 0;
    TrueFalse r = TrueFalse::TRUE;

    while (i<childs.size() && (r == TrueFalse::TRUE || r == TrueFalse::SEMITRUE)) {         //Mientras no haya recorrido todos sus hijos

        const auto& action = childs[i]; //La accion actual es el hijo[i]

        if(action->getStatus() == TaskStatus::RUNNING){                    // Si aun esta en ejecucion
            r = action->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);  // Miras si te queda tiempo y ejecutas
            if(r == TrueFalse::TRUE){
                action->setStatusSuccess();
            }
            if(r == TrueFalse::FALSE){
                action->setStatusFailure();
            }
        } else if(action->getStatus() == TaskStatus::SUCCESS){                    // Si ya es success sumo uno a i y paso al siguiente hijo (las condiciones ya seran del siguiente hijo) 
            i++;
        } else if(action->getStatus() == TaskStatus::FAILURE){         // Si es failure return false y se para la ejecucion de la accion padre pq no puede continuar
            return TrueFalse::FALSE;
        } else if(action->getStatus() == TaskStatus::WAITING){
            action->setStatusRunning();
        }
    }

    //for(auto& action : childs){
    //    action->setStatusWaiting();
    //}

    return TrueFalse::TRUE;
}

TrueFalse RecogerPowerup::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"Ejecuto nodo RPW\n";
    auto &iaComponent = em.ia.get(e.ia_key);

    int i = 0;
    TrueFalse r = TrueFalse::TRUE;

    while (i<childs.size() && (r == TrueFalse::TRUE || r == TrueFalse::SEMITRUE)) {         //Mientras no haya recorrido todos sus hijos

        const auto& action = childs[i]; //La accion actual es el hijo[i]

        if(action->getStatus() == TaskStatus::RUNNING){                    // Si aun esta en ejecucion
            r = action->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);  // Miras si te queda tiempo y ejecutas
            if(r == TrueFalse::TRUE){
                action->setStatusSuccess();
            }
            if(r == TrueFalse::FALSE){
                action->setStatusFailure();
            }
        } else if(action->getStatus() == TaskStatus::SUCCESS){                    // Si ya es success sumo uno a i y paso al siguiente hijo (las condiciones ya seran del siguiente hijo) 
            i++;
        } else if(action->getStatus() == TaskStatus::FAILURE){         // Si es failure return false y se para la ejecucion de la accion padre pq no puede continuar
            return TrueFalse::FALSE;
        } else if(action->getStatus() == TaskStatus::WAITING){
            action->setStatusRunning();
        }
    }

    for(auto& action : childs){
        action->setStatusWaiting();
    }

    return TrueFalse::TRUE;
}

/*
    Mueve al NPC siguiendo el caminoNPC que se ha creado anteriormente
    INPUTS -> NO
    OUTPUT -> NO -> El resultado es el movimiento mediante las fisics
*/
TrueFalse MoverNPC::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    // std::cout<<"MOVIENDOME\n";
    
    iaSystem.actualizarCamino(em, e);

    auto &iaComponent = em.ia.get(e.ia_key);
    iaComponent.estadoDebugger = 3;

    if (iaComponent.inPositionToPlaceBomb){
        return TrueFalse::TRUE;
    } else {
        return TrueFalse::SEMITRUE;
    }
}

/*
    Coloca una bomba en la posicion en la que esté
    INPUTS -> NO
    OUTPUT -> NO
*/
TrueFalse ColocarBomba::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){

    auto &iaComponent = em.ia.get(e.ia_key);
    // std::cout<<"BOMBA\n";
    bombsystem.crearBomba(e, em, mapsystem);
    iaComponent.inPositionToPlaceBomb = false;

    iaComponent.estadoDebugger = 4;

    return TrueFalse::TRUE;
}

/*
    Camino ataque
    Camino huir
    Camino camino romper
    Camino PW
*/
TrueFalse CrearCamino::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    auto &iaComponent = em.ia.get(e.ia_key);
    auto &fisicaComponent = em.fisicas.get(e.fisica_key);
    switch (iaComponent.nodoActual){
        case NodoClass::CAMINO:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapa, em, e, 0);
            break;

        case NodoClass::HUIR:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapa, em, e, 1);
            break;

        case NodoClass::POWERUP:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapa, em, e, 2);
            break;

        case NodoClass::ATACAR:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapa, em, e, 3);
            break;

        default:
            break;
    }
    // std::cout<<"CreoCamino\n";
        
    return TrueFalse::TRUE;
}

/*
    ==============================================================
                        EVALUATE DE CADA NODO
    ==============================================================
*/

/*
    Rubrica:
        dangerLevel             = 1 si esta en peligro 0 si no
        distanceToBomb          = distancia a la bomba 0 < 1
        hasPath                 = 1 si tiene 0 si no tiene
        distanceToTarget        = distancia al objetivo (casillas) 0 < 1
        distanceToNextNode      = distancia a la siguiente casilla (siempre va a ser la misma)
        inPositionToPlaceBomb   = 1 si esta en la posicion 0 si no esta en esa posicion
        canEscapeAfterBomb      = 1 si puede escapar 0 si no puede escapar
*/
float Huir::calculateUtility(EntityManager& em, Entity& e) const{
    auto &iaComponent = em.ia.get(e.ia_key);
    return iaComponent.peligrosidad;
}

float Camino::calculateUtility(EntityManager& em, Entity& e) const{
    auto &iaComponent = em.ia.get(e.ia_key);
    float a=0.3f, b=0.5f, c=0.8f;
    float utilidad = pow(iaComponent.areaNidos,c) * pow(1-iaComponent.bloquesTotales,a) * pow(1-iaComponent.peligrosidad,b);
    return utilidad;
    //de aqui falta ver el areaNido ya que devuleve un tipo conexion y deberia devolver un float
}

float Wait::calculateUtility(EntityManager& em, Entity& e) const{
    return 0;
    // if(mode == Mode::LEFT){
    //     return hasPath*(1-dangerLevel);
    // }else{
    //     return (1-hasPath) + (1-canEscapeAfterBomb);
    // }
        
}

float Atacar::calculateUtility(EntityManager& em, Entity& e) const{
    auto &iaComponent = em.ia.get(e.ia_key);
    float utilidad = iaComponent.distanciaNPC * (1-iaComponent.bombaCercana);
    return utilidad;
}

// MANU TE QUIERO MUCHO MUCHO ANIMO CON LAS FORMULAS <3

float RecogerPowerup::calculateUtility(EntityManager& em, Entity& e) const{
    auto &iaComponent = em.ia.get(e.ia_key);
    float a=0.3f, b=0.5f, c=0.8f;
    float utilidad = iaComponent.puedePW * (pow(1 - iaComponent.peligrosidad, a) * pow(iaComponent.distanciaPW, b) * pow(1 - iaComponent.pwObtenidos, c));
    return utilidad;

}

float MoverNPC::calculateUtility(EntityManager& em, Entity& e) const {
    return 0; // Implementación básica inicial
}

float ColocarBomba::calculateUtility(EntityManager& em, Entity& e) const {
    return 0; // Implementación básica inicial
}

float CrearCamino::calculateUtility(EntityManager& em, Entity& e) const {
    return 0; // Implementación básica inicial
}
/*
    ==============================================================
                RUN DEL NODO UTILITY SELECTOR
    ==============================================================
*/

void UtilitySelector::evaluateAndExecute(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    float maxUtility = -std::numeric_limits<float>::infinity();
    std::shared_ptr<Action> bestAction = nullptr;
    auto &iacomponentVariables = em.ia.get(e.ia_key);

    int contador = 0;

    for (auto& action : childs) {
        float utility = action->calculateUtility(em, e);
        if (utility > maxUtility) {
            maxUtility = utility;
            bestAction = action;
        }
        
        switch (contador){
        case 0:
            std::cout<<"HUIDA ::  |"<<utility<<"|\n";
            iacomponentVariables.huirlargo = utility;
            break;
        case 1:
            std::cout<<"CAMINO :: |"<<utility<<"|\n";
            iacomponentVariables.caminolargo = utility;
            break;
        case 2:
            std::cout<<"WAIT ::   |"<<utility<<"|\n";
            iacomponentVariables.powerlargo = utility;
            break;
        case 3:
            std::cout<<"ATACAR :: |"<<utility<<"|\n";
            iacomponentVariables.atacarlargo = utility;
            break;
        case 4:
            std::cout<<"TAKEPW :: |"<<utility<<"|\n";
            iacomponentVariables.defaultlargo = utility;
            break;
        
        default:
            break;
        }

        contador++;

    }
    std::cout<<"|||||||||||||||||||||||||||||||\n";

    if (bestAction) {
        bestAction->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);
    }
}


