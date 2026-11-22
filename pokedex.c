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
    printf("\n--- MIS POKEMONES (EN ORDEN DE CAPTURA) ---\n");
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
            // --- LIMPIEZA DE BUFFER ---
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            printf("\nHas llegado al final de la Pokedex.\n");
        }
    }
}

void capturar_pokemon() {
    char cNombreAux[TAM_NOMBRE];
    int iIndex = -1;

    printf("\nIngrese el nombre o ID del Pokemon a capturar: ");
    
    // Leemos de forma segura con fgets
    fgets(cNombreAux, TAM_NOMBRE, stdin);
    
    // Removemos el salto de línea (\n) que fgets guarda al final
    cNombreAux[strcspn(cNombreAux, "\n")] = '\0';

    // Validamos si el primer caracter es un número o una letra
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
    
    // Leemos de forma segura con fgets
    fgets(cNombreAux, TAM_NOMBRE, stdin);
    
    // Removemos el salto de línea
    cNombreAux[strcspn(cNombreAux, "\n")] = '\0';

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
// FUNCIONES DE ORDENAMIENTO (MERGE SORT)
// ==========================================

// Función auxiliar para mezclar los dos subarreglos
void merge(Pokemon arreglo[], int izq, int mid, int der) {
    int i, j, k;
    int n1 = mid - izq + 1;
    int n2 = der - mid;

    // Creamos arreglos temporales
    Pokemon *L = (Pokemon *)malloc(n1 * sizeof(Pokemon));
    Pokemon *R = (Pokemon *)malloc(n2 * sizeof(Pokemon));

    // Copiamos los datos a los temporales
    for (i = 0; i < n1; i++)
        L[i] = arreglo[izq + i];
    for (j = 0; j < n2; j++)
        R[j] = arreglo[mid + 1 + j];

    // Mezclamos los arreglos temporales de vuelta en arreglo[]
    i = 0; // Índice inicial del primer subarreglo
    j = 0; // Índice inicial del segundo subarreglo
    k = izq; // Índice inicial del arreglo mezclado

    while (i < n1 && j < n2) {
        // Compara por ID
        if (L[i].iId <= R[j].iId) {
            arreglo[k] = L[i];
            i++;
        } else {
            arreglo[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiamos los elementos restantes de L, si hay
    while (i < n1) {
        arreglo[k] = L[i];
        i++;
        k++;
    }

    // Copiamos los elementos restantes de R, si hay
    while (j < n2) {
        arreglo[k] = R[j];
        j++;
        k++;
    }

    // Liberamos la memoria de los temporales
    free(L);
    free(R);
}

// Función principal de Merge Sort (recursiva)
void mergeSort(Pokemon arreglo[], int izq, int der) {
    if (izq < der) {
        // Encontramos el punto medio
        int mid = izq + (der - izq) / 2;

        // Ordenamos la primera y segunda mitad
        mergeSort(arreglo, izq, mid);
        mergeSort(arreglo, mid + 1, der);

        // Mezclamos las mitades ordenadas
        merge(arreglo, izq, mid, der);
    }
}
void ordenar_pila() {
    if (pila_vacia(tope_pila) == 1) {
        printf("No has capturado Pokemones.\n");
        return;
    }

    // 1. Mostramos la pila tal cual (en orden de captura)
    imprime_pila(tope_pila);

    // 2. Copiamos los datos a un arreglo temporal SIN destruir la pila
    int iCantidad = 0;
    Pokemon arregloTemporal[MAX_POKEMON];
    pNodo aux = tope_pila; 

    while (aux != NULL) {
        arregloTemporal[iCantidad] = aux->infoPokemon;
        iCantidad++;
        aux = aux->pSiguiente; 
    }

    // 3. Llamamos a Merge Sort (Ordena el arreglo de menor a mayor)
    mergeSort(arregloTemporal, 0, iCantidad - 1);

    // 4. Mostramos el arreglo ordenado
    printf("\n--- POKEMONES ORDENADOS POR ID (MERGE SORT) ---\n");
    
    // Imprimimos al revés (del mayor al menor) para que el menor quede hasta abajo
    for (int i = iCantidad - 1; i >= 0; i--) {
        printf("%d. ID: %d | Nombre: %s | Tipo 1: %s | Tipo 2: %s\n",
               iCantidad - i, // Solo para que el contador inicie en 1
               arregloTemporal[i].iId, 
               arregloTemporal[i].cNombre,
               arregloTemporal[i].cTipo1, 
               arregloTemporal[i].cTipo2);
    }
    printf("-------------------------------------------\n");
    
    // NOTA: La pila original (tope_pila) permanece intacta.
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
        printf("5. Ordenar Pokemones (mergeSort)\n");
        printf("6. Liberar Pokemon\n");
        printf("0. Salir\n");
        printf("=======================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &iOpcion);
        // --- LIMPIEZA DE BUFFER (Muy importante para fgets) ---
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        // -----------------------------------------------------


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