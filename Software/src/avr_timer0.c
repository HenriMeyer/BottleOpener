#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t ms_counter = 0; // Software timer

// Initialize Timer0 → CTC Mode → 1 ms interrupt
void timer0_init(void)
{
    // Enable CTC Mode (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01); // CTC mode

    // Calculate Compare Match value:
    // F_CPU = 8 MHz, Prescaler 64 → 8 µs per tick
    // 1 ms = 125 ticks → OCR0A = 124 (0..124 → 125 ticks) --> 1000 ms
    OCR0A = 124;

    // Enable Compare Match interrupt
    TIMSK |= (0x10);

    // Reset counter
    TCNT0 = 0;
}

// Start Timer
void timer0_start(void)
{
    // Prescaler = 64 → Timer running → 8 µs per tick
    TCCR0B = (0x03);
}

// Stop Timer (and reset counter)
void timer0_stop(void)
{
    TCCR0B = 0;    // Stop Timer
    TCNT0 = 0;     // Reset counter
}

// "Restart" Timer → Stop + Start
void timer0_restart(void)
{
    timer0_stop();  // Stop and reset
    timer0_start(); // Start again
}

// Timer0 Compare Match Interrupt → triggered every 1 ms
ISR(TIMER0_COMPA_vect)
{
    ms_counter++; // Increment software counter in ms
}
