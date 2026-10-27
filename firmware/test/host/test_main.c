#include "battery.h"
#include "deck_power.h"
#include "rx5808.h"
#include "signal_logic.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned failures;

#define CHECK(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr); \
        ++failures; \
    } \
} while (0)

#define CHECK_NEAR(actual, expected, tolerance) \
    CHECK(fabsf((actual) - (expected)) <= (tolerance))

static void test_rx5808(void)
{
    uint16_t frequency = 0;
    CHECK(rx5808_channel_frequency(RX5808_BAND_R, 0, &frequency));
    CHECK(frequency == 5658);
    CHECK(rx5808_channels_mhz[RX5808_BAND_L][7] == 5621);
    CHECK(!rx5808_channel_frequency(RX5808_BAND_COUNT, 0, &frequency));
    CHECK(!rx5808_channel_frequency(RX5808_BAND_A, 8, &frequency));

    const uint16_t n = (uint16_t)((5800U - 479U) / 2U);
    const uint16_t expected = (uint16_t)(((n / 32U) << 7U) | (n % 32U));
    CHECK(rx5808_frequency_register(5800) == expected);
    CHECK(rx5808_frequency_register(478) == 0);
    const uint32_t frame = rx5808_write_frame(0x01, expected);
    CHECK((frame & 0x0FU) == 1U);
    CHECK((frame & (1UL << 4U)) != 0U);
    CHECK(((frame >> 5U) & 0xFFFFFU) == expected);
}

static void test_battery(void)
{
    const float cells[6] = {4.10f, 4.09f, 4.11f, 4.08f, 4.10f, 4.09f};
    float adc[6] = {0};
    float cumulative = 0.0f;
    for (unsigned i = 0; i < 6; ++i) {
        cumulative += cells[i];
        adc[i] = cumulative / BATTERY_DIVIDER_RATIO;
    }
    const float calibration[6] = {1, 1, 1, 1, 1, 1};
    battery_pack_t pack;
    battery_compute(adc, calibration, &pack);
    CHECK(pack.cell_count == 6);
    CHECK_NEAR(pack.total_v, 24.57f, 0.01f);
    CHECK_NEAR(pack.minimum_v, 4.08f, 0.01f);
    CHECK_NEAR(pack.maximum_v, 4.11f, 0.01f);
    CHECK_NEAR(pack.delta_v, 0.03f, 0.005f);
    CHECK(pack.valid);

    for (unsigned count = 1; count <= 6; ++count) {
        float adc_case[6] = {0};
        float tap = 0.0f;
        for (unsigned i = 0; i < count; ++i) {
            tap += 3.90f + 0.01f * (float)i;
            adc_case[i] = tap / BATTERY_DIVIDER_RATIO;
        }
        battery_compute(adc_case, calibration, &pack);
        CHECK(pack.cell_count == count);
        CHECK(pack.valid);
        CHECK_NEAR(pack.total_v, tap, 0.01f);
    }

    const float missing_tap[6] = {
        3.9f / BATTERY_DIVIDER_RATIO,
        7.8f / BATTERY_DIVIDER_RATIO,
        0.0f,
        15.6f / BATTERY_DIVIDER_RATIO,
        0.0f, 0.0f
    };
    battery_compute(missing_tap, calibration, &pack);
    CHECK(pack.cell_count == 4);
    CHECK(!pack.valid);

    const float empty[6] = {0};
    battery_compute(empty, calibration, &pack);
    CHECK(pack.cell_count == 0);
    CHECK(pack.minimum_v == 0.0f);
    CHECK(!pack.valid);
    CHECK(battery_cell_state(3.49f)[0] == 'W');
    CHECK(battery_cell_state(4.26f)[0] == 'E');
    CHECK(battery_is_storage_voltage(3.82f));
    CHECK(!battery_is_storage_voltage(3.90f));
}

static void test_signal(void)
{
    rssi_filter_t filter;
    rssi_filter_reset(&filter, 0.5f);
    CHECK_NEAR(rssi_filter_update(&filter, 10.0f), 10.0f, 0.001f);
    CHECK_NEAR(rssi_filter_update(&filter, 20.0f), 15.0f, 0.001f);
    CHECK_NEAR(filter.peak, 15.0f, 0.001f);
    rssi_filter_update(&filter, 0.0f);
    CHECK_NEAR(filter.peak, 15.0f, 0.001f);

    const scanner_sample_t samples[] = {
        {5800, 0.4f}, {5658, 0.9f}, {5740, 0.7f}, {5917, 0.8f}
    };
    scanner_sample_t strongest[3];
    scanner_strongest(samples, 4, strongest, 3);
    CHECK(strongest[0].frequency_mhz == 5658);
    CHECK(strongest[1].frequency_mhz == 5917);
    CHECK(strongest[2].frequency_mhz == 5740);
    CHECK(finder_beep_interval_ms(-1.0f) == 900);
    CHECK(finder_beep_interval_ms(0.0f) == 900);
    CHECK(finder_beep_interval_ms(1.024f) == 525);
    CHECK(finder_beep_interval_ms(2.048f) == 150);
    CHECK(finder_beep_interval_ms(3.0f) == 150);
}

static void test_deck_power(void)
{
    CHECK(deck_power_classify(0.0f) == DECK_POWER_NO_BATTERY);
    CHECK(deck_power_classify(2.49f) == DECK_POWER_NO_BATTERY);
    CHECK(deck_power_classify(3.30f) == DECK_POWER_SHUTDOWN);
    CHECK(deck_power_classify(3.31f) == DECK_POWER_LOW);
    CHECK(deck_power_classify(3.50f) == DECK_POWER_LOW);
    CHECK(deck_power_classify(3.51f) == DECK_POWER_NORMAL);
}

int main(void)
{
    test_rx5808();
    test_battery();
    test_signal();
    test_deck_power();
    if (failures) {
        fprintf(stderr, "%u host test(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    puts("All FpvDeck host tests passed");
    return EXIT_SUCCESS;
}
