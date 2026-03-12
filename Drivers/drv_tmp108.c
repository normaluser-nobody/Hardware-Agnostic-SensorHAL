#include "i2c_hal.h"
#include "sensor_hal.h"
#include <stddef.h>
#include <stdio.h>

sensor_status_t tmp108_init(SensorDeviceMeta *self) {
    return SENSOR_OK;
}

sensor_status_t tmp108_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
    uint8_t raw_data[2] = {0};
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

    if (my_bus->read_reg(my_bus->bus_context, self->device_address, 0x00, raw_data, 2) != I2C_OK) {
        return SENSOR_FAIL;
    }

    int16_t raw_combined = (raw_data[0] << 8) | raw_data[1];

    
    float temp_c = (float)raw_combined / 256.0f; 
    SensorDataScaler *self_data = (SensorDataScaler *)out->sensor_data;

    self_data->scalar = temp_c;
    return SENSOR_OK;
}

static const SensorVTable TMP108_VTable = {
    .init    = tmp108_init,
    .read    = tmp108_read,
    .trigger = NULL,
    .sleep   = NULL 
};
void tmp108_set(SensorObject *self, i2c_bus_t *bus, uint8_t device_address, void *data_buffer) {
    self->sensor_meta.device_address = device_address;
    self->sensor_meta.type = SENSOR_TYPE_SCALAR;
    self->sensor_meta.io_interface = bus; 
    self->sensor_meta.io_interface = bus; 
    self->sensor_packet.sensor_data = data_buffer;                   
    self->vtable=&TMP108_VTable;
}