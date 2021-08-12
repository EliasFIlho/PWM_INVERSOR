#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#define LED 2
#define LEDP 15
//#define LEDC_TEST_FADE_TIME    (3000)
#define HIGH_M (LEDC_HIGH_SPEED_MODE)
#define FADE_TIME_VARIATION 4.2

TaskHandle_t task1handle;
TaskHandle_t task2handle;
//TimerHandle_t xtimer;
SemaphoreHandle_t mutex;

void PWM1_CONTROL(void *pvParameters);
void PWM2_CONTROL(void *pvParameters);
//void Tcbf(TimerHandle_t xTimer);

const ledc_timer_config_t timer_config = {
    .speed_mode = HIGH_M,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK,
};

const ledc_timer_config_t timer_config_2 = {
    .speed_mode = HIGH_M,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_1,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK,
};

const ledc_channel_config_t PWM1 = {
    .gpio_num = LEDP,                   
    .speed_mode = HIGH_M,         
    .channel = LEDC_CHANNEL_0,         
    .intr_type = 0,     
    .timer_sel = LEDC_TIMER_0,         
    .duty = 0,                  
    .hpoint = 0, 

};

const ledc_channel_config_t PWM2 = {
    .gpio_num = LED,                   
    .speed_mode = HIGH_M,         
    .channel = LEDC_CHANNEL_1,         
    .intr_type = 0,     
    .timer_sel = LEDC_TIMER_1,         
    .duty = 0,                  
    .hpoint = 0, 

};



void app_main(){

mutex = xSemaphoreCreateMutex();
//xtimer = xTimerCreate("Timer1",pdMS_TO_TICKS(1000),pdFALSE,0,Tcbf);

xTaskCreate(PWM1_CONTROL,"TaskPWM1",configMINIMAL_STACK_SIZE+1024,NULL,2,&task1handle);
xTaskCreate(PWM2_CONTROL,"TaskPWM2",configMINIMAL_STACK_SIZE+1024,NULL,1,&task2handle);




ledc_timer_config(&timer_config);
ledc_timer_config(&timer_config_2);
ledc_channel_config(&PWM1);
ledc_channel_config(&PWM2);

ledc_fade_func_install(0);

//xTimerStart(xtimer,0);



}


void PWM1_CONTROL(void *pvParameters){
    while(1){
        xSemaphoreTake(mutex,portMAX_DELAY);
        ledc_set_fade_time_and_start(PWM1.speed_mode,PWM1.channel,244,pdMS_TO_TICKS(FADE_TIME_VARIATION),LEDC_FADE_WAIT_DONE);
       // vTaskDelay(pdMS_TO_TICKS(1000));
        ledc_set_fade_time_and_start(PWM1.speed_mode,PWM1.channel,0,pdMS_TO_TICKS(FADE_TIME_VARIATION),LEDC_FADE_WAIT_DONE);
        //vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void PWM2_CONTROL(void *pvParameters){
    while(1){
        xSemaphoreTake(mutex,portMAX_DELAY);
        ledc_set_fade_time_and_start(PWM2.speed_mode,PWM2.channel,244,pdMS_TO_TICKS(FADE_TIME_VARIATION),LEDC_FADE_WAIT_DONE);
        //vTaskDelay(pdMS_TO_TICKS(1000));
        ledc_set_fade_time_and_start(PWM2.speed_mode,PWM2.channel,0,pdMS_TO_TICKS(FADE_TIME_VARIATION),LEDC_FADE_WAIT_DONE);
        //vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
