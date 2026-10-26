#include "deck_power.h"

deck_power_state_t deck_power_classify(float voltage)
{
    if (voltage < 2.5f) return DECK_POWER_NO_BATTERY;
    if (voltage <= 3.3f) return DECK_POWER_SHUTDOWN;
    if (voltage <= 3.5f) return DECK_POWER_LOW;
    return DECK_POWER_NORMAL;
}

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"

#define HOST_BATTERY_DIVIDER_RATIO 3.0f /* schematic: 200k over 100k */
#define DECK_ADC_SAMPLES 8

static adc_oneshot_unit_handle_t s_adc;
static adc_cali_handle_t s_calibration;
static adc_channel_t s_channel;

esp_err_t deck_power_init(void)
{
    adc_unit_t unit;
    ESP_RETURN_ON_ERROR(adc_oneshot_io_to_channel(PIN_DECK_BATTERY_ADC, &unit,
                                                   &s_channel),
                        "deck_power", "map BAT_ADC GPIO");
    const adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_new_unit(&unit_cfg, &s_adc),
                        "deck_power", "create ADC unit");
    const adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_config_channel(s_adc, s_channel, &channel_cfg),
                        "deck_power", "configure BAT_ADC");

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    const adc_cali_curve_fitting_config_t cal_cfg = {
        .unit_id = unit,
        .chan = s_channel,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    return adc_cali_create_scheme_curve_fitting(&cal_cfg, &s_calibration);
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t deck_power_read_voltage(float *voltage)
{
    ESP_RETURN_ON_FALSE(voltage != NULL && s_adc != NULL &&
                            s_calibration != NULL,
                        ESP_ERR_INVALID_STATE, "deck_power", "not initialized");
    int raw_sum = 0;
    for (unsigned i = 0; i < DECK_ADC_SAMPLES; ++i) {
        int raw = 0;
        ESP_RETURN_ON_ERROR(adc_oneshot_read(s_adc, s_channel, &raw),
                            "deck_power", "read BAT_ADC");
        raw_sum += raw;
    }
    int millivolts = 0;
    ESP_RETURN_ON_ERROR(adc_cali_raw_to_voltage(
                            s_calibration, raw_sum / DECK_ADC_SAMPLES,
                            &millivolts),
                        "deck_power", "calibrate BAT_ADC");
    *voltage = (float)millivolts * HOST_BATTERY_DIVIDER_RATIO / 1000.0f;
    return ESP_OK;
}
#endif
