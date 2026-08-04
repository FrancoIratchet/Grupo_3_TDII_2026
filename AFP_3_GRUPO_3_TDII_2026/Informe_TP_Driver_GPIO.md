**Universidad Tecnológica Nacional - Facultad Regional Tucumán**
**Carrera:** Ingeniería Electrónica
**Asignatura:** Técnicas Digitales II

**Actividad de Formación Práctica N°3 - LAB - Año 2026**

**Tema:** Utilización de drivers en aplicaciones desarrolladas para sistemas embebidos, creación y desarrollo de un driver GPIO.
**Tipo de Actividad de Formación Práctica:** Laboratorio
**Unidad Temática 2.-** Arquitectura de un microprocesador.
**Unidad Temática 6.-** Microprocesadores.

**Trabajo de Laboratorio:** Creación de un módulo de software (driver) para la abstracción del módulo GPIO de STM32, aplicado en proyectos con STM32CubeIDE y placas de desarrollo STM32-N145RH-Nucleo.

### 1. Alcance

El objetivo de este trabajo de laboratorio es que los estudiantes logren:

* Crear un driver de funciones para el manejo del módulo GPIO de un microcontrolador.
* Aplicar el concepto de abstracción de hardware reemplazando las funciones de la Hardware Abstraction Layer (HAL) por funciones propias.
* Integrar el driver en aplicaciones existentes y verificar su correcto funcionamiento mediante la compilación y el debug.
* Presentar el trabajo en laboratorio con funcionamiento correcto y defensa técnica de lo elaborado.

### 2. Elementos / Instrumental a Utilizar

* Placa de desarrollo STM32-N145RH-Nucleo.
* LEDs en pruebas de encendido y apagado controlados por software.
* Computadora con software de desarrollo integrado STM32CubeIDE instalado.
* Software de Programación y Debugging: STM32CubeIDE para programar y depurar el código.
* Repositorio GitHub grupal para subir las aplicaciones desarrolladas.
* Documentación técnica del fabricante y material de la cátedra sobre la creación de drivers.

### 3. Actividades

a- Implementación del driver:

* Partir de la aplicación desarrollada en la actividad de formación práctica 1 (Ejemplo03_Push_Button3) para utilizar como base la implementación del módulo GPIO.
* Seguir los pasos detallados en la Guía de creación de drivers que se encuentra en el repositorio de la materia.
* Crear los directorios necesarios para los archivos de cabecera (`Inc`) y de implementación (`Src`) dentro de la carpeta `Drivers/API`.

b- Implementación modular:

* Definir en el archivo de cabecera (`API_GPIO.h`) los prototipos de las funciones públicas que se utilizarán para interactuar con el hardware (por ejemplo: `GPIO_Init()`, `toggleLed_GPIO()`, `writeLedOn_GPIO()`, `writeLedOff_GPIO()`, `readButton_GPIO()`).
* Mover la función de inicialización `MX_GPIO_Init()` desde `main.c` hacia el archivo fuente `API_GPIO.c`. Es importante recordar quitar el calificador `static` para que pueda ser llamada desde otros archivos.

c- Integración en aplicaciones previas:

* Modificar las Apps de la práctica anterior para usar el driver de GPIO y reemplazar las funciones de la HAL (como `HAL_GPIO_TogglePin()` y `HAL_GPIO_WritePin()`) por las nuevas funciones de la API.
* Reemplazar los identificadores de pines directos por `#define` locales en el `main.c`.

d- Pruebas y verificación:

* Compilar, depurar y probar en laboratorio cada aplicación modificada (Ejemplo01, Ejemplo02 y Ejemplo03).
* Evaluar la mejora en la legibilidad del código al utilizar funciones de alto nivel en lugar de llamadas a la HAL.

e- Informe de laboratorio:

El informe debe presentar los siguientes puntos:

* Introducción al uso de drivers en Apps desarrolladas para sistemas embebidos.
* Aplicaciones desarrolladas: nombre de la aplicación, autor de la modificación y observaciones.
* Link al repositorio grupal con el código final.

ESTUDIANTE: Iratchet Franco

FECHA DE INICIO: 01/08/26   FECHA DE PRESENTACIÓN: 04/08/26

CONFORMIDAD DEL DOCENTE:.....................................................................................

---

### 1- Introducción al uso de drivers y ventajas en aplicaciones comerciales

Para empezar a abordar este tema primero debemos diferenciar qué es una capa de abstracción o driver. En el desarrollo de aplicaciones embebidas, es muy común que los fabricantes de microcontroladores provean una HAL (Hardware Abstraction Layer) para facilitar el acceso al hardware. Sin embargo, utilizar directamente estas funciones ató a nuestro código principal a la implementación específica de ese fabricante.

La creación de un driver propio actúa como una capa intermedia. Por ejemplo, en lugar de llamar directamente a `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)`, creamos una función propia como `writeLedOn_GPIO()`.

Las ventajas en una aplicación comercial son significativas:

* **Portabilidad de código:** Si en el futuro decidimos cambiar el microcontrolador (por ejemplo, de STM32 a PIC o ESP32), el código en nuestro `main.c` no necesitará cambios. Solo tendremos que reescribir nuestro driver para que apunte a los nuevos registros o a la nueva HAL.
* **Legibilidad y Mantenibilidad:** El código principal `main.c` se vuelve mucho más limpio y fácil de leer. En lugar de llenar el bucle principal con configuraciones complejas de bits y pines, llamamos a funciones de alto nivel que describen exactamente qué queremos que haga el programa (ej: encender un LED, leer un botón).
* **Evitar errores:** Al encapsular la inicialización (como mover la función `MX_GPIO_Init()` al driver y quitar el `static`), forzamos a que el hardware se inicialice de una manera controlada y reutilizable.

### 2- Creación del driver GPIO

Para crear el driver lo primero que hicimos fue tomar la aplicación `Ejemplo03_Push_Button3` como base y seguir la guía del repositorio de la materia. En la carpeta de nuestro proyecto, dentro de `Drivers`, creamos una nueva carpeta llamada `API`. Dentro de esta, creamos los folders `Inc` y `Src`.

En el archivo **Header File (`API_GPIO.h`)**, definimos los tipos de datos necesarios y declaramos los prototipos de las funciones que nuestro driver expondrá al resto del programa. También nos aseguramos de agregar esta ruta a los Include Paths en las propiedades del proyecto para evitar errores de compilación.

En el archivo **Source File (`API_GPIO.c`)**, movimos la función de inicialización del GPIO (`MX_GPIO_Init()`). Un detalle importante fue eliminar la palabra clave `static` de la declaración original, ya que si se mantenía, la función solo sería visible dentro de su propio archivo y `main.c` no podría llamarla.

A continuación, se puede observar cómo implementamos las funciones básicas para el control de LEDs y lectura de botones. La función `toggleLed_GPIO` se encarga de invertir el estado de un pin, mientras que `writeLedOn_GPIO` y `writeLedOff_GPIO` permiten forzar el estado alto o bajo. Por último, `readButton_GPIO` verifica el estado de una entrada digital.

El driver creado reemplazará las funciones de la HAL para que las aplicaciones sean más modulares.

### 3- Aplicaciones desarrolladas

A continuación se detallan las modificaciones realizadas sobre las aplicaciones base del repositorio de la materia.

**Aplicación: Ejemplo03_Push_Button3**
**Autor de la modificación:** Iratchet Franco

**Observaciones:** Esta aplicación fue utilizada como base principal para el desarrollo del driver, siguiendo los pasos de la "Guía de creación de drivers". Se crearon los archivos `API_GPIO.h` y `API_GPIO.c`. Se eliminó la inicialización local `MX_GPIO_Init()` de `main.c` y se migró al driver. En `main.c`, se definieron etiquetas (`#define LED1 LD1_Pin`, etc.) para reemplazar el uso directo de los pines HAL. La lógica de lectura del pulsador fue reemplazada por `readButton_GPIO()`, y las operaciones de escritura de los LEDs fueron reemplazadas por `writeLedOn_GPIO()` y `writeLedOff_GPIO()`. No se encontraron inconvenientes mayores, aunque fue necesario asegurar que la carpeta `Inc` del driver estuviera correctamente configurada en los paths de compilación.

**Aplicación: Ejemplo02_Push_Button**
**Autor de la modificación:** Iratchet Franco

**Observaciones:** Esta aplicación posee una lógica muy similar a la de Ejemplo03. Las modificaciones consistieron en integrar el driver `API_GPIO` creado previamente. Se replicó la estructura de carpetas y se adaptó el `main.c` para utilizar las funciones de abstracción del driver. La experiencia fue fluida, ya que al tener la misma configuración de hardware, la implementación de las funciones del driver fue directa y no requirió ajustes en la lógica del bucle `while(1)`.

**Aplicación: Ejemplo01_Inicio_Blinking**
**Autor de la modificación:** Iratchet Franco

**Observaciones:** Esta es la aplicación más simple, dedicada al parpadeo (blinking) de un LED. Al igual que en los casos anteriores, se integró el driver `API_GPIO`. La diferencia principal radica en que la lógica original solo utilizaba la función `HAL_GPIO_TogglePin()`. Esta llamada fue reemplazada exitosamente por la función `toggleLed_GPIO()` provista por el driver. Al ser una aplicación básica, la prueba de concepto del driver para operaciones de conmutación de pines se validó correctamente. Se recomienda para futuras versiones probar el reemplazo también de las funciones de retraso (`HAL_Delay`) por un driver de retardo no bloqueante.

### 4- Link al repositorio grupal

El repositorio grupal en el que se encuentran las aplicaciones desarrolladas para esta actividad de formación práctica se ubica en la siguiente dirección:

**Link del repositorio Grupal:** [https://github.com/FrancoIratchet/Grupo_3_TDII_2026](https://github.com/FrancoIratchet/Grupo_3_TDII_2026)
