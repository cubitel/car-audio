/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.4.0-dev at Thu Feb  7 16:23:43 2019. */

#include "config.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const uint32_t Config_AudioConfig_volume_default = 24u;
const uint32_t Config_TunerConfig_frequency_default = 102700u;
const uint32_t Config_SystemConfig_activeTimeout_default = 1800u;
const uint32_t Config_SystemConfig_standbyTimeout_default = 3600u;
const uint32_t Config_SystemConfig_carType_default = 2u;


const pb_field_t Config_AudioConfig_fields[3] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, Config_AudioConfig, volume, volume, &Config_AudioConfig_volume_default),
    PB_FIELD(  2, BOOL    , OPTIONAL, STATIC  , OTHER, Config_AudioConfig, muteTuner, volume, 0),
    PB_LAST_FIELD
};

const pb_field_t Config_TunerConfig_fields[2] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, Config_TunerConfig, frequency, frequency, &Config_TunerConfig_frequency_default),
    PB_LAST_FIELD
};

const pb_field_t Config_SystemConfig_fields[4] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, Config_SystemConfig, activeTimeout, activeTimeout, &Config_SystemConfig_activeTimeout_default),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, Config_SystemConfig, standbyTimeout, activeTimeout, &Config_SystemConfig_standbyTimeout_default),
    PB_FIELD(  3, UINT32  , OPTIONAL, STATIC  , OTHER, Config_SystemConfig, carType, standbyTimeout, &Config_SystemConfig_carType_default),
    PB_LAST_FIELD
};

const pb_field_t Config_Config_fields[4] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, Config_Config, audio, audio, &Config_AudioConfig_fields),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, Config_Config, tuner, audio, &Config_TunerConfig_fields),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, Config_Config, system, tuner, &Config_SystemConfig_fields),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(Config_Config, audio) < 65536 && pb_membersize(Config_Config, tuner) < 65536 && pb_membersize(Config_Config, system) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_Config_AudioConfig_Config_TunerConfig_Config_SystemConfig_Config_Config)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(Config_Config, audio) < 256 && pb_membersize(Config_Config, tuner) < 256 && pb_membersize(Config_Config, system) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_Config_AudioConfig_Config_TunerConfig_Config_SystemConfig_Config_Config)
#endif


/* @@protoc_insertion_point(eof) */
