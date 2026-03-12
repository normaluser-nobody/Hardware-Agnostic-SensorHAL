#ifndef SENSOR_HAL_H
#define SENSOR_HAL_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SENSOR_TYPE_SCALAR,
    SENSOR_TYPE_VEC2,
    SENSOR_TYPE_VEC3,
} SensorDataType;

typedef struct {
    float scalar;
} SensorDataScaler;

typedef struct {
    float x, y;
} SensorDataVec2;

typedef struct {
    float x, y, z;
} SensorDataVec3;

typedef struct {
    uint64_t timestamp_us;
    void *sensor_data;
} SensorDevicePacket;

typedef enum {
    SENSOR_OK = 0,
    SENSOR_FAIL
} sensor_status_t;

typedef struct {
    uint8_t device_address;
    uint8_t channel;   
    SensorDataType type;     
    void *io_interface;     
} SensorDeviceMeta;

typedef struct {
    sensor_status_t (*init)(SensorDeviceMeta *self);
    sensor_status_t (*read)(SensorDeviceMeta *self, SensorDevicePacket *out);   
    sensor_status_t (*trigger)(SensorDeviceMeta *self);
    sensor_status_t (*sleep)(SensorDeviceMeta *self); 
} SensorVTable;

typedef struct {
    SensorDeviceMeta sensor_meta;
    const SensorVTable *vtable; 
    SensorDevicePacket sensor_packet;
} SensorObject;

#endif