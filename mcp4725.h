#ifndef MCP4725_H_
#define MCP4725_H_
#include <libpynq.h>
#include <stdint.h>

typedef struct mcp4725_t_ {
  uint8_t addr;
  uint8_t status;
  uint16_t DAC_r;
  uint16_t EEPROM_r;
  iic_index_t iic;
} mcp4725_t;

typedef enum {
  MCP4725_POWER_ON,
  MCP4725_POWER_OFF_1K,
  MCP4725_POWER_OFF_100K,
  MCP4725_POWER_OFF_500K,
  MCP_POWER_MODE_COUNT
} mcp4725_power_mode_t;

#define MCP4725_ADDR 0x60
#define MCP4725_GENERAL_CALL_RESET 0x06
#define MCP4725_GENERAL_CALL_WAKE_UP 0x09
#define MCP4725_EERPOM_RDY 0x80
#define MCP4725_DAC_POWER_MASK 0x06
#define MCP4725_EEPROM_POWER_MASK 0x60

#define mcp4725_is_eeprom_write_completed(mcp4725) ((mcp4725->status & MCP4725_EERPOM_RDY) >> 7)
#define mcp4725_get_dac_power_status(mcp4725) ((mcp4725.status & MCP4725_DAC_POWER_MASK) >> 1)
#define mcp4725_get_eeprom_power_status(mcp4725) ((mcp4725.EEPROM_r & MCP4725_EEPROM_POWER_MASK) >> 5)

/**
 * @param addr an IIC addres of the MCP4725 DAC (usually 0x60, 0x62 or 0x63).
 * @param iic, the IIC on which to connect this device.
 * @brief Initialises device and reserves memory for this device.
 * @return MCP4725_t device structure if succeed
 */
mcp4725_t mcp4725_init(uint8_t addr, iic_index_t iic);

#if 0
/* ============== PYNQ CURRENLTY DOES NOT SUPPORT GENERAL CALLS ============== */
//TODO: Remove from here as it is not device specific thing
/**
 * @param mcp4725 device on which to perform
 * @brief Performs general reset call
 * @return 0 if succeed
 */
uint8_t mcp4725_general_call_reset(mcp4725_t mcp4725);

/**
 * @param mcp4725 device on which to perform
 * @brief Performs general wake up call
 * @return 0 if succeed
 */
uint8_t mcp4725_general_call_wake_up(mcp4725_t mcp4725);
/* ============== PYNQ CURRENLTY DOES NOT SUPPORT GENERAL CALLS ============== */

#endif  // GENERAL CALLS

/**
 * @param mcp4725 device on which to check
 * @brief Checks if writing of EEPROM has completed
 * @return 1 if is completed and 0 if is not
 * @warning before checking user must manually read the status of the device
 * with function mcp4725_read_memory(mcp4725_t *mcp4725)
 */
#define mcp4725_is_eeprom_write_completed(mcp4725) ((mcp4725->status & MCP4725_EERPOM_RDY) >> 7)

/**
 * @param mcp4725 device on which to check
 * @brief Checks power status written in DAC registers
 * @return enum mcp4725_POWER_MODE_t
 * @warning before checking user must manually read the status of the device
 * with function mcp4725_read_memory(mcp4725_t *mcp4725)
 */
#define mcp4725_get_dac_power_status(mcp4725) ((mcp4725.status & MCP4725_DAC_POWER_MASK) >> 1)

/**
 * @param mcp4725 device on which to check
 * @brief Checks power status written in EEPROM
 * @return enum mcp4725_POWER_MODE_t
 * @warning before checking user must manually read the status of the device
 * with function mcp4725_read_memory(mcp4725_t *mcp4725)
 */
#define mcp4725_get_eeprom_power_status(mcp4725) ((mcp4725.EEPROM_r & MCP4725_EEPROM_POWER_MASK) >> 5)

/**
 * @param *mcp4725 pointer to initialised device
 * @brief reads DAC registers and EEPROM from the device (Datasheet page 26)
 * @return 0 if succeed
 */
uint8_t mcp4725_read_memory(mcp4725_t *mcp4725);

/**
 * @param *mcp4725 pointer to initialised device
 * @param value 16bit number which represents level,
 * 4 MSB are ignored, therefore 0 =< value =< 4095 = 0x0fff 
 * @param power_mask mcp4725_POWER_MODE_t setting power mode on device
 * on = 0
 * @brief writes DAC registers of the device (Datasheet page 25)
 * @return 0 if succeed
 */
uint8_t mcp4725_write_dac_register(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask);

/**
 * @param *mcp4725 pointer to initialised device
 * @param value 16bit number which represents level,
 * 4 MSB are ignored, therefore 0 =< value =< 4095 = 0x0fff 
 * @param power_mask mcp4725_POWER_MODE_t setting power mode on device
 * on = 0
 * @brief writes DAC registers and EEPROM of the device (Datasheet page 25)
 * @return 0 if succeed
 */
uint8_t mcp4725_write_dac_and_eeprom_register(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask);

/**
 * @param *mcp4725 pointer to initialised device
 * @param value 16bit number which represents level,
 * 4 MSB are ignored, therefore 0 =< value =< 4095 = 0x0fff 
 * @param power_mask mcp4725_POWER_MODE_t setting power mode on device
 * on = 0
 * @brief writes DAC registers of the device using fast mode(Datasheet page 24)
 * @return 0 if succeed
 */
uint8_t mcp4725_write_fast_mode(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask);

/**
 * @param *mcp4725 pointer to initialised device
 * @param power_mask mcp4725_POWER_MODE_t setting power mode on device
 * @brief writes power mode into DAC registers of the device using fast mode(Datasheet page 24)
 * @return 0 if succeed
 */
uint8_t mcp4725_set_power_mode(mcp4725_t *mcp4725, mcp4725_power_mode_t power_mask);
#endif
