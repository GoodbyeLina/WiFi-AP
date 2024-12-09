#ifndef WIFI_AP_H
#define WIFI_AP_H

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    bool synced_keyboard;
    bool synced_mouse;
    bool synced_gamepad;
    uint8_t state_matrix[256];
} DataPacket;

bool send_at_command(const char *cmd, const char *act)

void configure_esp8285();

void send_data_to_esp8285(uint8_t *data, int data_size);

int receive_data_from_esp8285(uint8_t *buffer, int buffer_size);

void sendDataOverWiFi(DataPacket packet);

void receiveDataFromWiFi();

#endif




