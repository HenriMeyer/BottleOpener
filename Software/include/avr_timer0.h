#ifndef AVR_TIMER0_H
#define AVR_TIMER0_H

extern volatile uint32_t ms_counter;

void timer0_init(void);
void timer0_start(void);
void timer0_stop(void);
void timer0_restart(void);

#endif
