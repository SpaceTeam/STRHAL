#include "blmb_settings.h"
#include "flash.h"
#include "main.h"

#define BLMB_SETTINGS_STATE_ADDRESS 0
#define BLMB_SETTINGS_FLASH_ADDRESS 1


#define BLMB_DEFAULT_STARTPOINT		0
#define BLMB_DEFAULT_ENDPOINT 		UINT16_MAX

static void BlmbSettings_StoreDefault(void)
{
	BLMB_Settings_t default_settings =
	{ 0 };
	default_settings.startpoint = BLMB_DEFAULT_STARTPOINT;
	default_settings.endpoint = BLMB_DEFAULT_ENDPOINT;
	default_settings.max_accel = UINT16_MAX;
	default_settings.max_speed = UINT16_MAX;
	default_settings.max_torque = UINT16_MAX;

	BlmbSettings_Store(&default_settings);

}

void BlmbSettings_Load(BLMB_Settings_t *settings)
{
	if (Flash_GetByteFromAddress(BLMB_SETTINGS_STATE_ADDRESS))
		BlmbSettings_StoreDefault();

	Flash_GetBlockStartingFromAddress(BLMB_SETTINGS_FLASH_ADDRESS, (uint8_t*) settings, sizeof(BLMB_Settings_t));

}
void BlmbSettings_Store(BLMB_Settings_t *settings)
{
	Flash_EraseSector(BLMB_SETTINGS_FLASH_ADDRESS);
	Flash_SetBlockStartingAtAddress(BLMB_SETTINGS_FLASH_ADDRESS, (uint8_t*) settings, sizeof(BLMB_Settings_t));
	Flash_SetByteAtAddress(BLMB_SETTINGS_STATE_ADDRESS,0);
}
