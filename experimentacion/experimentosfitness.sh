#!/bin/bash

rm -rf mediciones && mkdir mediciones
cd ../genetico && make experimentarFitness

for POPSIZE in {50..250..50}
do
	let SURVIVORS=${POPSIZE}/3
	./ricardofort.o 6 7 4 21 10 $POPSIZE 100 0.8f 0.05f 0.5f 1.0f 0.01f 1 0.0f 50 200 2> "../experimentacion/mediciones/fitness_pop${POPSIZE}.out"
done

for NSURVIVORS in {50..250..50}
do
	./ricardofort.o 6 7 4 21 ${NSURVIVORS} 250 10 0.8f 0.05f 0.5f 1.0f 0.01f 1 0.0f 50 200 2> "../experimentacion/mediciones/fitness_${NSURVIVORS}survivors.out"
done

cd ../experimentacion

# Uso: ./main ROWS COLS C PIECES NSURVIVORS POPSIZE GAMES PC PM T MR PRMATING FFUNCTION ALPHA EXTRATE GENS
#     ROWS           Filas.
#     COLS           Columnas.
#     C              Cantidad de fichas en línea necesarias para ganar.
#     PIECES         Fichas de cada jugador.
#     NSURVIVORS     Cantidad de sobrevivientes.
#     POPSIZE        Tamaño de la población.
#     GAMES          Cantidad de partidos para calcular el fitness.
#     PC             Probabilidad de que haya crossover en vez de mitosis.
#     PM             Probabilidad de que un alelo mute.
#     T              1 - probabilidad de que se produzca un corte en un punto dado durante el crossover.
#     MR             Radio de la mutación.
#     PRMATING       Probabilidad de que un individuo sobreviviente se reproduzca con uno aleatorio.
#     FFUNCTION      Función de fitness.
#     ALPHA          Importancia que se le da a la rapidez en la función de fitness 2.
#     EXTRATE        Cantidad de generaciones tras las que ocurre una extinción masiva.
#     GENS           Cantidad de generaciones de la evolución.
