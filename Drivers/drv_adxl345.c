#include "i2c_hal.h"
#include "sensor_hal.h"
#include <stddef.h>
#include <stdint.h>

#define ADXL345_DEVID          0x00  
#define ADXL345_POWER_CTL      0x2D  
#define ADXL345_DATA_FORMAT    0x31  
#define ADXL345_DATAX0         0x32  

sensor_status_t adxl345_init(SensorDeviceMeta *self) {
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;
    uint8_t data;

    if (my_bus->read_reg(my_bus->bus_context, self->device_address, ADXL345_DEVID, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }
    if (data != 0xE5) return SENSOR_FAIL;

    data = 0x08; 
    if (my_bus->write_reg(my_bus->bus_context, self->device_address, ADXL345_POWER_CTL, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }

    data = 0x00;
    if (my_bus->write_reg(my_bus->bus_context, self->device_address, ADXL345_DATA_FORMAT, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }

    return SENSOR_OK;
}

sensor_status_t adxl345_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
    uint8_t raw[6] = {0};
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

    for (int i = 0; i < 6; i++) {
        if (my_bus->read_reg(my_bus->bus_context, self->device_address, ADXL345_DATAX0 + i, &raw[i], 1) != I2C_OK) {
            return SENSOR_FAIL;
        }
    }

    int16_t x_raw = (int16_t)((raw[1] << 8) | raw[0]);
    int16_t y_raw = (int16_t)((raw[3] << 8) | raw[2]);
    int16_t z_raw = (int16_t)((raw[5] << 8) | raw[4]);
    
    SensorDataVec3 *self_data = (SensorDataVec3 *)out->sensor_data;

    self_data->x = (float)x_raw * 0.0039f;
    self_data->y = (float)y_raw * 0.0039f;
    self_data->z = (float)z_raw * 0.0039f;

    return SENSOR_OK;
}
static const SensorVTable ADXL345_VTable = {
    .init    = adxl345_init,
    .read    = adxl345_read,
    .trigger = NULL,
    .sleep   = NULL 
};

void adxl345_set(SensorObject *self, i2c_bus_t *bus, uint8_t device_address,void *data_buffer) {
    self->sensor_meta.device_address = device_address;
    self->sensor_meta.type = SENSOR_TYPE_VEC3; 
    self->sensor_meta.io_interface = bus; 
    self->sensor_meta.channel = 0;                   
    self->sensor_packet.sensor_data = data_buffer;                   
    self->vtable = &ADXL345_VTable;
}