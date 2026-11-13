#include "ringbuffer.h"

Ringbuffer<uint8_t, 256U> uart_rx;
Ringbuffer<uint8_t, 256U> uart_tx;