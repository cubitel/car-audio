/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.0-dev at Thu Dec 20 11:26:33 2018. */

#ifndef PB_CONFIG_CONFIG_PB_H_INCLUDED
#define PB_CONFIG_CONFIG_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _Config_AudioConfig {
    bool has_volume;
    uint32_t volume;
    bool has_muteTuner;
    bool muteTuner;
/* @@protoc_insertion_point(struct:Config_AudioConfig) */
} Config_AudioConfig;

typedef struct _Config_SystemConfig {
    bool has_activeTimeout;
    uint32_t activeTimeout;
    bool has_standbyTimeout;
    uint32_t standbyTimeout;
/* @@protoc_insertion_point(struct:Config_SystemConfig) */
} Config_SystemConfig;

typedef struct _Config_TunerConfig {
    bool has_frequency;
    uint32_t frequency;
/* @@protoc_insertion_point(struct:Config_TunerConfig) */
} Config_TunerConfig;

typedef struct _Config_Config {
    bool has_audio;
    Config_AudioConfig audio;
    bool has_tuner;
    Config_TunerConfig tuner;
    bool has_system;
    Config_SystemConfig system;
/* @@protoc_insertion_point(struct:Config_Config) */
} Config_Config;

/* Default values for struct fields */
extern const uint32_t Config_AudioConfig_volume_default;
extern const uint32_t Config_TunerConfig_frequency_default;
extern const uint32_t Config_SystemConfig_activeTimeout_default;
extern const uint32_t Config_SystemConfig_standbyTimeout_default;

/* Initializer values for message structs */
#define Config_AudioConfig_init_default          {false, 24u, false, 0}
#define Config_TunerConfig_init_default          {false, 102700u}
#define Config_SystemConfig_init_default         {false, 1800u, false, 3600u}
#define Config_Config_init_default               {false, Config_AudioConfig_init_default, false, Config_TunerConfig_init_default, false, Config_SystemConfig_init_default}
#define Config_AudioConfig_init_zero             {false, 0, false, 0}
#define Config_TunerConfig_init_zero             {false, 0}
#define Config_SystemConfig_init_zero            {false, 0, false, 0}
#define Config_Config_init_zero                  {false, Config_AudioConfig_init_zero, false, Config_TunerConfig_init_zero, false, Config_SystemConfig_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define Config_AudioConfig_volume_tag            1
#define Config_AudioConfig_muteTuner_tag         2
#define Config_SystemConfig_activeTimeout_tag    1
#define Config_SystemConfig_standbyTimeout_tag   2
#define Config_TunerConfig_frequency_tag         1
#define Config_Config_audio_tag                  1
#define Config_Config_tuner_tag                  2
#define Config_Config_system_tag                 3

/* Struct field encoding specification for nanopb */
extern const pb_field_t Config_AudioConfig_fields[3];
extern const pb_field_t Config_TunerConfig_fields[2];
extern const pb_field_t Config_SystemConfig_fields[3];
extern const pb_field_t Config_Config_fields[4];

/* Maximum encoded size of messages (where known) */
#define Config_AudioConfig_size                  8
#define Config_TunerConfig_size                  6
#define Config_SystemConfig_size                 12
#define Config_Config_size                       32

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define CONFIG_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif