#ifndef TinyFrame_TypesHPP
#define TinyFrame_TypesHPP

#include <cstdint>  // for uint8_t etc

#include "TF_Config.hpp"

namespace TinyFrame_n{

// Checksum type (0 = none, 8 = ~XOR, 16 = CRC16 0x8005, 32 = CRC32)
enum class TF_CKSUM_t{
    NONE     = 0,  // no checksums
    CUSTOM8  = 1,  // Custom 8-bit checksum
    CUSTOM16 = 2,  // Custom 16-bit checksum
    CUSTOM32 = 3,  // Custom 32-bit checksum
    XOR      = 8,  // inverted xor of all payload bytes
    CRC8     = 9,  // Dallas/Maxim CRC8 (1-wire)
    CRC16    = 16, // CRC16 with the polynomial 0x8005 (x^16 + x^15 + x^2 + 1)
    CRC32    = 32, // CRC32 with the polynomial 0xedb88320
};

//region Resolve data types

typedef size_t TF_LEN; // TF_LEN_BYTES
typedef size_t TF_TYPE; // TF_TYPE_BYTES
typedef size_t TF_ID; // TF_ID_BYTES

// TODO: runtime checks
// #if TF_ID_BYTES != 1 and TF_ID_BYTES != 2 and TF_ID_BYTES != 4
//     #error Bad value of TF_ID_BYTES, must be 1, 2 or 4
// #endif

#define _TF_FN

template <TF_CKSUM_t TF_CKSUM_TYPE>
struct CKSUM_TYPE_MAP;

template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::NONE> {using type = uint8_t;};
template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::XOR> {using type = uint8_t;};
template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CUSTOM8> {using type = uint8_t;};
template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CRC8> {using type = uint8_t;};

template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CRC16> {using type = uint16_t;};
template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CUSTOM16> {using type = uint16_t;};

template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CRC32> {using type = uint32_t;};
template <> struct CKSUM_TYPE_MAP<TF_CKSUM_t::CUSTOM32> {using type = uint32_t;};


template<TF_CKSUM_t TF_CKSUM_TYPE>
using TF_CKSUM = typename CKSUM_TYPE_MAP<TF_CKSUM_TYPE>::type;

//endregion

//---------------------------------------------------------------------------

/** Peer bit enum (used for init) */
typedef enum {
    TF_SLAVE = 0,
    TF_MASTER = 1,
} TF_Peer;


/** Response from listeners */
typedef enum {
    TF_NEXT = 0,   //!< Not handled, let other listeners handle it
    TF_STAY = 1,   //!< Handled, stay
    TF_RENEW = 2,  //!< Handled, stay, renew - useful only with listener timeout
    TF_CLOSE = 3,  //!< Handled, remove self
} TF_Result;


// ---------------------------------- INTERNAL ----------------------------------
// This is publicly visible only to allow static init.

enum TF_State_ {
    TFState_SOF = 0,      //!< Wait for SOF
    TFState_LEN,          //!< Wait for Number Of Bytes
    TFState_HEAD_CKSUM,   //!< Wait for header Checksum
    TFState_ID,           //!< Wait for ID
    TFState_TYPE,         //!< Wait for message type
    TFState_DATA,         //!< Receive payload
    TFState_DATA_CKSUM    //!< Wait for Checksum
};

/** Data structure for sending / receiving messages */
struct TF_Msg{
    TF_ID frame_id;       //!< message ID
    bool is_response;     //!< internal flag, set when using the Respond function. frame_id is then kept unchanged.
    TF_TYPE type;         //!< received or sent message type

    /**
     * Buffer of received data, or data to send.
     *
     * - If (data == nullptr) in an ID listener, that means the listener timed out and
     *   the user should free any userdata and take other appropriate actions.
     *
     * - If (data == nullptr) and length is not zero when sending a frame, that starts a multi-part frame.
     *   This call then must be followed by sending the payload and closing the frame.
     */
    const uint8_t *data;
    TF_LEN len; //!< length of the payload

    /**
     * Custom user data for the ID listener.
     *
     * This data will be stored in the listener slot and passed to the ID callback
     * via those same fields on the received message.
     */
    void *userdata;
    void *userdata2;

};

} // TinyFrame_n

#endif // TinyFrame_TypesHPP