# Pokédex - Proyecto de Estructuras de Datos

## Descripción
Este proyecto es una Pokédex desarrollada en lenguaje C como parte de la materia de Estructuras de Datos. El programa permite cargar una base de datos de 1025 Pokémon desde un archivo `.csv`, visualizarlos, capturarlos (usando una pila dinámica), buscarlos (por ID o nombre) y ordenarlos mediante el algoritmo de ordenamiento QuickSort.

## Autor
* **Jurado Flores Santiago**
* **Materia:** Estructuras de Datos
* **Institución:** UNAM FCA - Informatica

## Características y Estructuras Utilizadas
El programa implementa las siguientes estructuras y algoritmos vistos en clase:
*   **Arreglos (Vectores):** Utilizados para almacenar la base de datos general de los 1025 Pokémon.
*   **Pilas Dinámicas (LIFO):** Utilizadas para gestionar los Pokémon capturados por el usuario (el último capturado es el primero en mostrarse y liberarse).
*   **Búsqueda Lineal:** Implementada para encontrar Pokémon por su nombre o ID dentro del arreglo.
*   **Ordenamiento QuickSort:** Utilizado para ordenar los Pokémon capturados (guardados en la pila) por su número de ID.

## Requisitos previos
Para compilar y ejecutar este proyecto, necesitas:
*   Un compilador de C (GCC o MinGW para Windows).
*   Tener el archivo `pokemones.csv` en la misma carpeta que el código fuente.
*   Tener el archivo `pokedex.c`.

