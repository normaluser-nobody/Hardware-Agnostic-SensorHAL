#include "i2c_hal.h"
#include "stm32f1xx_hal.h" 
i2c_status_t stm32_i2c_read(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)bus_context;

    uint16_t st_address = (uint16_t)(dev_addr << 1);

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, st_address, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);

    if (status == HAL_OK)      return I2C_OK;
    if (status == HAL_BUSY)    return I2C_ERR_BUSY;
    if (status == HAL_TIMEOUT) return I2C_ERR_TIMEOUT;
    
    return I2C_ERR_NACK; 
}

i2c_status_t stm32_i2c_write(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)bus_context;
    uint16_t st_address = (uint16_t)(dev_addr << 1);

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, st_address, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);

    if (status == HAL_OK)      return I2C_OK;
    if (status == HAL_BUSY)    return I2C_ERR_BUSY;
    if (status == HAL_TIMEOUT) return I2C_ERR_TIMEOUT;
    
    return I2C_ERR_NACK;
}
i2c_bus_t stm32_create_i2c_bus(I2C_HandleTypeDef *hardware_handle) {
    i2c_bus_t my_bus;
    
    my_bus.bus_context = hardware_handle;
    
    my_bus.read_reg  = stm32_i2c_read;
    my_bus.write_reg = stm32_i2c_write;

    return my_bus;
}