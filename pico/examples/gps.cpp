#include <stdio.h>

using namespace std;

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "tusb.h" // wait for USB

#include <gcigps.hpp>

#define LED_PIN 25
#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 8
#define UART_RX_PIN 9

int main() {
  stdio_init_all();

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  gci::GPS gps;

  uint baud = uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_fifo_enabled(UART_ID, true);
  uart_set_translate_crlf(UART_ID, true);

  if (uart_is_enabled(UART_ID)) printf("/-- UART is READY at %u baud --/\n", baud);
  else printf("*** UART init FAILED ***\n");

  // uart_puts(UART_ID, GCI_RMCGGA);
  // uart_puts(UART_ID, GCI_UPDATE_1HZ);
  // uart_puts(UART_ID, GCI_NOANTENNA);

  uart_write_blocking(UART_ID, GCI_RMCGGA, sizeof(GCI_RMCGGA));
  // uart_write_blocking(UART_ID, reinterpret_cast<const uint8_t*>(GCI_UPDATE_1HZ), strlen(GCI_UPDATE_1HZ));
  uart_write_blocking(UART_ID, GCI_UPDATE_1HZ, sizeof(GCI_UPDATE_1HZ));
  uart_write_blocking(UART_ID, GCI_NOANTENNA, sizeof(GCI_NOANTENNA));

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  puts("/// GPS START ///\n");

  while (1) {
    // gpio_put(LED_PIN, 0);
    // sleep_ms(500);
    // gpio_put(LED_PIN, 1);
    // sleep_ms(500);

    // puts(".");

    gga_t gga;
    rmc_t rmc;

    // int avail = gpsser.available();
    bool ok = false;
    // while (avail--) {
    while (uart_is_readable(UART_ID)) {
    // for (int i=32; i>0; --i) {
      // int c = gpsser.read();
      // char c = uart_getc(UART_ID);
      uint8_t c;
      uart_read_blocking(UART_ID, &c, 1);
      char cc = static_cast<char>(c);
      // if (c == 13) cc = '>';
      // if (c == 10) cc = '<';
      printf("%c", cc);
      ok = gps.read(cc);
      if (ok) break;
    }

    if (!ok) continue;

    gci::GpsID id = gps.get_id();
    if (id == gci::GpsID::GGA) {
      ok = gps.get_msg(gga);
      if (ok) printf("GGA lat: %f lon: %f\n", gga.lat, gga.lon);
      else printf("Bad parsing\n");
    }
    // else if (id == gci::GpsID::RMC) {
    //   ok = gps.get_msg(rmc);
    //   if (!ok) printf("RMC lat: %f lon: %f\n", rmc.lat, rmc.lon);
    // }
  }
}