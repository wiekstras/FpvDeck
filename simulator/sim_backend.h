#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool pack_connected;
    bool signal_lost;
    bool video_noise;
    bool deck_low;
    bool cell_imbalance;
    unsigned cell_count;
    uint16_t tuned_frequency_mhz;
} sim_state_t;

const sim_state_t *sim_state(void);
void sim_set_pack_connected(bool enabled);
void sim_set_signal_lost(bool enabled);
void sim_set_video_noise(bool enabled);
void sim_set_deck_low(bool enabled);
void sim_set_cell_imbalance(bool enabled);
void sim_set_cell_count(unsigned count);

