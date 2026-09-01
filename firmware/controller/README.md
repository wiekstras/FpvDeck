# FPVDeck controller firmware

This directory contains the allocation-free, host-tested wire protocol and a
hardware-abstraction-backed Test PCB command dispatcher. It is not yet a flashable
board target because Rev A routing and clock/power configuration are not released.
Clock, GPIO, USB, ADC, watchdog, power, and boot configuration will be generated
only after the exact MCU/package and Rev A schematic are frozen from datasheets.

Firmware responsibilities: independent watchdog, debounced navigation inputs,
ADS70xx/ADS8688 sampling, temperatures/fan, power button and shutdown state
machine, rail-good monitoring, buzzer/LED/haptics, device identity, heartbeat,
and failure-safe outputs. Flight-control gimbal data remains on the T8L board.

The host tests exercise ADC/tap reads, self-test masks, output commands and unknown
command handling without an STM32 HAL. Board-specific STM32Cube startup, USB CDC,
ADS8688A SPI and watchdog code is the next layer once the schematic pin review is
closed; keeping it out now avoids generating unsafe pin assignments from a draft.
