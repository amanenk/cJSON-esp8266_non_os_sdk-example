#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "cJSON.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map)
    {
    case FLASH_SIZE_4M_MAP_256_256:
        rf_cal_sec = 128 - 5;
        break;

    case FLASH_SIZE_8M_MAP_512_512:
        rf_cal_sec = 256 - 5;
        break;

    case FLASH_SIZE_16M_MAP_512_512:
    case FLASH_SIZE_16M_MAP_1024_1024:
        rf_cal_sec = 512 - 5;
        break;

    case FLASH_SIZE_32M_MAP_512_512:
    case FLASH_SIZE_32M_MAP_1024_1024:
        rf_cal_sec = 1024 - 5;
        break;

    case FLASH_SIZE_64M_MAP_1024_1024:
        rf_cal_sec = 2048 - 5;
        break;
    case FLASH_SIZE_128M_MAP_1024_1024:
        rf_cal_sec = 4096 - 5;
        break;
    default:
        rf_cal_sec = 0;
        break;
    }
    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_init(void)
{
    uart_init(115200, 115200);
    os_printf("SDK version:%s\n", system_get_sdk_version());

    // Disable WiFi
    wifi_set_opmode(NULL_MODE);

    cJSON *json = cJSON_Parse("{\"hello\":\"word\"}");

    const cJSON *name = NULL;
    name = cJSON_GetObjectItem(json, "hello");
    if (name->type == cJSON_String && name->valuestring != NULL)
    {
        os_printf("hello: \"%s\"\n", name->valuestring);
    }
    os_free(json);

    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("james"));

    char *string = cJSON_Print(root);

    os_printf("string: %s\n", string);

    os_free(root);
}
