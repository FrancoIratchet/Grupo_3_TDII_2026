/**
  ******************************************************************************
  * @file    API_Delay.c
  * @brief   Implementación del driver para funciones de retardo no bloqueante.
  *          Utiliza HAL_GetTick() basado en el timer SysTick del microcontrolador.
  ******************************************************************************
  * @author  Franco Iratchet
  ******************************************************************************
  */

#include "API_Delay.h"
#include "stm32f4xx_hal.h"

/**
  * @brief  Inicializa la estructura de retardo. Carga la duración requerida
  *         e inicializa el flag running en false (no iniciado).
  * @param  delay: Puntero a la estructura delay_t
  * @param  duration: Duración del retardo en milisegundos
  * @retval None
  */
void delayInit(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;

    delay->duration = duration;       // Carga la duración requerida //
    delay->running = false;           // No inicia el conteo todavía //
}

/**
  * @brief  Verifica si el retardo se cumplió. Si el retardo no está corriendo,
  *         toma la marca de tiempo actual y lo inicia. Si está corriendo,
  *         verifica si transcurrió el tiempo configurado.
  * @param  delay: Puntero a la estructura delay_t
  * @retval true: si el tiempo del retardo se cumplió
  *         false: si el retardo aún no se cumplió o se acaba de iniciar
  */
bool_t delayRead(delay_t * delay) {
    if (delay == NULL) return false;

    // Si el retardo no está corriendo, toma la marca de tiempo actual y lo inicia
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    }
    // Si ya está corriendo, verifica si transcurrió el tiempo configurado
    else {
        if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
            delay->running = false;   // Restablece el flag al cumplir el plazo
            return true;              // Retorna 'true' indicando que se cumplió
        }
        return false;
    }
}

/**
  * @brief  Actualiza la duración del tiempo de retardo.
  * @param  delay: Puntero a la estructura delay_t
  * @param  duration: Nueva duración del retardo en milisegundos
  * @retval None
  */
void delayWrite(delay_t * delay, tick_t duration) {
    if (delay == NULL) return;

    delay->duration = duration;  // Actualiza la duración del tiempo de retardo //
}
