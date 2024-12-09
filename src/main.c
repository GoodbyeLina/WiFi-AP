#include <pico/stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#include "wifi_ap.h"
#include "i2c.h"


// 定义结构体来组织数据
typedef struct {
    bool synced_keyboard;
    bool synced_mouse;
    bool synced_gamepad;
    uint8_t state_matrix[256];
} DataPacket;


int main() {

    stdio_init_all();

    gpio_init(PIN_LED_PICO);
    gpio_set_dir(PIN_LED_PICO, GPIO_OUT);
    gpio_put(PIN_LED_PICO, true);

    init_uart();
    
    // I2C 初始化，SSD1306 初始化
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // 初始化显示屏
    SSD1306_init();

    // 为整个帧的渲染区域初始化（SSD1306_WIDTH像素，SSD1306_NUM_PAGES页）
    struct render_area frame_area = {
        start_col: 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
        };

    calc_render_area_buflen(&frame_area);   // 计算渲染区域缓冲区长度

    // 清除整个显示屏
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    // 显示屏写字
    char *text[] = {
        "A long time ago",
        "  on an OLED ",
        "   display",
        " far far away",
        "Lived a small",
        "red raspberry",
        "by the name of",
        "    PICO"
    };

    int y = 0;
    for (uint i = 0 ;i < count_of(text); i++) {
        WriteString(buf, 5, y, text[i]);
        y+=8;
    }
    render(buf, &frame_area);

    configure_esp8285();

    uint8_t receive_buffer[100];

    // 从ESP8285接收数据
    int bytes_received = receive_data_from_esp8285(receive_buffer, sizeof(receive_buffer));
    if (bytes_received > 0) {
        // 对接收的数据进行处理，比如打印出来
        for (int i = 0; i < bytes_received; i++) {
            printf("%02x ", receive_buffer[i]);
        }
        printf("\n");
    }

    // 可以在这里准备要发送的反馈数据并发送到ESP8285（可选）

    while (1){
        gpio_put(PIN_LED_PICO, true);
        sleep_ms(500);
        gpio_put(PIN_LED_PICO, false);
        sleep_ms(500);

        SSD1306_scroll(true);
        send_data_to_esp8285(*text, sizeof(text));
        sleep_ms(5000);
        SSD1306_scroll(false);

    }
    return 0;
}