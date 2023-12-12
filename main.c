#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#include "ssd1306/ssd1306.h"
    

// init OLED Using I2C 
void setup_SSD1306(void) {
    i2c_init(i2c1, 400000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
}

void display_data(const char *data) {
    char core[] = "Core";
    ssd1306_t disp; 
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1); // init OLED
    ssd1306_clear(&disp); // CLear all befor print 
    // print square + center line + content
    ssd1306_draw_empty_square(&disp,0,0,126,62); 
    ssd1306_draw_string(&disp, 40, 10, 2.8, core);
    ssd1306_draw_string(&disp, 30, 40, 2.8, data);
    ssd1306_draw_line(&disp, 0,32,126,32);
    ssd1306_show(&disp); // show 
}

int main() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
    setup_SSD1306();
    char src[4] = " C";
    char data[] = "12.1 C";
    while (1)
    {
        uint16_t raw = adc_read();
        const float conversion_factor = 3.3f / (1<<12);
        float result = 27 - ((raw * conversion_factor) -0.706)/0.001721;
        snprintf(data, 5, "%f", result);
        strncat(data, src, 2);
        printf("Temp = %.1f C\n", result);
        display_data(data);
        sleep_ms(1000);
    }
    return 0;
}
