/*
	Name: pokedex.c
	Author: Santiago jurado flores
	Date: 15/11/26
	Description: Proyecto de Estructuras de Datos - Pokédex usando arreglos y pilas dinámicas.
*/
//directivas al preprocesador
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES ---
#define MAX_POKEMON 1025
#define TAM_NOMBRE 50
#define TAM_TIPO 20
#define TAM_LINEA 256

/*
	Name: Pokemon
	Description: Estructura que define los atributos basicos de un Pokemon en la Pokedex
	Valores: 
        iId (entero), cNombre (cadena), cTipo1 (cadena), cTipo2 (cadena), iEncontrado (entero 0 o 1)
*/
typedef struct {
    int iId;
    char cNombre[TAM_NOMBRE];
    char cTipo1[TAM_TIPO];
    char cTipo2[TAM_TIPO];
    int iEncontrado; // 0 = No, 1 = Sí
} Pokemon;

/*
	Name: tipoNodo
	Description: Estructura que representa un nodo para la pila dinamica de Pokemon capturados
	Valores: 
        infoPokemon (estructura Pokemon), pSiguiente (apuntador al siguiente nodo)
*/
typedef struct nodo {
    Pokemon infoPokemon;
    struct nodo *pSiguiente;
} tipoNodo;

/*
	Name: pNodo
	Description: Alias de tipo de dato para facilitar la declaracion de apuntadores a nodos
	Valores: 
        Apuntador a una estructura de tipo tipoNodo
*/
typedef tipoNodo *pNodo;

// --- VARIABLES GLOBALES ---

/*
	Name: arregloPokedex
	Description: Arreglo principal que almacena todos los Pokemon cargados desde el archivo CSV
	Valores: 
        Arreglo de estructuras Pokemon de tamaño MAX_POKEMON (1025)
*/
Pokemon arregloPokedex[MAX_POKEMON];

/*
	Name: iTotalPokemones
	Description: Lleva la cuenta exacta de cuantos Pokemon se cargaron exitosamente del archivo
	Valores: 
        Numero entero inicializado en 0
*/
int iTotalPokemones = 0;

/*
	Name: tope_pila
	Description: Apuntador que guarda la direccion del ultimo Pokemon capturado (el tope de la pila)
	Valores: 
        Apuntador a tipoNodo inicializado en NULL (pila vacia)
*/
pNodo tope_pila = NULL;

/*
	Name: iCapturados
	Description: Contador que lleva el registro de cuantos Pokemon ha capturado el usuario
	Valores: 
        Numero entero inicializado en 0
*/
int iCapturados = 0;

/*
	Name: cargar_desde_csv
	Description: Abre el archivo CSV, lee los datos y los guarda en el arreglo global
	Parametros: ruta (cadena de texto con el nombre del archivo)
*/
void cargar_desde_csv(const char *ruta) {
    FILE *pArchivo = fopen(ruta, "r"); // Abre el archivo en modo lectura
    if (pArchivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", ruta);
        return;
    }

    char cLinea[TAM_LINEA];
    fgets(cLinea, sizeof(cLinea), pArchivo); // Lee y salta la primera línea (encabezados)

    // Lee línea por línea hasta el final del archivo o hasta llegar al límite
    while (fgets(cLinea, sizeof(cLinea), pArchivo) != NULL && iTotalPokemones < MAX_POKEMON) {
        
        // Separa los datos usando la coma como delimitador
        int iLeidos = sscanf(cLinea, "%d,%49[^,],%19[^,],%19[^\n]",
                             &arregloPokedex[iTotalPokemones].iId,
                             arregloPokedex[iTotalPokemones].cNombre,
                             arregloPokedex[iTotalPokemones].cTipo1,
                             arregloPokedex[iTotalPokemones].cTipo2);

        if (iLeidos >= 3) {
            arregloPokedex[iTotalPokemones].iEncontrado = 0; // Inicializa como NO capturado
            iTotalPokemones++; // Aumenta el contador de Pokemon reales cargados
        }
    }
    fclose(pArchivo); // Cierra el archivo
    printf("Se cargaron %d Pokemon exitosamente.\n", iTotalPokemones);
}


// ==========================================
// FUNCIONES DE LA PILA (Capturados)
// ==========================================

/*
	Name: pila_vacia
	Description: Verifica si la pila no tiene elementos
	Parametros: pila (apuntador al tope de la pila)
*/
int pila_vacia(pNodo pila) {
    if (pila == NULL) return 1; // Retorna verdadero si apunta a NULL
    return 0; // Retorna falso si hay al menos un nodo
}

/*
	Name: Push
	Description: Agrega un nuevo Pokemon al tope de la pila
	Parametros: ultimo (apuntador al tope por referencia), nuevoPokemon (estructura a guardar)
*/
void Push(pNodo *ultimo, Pokemon nuevoPokemon) {
    pNodo nuevoNodo;
    nuevoNodo = (pNodo)malloc(sizeof(tipoNodo)); // Reserva memoria para el nuevo nodo
    nuevoNodo->infoPokemon = nuevoPokemon; // Copia los datos del Pokemon al nodo
    nuevoNodo->pSiguiente = *ultimo; // El nuevo nodo apunta al que era el tope anterior
    *ultimo = nuevoNodo; // El nuevo nodo se convierte en el nuevo tope
}

/*
	Name: Pop
	Description: Elimina el Pokemon del tope de la pila y guarda sus datos
	Parametros: pila (apuntador al tope por referencia), pokemonSacado (apuntador a estructura)
*/
void Pop(pNodo *pila, Pokemon *pokemonSacado) {
    pNodo nodoAux = *pila; // Guarda el tope actual en un auxiliar
    *pokemonSacado = nodoAux->infoPokemon; // Pasa los datos del Pokemon al apuntador
    *pila = nodoAux->pSiguiente; // El siguiente nodo se vuelve el nuevo tope
    free(nodoAux); // Libera la memoria del nodo que acabamos de sacar
}

/*
	Name: imprime_pila
	Description: Recorre y muestra todos los Pokemon capturados en la pila
	Parametros: pila (apuntador al tope de la pila)
*/
void imprime_pila(pNodo pila) {
    pNodo aux;
    int iContador = 1;
    printf("\n--- MIS POKEMONES (EN ORDEN DE CAPTURA) ---\n");
    
    // Recorre desde el tope hasta el final
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

/*
	Name: buscar_por_id
	Description: Busca un Pokemon en el arreglo global por su numero de ID
	Parametros: iIdBuscado (numero entero a buscar)
*/
int buscar_por_id(int iIdBuscado) {
    // Recorre todo el arreglo de Pokemon
    for (int i = 0; i < iTotalPokemones; i++) {
        if (arregloPokedex[i].iId == iIdBuscado) return i; // Retorna el indice si lo encuentra
    }
    return -1; // Retorna -1 si no existe
}

/*
	Name: buscar_por_nombre
	Description: Busca un Pokemon en el arreglo global por su nombre
	Parametros: cNomBuscado (cadena de texto a buscar)
*/
int buscar_por_nombre(char *cNomBuscado) {
    // Recorre todo el arreglo comparando cadenas con strcmp
    for (int i = 0; i < iTotalPokemones; i++) {
        if (strcmp(arregloPokedex[i].cNombre, cNomBuscado) == 0) return i; 
    }
    return -1; 
}

/*
	Name: ver_pokedex
	Description: Muestra la Pokedex paginada de 5 en 5, ocultando datos si no estan capturados
	Parametros: Ninguno
*/
void ver_pokedex() {
    int iMostrados = 0;
    int iRespuesta = 1;

    // Mientras queden Pokemon por mostrar y el usuario quiera seguir
    while (iMostrados < iTotalPokemones && iRespuesta == 1) {
        printf("\n--- POKEDEX ---\n");
        
        // Imprime un bloque de 5 Pokemon
        for (int i = iMostrados; i < iMostrados + 5 && i < iTotalPokemones; i++) {
            if (arregloPokedex[i].iEncontrado == 1) {
                // Si esta capturado, muestra los datos
                printf("#%03d | %-12s | Tipo: %-10s | %s\n", 
                       arregloPokedex[i].iId, arregloPokedex[i].cNombre, 
                       arregloPokedex[i].cTipo1, arregloPokedex[i].cTipo2);
            } else {
                // Si no, muestra signos de interrogacion
                printf("#%03d | ???          | Tipo: ???\n", arregloPokedex[i].iId);
            }
        }
        iMostrados += 5; // Avanza el contador de paginacion

        // Si aun quedan Pokemon, pregunta si quiere ver mas
        if (iMostrados < iTotalPokemones) {
            printf("\nMostrar los siguientes 5? (1=Si, 0=No): ");
            scanf("%d", &iRespuesta);
            
            // Limpieza del buffer para que no se salte la siguiente lectura
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            printf("\nHas llegado al final de la Pokedex.\n");
        }
    }
}

/*
	Name: capturar_pokemon
	Description: Solicita nombre o ID, busca el Pokemon y lo agrega a la pila de capturados
	Parametros: Ninguno
*/
void capturar_pokemon() {
    char cNombreAux[TAM_NOMBRE];
    int iIndex = -1;

    printf("\nIngrese el nombre o ID del Pokemon a capturar: ");
    
    // Lee la entrada permitiendo espacios
    fgets(cNombreAux, TAM_NOMBRE, stdin);
    
    // Remueve el salto de linea del final
    cNombreAux[strcspn(cNombreAux, "\n")] = '\0';

    // Verifica si el primer caracter es un numero para saber si buscar por ID o nombre
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
            arregloPokedex[iIndex].iEncontrado = 1; // Marca como capturado
            iCapturados++; // Aumenta el contador
            Push(&tope_pila, arregloPokedex[iIndex]); // Lo mete a la pila
            printf("¡%s capturado con exito!\n", arregloPokedex[iIndex].cNombre);
        }
    }
}

/*
	Name: buscar_pokemon
	Description: Busca un Pokemon y muestra su informacion dependiendo de si esta capturado o no
	Parametros: Ninguno
*/
void buscar_pokemon() {
    char cNombreAux[TAM_NOMBRE];
    int iIndex = -1;

    printf("\nIngrese el nombre o ID del Pokemon a buscar: ");
    
    fgets(cNombreAux, TAM_NOMBRE, stdin); // Lee la entrada
    cNombreAux[strcspn(cNombreAux, "\n")] = '\0'; // Remueve salto de linea

    // Decide el tipo de busqueda segun el primer caracter
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
        
        // Muestra tipos si esta capturado, o ??? si no lo esta
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

/*
	Name: liberar_pokemon
	Description: Elimina el ultimo Pokemon capturado (tope de la pila) y actualiza su estado
	Parametros: Ninguno
*/
void liberar_pokemon() {
    Pokemon pokemonSacado;
    
    if (pila_vacia(tope_pila) == 1) {
        printf("No has capturado Pokemones.\n");
    } else {
        Pop(&tope_pila, &pokemonSacado); // Saca al ultimo Pokemon de la pila
        
        // Lo busca en el arreglo para volver a marcarlo como NO capturado
        int iIndex = buscar_por_id(pokemonSacado.iId);
        if (iIndex != -1) {
            arregloPokedex[iIndex].iEncontrado = 0;
        }
        iCapturados--; // Reduce el contador
        printf("%s ha sido liberado.\n", pokemonSacado.cNombre);
    }
}


// ==========================================
// FUNCIONES DE ORDENAMIENTO (MERGE SORT)
// ==========================================

/*
	Name: merge
	Description: Funcion auxiliar del Merge Sort que mezcla dos subarreglos ya ordenados
	Parametros: arreglo (arreglo de Pokemon), izq, mid, der (indices de rango)
*/
void merge(Pokemon arreglo[], int izq, int mid, int der) {
    int i, j, k;
    int n1 = mid - izq + 1;
    int n2 = der - mid;

    // Reserva memoria para los arreglos temporales izquierdo y derecho
    Pokemon *L = (Pokemon *)malloc(n1 * sizeof(Pokemon));
    Pokemon *R = (Pokemon *)malloc(n2 * sizeof(Pokemon));

    // Copia los datos a los temporales
    for (i = 0; i < n1; i++) L[i] = arreglo[izq + i];
    for (j = 0; j < n2; j++) R[j] = arreglo[mid + 1 + j];

    i = 0; j = 0; k = izq; 

    // Compara y mezcla los elementos de L y R de menor a mayor
    while (i < n1 && j < n2) {
        if (L[i].iId <= R[j].iId) {
            arreglo[k] = L[i]; // El de la izquierda es menor, se queda
            i++;
        } else {
            arreglo[k] = R[j]; // El de la derecha es menor, se queda
            j++;
        }
        k++;
    }

    // Copia los elementos restantes de L, si quedaron
    while (i < n1) {
        arreglo[k] = L[i];
        i++; k++;
    }

    // Copia los elementos restantes de R, si quedaron
    while (j < n2) {
        arreglo[k] = R[j];
        j++; k++;
    }

    free(L); // Libera el arreglo temporal izquierdo
    free(R); // Libera el arreglo temporal derecho
}

/*
	Name: mergeSort
	Description: Funcion recursiva que divide el arreglo a la mitad y lo ordena usando merge
	Parametros: arreglo (arreglo de Pokemon), izq, der (indices de rango)
*/
void mergeSort(Pokemon arreglo[], int izq, int der) {
    if (izq < der) {
        int mid = izq + (der - izq) / 2; // Calcula el punto medio

        mergeSort(arreglo, izq, mid); // Ordena la mitad izquierda
        mergeSort(arreglo, mid + 1, der); // Ordena la mitad derecha

        merge(arreglo, izq, mid, der); // Mezcla ambas mitades
    }
}

/*
	Name: ordenar_pila
	Description: Copia la pila a un arreglo, lo ordena con Merge Sort y lo muestra sin destruir la pila
	Parametros: Ninguno
*/
void ordenar_pila() {
    if (pila_vacia(tope_pila) == 1) {
        printf("No has capturado Pokemones.\n");
        return;
    }

    imprime_pila(tope_pila); // Muestra la pila original

    int iCantidad = 0;
    Pokemon arregloTemporal[MAX_POKEMON];
    pNodo aux = tope_pila; 

    // Copia los datos a un arreglo temporal usando un puntero auxiliar
    while (aux != NULL) {
        arregloTemporal[iCantidad] = aux->infoPokemon;
        iCantidad++;
        aux = aux->pSiguiente; 
    }

    mergeSort(arregloTemporal, 0, iCantidad - 1); // Llama a Merge Sort

    printf("\n--- POKEMONES ORDENADOS POR ID (MERGE SORT) ---\n");
    
    // Imprime de mayor a menor para que el ID mas bajo quede hasta abajo
    for (int i = iCantidad - 1; i >= 0; i--) {
        printf("%d. ID: %d | Nombre: %s | Tipo 1: %s | Tipo 2: %s\n",
               iCantidad - i, // Contador visual
               arregloTemporal[i].iId, 
               arregloTemporal[i].cNombre,
               arregloTemporal[i].cTipo1, 
               arregloTemporal[i].cTipo2);
    }
    printf("-------------------------------------------\n");
}


// ==========================================
// FUNCION PRINCIPAL
// ==========================================
/*
	Name: main
	Description: Funcion principal que controla el flujo del menu principal
	Parametros: Ninguno
*/
int main() {
    int iOpcion;

    cargar_desde_csv("pokemones.csv"); // Carga los datos al iniciar

    do {
        // Imprime el menu
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
        
        // Limpia el buffer despues de leer el numero
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Ejecuta la accion elegida
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
        
        // Pausa la pantalla para que el usuario pueda leer
        if (iOpcion != 0) {
            system("pause"); 
        }

    } while (iOpcion != 0); // Repite hasta que el usuario elija salir

    return 0;
}