
#ifndef STRING_PARSER__H
#define STRING_PARSER__H

#include "queue.h"

typedef struct _ctd_struct {
    float temperature;      /**< 温度 */
    float conductivity;     /**< 电导率（用于计算盐度） */
    float pressure;         /**< 压力（用于计算深度） */
#if 0
    float pressure_sensor_tmperature;   /**< 压力传感器的温度 */
#endif
#if 1
    float oxygen;           /**< 含氧量 */
    float salinity;         /**< 盐度 */
    float sound_velocity;   /**< 声速 */
    float specific_conductivity;        /**< 电导率 */
    long  seconds;          /**< 时间秒数 */
#endif
//    float depth;
//    float local_density;
} CTD_S;

typedef enum _ctd_enum {
    TEMPERATURE = 0,
    CONDUCTIVITY,
    PRESSURE,
    PRESS_SENSOR_TEPE,
    OXYGEN,
    SALINITY,
    SOUND_VELOCITY,
    SPEC_CONDUCT,
    SECONDS
} CTD_U;

typedef struct _ctdQueue {
    Queue_Elem elem;
    int payload;
} CTD_PACKET_T;

int string_token(const char *i_str, unsigned int i_len, char delim,
                 char *o_buf, unsigned int o_buflen);

#endif /* STRING_PARSER__H */
