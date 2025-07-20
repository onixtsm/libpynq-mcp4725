bool iic_write_buffer(const iic_index_t iic, const uint8_t addr, uint8_t *data, uint16_t data_length) {
  if (!(iic >= IIC0 && iic < NUM_IICS)) {
    pynq_error("invalid IIC %d, must be 0..%d-1\n", iic, NUM_IICS);
  }
  if (iic_handles[iic].ptr == NULL) {
    pynq_error("IIC%d has not been initialized.\n", iic);
  }
  uint8_t ByteCount = XIic_Send((UINTPTR)iic_handles[iic].ptr, addr, data, data_length, XIIC_STOP);
  return ByteCount != data_length;
}

bool iic_read_buffer(const iic_index_t iic, const uint8_t addr, uint8_t *data, uint16_t data_length) {
  if (!(iic >= IIC0 && iic < NUM_IICS)) {
    pynq_error("invalid IIC %d, must be 0..%d-1\n", iic, NUM_IICS);
  }
  if (iic_handles[iic].ptr == NULL) {
    pynq_error("IIC%d has not been initialized.\n", iic);
  }

  uint8_t ByteCount = XIic_Recv((UINTPTR)iic_handles[iic].ptr, addr, data, data_length, XIIC_STOP);

  return ByteCount != data_length;
}

