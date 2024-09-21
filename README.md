# Space Invaders Emulator

Este es un emulador del juego **Space Invaders** desarrollado en C++ utilizando SDL2 para el manejo de gráficos y SDL2_mixer para el sonido. El emulador imita el comportamiento del hardware basado en el procesador **Intel 8080**, ejecutando el clásico juego de arcade **Space Invaders**.

## Tabla de Contenidos

- [Introducción](#introducción)
- [Requisitos](#requisitos)
- [Compilación y Ejecución](#compilación-y-ejecución)
- [Estructura del Proyecto](#estructura-del-proyecto)
- [Detalles Técnicos](#detalles-técnicos)
  - [Estructura del Procesador Intel 8080](#estructura-del-procesador-intel-8080)
  - [Conjunto de Instrucciones del Intel 8080](#conjunto-de-instrucciones-del-intel-8080)
  - [Puertos de Entrada/Salida](#puertos-de-entrada-salida)
  - [Memoria del Juego](#memoria-del-juego)
- [Diagrama del Procesador](#diagrama-del-procesador)
- [Referencias](#referencias)

## Introducción

Este proyecto emula el hardware original del juego **Space Invaders**, permitiendo jugarlo en sistemas modernos a través de SDL2. El procesador **Intel 8080** es el núcleo de la emulación, y todas las operaciones de gráficos, sonido y controles están simuladas para replicar el comportamiento del juego arcade original.

## Requisitos

Para compilar y ejecutar el emulador necesitarás tener instalados los siguientes paquetes:

- **g++** (compilador de C++)
- **SDL2** (para gráficos)
- **SDL2_mixer** (para sonido)
- Las ROMs del juego Space Invaders (invaders.e, invaders.f, invaders.g, invaders.h)

### Instalación de dependencias en Arch Linux

```bash
sudo pacman -S g++ sdl2 sdl2_mixer
```

## Compilación y Ejecución
#### Compilar el Emulador

Puedes compilar el emulador manualmente utilizando el siguiente comando:
```bash
g++ -o space_invaders src/main.cpp src/cpu.cpp src/graphics.cpp -lSDL2 -lSDL2_mixer
```

También puedes utilizar el **Makefile** incluido en el proyecto:
```bash
make
```

## Ejecución del Emulador

Una vez compilado, puedes ejecutar el emulador con los archivos de ROM de Space Invaders:

```bash
./space_invaders invaders.h invaders.g invaders.f invaders.e
```

## Estructura del Proyecto

La estructura del proyecto es la siguiente:

```bash
project/
├── src/
│   ├── main.cpp        # Archivo principal que controla el ciclo del emulador
│   ├── cpu.cpp         # Emulación del CPU Intel 8080
│   ├── cpu.h           # Declaraciones y definiciones del CPU
│   ├── graphics.cpp    # Controla los gráficos usando SDL2
│   ├── graphics.h      # Declaraciones de la clase Graphics
├── sounds/
│   ├── shot.wav        # Sonido de disparo
│   └── explosion.wav   # Sonido de explosión
└── README.md           # Este archivo README
```

## Detalles Técnicos
### Estructura del Procesador Intel 8080

El procesador Intel 8080 es un CPU de 8 bits que cuenta con un conjunto de registros y una memoria de 64KB. A continuación se describe la estructura básica del procesador:

### Registros del CPU

El procesador tiene 7 registros de propósito general (A, B, C, D, E, H, L), además de un contador de programa (PC) y un puntero de pila (SP).

```bash
+--------------+---------+-------------+
| Registro     | Nombre  | Propósito    |
+--------------+---------+-------------+
| Acumulador   | A       | Operaciones aritméticas y lógicas |
| Registro B   | B       | Propósito general |
| Registro C   | C       | Propósito general |
| Registro D   | D       | Propósito general |
| Registro E   | E       | Propósito general |
| Registro H   | H       | Propósito general |
| Registro L   | L       | Propósito general |
| Program Counter | PC    | Dirección de la siguiente instrucción |
| Stack Pointer  | SP     | Dirección de la parte superior de la pila |
+--------------+---------+-------------+
```

Conjunto de Instrucciones del Intel 8080

El procesador Intel 8080 tiene un conjunto completo de instrucciones, entre las cuales se encuentran:

    MOV r1, r2: Copiar datos entre registros.
    MVI r, data: Mover un valor inmediato a un registro.
    LDA addr: Cargar un byte de una dirección de memoria en el acumulador.
    STA addr: Almacenar el valor del acumulador en una dirección de memoria.
    JMP addr: Saltar a una dirección de memoria.

Para ver la lista completa de instrucciones, consulta la documentación oficial del Intel 8080.
Puertos de Entrada/Salida

Space Invaders utiliza varios puertos de entrada y salida para manejar gráficos, sonido y controles:

    Port 1: Maneja los controles del jugador (izquierda, derecha, disparo).
    Port 2: Controla el registro de desplazamiento gráfico.
    Port 3 y 5: Manejan los efectos de sonido (disparo, explosiones).
    Port 6: Control de video (no implementado en este emulador).

## Memoria del Juego

Las ROMs de Space Invaders se dividen en cuatro archivos que se cargan en las siguientes direcciones de la memoria emulada:

```bash
+------------------+---------------------------+
| Dirección Memoria | Archivo ROM               |
+------------------+---------------------------+
| 0x0000 - 0x07FF  | invaders.h                |
| 0x0800 - 0x0FFF  | invaders.g                |
| 0x1000 - 0x17FF  | invaders.f                |
| 0x1800 - 0x1FFF  | invaders.e                |
+------------------+---------------------------+
```

## Diagrama del Procesador

El siguiente diagrama muestra un esquema básico de la estructura del Intel 8080:

```bash
      +-----------------------------------------+
      |                CPU Intel 8080           |
      +-----------------------------------------+
      |  Registros:  A  B  C  D  E  H  L        |
      |  Program Counter (PC)                   |
      |  Stack Pointer (SP)                     |
      |  Flags: Z, S, P, CY, AC                 |
      +-----------------------------------------+
      |            Conjunto de Instrucciones    |
      +-----------------------------------------+
      |         Comunicación con la Memoria     |
      +-----------------------------------------+
```
