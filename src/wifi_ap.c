

#include "pico/stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#include "wifi_ap.h"

const char* ssid = "HUAWEI-CR18QS";
const char* password = "wxy19990308";


bool send_at_command(const char *cmd, const char *act)
{
    int i = 0;
    uint64_t t = 0;

    uart_puts(UART_ID, cmd);
    uart_puts(UART_ID, "\r\n");

    t = time_us_64();
    while (time_us_64() - t < 2500 * 1000)
    {
        while (uart_is_readable_within_us(UART_ID, 2000))
        {
            buf[i++] = uart_getc(UART_ID);
        }
        if (i > 0)
        {
            buf[i] = '\0';
            printf("%s\r\n", buf);
            if (strstr(buf, act) != NULL)
            {
                return true;
            }
            else
            {
                i = 0;
            }
        }
    }
    //printf("false\r\n");
    return false;
}



void configure_esp8285() {
    // 发送AT指令让ESP8285重启并进入STA模式
    send_at_command("AT+RST", "OK");
    sleep_ms(5000);
    send_at_command("AT+CWMODE=2", "OK");   // 设置成 station 模式
    sleep_ms(1000);

    // 设置要连接的WiFi网络SSID和密码
    char command[100];
    sprintf(command, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, password); // 临时连接 AP
    send_at_command(command);
    sleep_ms(5000);
}

void send_data_to_esp8285(uint8_t *data, int data_size) {
    for (int i = 0; i < data_size; i++) {
        uart_putc(UART_ID, data[i]);
    }
}

int receive_data_from_esp8285(uint8_t *buffer, int buffer_size) {
    int bytes_read = 0;
    while (uart_is_readable(UART_ID)) {
        buffer[bytes_read] = uart_getc(UART_ID);
        bytes_read++;
        if (bytes_read >= buffer_size) {
            break;
        }
    }
    return bytes_read;
}

void sendDataOverWiFi(DataPacket packet) {
    // 将结构体转换为字节数组，假设我们使用的网络协议需要发送字节流形式的数据
    uint8_t buffer[sizeof(DataPacket)];
    memcpy(buffer, &packet, sizeof(DataPacket));
    
    // 这里需要调用Esp8285相关的WiFi发送函数发送数据，以下是简单示意（实际函数名等根据Esp8285的库来定）
    esp8285_send_data(buffer, sizeof(DataPacket));
}

// 接收数据示例函数
void receiveDataFromWiFi() {
    // 假设接收到的数据存放在这个buffer中，长度为sizeof(DataPacket)，实际接收逻辑根据Esp8285库和网络情况定
    uint8_t received_buffer[sizeof(DataPacket)];
    esp8285_receive_data(received_buffer, sizeof(DataPacket));
    
    DataPacket received_packet;
    memcpy(&received_packet, received_buffer, sizeof(DataPacket));
    
    // 此时就可以使用received_packet中的各个成员变量进行后续处理了
    bool keyboard_status = received_packet.synced_keyboard;
    bool mouse_status = received_packet.synced_mouse;
    bool gamepad_status = received_packet.synced_gamepad;
    uint8_t *matrix_data = received_packet.state_matrix;
    // 后续更多针对接收到的数据的处理逻辑......
}