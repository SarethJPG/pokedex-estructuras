flowchart TD
    Inicio([Inicio del Programa]) --> CargarCSV["Cargar Pokédex desde CSV al arreglo global"]
    CargarCSV --> Menu["Mostrar Menú de Opciones"]
    
    Menu --> LeerOpc[/"Leer opcion del usuario"/]
    LeerOpc --> Switch{"Switch opcion"}
    
    %% CASE 1
    Switch -- "1. Ver Pokédex" --> InitPag["Iniciar mostrados = 0"]
    InitPag --> LoopPag{"mostrados < total_pokemones"}
    LoopPag -- Si --> Mostrar5["Mostrar 5 Pokémon desde mostrados\nSi encontrado==1 mostrar datos\nSi no, mostrar ???"]
    Mostrar5 --> Sumar5["mostrados = mostrados + 5"]
    Sumar5 --> Preguntar[/"Preguntar: Mostrar siguientes? 1=Si, 0=No"/]
    Preguntar --> EsUno{"respuesta_pag == 1"}
    EsUno -- Si --> LoopPag
    EsUno -- No --> Menu
    LoopPag -- No, Fin --> Pausa["System pause / Esperar Enter"]
    Pausa --> Menu

    %% CASE 2
    Switch -- "2. Capturar Pokémon" --> PedirC[/"Ingrese Nombre o ID a capturar"/]
    PedirC --> RevisarC{"nombre_aux[0] es numero"}
    RevisarC -- Si --> BuscarIDC["Llamar a BuscarPorID"]
    RevisarC -- No --> BuscarNomC["Llamar a BuscarPorNombre"]
    BuscarIDC --> ResC{"Encontrado"}
    BuscarNomC --> ResC
    ResC -- No --> MsgNoEncC["Mostrar: Pokémon no encontrado"]
    MsgNoEncC --> Menu
    ResC -- Si --> Capturar["Llamar a AgregarAPila\npush a tope_pila"]
    Capturar --> MsgCap["Mostrar: Pokémon Capturado!"]
    MsgCap --> Menu

    %% CASE 3
    Switch -- "3. Pokébolas" --> CheckVacia1["Llamar a pilaVacia"]
    CheckVacia1 --> EsVacia1{"Pila vacia"}
    EsVacia1 -- Si --> MsgVacia1["Mostrar: No has capturado Pokémon"]
    MsgVacia1 --> Menu
    EsVacia1 -- No --> MostrarPila["Recorrer Pila y mostrar capturados"]
    MostrarPila --> Menu

    %% CASE 4
    Switch -- "4. Buscar Pokémon" --> PedirB[/"Ingrese Nombre o ID a buscar"/]
    PedirB --> RevisarB{"nombre_aux[0] es numero"}
    RevisarB -- Si --> BuscarIDB["Llamar a BuscarPorID"]
    RevisarB -- No --> BuscarNomB["Llamar a BuscarPorNombre"]
    BuscarIDB --> ResB{"Encontrado"}
    BuscarNomB --> ResB
    ResB -- No --> MsgNoEncB["Mostrar: Pokémon no encontrado"]
    MsgNoEncB --> Menu
    ResB -- Si --> CheckCap{"encontrado_bandera == 1"}
    CheckCap -- Si --> MostrarDatos["Mostrar datos completos del Pokémon"]
    MostrarDatos --> Menu
    CheckCap -- No --> MostrarOculto["Mostrar solo Nombre e ID con ???"]
    MostrarOculto --> Menu

    %% CASE 5
    Switch -- "5. Ordenar Pokémon" --> CheckVacia2["Llamar a pilaVacia"]
    CheckVacia2 --> EsVacia2{"Pila vacia"}
    EsVacia2 -- Si --> MsgVacia2["Mostrar: No has capturado Pokémon"]
    MsgVacia2 --> Menu
    EsVacia2 -- No --> MostrarDesordenado["Mostrar Pila en orden de captura"]
    MostrarDesordenado --> QuickSort["Llamar a QuickSort\nOrdenar por ID"]
    QuickSort --> MostrarOrdenado["Mostrar Pila ordenada por ID"]
    MostrarOrdenado --> Menu

    %% CASE 6
    Switch -- "6. Liberar Pokémon" --> CheckVacia3["Llamar a pilaVacia"]
    CheckVacia3 --> EsVacia3{"Pila vacia"}
    EsVacia3 -- Si --> MsgVacia3["Mostrar: No has capturado Pokémon"]
    MsgVacia3 --> Menu
    EsVacia3 -- No --> Liberar["Llamar a Funcion Pop\nEliminar tope_pila"]
    Liberar --> MsgLiberar["Mostrar: Pokémon liberado"]
    MsgLiberar --> Menu

    %% DEFAULT Y SALIR
    Switch -- "0. Salir" --> Fin(["Fin del Programa"])
    Switch -- "Otro (Default)" --> MsgError["Mostrar: Opcion no valida"]
    MsgError --> Menu