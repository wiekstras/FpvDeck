#include "video.h"

#include "board_pins.h"
#include "driver/isp.h"
#include "esp_cam_ctlr.h"
#include "esp_cam_ctlr_isp_dvp.h"
#include "esp_check.h"
#include "esp_heap_caps.h"

#define VIDEO_WIDTH 720U
#define VIDEO_PAL_FIELD_HEIGHT 288U
#define VIDEO_NTSC_FIELD_HEIGHT 240U
#define VIDEO_BYTES_PER_PIXEL 2U

static isp_proc_handle_t s_isp;
static esp_cam_ctlr_handle_t s_camera;
static esp_cam_ctlr_trans_t s_transaction;
static video_standard_t s_standard;

esp_err_t video_init(video_standard_t standard)
{
    if (s_camera != NULL) return ESP_ERR_INVALID_STATE;
    const uint16_t height = standard == VIDEO_STANDARD_PAL
                                ? VIDEO_PAL_FIELD_HEIGHT
                                : VIDEO_NTSC_FIELD_HEIGHT;
    const esp_isp_processor_cfg_t isp_cfg = {
        /* TVP5150 SCLK is 27 MHz; the ISP processing clock is at least 2x. */
        .clk_hz = 54U * 1000U * 1000U,
        .input_data_source = ISP_INPUT_DATA_SOURCE_DVP,
        .input_data_color_type = ISP_COLOR_YUV422,
        .output_data_color_type = ISP_COLOR_RGB565,
        .yuv_range = ISP_COLOR_RANGE_LIMIT,
        .yuv_std = ISP_YUV_CONV_STD_BT601,
        .has_line_start_packet = false,
        .has_line_end_packet = false,
        .h_res = VIDEO_WIDTH,
        .v_res = height,
    };
    ESP_RETURN_ON_ERROR(esp_isp_new_processor(&isp_cfg, &s_isp), "video",
                        "create ISP");
    ESP_RETURN_ON_ERROR(esp_isp_enable(s_isp), "video", "enable ISP");

    const esp_cam_ctlr_isp_dvp_cfg_t dvp_cfg = {
        .data_width = 8,
        .data_io = {PIN_DVP_D0, PIN_DVP_D1, PIN_DVP_D2, PIN_DVP_D3,
                    PIN_DVP_D4, PIN_DVP_D5, PIN_DVP_D6, PIN_DVP_D7},
        .pclk_io = PIN_DVP_PCLK,
        .hsync_io = PIN_DVP_HSYNC,
        .vsync_io = PIN_DVP_VSYNC,
        .de_io = -1,
        .io_flags.vsync_invert = 0,
        .queue_items = 2,
    };
    ESP_RETURN_ON_ERROR(esp_cam_new_isp_dvp_ctlr(s_isp, &dvp_cfg, &s_camera),
                        "video", "create DVP controller");

    s_transaction.buflen = VIDEO_WIDTH * height * VIDEO_BYTES_PER_PIXEL;
    s_transaction.buffer = esp_cam_ctlr_alloc_buffer(
        s_camera, s_transaction.buflen, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
    ESP_RETURN_ON_FALSE(s_transaction.buffer != NULL, ESP_ERR_NO_MEM,
                        "video", "allocate field buffer");
    ESP_RETURN_ON_ERROR(esp_cam_ctlr_enable(s_camera), "video", "enable DVP");
    ESP_RETURN_ON_ERROR(esp_cam_ctlr_start(s_camera), "video", "start DVP");
    s_standard = standard;
    return ESP_OK;
}

esp_err_t video_capture_field(video_frame_t *frame, uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(frame != NULL && s_camera != NULL, ESP_ERR_INVALID_STATE,
                        "video", "video not initialized");
    ESP_RETURN_ON_ERROR(esp_cam_ctlr_receive(s_camera, &s_transaction, timeout_ms),
                        "video", "capture field");
    frame->pixels = s_transaction.buffer;
    frame->size_bytes = s_transaction.buflen;
    frame->width = VIDEO_WIDTH;
    frame->height = s_standard == VIDEO_STANDARD_PAL
                        ? VIDEO_PAL_FIELD_HEIGHT
                        : VIDEO_NTSC_FIELD_HEIGHT;
    frame->standard = s_standard;
    return ESP_OK;
}
