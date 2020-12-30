#ifndef configHandler_h
#define configHandler_h

#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define CONFIG_NVS_NAMESPACE   "LOGGER_CONFIG"
#define CONFIG_KEY             "MY_CONFIG"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct logger_config{
    char* id;
    uint16_t interval;
    uint16_t samples;
}logger_config_t;

esp_err_t configInit(logger_config_t* config);
esp_err_t configWrite(logger_config_t config);
esp_err_t configRead(logger_config_t* config);

#ifdef __cplusplus
}
#endif
#endif