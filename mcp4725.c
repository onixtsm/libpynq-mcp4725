#include "mcp4725.h"


static uint8_t iic_send_byte(iic_index_t iic, uint8_t addr, uint8_t data) {
  return (uint8_t)iic_write_buffer(iic, addr, &data, 1);
}

mcp4725_t mcp4725_init(uint8_t addr, iic_index_t iic) {
  mcp4725_t mcp4725 = {0};
  mcp4725.addr = addr;
  mcp4725.iic = iic;
  return mcp4725;
}

uint8_t mcp4725_general_call_reset(mcp4725_t mcp4725) {
  return iic_send_byte(mcp4725.iic, 0x00, MCP4725_GENERAL_CALL_RESET);
}

uint8_t mcp4725_general_call_wake_up(mcp4725_t mcp4725) {
  return iic_send_byte(mcp4725.iic, mcp4725.addr, MCP4725_GENERAL_CALL_WAKE_UP);
}

uint8_t mcp4725_read_memory(mcp4725_t *mcp4725) {
  uint8_t data[5] = {0};  // I love Julia
  uint8_t error = iic_read_buffer(mcp4725->iic, mcp4725->addr, data, 5);
  if (error) {
    pynq_error("Could not read memory of IIC(%d) device at %04x\n", mcp4725->iic, mcp4725->addr);
    return error;
  }
  mcp4725->status = data[0];
  mcp4725->DAC_r = (data[1] << 8) | data[2];
  mcp4725->EEPROM_r = (data[3] << 8) | data[4];
  return error;
}

uint8_t mcp4725_write_dac_register(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask) {
  value <<= 4;
  uint8_t data[3] = {0x40 | (power_mask << 1), ((uint8_t *)&value)[1], ((uint8_t *)&value)[0]};

  uint8_t error = iic_write_buffer(mcp4725->iic, mcp4725->addr, data, 3);
  if (error) {
    return error;
  }
  mcp4725->DAC_r = value;
  mcp4725->status = power_mask << 1;
  return error;
}

uint8_t mcp4725_write_dac_and_eeprom_register(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask) {
  value <<= 4;
  uint8_t data[3] = {0x60 | (power_mask << 1), ((uint8_t *)&value)[1], ((uint8_t *)&value)[0]};

  uint8_t error = iic_write_buffer(mcp4725->iic, mcp4725->addr, data, 3);
  if (error) {
    return error;
  }
#define MAX_ERRORS 5
  uint8_t error_count = 0;
  for (;;) {
    error = mcp4725_read_memory(mcp4725);
    if (error) {
      error_count++;
    }
    if (error_count >= MAX_ERRORS) {
      return error;
    }
    if (mcp4725_is_eeprom_write_completed(mcp4725)) {
      printf("Done\n");
      break;
    }
    sleep_msec(100);
  }
  mcp4725->DAC_r = value;
  mcp4725->EEPROM_r = value;
  mcp4725->status = power_mask << 1;
  return error;
}

uint8_t mcp4725_write_fast_mode(mcp4725_t *mcp4725, uint16_t value, mcp4725_power_mode_t power_mask) {
  value &= 0x0fff;  // Remove power bits
  uint8_t data[2] = {(power_mask << 4) | ((uint8_t *)&value)[1], ((uint8_t *)&value)[0]};

  uint8_t error = iic_write_buffer(mcp4725->iic, mcp4725->addr, data, 2);
  if (error) {
    return error;
  }
  mcp4725->DAC_r = value;
  mcp4725->status = power_mask << 1;
  return error;
}

uint8_t mcp4725_set_power_mode(mcp4725_t *mcp4725, mcp4725_power_mode_t power_mask) {
  uint16_t value = mcp4725->DAC_r;
  if (value > 4095) {
    value = value << 4;
  }
  return mcp4725_write_fast_mode(mcp4725, value, power_mask);
}
