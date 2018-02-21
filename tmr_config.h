/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       tmr_config.h
 */
#ifndef _TIMER_H_
#define _TIMMER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <Ticker.h>

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define TMR_ESTABLISH_CONNECTION_TIMEOUT_MS   (16000)
#define TMR_RECONNECT_TIMEOUT_MS              (20000)

/* ==================================================================== */
/* ===================== function declarations ======================== */
/* ==================================================================== */

/* Functions are defined in WebSockerServer main file */
void Start_est_connection_tmr(uint16_t tmout);
void Start_reconnect_tmr(uint16_t tmout);
void Stop_reconnect_tmr();

#endif /* _TIMER_H_ */

/* EOF */
