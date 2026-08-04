/**
  ******************************************************************************
  * @file    API_Delay.h
  * @brief   Driver para funciones de retardo no bloqueante utilizando SysTick.
  *          Reemplaza la función HAL_Delay() por funciones que no bloquean
  *          la ejecución del programa principal.
  ******************************************************************************
  * @author  Franco Iratchet
  ******************************************************************************
  */

#ifndef API_DELAY_INC_API_DELAY_H_
#define API_DELAY_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>  //Bibliotecas requeridas para los tipos de variables
#include <stdbool.h> //Bibliotecas requeridas para los tipos de variables

// variables //
typedef uint32_t tick_t;
typedef bool bool_t;

// Estructura de control para cada retardo //
typedef struct {
    tick_t startTime;    // Marca de tiempo de inicio //
    tick_t duration;     // Duración del retardo en ms //
    bool_t running;      // Estado del retardo (activo / inactivo) //
} delay_t;

// Declaración de funciones //
void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);

#endif /* API_DELAY_INC_API_DELAY_H_ */
