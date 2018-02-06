#ifndef _TIMER_H_
#define _TIMMER_H_

#include <Ticker.h>

#define TMR_ESTABLISH_CONNECTION_TIMEOUT_MS   (16000)
#define TMR_RECONNECT_TIMEOUT_MS              (20000)

/* Functions are defined in WebSockerServer main file */
void Start_est_connection_tmr(uint16_t tmout);
void Start_reconnect_tmr(uint16_t tmout);
void Stop_reconnect_tmr();

#endif /* _TIMER_H_ */
