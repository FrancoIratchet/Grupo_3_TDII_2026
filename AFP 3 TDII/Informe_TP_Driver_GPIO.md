# Informe Trabajo Práctico: Desarrollo de Driver GPIO

**Materia:** Técnicas Digitales II
**Carrera:** Ingeniería Electrónica (Cuarto Año)
**Universidad:** UTN-FRT
**Autor:** Franco Iratchet

---

## 5.1. Introducción al tema: utilización de drivers en Apps desarrolladas para sistemas embebidos

La arquitectura de software para sistemas embebidos se beneficia enormemente del uso de capas de abstracción, conocidas comúnmente como drivers o APIs. En el contexto de la programación de microcontroladores STM32, desarrollar una aplicación (App) utilizando drivers propios que envuelvan las funciones de la Hardware Abstraction Layer (HAL) o de los registros del hardware presenta múltiples ventajas.

La principal ventaja de utilizar drivers es la **portabilidad y la abstracción del hardware**. Al encapsular las llamadas a la HAL en funciones propias (por ejemplo, `writeLedOn_GPIO()` en lugar de `HAL_GPIO_WritePin()`), el código principal de la aplicación (`main.c`) deja de depender estrictamente de la estructura y nombres de la HAL proporcionada por el fabricante. Si en el futuro se decide migrar el proyecto a una familia diferente de microcontroladores (por ejemplo, pasar de STM32 a ESP32 o a un AVR), la lógica de la aplicación en `main.c` permanecerá intacta; solo será necesario reescribir el driver específico para la nueva plataforma.

Otra ventaja significativa es la **legibilidad y mantenibilidad del código**. Las aplicaciones embebidas tienden a volverse complejas rápidamente. Al delegar las tareas de bajo nivel (como la configuración de registros, habilitación de relojes y escritura de pines) a un archivo fuente dedicado (como `API_GPIO.c`), el flujo principal de la aplicación resulta más limpio, conciso y fácil de depurar.

El desarrollo de un driver en STM32CubeIDE sigue una serie de pasos generales que es importante tener en cuenta para evitar errores comunes:

1. **Creación de la estructura de directorios:** Se deben crear las carpetas `Inc` (para los archivos de cabecera `.h`) y `Src` (para los archivos fuente `.c`) dentro de un directorio destinado a las APIs (por ejemplo, `Drivers/API`). Es crucial conservar las extensiones de los archivos al crearlos en el IDE.
2. **Configuración de los Include Paths:** Para que el compilador reconozca los nuevos archivos de cabecera, es obligatorio agregar la ruta `Drivers/API/Inc` a la configuración del proyecto (`Properties -> C/C++ General -> Paths and Symbols`). De lo contrario, se producirán errores de compilación al intentar incluir el header.
3. **Migración del código de inicialización:** El código de inicialización de los periféricos (como `MX_GPIO_Init()`) debe cortarse desde `main.c` y pegarse en el archivo fuente del driver. Además, la declaración original en `main.c` (que suele ser `static`) debe eliminarse o comentarse para que la función sea accesible desde otros archivos.
4. **Eliminación de modificadores de ámbito:** Un error frecuente es olvidar quitar la palabra clave `static` de la declaración de la función de inicialización en el archivo header y en el código fuente. El modificador `static` restringe el ámbito de la función al archivo donde se encuentra declarada, lo que impedirá que `main.c` pueda invocarla, resultando en errores de compilación.

## 5.2. Aplicaciones desarrolladas

A continuación, se detallan las modificaciones realizadas sobre las aplicaciones base del repositorio de la materia.

### Aplicación: Ejemplo03_Push_Button3
- **Autor de la modificación:** Franco Iratchet
- **Observaciones:** Esta aplicación fue utilizada como base principal para el desarrollo del driver, siguiendo los pasos de la "Guía de creación de drivers". Se crearon los archivos `API_GPIO.h` y `API_GPIO.c` dentro de la carpeta `Drivers/API`. Se eliminó la inicialización local `MX_GPIO_Init()` de `main.c` y se migró al driver. 
En `main.c`, se definieron etiquetas (`#define LED1 LD1_Pin`, etc.) para reemplazar el uso directo de los pines HAL. La lógica de lectura del pulsador (`HAL_GPIO_ReadPin`) fue reemplazada por `readButton_GPIO()`, y las operaciones de escritura de los LEDs (`HAL_GPIO_WritePin`) fueron reemplazadas por `writeLedOn_GPIO()` y `writeLedOff_GPIO()`. No se encontraron inconvenientes mayores, aunque fue necesario asegurar que la carpeta `Inc` del driver estuviera correctamente configurada en los paths de compilación para evitar errores de "archivo no encontrado".

### Aplicación: Ejemplo02_Push_Button
- **Autor de la modificación:** Franco Iratchet
- **Observaciones:** Esta aplicación posee una lógica muy similar a la de Ejemplo03. Las modificaciones consistieron en integrar el driver `API_GPIO` creado previamente. Se replicó la estructura de carpetas y se adaptó el `main.c` para utilizar las funciones de abstracción del driver. La experiencia fue fluida, ya que al tener la misma configuración de hardware (STM32 Nucleo F413ZH), la implementación de las funciones del driver (como `toggleLed_GPIO`, `readButton_GPIO`) fue directa y no requirió ajustes en la lógica del bucle `while(1)`.

### Aplicación: Ejemplo01_Inicio_Blinking
- **Autor de la modificación:** Franco Iratchet
- **Observaciones:** Esta es la aplicación más simple, dedicada al parpadeo (blinking) de un LED. Al igual que en los casos anteriores, se integró el driver `API_GPIO`. La diferencia principal radica en que la lógica original solo utilizaba la función `HAL_GPIO_TogglePin()`. Esta llamada fue reemplazada exitosamente por la función `toggleLed_GPIO()` provista por el driver. Al ser una aplicación básica, la prueba de concepto del driver para operaciones de conmutación de pines se validó correctamente. Se recomienda para futuras versiones probar el reemplazo también de las funciones de retraso (`HAL_Delay`) por un driver de retardo no bloqueante, aunque esto excede el alcance del presente trabajo.

## 5.3. Link al repositorio grupal

El repositorio grupal en el que se encuentran las aplicaciones desarrolladas para esta actividad de formación práctica se ubica en la siguiente dirección:

[https://github.com/FrancoIratchet/Tecnicas_Digitales_II](https://github.com/FrancoIratchet/Tecnicas_Digitales_II)
