#ifndef __QL_RESET_INFO_H__
#define __QL_RESET_INFO_H__

#include <stdint.h>

/// Reset cause of the bootloader
typedef struct {
  /// Reset reason as defined in the [reset information](@ref ResetInterface)
  uint16_t reason;
  /// Signature indicating whether the reset reason is valid
  uint16_t signature;
} BootloaderResetCause_t;

/// Unknown bootloader cause (should never occur)
#define BOOTLOADER_RESET_REASON_UNKNOWN       0x0200u
/// Bootloader caused reset telling app to run
#define BOOTLOADER_RESET_REASON_GO            0x0201u
/// Application requested that bootloader runs
#define BOOTLOADER_RESET_REASON_BOOTLOAD      0x0202u
/// Bootloader detected bad external upgrade image
#define BOOTLOADER_RESET_REASON_BADIMAGE      0x0203u
/// Fatal Error or assert in bootloader
#define BOOTLOADER_RESET_REASON_FATAL         0x0204u
/// Forced bootloader activation
#define BOOTLOADER_RESET_REASON_FORCE         0x0205u
/// OTA Bootloader mode activation
#define BOOTLOADER_RESET_REASON_OTAVALID      0x0206u
/// Bootloader initiated deep sleep
#define BOOTLOADER_RESET_REASON_DEEPSLEEP     0x0207u
/// Application verification failed
#define BOOTLOADER_RESET_REASON_BADAPP        0x0208u
/// Bootloader requested that first stage upgrades main bootloader
#define BOOTLOADER_RESET_REASON_UPGRADE       0x0209u
/// Bootloader timed out waiting for upgrade image
#define BOOTLOADER_RESET_REASON_TIMEOUT       0x020Au
/// Soft-reset was forced to handle a fault
#define BOOTLOADER_RESET_REASON_FAULT         0x020Bu
/// Soft-reset was forced to handle a security fault
#define BOOTLOADER_RESET_REASON_TZ_FAULT      0x020Cu

/// Insufficient slot space to re-create a new firmware
#define BOOTLOADER_RESET_REASON_NO_SLOT_SPACE 0x020Du
/// CRC mismatch of the newly re-constructed firmware
#define BOOTLOADER_RESET_REASON_BADCRC        0x020Eu
/// Re-creation of the new application using the DDFU library failed
#define BOOTLOADER_RESET_REASON_DDFU_FAIL     0x020Fu

#define BOOTLOADER_RESET_REASON_UPG_SUCCESS   0x0301u

/// Reset signature is valid
#define BOOTLOADER_RESET_SIGNATURE_VALID      0xF00Fu
/// Reset signature is invalid
#define BOOTLOADER_RESET_SIGNATURE_INVALID    0xC33Cu

/** @} (end addtogroup ResetInterface) */
/** @} (end addtogroup CommonInterface) */
/** @} (end addtogroup Interface) */

#endif // __QL_RESET_INFO_H__
