#ifndef SRC_INCLUDE_PULSE_H_
#define SRC_INCLUDE_PULSE_H_

typedef struct _water_conter_t {
    uint8_t  status     :1;
    uint8_t  pressed    :1;
    uint8_t  counter    :6;
    uint16_t bit;
} water_counter_t;

void init_pulse();
uint8_t task_counters();
uint32_t check_counter_overflow(uint32_t check_count);

#endif /* SRC_INCLUDE_PULSE_H_ */
