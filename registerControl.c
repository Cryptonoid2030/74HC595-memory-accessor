#include "pico/stdlib.h"

#define DATA_PIN 0
#define CLOCK_PIN 1
#define LATCH_PIN 2

void latch_data();

void pulse_clock(uint pin) {
    gpio_put(pin, 1);
    sleep_us(1);  // Reduced delay to 10us for better timing
    gpio_put(pin, 0);
    sleep_us(1);  // Reduced delay to 10us for better timing
}

void latch_data() {
    pulse_clock(LATCH_PIN);
}

void shift_out(int write_to_output) {
    pulse_clock(CLOCK_PIN);
    if (write_to_output) latch_data(); // write to output after shifting
}

void write_bit(bool bit_val) {
    gpio_put(DATA_PIN, bit_val);
    sleep_us(1);  // Reduced delay to 10us for better timing
    shift_out(0);
    sleep_us(1);
}

void write_byte(uint byte, int bits) {
    if (!(bits)) {
        return;
    }
    write_bit((byte >> (7 - (bits - 1))) & 0x01);  // mask data for MSB-first
    sleep_us(1);  // Reduced delay to 10us for better timing
    write_byte(byte, bits-1);
}

void write_byte_helper(uint byte) {
    // write exactly 8 bits
    write_byte(byte, 8);
    sleep_us(1);  // Reduced delay to 10us for better timing
    latch_data();
}

int main() {
    sleep_ms(50);
    stdio_init_all();

    gpio_init(DATA_PIN);
    gpio_init(CLOCK_PIN);
    gpio_init(LATCH_PIN);
    gpio_set_dir(DATA_PIN, GPIO_OUT);
    gpio_set_dir(CLOCK_PIN, GPIO_OUT);
    gpio_set_dir(LATCH_PIN, GPIO_OUT);
    sleep_ms(50);  // Initial delay for GPIO setup

    //write_byte_helper(0x00); // Clear bits first
    // write_byte_helper(0xAA); // 1010_1010
    // write_byte_helper(0xF0); // 1111_0000
    //write_byte_helper(0xFF); // 1111_1111
    while (1) {
        write_byte_helper(0b01101000); // h in ascii
        sleep_ms(1000);
        write_byte_helper(0b01100101); // e in ascii
        sleep_ms(1000);
        write_byte_helper(0b01001100); // l in ascii
        sleep_ms(1000);
        write_byte_helper(0b01001100); // l in ascii
        sleep_ms(1000);
        write_byte_helper(0b01101111); // 0 in ascii
        sleep_ms(1000);
    }
}
