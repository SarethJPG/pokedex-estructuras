/*
	Name: pokedex.c
	Author: SarethJPG
	Date: 15/11/26
	Description: Proyecto de Estructuras de Datos - Pokédex usando arreglos y pilas dinámicas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES ---
#define MAX_POKEMON 1025
#define TAM_NOMBRE 50
#define TAM_TIPO 20
#define TAM_LINEA 256

// --- ESTRUCTURAS ---

typedef struct {
    int iId;
    char cNombre[TAM_NOMBRE];
    char cTipo1[TAM_TIPO];
    char cTipo2[TAM_TIPO];
    int iEncontrado; // 0 = No, 1 = Sí
} Pokemon;

typedef struct nodo {
    Pokemon infoPokemon;
    struct nodo *pSiguiente;
} tipoNodo;

typedef tipoNodo *pNodo;

// --- VARIABLES GLOBALES ---
Pokemon arregloPokedex[MAX_POKEMON];
int iTotalPokemones = 0;
pNodo tope_pila = NULL;
int iCapturados = 0;


// ==========================================
// FUNCIONES DE ARCHIVO
// ==========================================

void cargar_desde_csv(const char *ruta) {
    FILE *pArchivo = fopen(ruta, "r");
    if (pArchivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", ruta);
        return;
    }

    char cLinea[TAM_LINEA];
    // Leer y saltar la primera línea (encabezados)
    fgets(cLinea, sizeof(cLinea), pArchivo);

    // Leer línea por línea
    while (fgets(cLinea, sizeof(cLinea), pArchivo) != NULL && iTotalPokemones < MAX_POKEMON) {
        int iLeidos = sscanf(cLinea, "%d,%49[^,],%19[^,],%19[^\n]",
                             &arregloPokedex[iTotalPokemones].iId,
                             arregloPokedex[iTotalPokemones].cNombre,
                             arregloPokedex[iTotalPokemones].cTipo1,
                             arregloPokedex[iTotalPokemones].cTipo2);

        if (iLeidos >= 3) {
            arregloPokedex[iTotalPokemones].iEncontrado = 0;
            iTotalPokemones++;
        }
    }
    fclose(pArchivo);
    printf("Se cargaron %d Pokemon exitosamente.\n", iTotalPokemones);
}


// ==========================================
// FUNCIONES DE LA PILA (Capturados)
// ==========================================

int pila_vacia(pNodo pila) {
    if (pila == NULL) return 1;
    return 0;
}

void Push(pNodo *ultimo, Pokemon nuevoPokemon) {
    pNodo nuevoNodo;
    nuevoNodo = (pNodo)malloc(sizeof(tipoNodo));
    nuevoNodo->infoPokemon = nuevoPokemon;
    nuevoNodo->pSiguiente = *ultimo;
    *ultimo = nuevoNodo;
}

void Pop(pNodo *pila, Pokemon *pokemonSacado) {
    pNodo nodoAux = *pila;
    *pokemonSacado = nodoAux->infoPokemon;
    *pila = nodoAux->pSiguiente;
    free(nodoAux);
}

void imprime_pila(pNodo pila) {
    pNodo aux;
    int iContador = 1;
    printf("\n--- MIS POKEBOLAS (EN ORDEN DE CAPTURA) ---\n");
    for (aux = pila; aux != NULL; aux = aux->pSiguiente) {
        printf("%d. ID: %d | Nombre: %s | Tipo 1: %s | Tipo 2: %s\n",
               iContador, aux->infoPokemon.iId, aux->infoPokemon.cNombre,
               aux->infoPokemon.cTipo1, aux->infoPokemon.cTipo2);
        iContador++;
    }
    printf("-------------------------------------------\n");
}


// ==========================================
// FUNCIONES DE BUSQUEDA Y MENU
// ==========================================

int buscar_por_id(int iIdBuscado) {
    for (int i = 0; i < iTotalPokemones; i++) {
        if (arregloPokedex[i].iId == iIdBuscado) return i;
    }
    return -1;
}

int buscar_por_nombre(char *cNomBuscado) {
    for (int i = 0; i < iTotalPokemones; i++) {
        if (strcmp(arregloPokedex[i].cNombre, cNomBuscado) == 0) return i;
    }
    return -1;
}

void ver_pokedex() {
    int iMostrados = 0;
    int iRespuesta = 1;

    while (iMostrados < iTotalPokemones && iRespuesta == 1) {
        printf("\n--- POKEDEX ---\n");
        for (int i = iMostrados; i < iMostrados + 5 && i < iTotalPokemones; i++) {
            if (arregloPokedex[i].iEncontrado == 1) {
                printf("#%03d | %-12s | Tipo: %-10s | %s\n", 
                       arregloPokedex[i].iId, arregloPokedex[i].cNombre, 
                       arregloPokedex[i].cTipo1, arregloPokedex[i].cTipo2);
            } else {
                printf("#%03d | ???          | Tipo: ???\n", arregloPokedex[i].iId);
            }
        }
        iMostrados += 5;

        if (iMostrados < iTotalPokemones) {
            printf("\nMostrar los siguientes 5? (1=Si, 0=No): ");
            scanf("%d", &iRespuesta);
        } else {
            printf("\nHas llegado al final de la Pokedex.\n");
        }
    }
}

void capturar_pokemon() {
    char cNombreAux[TAM_NOMBRE];
    int iIndex = -1;

    printf("\nIngrese el nombre o ID del Pokemon a capturar: ");
    scanf("%s", cNombreAux);

    if (cNombreAux[0] >= '0' && cNombreAux[0] <= '9') {
        iIndex = buscar_por_id(atoi(cNombreAux));
    } else {
        iIndex = buscar_por_nombre(cNombreAux);
    }

    if (iIndex == -1) {
        printf("Pokemon no encontrado.\n");
    } else {
        if (arregloPokedex[iIndex].iEncontrado == 1) {
            printf("Ya tienes este Pokemon capturado.\n");
        } else {
            arregloPokedex[iIndex].iEncontrado = 1;
            iCapturados++;
            Push(&tope_pila, arregloPokedex[iIndex]);
            printf("¡%s capturado con exito!\n", arregloPokedex[iIndex].cNombre);
        }
    }
}

void buscar_pokemon() {
    char cNombreAux[TAM_NOMBRE];
    int iIndex = -1;

    printf("\nIngrese el nombre o ID del Pokemon a buscar: ");
    scanf("%s", cNombreAux);

    if (cNombreAux[0] >= '0' && cNombreAux[0] <= '9') {
        iIndex = buscar_por_id(atoi(cNombreAux));
    } else {
        iIndex = buscar_por_nombre(cNombreAux);
    }

    if (iIndex == -1) {
        printf("Pokemon no encontrado en los registros.\n");
    } else {
        printf("\n--- INFORMACION DEL POKEMON ---\n");
        printf("ID: %d\n", arregloPokedex[iIndex].iId);
        printf("Nombre: %s\n", arregloPokedex[iIndex].cNombre);
        if (arregloPokedex[iIndex].iEncontrado == 1) {
            printf("Tipo 1: %s\n", arregloPokedex[iIndex].cTipo1);
            printf("Tipo 2: %s\n", arregloPokedex[iIndex].cTipo2);
        } else {
            printf("Tipo 1: ???\n");
            printf("Tipo 2: ???\n");
            printf("(Aun no has capturado este Pokemon)\n");
        }
    }
}

void liberar_pokemon() {
    Pokemon pokemonSacado;
    
    if (pila_vacia(tope_pila) == 1) {
        printf("No has capturado Pokemones.\n");
    } else {
        Pop(&tope_pila, &pokemonSacado);
        int iIndex = buscar_por_id(pokemonSacado.iId);
        if (iIndex != -1) {
            arregloPokedex[iIndex].iEncontrado = 0;
        }
        iCapturados--;
        printf("%s ha sido liberado.\n", pokemonSacado.cNombre);
    }
}


// ==========================================
// FUNCIONES DE ORDENAMIENTO (QUICK SORT)
// ==========================================

void intercambiar(Pokemon *a, Pokemon *b) {
    Pokemon temp = *a;
    *a = *b;
    *b = temp;
}

int particion(Pokemon arreglo[], int iBajo, int iAlto) {
    int iPivote = arreglo[iAlto].iId;
    int i = (iBajo - 1);

    for (int j = iBajo; j <= iAlto - 1; j++) {
        if (arreglo[j].iId < iPivote) {
            i++;
            intercambiar(&arreglo[i], &arreglo[j]);
        }
    }
    intercambiar(&arreglo[i + 1], &arreglo[iAlto]);
    return (i + 1);
}

void quickSort(Pokemon arreglo[], int iBajo, int iAlto) {
    if (iBajo < iAlto) {
        int iPi = particion(arreglo, iBajo, iAlto);
        quickSort(arreglo, iBajo, iPi - 1);
        quickSort(arreglo, iPi + 1, iAlto);
    }
}

void ordenar_pila() {
    if (pila_vacia(tope_pila) == 1) {
        printf("No has capturado Pokemones.\n");
        return;
    }

    imprime_pila(tope_pila);

    int iCantidad = 0;
    Pokemon arregloTemporal[MAX_POKEMON];
    Pokemon temp;
    
    while (pila_vacia(tope_pila) == 0) {
        Pop(&tope_pila, &temp);
        arregloTemporal[iCantidad] = temp;
        iCantidad++;
    }

    quickSort(arregloTemporal, 0, iCantidad - 1);

    for (int i = 0; i < iCantidad; i++) {
        Push(&tope_pila, arregloTemporal[i]);
    }

    printf("\n--- POKEMONES ORDENADOS POR ID ---\n");
    imprime_pila(tope_pila);
}


// ==========================================
// FUNCION PRINCIPAL
// ==========================================
int main() {
    int iOpcion;

    cargar_desde_csv("pokemones.csv");

    do {
        printf("\n======= POKEDEX DE ESTRUCTURAS =======\n");
        printf("1. Ver Pokedex\n");
        printf("2. Capturar Pokemon\n");
        printf("3. Ver Mis Pokebolas (Pila)\n");
        printf("4. Buscar Pokemon\n");
        printf("5. Ordenar Pokemones (QuickSort)\n");
        printf("6. Liberar Pokemon\n");
        printf("0. Salir\n");
        printf("=======================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &iOpcion);

        switch (iOpcion) {
            case 1: ver_pokedex(); break;
            case 2: capturar_pokemon(); break;
            case 3:
                if (pila_vacia(tope_pila) == 1) {
                    printf("No has capturado Pokemones.\n");
                } else {
                    imprime_pila(tope_pila);
                }
                break;
            case 4: buscar_pokemon(); break;
            case 5: ordenar_pila(); break;
            case 6: liberar_pokemon(); break;
            case 0: printf("Saliendo del programa...\n"); break;
            default: printf("Opcion no valida. Intenta de nuevo.\n");
        }
        
        if (iOpcion != 0) {
            system("pause"); // Pausa nativa para Windows
        }

    } while (iOpcion != 0);

    return 0;
}