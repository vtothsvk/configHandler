#include "configHandler.h"

esp_err_t configInit(logger_config_t* config){
    //nvs init
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //read saved config
    ret = configRead(config);

    return ret;
}

esp_err_t configRead(logger_config_t* config) {
    nvs_handle_t my_handle;

    //open nvs namespace
    esp_err_t ret = nvs_open(CONFIG_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    
    if (ret != ESP_OK) {
        printf("NVS open error %d\r\n", ret);
        return ret;
    } else {
        //get size needed for blob storage
        size_t bloblen;
        ret = nvs_get_blob(my_handle, CONFIG_KEY, NULL, &bloblen);

        if (ret == ESP_ERR_NVS_NOT_FOUND){
            printf("No config found\r\n");
            nvs_close(my_handle);
            return ret;
        }//if (ret == ESP_ERR_NVS_NOT_FOUND)

        //read saved config
        uint8_t buffer[bloblen + 1];
        ret = nvs_get_blob(my_handle, CONFIG_KEY, (uint8_t*)&buffer[0], &bloblen);
        switch (ret) {
            case ESP_OK:
                *config = *(logger_config_t*)&buffer[0]; //output the read buffer
                break;
            
            default:
                printf("Error (%s) reading!\n", esp_err_to_name(ret));
                nvs_close(my_handle);
                return ret;
                break;
        }//switch (ret)

        //close nvs namespace
        nvs_close(my_handle);
    }//if (ret != ESP_OK) 

    return ret;
}

esp_err_t configWrite(logger_config_t config){
    nvs_handle_t my_handle;
    uint8_t buffer[sizeof(config)];
    memcpy(&buffer[0], &config, sizeof(config));

    //open nvs namespace
    esp_err_t ret = nvs_open(CONFIG_NVS_NAMESPACE, NVS_READWRITE, &my_handle);

    if (ret != ESP_OK) {
        printf("NVS open error %d\r\n", ret);
        return ret;
    } else {
        ret = nvs_set_blob(my_handle, CONFIG_KEY, &config, sizeof(config));
        
        if (ret) {
            nvs_close(my_handle);
            return ret;
        }//if (ret)

        ret = nvs_commit(my_handle);
        nvs_close(my_handle);
    }//if (ret != ESP_OK) 

    return ret;
}