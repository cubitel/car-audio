/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.4.0-dev at Thu Dec 20 16:53:14 2018. */

#include "main.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t Main_AudioStatus_fields[3] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_AudioStatus, volume, volume, 0),
    PB_FIELD(  2, BOOL    , OPTIONAL, STATIC  , OTHER, Main_AudioStatus, muteTuner, volume, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_TunerStatus_fields[7] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_TunerStatus, frequency, frequency, 0),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, Main_TunerStatus, rssi, frequency, 0),
    PB_FIELD(  3, INT32   , OPTIONAL, STATIC  , OTHER, Main_TunerStatus, snr, rssi, 0),
    PB_FIELD(  4, STRING  , OPTIONAL, STATIC  , OTHER, Main_TunerStatus, rdsPsName, snr, 0),
    PB_FIELD(  5, UINT32  , OPTIONAL, STATIC  , OTHER, Main_TunerStatus, rdsGroups, rdsPsName, 0),
    PB_FIELD(  6, BOOL    , OPTIONAL, STATIC  , OTHER, Main_TunerStatus, stereo, rdsGroups, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_SystemStatus_fields[2] = {
    PB_FIELD(  1, BOOL    , OPTIONAL, STATIC  , FIRST, Main_SystemStatus, active, active, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_EngineStatus_fields[2] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_EngineStatus, rpm, rpm, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_StatusUpdate_fields[5] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, Main_StatusUpdate, audio, audio, &Main_AudioStatus_fields),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, Main_StatusUpdate, tuner, audio, &Main_TunerStatus_fields),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, Main_StatusUpdate, system, tuner, &Main_SystemStatus_fields),
    PB_FIELD(  4, MESSAGE , OPTIONAL, STATIC  , OTHER, Main_StatusUpdate, engine, system, &Main_EngineStatus_fields),
    PB_LAST_FIELD
};

const pb_field_t Main_AudioControlRequest_fields[3] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_AudioControlRequest, volume, volume, 0),
    PB_FIELD(  2, BOOL    , OPTIONAL, STATIC  , OTHER, Main_AudioControlRequest, muteTuner, volume, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_TunerControlRequest_fields[3] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_TunerControlRequest, frequency, frequency, 0),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, Main_TunerControlRequest, stationId, frequency, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_PropertyValue_fields[4] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_PropertyValue, propertyId, propertyId, 0),
    PB_ONEOF_FIELD(value, 101, INT32   , ONEOF, STATIC  , OTHER, Main_PropertyValue, intValue, propertyId, 0),
    PB_ONEOF_FIELD(value, 102, STRING  , ONEOF, STATIC  , UNION, Main_PropertyValue, stringValue, propertyId, 0),
    PB_LAST_FIELD
};

const pb_field_t Main_RPCRequest_fields[6] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_RPCRequest, requestId, requestId, 0),
    PB_ONEOF_FIELD(request, 101, MESSAGE , ONEOF, STATIC  , OTHER, Main_RPCRequest, audio, requestId, &Main_AudioControlRequest_fields),
    PB_ONEOF_FIELD(request, 102, MESSAGE , ONEOF, STATIC  , UNION, Main_RPCRequest, tuner, requestId, &Main_TunerControlRequest_fields),
    PB_ONEOF_FIELD(request, 103, MESSAGE , ONEOF, STATIC  , UNION, Main_RPCRequest, getProperty, requestId, &Main_PropertyValue_fields),
    PB_ONEOF_FIELD(request, 104, MESSAGE , ONEOF, STATIC  , UNION, Main_RPCRequest, setProperty, requestId, &Main_PropertyValue_fields),
    PB_LAST_FIELD
};

const pb_field_t Main_RPCResponse_fields[3] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, Main_RPCResponse, requestId, requestId, 0),
    PB_ONEOF_FIELD(response, 103, MESSAGE , ONEOF, STATIC  , OTHER, Main_RPCResponse, getProperty, requestId, &Main_PropertyValue_fields),
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
PB_STATIC_ASSERT((pb_membersize(Main_StatusUpdate, audio) < 65536 && pb_membersize(Main_StatusUpdate, tuner) < 65536 && pb_membersize(Main_StatusUpdate, system) < 65536 && pb_membersize(Main_StatusUpdate, engine) < 65536 && pb_membersize(Main_RPCRequest, request.audio) < 65536 && pb_membersize(Main_RPCRequest, request.tuner) < 65536 && pb_membersize(Main_RPCRequest, request.getProperty) < 65536 && pb_membersize(Main_RPCRequest, request.setProperty) < 65536 && pb_membersize(Main_RPCResponse, response.getProperty) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_Main_AudioStatus_Main_TunerStatus_Main_SystemStatus_Main_EngineStatus_Main_StatusUpdate_Main_AudioControlRequest_Main_TunerControlRequest_Main_PropertyValue_Main_RPCRequest_Main_RPCResponse)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for Main_PropertyValue.stringValue is too large. Define PB_FIELD_16BIT to fix this.
#endif


/* @@protoc_insertion_point(eof) */