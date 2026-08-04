**Universidad Tecnológica Nacional - Facultad Regional Tucumán**
**Carrera:** Ingeniería Electrónica
**Asignatura:** Técnicas Digitales II

**Actividad de Formación Práctica N°4 - LAB - Año 2026**

**Tema:** Programación de microcontroladores: funciones no bloqueantes, aplicación con SysTick en STM32CubeIDE.
**Tipo de Actividad de Formación Práctica:** Laboratorio
**Unidad Temática 2.-** Arquitectura de un microprocesador.
**Unidad Temática 6.-** Microprocesadores.

**Trabajo de Laboratorio:** Implementación de un driver de funciones de retardo no bloqueantes utilizando SysTick, aplicado en proyectos con STM32CubeIDE y placas de desarrollo STM32-N145RH-Nucleo.

### 1. Alcance

El objetivo de este trabajo de laboratorio es que los estudiantes logren:

* Implementar un módulo de software para retardos no bloqueantes.
* Aplicar el concepto de funciones no bloqueantes en aplicaciones prácticas con STM32CubeIDE.
* Integrar el driver en aplicaciones existentes y reemplazar funciones bloqueantes como `HAL_Delay()`.
* Presentar el trabajo en laboratorio con funcionamiento correcto y defensa técnica de lo elaborado.

### 2. Elementos / Instrumental a Utilizar

* Placa de desarrollo STM32-N145RH-Nucleo.
* LEDs en pruebas de encendido y apagado controlados por software.
* Computadora con software de desarrollo integrado STM32CubeIDE instalado.
* Software de Programación y Debugging: STM32CubeIDE para programar y depurar el código.
* Repositorio GitHub grupal para subir las aplicaciones desarrolladas.
* Documentación técnica del fabricante y material de la cátedra sobre retardos no bloqueantes.

### 3. Actividades

a- Implementación del driver:

* Partir de un proyecto nuevo e implementar en `main.c` y `main.h` las funciones para retardos no bloqueantes, tomando como base las aplicaciones realizadas en actividad introductoria.
* Definir tipos personalizados como `tick_t`, `bool_t` y `delay_t`.
* Implementar las funciones `delayInit()`, `delayRead()` y `delayWrite()` según especificaciones:
  1. `delayInit()`: Cargar duración, inicializar flag running en false, no iniciar conteo.
  2. `delayRead()`: Verificar estado, tomar marca de tiempo con `HAL_GetTick()`, evaluar cumplimiento de tiempo y reiniciar flag si se cumplió.
  3. `delayWrite()`: Cambiar duración de un delay existente.

b- Implementación modular:

* Crear el módulo API delay en `/Drivers/API/src` y `/Drivers/API/inc` con sus funciones públicas y privadas.
* Mover definiciones y prototipos a `API_Delay.h` e implementaciones a `API_Delay.c`.

c- Integración en aplicaciones previas:

* Modificar las Apps de la práctica anterior para usar el driver no bloqueante y reemplazar `HAL_Delay()`.

d- Pruebas y verificación:

* Compilar, depurar y probar en laboratorio cada aplicación modificada.
* Evaluar la mejora en el comportamiento de las aplicaciones con el uso del driver no bloqueante.

e- Informe de laboratorio:

El informe debe presentar los siguientes puntos:

* Introducción al uso de retardos no bloqueantes y ventajas en aplicaciones comerciales.
* Lista de aplicaciones modificadas, autores y observaciones.
* Link al repositorio grupal con el código final.

ESTUDIANTE: Iratchet Franco

FECHA DE INICIO: 01/08/26   FECHA DE PRESENTACIÓN: 04/08/26

CONFORMIDAD DEL DOCENTE:.....................................................................................

---

### 1- Introducción al uso de retardos no bloqueantes y ventajas en aplicaciones comerciales

Para empezar a abordar este tema primero debemos diferenciar qué es una función bloqueante y una no bloqueante. ¿En qué se diferencian? La diferencia está en qué es lo que hace el procesador mientras espera.

La función bloqueante, por ejemplo: `HAL_Delay()`. Esta detiene por completo la ejecución del programa durante el tiempo especificado. El procesador entra en un bucle cerrado donde solo "cuenta tiempo" y no puede responder a nada más.

La función no bloqueante, no congela el programa. En lugar de detenerse, le pregunta al reloj del sistema qué hora es, calcula cuándo debe realizar la acción, y mientras tanto sigue ejecutando otras tareas.

Ahora entenderemos en que se basan las funciones no bloqueantes:

1- **Un contador de tiempo de hardware:** El microcontrolador tiene un reloj interno que cuenta los milisegundos transcurridos desde que se encendió.
2- **Evaluación de diferencia de tiempo:** En cada vuelta del bucle principal (main o loop), el programa calcula el tiempo transcurrido comparando el valor actual del contador con el valor de inicio.
3- **Condicionales de disparo (Polling / Máquinas de estado):** Si la diferencia es mayor o igual al intervalo deseado, se ejecuta la acción y se actualiza el tiempo de inicio. Si no, el programa simplemente continúa con la siguiente instrucción.

**Las ventajas en una aplicación comercial:**

En un producto comercial (un electrodoméstico, un equipo médico, un tablero de auto o un sistema de alarma), el uso de delays bloqueantes suele considerarse un error grave de arquitectura.

* **Multitarea simulada (Concurrencia):** Permite ejecutar varias tareas a ritmos diferentes de forma simultánea. Por ejemplo, parpadear un LED cada 500 ms, leer un sensor de temperatura cada 2 segundos y enviar datos por Wi-Fi cada 10 segundos, todo sin que una tarea entorpezca a la otra.
* **Respuesta inmediata a entradas del usuario (Interfaz responsiva):** Si un cliente presiona un botón en una pantalla táctil o un teclado mientras la aplicación usa `delay()`, el sistema no responderá hasta que termine la espera. Con código no bloqueante, la pulsación se detecta al instante.
* **Seguridad crítica y monitoreo en tiempo real:** Si el equipo necesita detectar una falla grave (como sobrecalentamiento o parada de emergencia), no puede darse el lujo de estar "dormido" dentro de un `delay()`. Debe poder leer las interrupciones y sensores en microsegundos.
* **Eficiencia y gestión de energías:** En aplicaciones a baterías, el procesador puede aprovechar el tiempo libre entre tareas para ponerse en modos de bajo consumo (Sleep mode) y despertarse solo cuando sea necesario.

### 2- Creación del driver

Para crear el driver lo primero que hicimos fue realizar un proyecto nuevo y utilizamos de prueba para la creación del mismo, en Drivers creamos un nuevo folder llamado `API_Delay`, dentro del mismo creamos dos folders llamados `Inc` en el cual se incluye el Header File y `Src` en el cual se incluye el Source File. En Header File definimos variables y las funciones las cuales en el Source File desarrollamos.

**Header File (`API_Delay.h`):**

```c
#ifndef API_DELAY_INC_API_DELAY_H_
#define API_DELAY_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
    tick_t startTime;    // Marca de tiempo de inicio
    tick_t duration;     // Duración del retardo en ms
    bool_t running;      // Estado del retardo (activo / inactivo)
} delay_t;

void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);

#endif /* API_DELAY_INC_API_DELAY_H_ */
```

Se puede observar que utilizamos variables de 32bits y tipo bool, para el caso de `delayRead` el cual únicamente debe leer si está activo o inactivo, anteriormente se puede observar que se creó una estructura para poder realizar con mayor facilidad las funciones.

A continuación se puede observar el archivo `API_Delay.c`, en el cual desarrollamos las funciones declaradas en el `API_Delay.h`. Aquí podemos encontrar la función `delayInit` la cual internamente es un IF el cual nos indica cuando debe empezar a correr el conteo.

La función `delayRead`, trabaja con valores true / false, como dijimos anteriormente trabaja avisando si la misma función está activa / inactiva por esta razón es del tipo bool. Las funciones creadas trabajan con IF los cuales nos indican si el conteo está corriendo y si está corriendo se pregunta si llegó al tiempo establecido o si debe seguir contando.

La función `delayWrite` se encarga de actualizar la duración del tiempo de retardo, es decir, si no agregamos un valor de delay la duración sigue actualizándose, cuando cambiemos el valor de tiempo la duración se actualizará siendo el valor establecido de "delay".

```c
void delayInit(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;
    delay->duration = duration;
    delay->running = false;
}

bool_t delayRead(delay_t * delay) {
    if (delay == NULL) return false;
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    }
    else {
        if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
            delay->running = false;
            return true;
        }
        return false;
    }
}

void delayWrite(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;
    delay->duration = duration;
}
```

El driver creado reemplazará la función `HAL_Delay()` para que las aplicaciones creadas sean con funciones no bloqueantes.

### 3- Aplicaciones modificadas

A continuación se detallan las modificaciones realizadas sobre las aplicaciones base.

**Aplicación:** App_1_1_Grupo_3_2026 – Encendido secuencial de los LEDs.
**Autor de la modificación:** Franco Iratchet
**Observaciones:** El while(1) está dando vueltas a máxima velocidad sin trabarse. Solo entra al switch cada 200 ms gracias a que `delayRead(&pasoDelay)` actúa como un "filtro" de tiempo, gracias al uso del driver `delay_t`. Delway no hizo falta crear tres objetos delay_t distintos para los tres LEDs. Como el tiempo de paso es siempre constante (200 ms) utilizando una sola estructura pasoDelay para marcar el ritmo de toda la aplicación.

**Aplicación:** App_1_2_Grupo_3_2026 – Inversión de la secuencia mediante el pulsador.
**Autor de la modificación:** Franco Iratchet
**Observaciones:** La función `checkButtonAndUpdateDirection()` se ejecuta en cada iteración del while(1). Al no usar `HAL_Delay()`, detecta el flanco ascendente al instante sin importar en qué punto de la cuenta de 200 ms esté la secuencia, además, como el sentido se evalúa dentro del if (`delayRead(&pasoDelay)`), si apretás el botón, el cambio de dirección se aplica de inmediato en el siguiente paso de tiempo.

**Aplicación:** App_1_3_Grupo_3_2026 – Selección de Secuencia mediante el pulsador.
**Autor de la modificación:** Franco Iratchet
**Observaciones:** Gracias a las modificaciones podemos lograr que la aplicación no se clave como cuando se utilizaba `HAL_Delay`, de esta manera logramos que si queremos que la CPU quede libre para realizar alguna tarea en paralelo, además logramos que se reinicie solo y logra manejar muy bien el desbordamiento gracias a la resta `HAL_GetTick() - startTime` que evita que se clave todo cuando el contador del SysTick pega la vuelta a cero.

### 4- Link al repositorio grupal

El repositorio grupal en el que se encuentran las aplicaciones desarrolladas para esta actividad de formación práctica se ubica en la siguiente dirección:

**Link del repositorio Grupal:** [https://github.com/FrancoIratchet/Grupo_3_TDII_2026](https://github.com/FrancoIratchet/Grupo_3_TDII_2026)
