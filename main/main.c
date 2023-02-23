
#include <driver/uart.h>
#include <stdio.h>
#include <string.h>

#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define UART_PORT 1
static const char* TAG = "uart-write";

void app_main(void) {
  uart_config_t config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB,
  };
  ESP_ERROR_CHECK(uart_param_config(UART_PORT, &config));
  ESP_ERROR_CHECK(uart_set_pin(UART_PORT, 17, 18, 20, 21));
  const int uart_buffer_size = (1024 * 2);
  QueueHandle_t uart_queue;
  ESP_ERROR_CHECK(uart_driver_install(UART_PORT, uart_buffer_size,
                                      uart_buffer_size, 10, &uart_queue, 0));
  char* data = "Hello World.\n";
  while (1) {
    int len = uart_write_bytes(UART_PORT, (const char*)data, strlen(data));
    if (len > 0) {
      ESP_LOGI(TAG, "Tx: %s Len: %d", data, len);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}