/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.0-dev at Mon Dec 31 17:21:36 2018. */

#ifndef PB_MAIN_MAIN_PB_H_INCLUDED
#define PB_MAIN_MAIN_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _Main_AudioControlRequest {
    bool has_volume;
    int32_t volume;
    bool has_muteTuner;
    bool muteTuner;
/* @@protoc_insertion_point(struct:Main_AudioControlRequest) */
} Main_AudioControlRequest;

typedef struct _Main_AudioStatus {
    bool has_volume;
    int32_t volume;
    bool has_muteTuner;
    bool muteTuner;
/* @@protoc_insertion_point(struct:Main_AudioStatus) */
} Main_AudioStatus;

typedef struct _Main_EngineStatus {
    bool has_rpm;
    int32_t rpm;
/* @@protoc_insertion_point(struct:Main_EngineStatus) */
} Main_EngineStatus;

typedef struct _Main_PropertyValue {
    bool has_propertyId;
    int32_t propertyId;
    pb_size_t which_value;
    union {
        int32_t intValue;
        char stringValue[256];
    } value;
/* @@protoc_insertion_point(struct:Main_PropertyValue) */
} Main_PropertyValue;

typedef struct _Main_SystemStatus {
    bool has_active;
    bool active;
    bool has_bootCause;
    int32_t bootCause;
    bool has_faultAddress;
    uint32_t faultAddress;
/* @@protoc_insertion_point(struct:Main_SystemStatus) */
} Main_SystemStatus;

typedef struct _Main_TunerControlRequest {
    bool has_frequency;
    int32_t frequency;
    bool has_stationId;
    int32_t stationId;
/* @@protoc_insertion_point(struct:Main_TunerControlRequest) */
} Main_TunerControlRequest;

typedef struct _Main_TunerStatus {
    bool has_frequency;
    int32_t frequency;
    bool has_rssi;
    int32_t rssi;
    bool has_snr;
    int32_t snr;
    bool has_rdsPsName;
    char rdsPsName[10];
    bool has_rdsGroups;
    uint32_t rdsGroups;
    bool has_stereo;
    bool stereo;
/* @@protoc_insertion_point(struct:Main_TunerStatus) */
} Main_TunerStatus;

typedef struct _Main_RPCRequest {
    bool has_requestId;
    int32_t requestId;
    pb_size_t which_request;
    union {
        Main_AudioControlRequest audio;
        Main_TunerControlRequest tuner;
        Main_PropertyValue getProperty;
        Main_PropertyValue setProperty;
    } request;
/* @@protoc_insertion_point(struct:Main_RPCRequest) */
} Main_RPCRequest;

typedef struct _Main_RPCResponse {
    bool has_requestId;
    int32_t requestId;
    pb_size_t which_response;
    union {
        Main_PropertyValue getProperty;
    } response;
/* @@protoc_insertion_point(struct:Main_RPCResponse) */
} Main_RPCResponse;

typedef struct _Main_StatusUpdate {
    bool has_audio;
    Main_AudioStatus audio;
    bool has_tuner;
    Main_TunerStatus tuner;
    bool has_system;
    Main_SystemStatus system;
    bool has_engine;
    Main_EngineStatus engine;
/* @@protoc_insertion_point(struct:Main_StatusUpdate) */
} Main_StatusUpdate;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Main_AudioStatus_init_default            {false, 0, false, 0}
#define Main_TunerStatus_init_default            {false, 0, false, 0, false, 0, false, "", false, 0, false, 0}
#define Main_SystemStatus_init_default           {false, 0, false, 0, false, 0}
#define Main_EngineStatus_init_default           {false, 0}
#define Main_StatusUpdate_init_default           {false, Main_AudioStatus_init_default, false, Main_TunerStatus_init_default, false, Main_SystemStatus_init_default, false, Main_EngineStatus_init_default}
#define Main_AudioControlRequest_init_default    {false, 0, false, 0}
#define Main_TunerControlRequest_init_default    {false, 0, false, 0}
#define Main_PropertyValue_init_default          {false, 0, 0, {0}}
#define Main_RPCRequest_init_default             {false, 0, 0, {Main_AudioControlRequest_init_default}}
#define Main_RPCResponse_init_default            {false, 0, 0, {Main_PropertyValue_init_default}}
#define Main_AudioStatus_init_zero               {false, 0, false, 0}
#define Main_TunerStatus_init_zero               {false, 0, false, 0, false, 0, false, "", false, 0, false, 0}
#define Main_SystemStatus_init_zero              {false, 0, false, 0, false, 0}
#define Main_EngineStatus_init_zero              {false, 0}
#define Main_StatusUpdate_init_zero              {false, Main_AudioStatus_init_zero, false, Main_TunerStatus_init_zero, false, Main_SystemStatus_init_zero, false, Main_EngineStatus_init_zero}
#define Main_AudioControlRequest_init_zero       {false, 0, false, 0}
#define Main_TunerControlRequest_init_zero       {false, 0, false, 0}
#define Main_PropertyValue_init_zero             {false, 0, 0, {0}}
#define Main_RPCRequest_init_zero                {false, 0, 0, {Main_AudioControlRequest_init_zero}}
#define Main_RPCResponse_init_zero               {false, 0, 0, {Main_PropertyValue_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define Main_AudioControlRequest_volume_tag      1
#define Main_AudioControlRequest_muteTuner_tag   2
#define Main_AudioStatus_volume_tag              1
#define Main_AudioStatus_muteTuner_tag           2
#define Main_EngineStatus_rpm_tag                1
#define Main_PropertyValue_intValue_tag          101
#define Main_PropertyValue_stringValue_tag       102
#define Main_PropertyValue_propertyId_tag        1
#define Main_SystemStatus_active_tag             1
#define Main_SystemStatus_bootCause_tag          2
#define Main_SystemStatus_faultAddress_tag       3
#define Main_TunerControlRequest_frequency_tag   1
#define Main_TunerControlRequest_stationId_tag   2
#define Main_TunerStatus_frequency_tag           1
#define Main_TunerStatus_rssi_tag                2
#define Main_TunerStatus_snr_tag                 3
#define Main_TunerStatus_rdsPsName_tag           4
#define Main_TunerStatus_rdsGroups_tag           5
#define Main_TunerStatus_stereo_tag              6
#define Main_RPCRequest_audio_tag                101
#define Main_RPCRequest_tuner_tag                102
#define Main_RPCRequest_getProperty_tag          103
#define Main_RPCRequest_setProperty_tag          104
#define Main_RPCRequest_requestId_tag            1
#define Main_RPCResponse_getProperty_tag         103
#define Main_RPCResponse_requestId_tag           1
#define Main_StatusUpdate_audio_tag              1
#define Main_StatusUpdate_tuner_tag              2
#define Main_StatusUpdate_system_tag             3
#define Main_StatusUpdate_engine_tag             4

/* Struct field encoding specification for nanopb */
extern const pb_field_t Main_AudioStatus_fields[3];
extern const pb_field_t Main_TunerStatus_fields[7];
extern const pb_field_t Main_SystemStatus_fields[4];
extern const pb_field_t Main_EngineStatus_fields[2];
extern const pb_field_t Main_StatusUpdate_fields[5];
extern const pb_field_t Main_AudioControlRequest_fields[3];
extern const pb_field_t Main_TunerControlRequest_fields[3];
extern const pb_field_t Main_PropertyValue_fields[4];
extern const pb_field_t Main_RPCRequest_fields[6];
extern const pb_field_t Main_RPCResponse_fields[3];

/* Maximum encoded size of messages (where known) */
#define Main_AudioStatus_size                    13
#define Main_TunerStatus_size                    53
#define Main_SystemStatus_size                   19
#define Main_EngineStatus_size                   11
#define Main_StatusUpdate_size                   104
#define Main_AudioControlRequest_size            13
#define Main_TunerControlRequest_size            22
#define Main_PropertyValue_size                  271
#define Main_RPCRequest_size                     286
#define Main_RPCResponse_size                    286

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define MAIN_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
