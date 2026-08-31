# FPVDeck controller firmware

This directory currently contains the allocation-free, host-tested wire protocol
core shared by future STM32G0 firmware. It is not yet a flashable board target.
Clock, GPIO, USB, ADC, watchdog, power, and boot configuration will be generated
only after the exact MCU/package and Rev A schematic are frozen from datasheets.

Firmware responsibilities: independent watchdog, debounced navigation inputs,
ADS70xx/ADS8688 sampling, temperatures/fan, power button and shutdown state
machine, rail-good monitoring, buzzer/LED/haptics, device identity, heartbeat,
and failure-safe outputs. Flight-control gimbal data remains on the T8L board.

