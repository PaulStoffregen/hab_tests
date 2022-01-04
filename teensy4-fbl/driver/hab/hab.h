#ifndef HAB_H
#define HAB_H

/* This file is derived from the document "High Assurance Boot Version 4
 * Application Programming Interface Reference Manual".
 */
#define HAB_HDR_BYTES 4  /* cannot use sizeof(hab_hdr_t) in preprocessor */


typedef enum
{
  HAB_TAG_IVT = 0xd1,       /*!< Image Vector Table */
  HAB_TAG_DCD = 0xd2,       /*!< Device Configuration Data */
  HAB_TAG_CSF = 0xd4,       /*!< Command Sequence File */
  HAB_TAG_CRT = 0xd7,       /*!< Certificate */
  HAB_TAG_SIG = 0xd8,       /*!< Signature */
  HAB_TAG_EVT = 0xdb,       /*!< Event */
  HAB_TAG_RVT = 0xdd,       /*!< ROM Vector Table */
  HAB_TAG_WRP = 0x81,       /*!< Wrapped Key */
  HAB_TAG_MAC = 0xac,       /*!< Message Authentication Code */
}T_HAB_TAG, hab_tag_t;


#define HAB_MAJOR_VERSION  4    /*!< Major version of this HAB release */
#define HAB_MINOR_VERSION  0    /*!< Minor version of this HAB release */

#define HAB_MAJ_BF         4, 4
#define HAB_MIN_BF         0, 4

/* Full version of this HAB release */
#define HAB_VERSION HAB_VERSION_DECL(HAB_MAJOR_VERSION, HAB_MINOR_VERSION) 

/* Base version for this HAB release */
#define HAB_BASE_VERSION HAB_VERSION_DECL(HAB_MAJOR_VERSION, 0)


/* Command tags
 *
 * Tag values 0xb0 .. 0xcf are reserved for HAB.  Values 0xf0 .. 0xff
 * are available for custom use.
 */
typedef enum
{
  HAB_CMD_SET_ITEM = 0xb1,  /**< Set Item */
  HAB_CMD_INS_KEY = 0xbe,   /**< Install Key */
  HAB_CMD_AUTH_DATA = 0xca, /**< Authenticate Data */
  HAB_CMD_WRT_DATA = 0xcc,  /**< Write Data */
  HAB_CMD_CHK_DATA = 0xcf,  /**< Check Data */
  HAB_CMD_NOP = 0xc0,       /**< No Operation */
  HAB_CMD_INIT = 0xb4,      /**< Initialize */
  HAB_CMD_UNLOCK = 0xb2,    /**< Unlock */
}T_HAB_CMD_ID, hab_command_id_t;

#define UINT32_TO_BYTES(word) \
  (uint8)((word)>>24), (uint8)((word)>>16), (uint8)((word)>>8), (uint8)((word) >> 0)

#define BYTES_TO_UINT32(b3, b2, b1, b0) \
  ((uint32) ( (((uint8)(b3)) << 24)     \
            | (((uint8)(b2)) << 16)     \
            | (((uint8)(b1)) <<  8)     \
            | (((uint8)(b0)) <<  0)))


#define UINT16_TO_BYTES(halfword) \
  (uint8)((halfword)>>8), (uint8)(halfword)

#define BYTES_TO_UINT16(b1, b0)     \
  ((uint16) ( (((uint8)(b1)) <<  8) \
            | (((uint8)(b0)) <<  0)))

#define HAB_GET_STRUCT_SIZE(hdr) \
  BYTES_TO_UINT16((hdr).length[0], (hdr).length[1])

#define HAB_CMD_HDR_DECL(tag, bytes, par) \
    (uint8)(tag), (uint8)((bytes)>>8), (uint8)(bytes), (uint8)(par)

#define HAB_VERSION_DECL(maj, min) \
  BF_SET(maj, HAB_MAJ_BF) | BF_SET(min, HAB_MIN_BF)

#define HAB_GET_MAJOR_VERSION(ver) \
  BF_GET(ver, HAB_MAJ_BF)

#define HAB_GET_MINOR_VERSION(ver) \
  BF_GET(ver, HAB_MIN_BF)

#define DCD_DATA(addr, data) UINT32_TO_BYTES(addr), UINT32_TO_BYTES(data)

/*
 *   IVT  header (goes in the struct's hab_hdr_t field, not a byte array)
 */
#define HAB_IVT_HDR_DECL(size, hab_version) \
  { \
    HAB_TAG_IVT, \
    { \
      (uint8)((size)>>8), \
      (uint8)((size)>>0), \
    }, \
    hab_version, \
  }


#define HAB_DCD_HDR_DECL(size) \
  HAB_TAG_DCD,\
  UINT16_TO_BYTES(size + sizeof(T_HAB_HDR)),\
  HAB_VERSION,
  

#define HAB_CMD_DECL(cmdType, listLen, width, flags) \
  cmdType,\
  UINT16_TO_BYTES(listLen + sizeof(T_HAB_HDR)),\
  ((flags) << 3) | (width & 0x7)


/* Supported widths of data commands. */
#define HAB_DATA_WIDTH_BYTE 1    /*< 8-bit value */
#define HAB_DATA_WIDTH_HALF 2    /*< 16-bit value */
#define HAB_DATA_WIDTH_WORD 4    /*< 32-bit value */

/* Header field components */
typedef struct
{
  uint8 tag;              /**< Tag field */
  uint8 length[2];        /**< Length field in bytes (big-endian) */
  uint8 par;              /**< Parameters field */
}T_HAB_HDR;

/* Image entry function prototype
 *
 * This typedef serves as the return type for hab_rvt.authenticate_image().  It
 * specifies a void-void function pointer, but can be cast to another function
 * pointer type if required.
 */
typedef void (*T_HAB_ENTRY)(void);


/* Target check types */
typedef enum
{
  HAB_TRG_MEM = 0x0f, /*!< Check memory white list */
  HAB_TRG_PER = 0xf0, /*!< Check peripheral white list */
  HAB_TRG_ANY = 0x55, /*!< Check memory & peripheral white list */
}T_HAB_TRG, hab_target_t;


/* Security configuration types */
typedef enum
{
  HAB_CFG_FAB = 0x00,         /**< @rom Un-programmed IC */
  HAB_CFG_RETURN = 0x33,      /**< Field Return IC */
  HAB_CFG_OPEN = 0xf0,        /**< Non-secure IC */
  HAB_CFG_CLOSED = 0xcc       /**< Secure IC */
}T_HAB_CFG, hab_config_t;

/* Security state types
 */
typedef enum
{
  HAB_STATE_INITIAL   = 0x33, /*!< Initialising state (transitory) */
  HAB_STATE_CHECK     = 0x55, /*!< Check state (non-secure) */
  HAB_STATE_NONSECURE = 0x66, /*!< Non-secure state */
  HAB_STATE_TRUSTED   = 0x99, /*!< Trusted state */
  HAB_STATE_SECURE    = 0xaa, /*!< Secure state */
  HAB_STATE_FAIL_SOFT = 0xcc, /*!< Soft fail state */
  HAB_STATE_FAIL_HARD = 0xff, /*!< Hard fail state (terminal) */
  HAB_STATE_NONE      = 0xf0, /*!< No security state machine */
  HAB_STATE_MAX
}T_HAB_STATE, hab_state_t;

/* HAB status types
 */
typedef enum
{
  HAB_STATUS_ANY     = 0x00, /*!< Match any status in hab_rvt.report_event() */
  HAB_STATUS_FAIL    = 0x33, /*!< Operation failed */
  HAB_STATUS_WARN    = 0x69, /*!< Operation completed with warning */
  HAB_STATUS_SUCCESS = 0xf0, /*!< Operation completed successfully */
  HAB_STATUS_MAX
}T_HAB_STATUS, hab_status_t;

/* Failure or warning reasons
 */
typedef enum
{
  HAB_RSN_ANY = 0x00,               /*!< Match any reason in hab_rvt.reportEvent() */
  HAB_RSN_ENG_FAIL = 0x30,          /*!< Engine failure. */
  HAB_RSN_INVAL_ADDRESS = 0x22,     /*!< Invalid address: access denied. */
  HAB_RSN_INVAL_ASSERTION = 0x0c,   /*!< Invalid assertion. */
  HAB_RSN_INVAL_CALL = 0x28,        /*!< Function called out of sequence. */
  HAB_RSN_INVAL_CERTIFICATE = 0x21, /*!< Invalid certificate. */
  HAB_RSN_INVAL_COMMAND = 0x06,     /*!< Invalid command: command malformed. */
  HAB_RSN_INVAL_CSF = 0x11,         /*!< Invalid @ref csf. */
  HAB_RSN_INVAL_DCD = 0x27,         /*!< Invalid @ref dcd. */
  HAB_RSN_INVAL_INDEX = 0x0f,       /*!< Invalid index: access denied. */
  HAB_RSN_INVAL_IVT = 0x05,         /*!< Invalid @ref ivt. */
  HAB_RSN_INVAL_KEY = 0x1d,         /*!< Invalid key. */
  HAB_RSN_INVAL_RETURN = 0x1e,      /*!< Failed callback function. */
  HAB_RSN_INVAL_SIGNATURE = 0x18,   /*!< Invalid signature. */
  HAB_RSN_INVAL_SIZE = 0x17,        /*!< Invalid data size. */
  HAB_RSN_MEM_FAIL = 0x2e,          /*!< Memory failure. */
  HAB_RSN_OVR_COUNT = 0x2b,         /*!< Expired poll count. */
  HAB_RSN_OVR_STORAGE = 0x2d,       /*!< Exhausted storage region. */
  HAB_RSN_UNSUP_ALGORITHM = 0x12,   /*!< Unsupported algorithm. */
  HAB_RSN_UNSUP_COMMAND = 0x03,     /*!< Unsupported command. */
  HAB_RSN_UNSUP_ENGINE = 0x0a,      /*!< Unsupported engine. */
  HAB_RSN_UNSUP_ITEM = 0x24,        /*!< Unsupported configuration item. */
  HAB_RSN_UNSUP_KEY = 0x1b,         /*!< Unsupported key type or parameters. */
  HAB_RSN_UNSUP_PROTOCOL = 0x14,    /*!< Unsupported protocol. */
  HAB_RSN_UNSUP_STATE = 0x09,       /*!< Unsuitable state. */
  HAB_RSN_MAX
}T_HAB_RSN, hab_reason_t;

/* Audit logging contexts.
 *   This list is sorted in order of increasing priority: where two contexts
 *   might apply, the latter one is used.
 */
typedef enum
{
  HAB_CTX_ANY = 0x00,         /**< Match any context in hab_rvt.report_event() */
  HAB_CTX_FAB = 0xff,         /**< @rom Event logged in hab_fab_test() */
  HAB_CTX_ENTRY = 0xe1,       /**< Event logged in hab_rvt.entry() */
  HAB_CTX_TARGET = 0x33,      /**< Event logged in hab_rvt.check_target() */
  HAB_CTX_AUTH = 0x0a,        /**< Event logged in hab_rvt.authenticate_image() */
  HAB_CTX_DCD = 0xdd,         /**< Event logged in hab_rvt.run_dcd() */
  HAB_CTX_CSF = 0xcf,         /**< Event logged in hab_rvt.run_csf() */
  HAB_CTX_CMD = 0xc0,         /**< Event logged executing CSF or DCD command */
  HAB_CTX_AUTH_DATA = 0xdb,   /**< Authenticated data block */
  HAB_CTX_ASSERT = 0xa0,      /**< Event logged in hab_rvt.assert() */
  HAB_CTX_EXIT = 0xee,        /**< Event logged in hab_rvt.exit() */
  HAB_CTX_MAX
}T_HAB_CTX, hab_context_t;


typedef enum
{
  HAB_ENG_ANY = 0x00,         /**< First compatible engine will be selected automatically (no engine configuration parameters are allowed) */
  HAB_ENG_SCC = 0x03,         /**< Security controller */
  HAB_ENG_RTIC = 0x05,        /**< Run-time integrity checker */
  HAB_ENG_SAHARA = 0x06,      /**< Crypto accelerator */
  HAB_ENG_CSU = 0x0a,         /**< Central Security Unit */
  HAB_ENG_SRTC = 0x0c,        /**< Secure clock */
  HAB_ENG_DCP = 0x1b,         /**< Data co-processor */
  HAB_ENG_CAAM = 0x1d,        /**< Cryptographic Acceleration and Assurance Module */
  HAB_ENG_SNVS = 0x1e,        /**< Secure non-volatile storage */
  HAB_ENG_OCOTP = 0x21,       /**< Fuse controller */
  HAB_ENG_DTCP = 0x22,        /**< DTCP co-processor */
  HAB_ENG_ROM = 0x36,         /**< Protected ROM area */
  HAB_ENG_HDCP = 0x24,        /**< HDCP co-processor */
  HAB_ENG_SW = 0xff,          /**< Software engine */
}T_HAB_ENG, hab_engine_t;


typedef enum
{
  HAB_PCL_SRK = 0x03,         /**< SRK certificate format */
  HAB_PCL_X509 = 0x09,        /**< X.509v3 certificate format */
  HAB_PCL_CMS = 0xc5,         /**< CMS/PKCS#7 signature format */
  HAB_PCL_BLOB = 0xbb,        /**< SHW-specific wrapped key format */
  HAB_PCL_AEAD = 0xa3,        /**< Proprietary AEAD MAC format */
}T_HAB_PCL, hab_protocol_t;


typedef enum
{
  HAB_ALG_ANY = 0x00,         /**< Algorithm type ANY */
  HAB_ALG_HASH = 0x01,        /**< Hash algorithm type */
  HAB_ALG_SIG = 0x02,         /**< Signature algorithm type */
  HAB_ALG_FF = 0x03,          /**< Finite field arithmetic */
  HAB_ALG_EC = 0x04,          /**< Elliptic curve arithmetic */
  HAB_ALG_CIPHER = 0x05,      /**< Cipher algorithm type */
  HAB_ALG_MODE = 0x06,        /**< Cipher/hash mode */
  HAB_ALG_WRAP = 0x07,        /**< Key wrap algorithm type */
  HAB_ALG_SHA1 = 0x11,        /**< SHA-1 algorithm */
  HAB_ALG_SHA256 = 0x17,      /**< SHA-256 algorithm */
  HAB_ALG_SHA512 = 0x1b,      /**< SHA-512 algorithm */
  HAB_ALG_PKCS1 = 0x21,       /**< PKCS#1 RSA signature algorithm */
  HAB_ALG_AES = 0x55,         /**< AES algorithm */
  HAB_ALG_CCM_MODE = 0x66,    /**< Counter with CBC-MAC */
  HAB_ALG_BLOB = 0x71,        /**< SHW-specific key wrap algorithm */
}T_HAB_ALG, hab_algorithm_t;


/* Assertion types. */
typedef enum
{
  HAB_ASSERT_BLOCK = 0, /*!< Assert that a memory block was authenticated */
}T_HAB_ASSERT, hab_assertion_t;


/*! Loader callback.
 *
 * @par Purpose
 *
 * This function must be supplied by the library caller if required. It is
 * intended to finalise image loading in those boot modes where only a portion
 * of the image is loaded to a temporary initial location prior to device
 * configuration.
 * 
 * @par Operation 
 *
 * This function is called during hab_rvt.authenticate_image() between running
 * the @ref dcd and @ref csf.  The operation of this function is defined by
 * the caller.
 *
 * @param[in,out] start Initial (possibly partial) image load address on
 * entry.  Final image load address on exit.
 *
 * @param[in,out] bytes Initial (possibly partial) image size on entry.  Final
 * image size on exit.
 *
 * @param[in] boot_data Initial @ref ivt Boot Data load address.
 *
 * @remark The interpretation of the Boot Data is defined by the caller.
 * Different boot components or modes may use different boot data, or even
 * different loader callback functions.
 *
 * @warning It should not be assumed by this function that the Boot Data is
 * valid or authentic.
 *
 * @warning It is the responsibility of the loader callback to check the final
 * image load addresses using hab_rvt.check_target() prior to copying any image
 * data.
 *
 * @pre The (possibly partial) image has been loaded in the initial load
 * address, and the Boot Data is within the initial image.
 *
 * @pre The @ref dcd has been run, if provided.
 *
 * @post The final image load addresses pass hab_rvt.check_target().
 *
 * @retval #HAB_SUCCESS if all operations completed successfully,
 *
 * @retval #HAB_FAILURE otherwise.
 */
typedef T_HAB_STATUS (*T_HAB_LDR_CALLBACK)(void** start, uint32* bytes, const void* bootData);

typedef struct
{
  T_HAB_HDR hdr;
  uint8 data[0];
}T_HAB_CMD;

typedef struct
{
  T_HAB_HDR hdr;
  uint32 address;
  uint32 valMask;
}T_HAB_CMD_WRT_DATA;

typedef struct
{
  T_HAB_HDR hdr;
  uint32 address;
  uint32 mask;
  uint32 count;
}T_HAB_CMD_CHK_DATA;

typedef struct
{
  T_HAB_HDR hdr;
  uint8 null;
  uint8 alg;
  uint8 eng;
  uint8 cfg;
}T_HAB_CMD_SET_ITEM;

typedef struct
{
  T_HAB_HDR hdr;
  uint32 value;
}T_HAB_CMD_INIT;

typedef struct
{
  T_HAB_HDR hdr;
  uint32 value;
}T_HAB_CMD_UNLOCK;

typedef struct
{
  T_HAB_HDR hdr;
  uint8  pcl;
  uint8  alg;
  uint8  src;
  uint8  trg;
  uint32 keyAddr;
  uint32 crtHash[0];
}T_HAB_CMD_INS_KEY;

typedef struct
{
  uint32 blkAddr;
  uint32 blkSize;
}T_HAB_BLK_DEF;


typedef struct
{
  T_HAB_HDR hdr;
  uint8  key;
  uint8  pcl;
  uint8  eng;
  uint8  cfg;
  uint32 crtAddr;
  T_HAB_BLK_DEF blk[0];
}T_HAB_CMD_AUTH_DATA;


typedef struct
{
  T_HAB_HDR hdr; /*!< Header with HAB_TAG_EVT, length and hab version fields */
  uint8 sts;     /*!< Logged status level */
  uint8 rsn;     /*!< Logged reason */
  uint8 ctx;     /*!< Context from which the event is logged */
  uint8 eng;     /*!< Engine associated with failure */
  uint8 data[0]; /*!< Context dependent data */
}T_HAB_EVT;


/*!
 * The @ref rvt consists of a @ref hdr followed by a list of addresses as
 * described further below.
 *
 * 0x00 hdr
 * 0x04 entry
 * 0x08 exit
 * 0x0C check_target
 * 0x10 authenticate_image
 * 0x14 run_dcd
 * 0x18 run_csf
 * 0x1C assert
 * 0x20 report_event
 * 0x24 report_status
 * 0x28 failsafe
 *
 * 0x2C autehticate_image_no_dcd  HAB_VER >= 4.2.0
 * 0x30 get_version               HAB_VER >= 4.2.5
 */

typedef struct
{
  /*! @ref hdr with tag #HAB_TAG_RVT, length and HAB version fields
   *  (see @ref data)
   */
  T_HAB_HDR hdr;

  /*!
   * Enter and initialise HAB library.
   *
   * @par Purpose:
   *   This function initialises the HAB library and @ref shw plugins.  It is
   *   intended for use by post-ROM boot stage components, via the @ref rvt,
   *   prior to calling any other HAB functions other than
   *   hab_rvt.report_event() and hab_rvt.report_status().
   * 
   * @ifrom
   *   It is also intended for use by the boot ROM via hab_rvt.entry().
   * @endrom
   * 
   * @par Operation 
   *   This function performs the following operations every time it is called:
   *   - Initialise the HAB library internal state
   *   - Initialise the internal secret key store (cleared at the next hab_rvt.exit())
   *   - Run the entry sequence of each available @ref shw plugin
   *
   *   When first called from boot ROM, this function also performs the
   *   following operations prior to those given above:
   *   - Initialise the internal public key store (persists beyond hab_rvt.exit())
   *   - Run the self-test sequence of each available @ref shw plugin
   *   - If a state machine is present and enabled, change the security state as follows:
   *     - If the IC is configured as #HAB_CFG_OPEN or #HAB_CFG_RETURN, move to
   *       #HAB_STATE_NONSECURE
   *     - If the IC is configured as #HAB_CFG_CLOSED, move to
   *       #HAB_STATE_TRUSTED
   *     - Otherwise, leave the security state unchanged
   *
   *   If any failure occurs in the operations above:
   *   - An audit event is logged
   *   - All remaining operations are abandoned (except that all @ref shw
   *     self-test and entry sequences are still executed)
   *   - If a state machine is present and enabled, the security state is set
   *     as follows:
   *     @ifrom
   *     - Unless the IC is configured as #HAB_CFG_FAB,@endrom move to
   *       #HAB_STATE_NONSECURE.  Note that if a security violation has been
   *       detected by the HW, the final state will be #HAB_STATE_FAIL_SOFT or
   *       #HAB_STATE_FAIL_HARD depending on the HW configuration.
   *     @endrom
   *
   *   @warning
   *     Boot sequences may comprise several images with each launching
   *     the next as well as alternative images should one boot device or boot
   *     image be unavailable or unusable.  The authentication of each image in
   *     a boot sequence must be bracketed by its own hab_rvt.entry()
   *     ... hab_rvt.exit() pair in order to ensure that security state
   *     information gathered for one image cannot be misapplied to another
   *     image.
   *
   *   @ifrom 
   *
   *   @warning This applies to each boot path in boot ROM as well, except for
   *   the fabrication test path.
   *
   *   @endrom
   *
   * @post HAB library internal state is initialised.
   *
   * @post Available @ref shw plugins are initialised.
   *
   * @post If a failure or warning occurs during @ref shw plugin
   * initialisation, an audit event is logged with the relevant @ref eng
   * tag.  The status and reason logged are described in the relevant @ref
   * shw plugin documentation.
   *
   * @post Security state is initialised, if a state machine is present and
   * enabled.
   *
   * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
   * although unsuccessful operations will still generate audit log events,
   *
   * @retval #HAB_SUCCESS on other ICs if all commands completed
   * without failure (even if warnings were generated),
   *
   * @retval #HAB_FAILURE otherwise.
   */
  T_HAB_STATUS (*entry)(void);

  /* Finalise and exit HAB library.
   * @ingroup exit
   *
   * @par Purpose
   *
   * This function finalises the HAB library and @ref shw plugins.  It is
   * intended for use by post-ROM boot stage components, via the @ref rvt,
   * after calling other HAB functions and prior to launching the next boot
   * stage or switching to another boot path.
   *
   * @ifrom It is also intended for use by the boot ROM via hab_rvt.exit().
   * @endrom
   * 
   * @par Operation 
   *
   * This function performs the following operations:
   *
   * - Finalise the HAB library internal state
   * - Clear the internal secret key store
   * - Run the finalisation sequence of each available @ref shw plugin
   *
   * If any failure occurs, an audit event is logged and all remaining
   * operations are abandoned (except that all @ref shw exit sequences are
   * still executed).
   *
   * @warning See warnings for hab_rvt.entry().
   *
   * @post #HAB_ASSERT_BLOCK records are cleared from audit log.  Note that
   * other event records are not cleared.
   *
   * @post Any public keys installed by @ref csf commands remain active.
   *
   * @post Any secret keys installed by @ref csf commands are deleted.
   *
   * @post Available @ref shw plugins are in their final state as described
   * in the relevant sections.
   *
   * @post If a failure or warning occurs, an audit event is logged with the
   * @ref eng tag of the @ref shw plugin concerned.  The status and reason
   * logged are described in the relevant @ref shw plugin documentation.
   *
   * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
   * although unsuccessful operations will still generate audit log events,
   *
   * @retval #HAB_SUCCESS on other ICs if all commands completed
   * without failure (even if warnings were generated),
   *
   * @retval #HAB_FAILURE otherwise.
   */
  T_HAB_STATUS (*exit)(void);

    /** Check target address
     * @ingroup chk_tgt
     *
     * @par Purpose
     *
     * This function reports whether or not a given target region is allowed
     * for either peripheral configuration or image loading in memory.  It is
     * intended for use by post-ROM boot stage components, via the @ref rvt,
     * in order to avoid configuring security-sensitive peripherals, or
     * loading images over sensitive memory regions or outside recognised
     * memory devices in the address map.
     * 
     * @ifrom It is also available for use by the boot ROM, both directly via
     * hab_rvt.check_target() and indirectly via hab_rvt.authenticate_image(). 
     * @endrom
     *
     * @par Operation 
     *
     * The lists of allowed target regions vary by IC and core, and should be
     * taken from the @ref ref_rug.
     *
     * @ifrom The allowed register sets for peripheral configuration and memory
     * regions for image loading are defined in the @ref hal by
     * #hab_hal_peripheral and #hab_hal_memory respectively. @endrom
     * 
     * @param[in] type Type of target (memory, peripheral or any in which both
     * the memory and peripheral regions are checked)
     *
     * @param[in] start Address of target region
     *
     * @param[in] bytes Size of target region
     *
     * @post if the given target region goes beyond the allowed regions, an
     * audit event is logged with status #HAB_FAILURE and reason
     * #HAB_INV_ADDRESS, together with the call parameters.  See the @ref evt
     * record documentation for details.
     *
     * @post For successful commands, no audit event is logged.
     *
     * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
     * although unsuccessful operations will still generate audit log events,
     *
     * @retval #HAB_SUCCESS if the given target region lies wholly within the
     * allowed regions for the requested type of target.
     *
     * @retval #HAB_FAILURE otherwise
     */
  T_HAB_STATUS (*checkTarget)(T_HAB_TRG type, const void* start, uint32 bytes);

    /** Authenticate image.
     * @ingroup auth_img
     *
     * @par Purpose
     *
     * This function combines DCD, CSF and Assert functions in a standard
     * sequence in order to authenticate a loaded image.  It is intended for
     * use by post-ROM boot stage components, via the @ref rvt.  Support for
     * images partially loaded to an initial location is provided via a
     * callback function.
     * 
     * @ifrom It is also available for use by the boot ROM via
     * hab_rvt.authenticate_image(). @endrom
     * 
     * @par Operation 
     *
     * This function performs the following sequence of operations:
     * - Check that the initial image load addresses pass
     * hab_rvt.check_target().
     * - Check that the IVT offset lies within the initial image bounds.
     * - Check that the @ref ivt @a self and @a entry pointers are not NULL
     * - Check the @ref ivt header for consistency and compatability.
     * - If provided in the @ref ivt, calculate the @ref dcd initial location,
     * check that it lies within the initial image bounds, and run the @ref
     * dcd commands.
     * - If provided in the @ref ivt, calculate the Boot Data initial location
     * and check that it lies within the initial image bounds.
     * - If provided in the parameters, invoke the callback function with the
     * initial image bounds and initial location of the @ref ivt Boot Data.
     * 
     * From this point on, the full image is assumed to be in its final
     * location. The following operations will be performed on all IC 
     * configurations (#hab_config), but will be only enforced on an IC 
     * configured as #HAB_CFG_CLOSED:
     * - Check that the final image load addresses pass hab_rvt.check_target().
     * - Check that the CSF lies within the image bounds, and run the CSF
     * commands.
     * - Check that all of the following data have been authenticated (using
     * their final locations):
     *   - IVT;
     *   - DCD (if provided);
     *   - Boot Data (initial byte if provided);
     *   - Entry point (initial word).
     *
     * @param[in] cid Caller ID, used to identify which SW issued this call.
     *
     * @param[in] ivt_offset Offset in bytes of the IVT from the image start
     * address.
     *
     * @param[in,out] start Initial (possibly partial) image load address on
     * entry.  Final image load address on exit.
     *
     * @param[in,out] bytes Initial (possibly partial) image size on entry.
     * Final image size on exit.
     *
     * @param[in] loader Callback function to load the full image to its final
     * load address.  Set to NULL if not required.
     *
     * @remark Caller ID may be bound to signatures verified using keys
     * installed with #HAB_CMD_INS_KEY_CID flag. See @ref cmd_ins_key and @ref
     * bnd_obj for details.
     *
     * @remark A @a loader callback function may be supplied even if the image
     * is already loaded to its final location on entry.
     *
     * @remark Boot Data (boot_data in @ref ivt) will be ignored if the 
     * @a loader callback function point is set to Null.
     *
     * @warning The @a loader callback function should lie within existing
     * authenticated areas. @ifrom Or within the ROM. @endrom
     *
     * @warning It is the responsibility of the caller to check the initial
     * image load addresses using hab_rvt.check_target() prior to loading the
     * initial image and calling this function.
     *
     * @warning After completion of hab_rvt.authenticate_image(), the caller
     * should test using hab_rvt.assert() that the Boot Data was
     * authenticated.
     *
     * @post The post-conditions of the functions hab_rvt.check_target(),
     * hab_rvt.run_dcd(), hab_rvt.run_csf() and hab_rvt.assert() apply also to
     * this function.  In particular, any audit events logged within the given
     * functions have the context field appropriate to that function rather
     * than #HAB_CTX_AUTHENTICATE.  In addition, the side-effects and
     * post-conditions of any callback function supplied apply.
     *
     * @post If a failure or warning occurs outside these contexts, an audit
     * event is logged with status:
     *   - #HAB_FAILURE, with further reasons:
     *     - #HAB_INV_ADDRESS: initial or final image addresses outside allowed
     *     regions
     *     - #HAB_INV_ADDRESS: IVT, DCD, Boot Data or CSF outside image bounds
     *     - #HAB_INV_ADDRESS: IVT @a self or @a entry pointer is NULL 
     *     - #HAB_INV_CALL: hab_rvt.entry() not run successfully prior to call
     *     - #HAB_INV_IVT: IVT malformed
     *     - #HAB_INV_IVT: IVT version number is less than HAB library version
     *     - #HAB_INV_RETURN: Callback function failed
     *
     * @retval entry field from @ref ivt on an IC not configured as
     * #HAB_CFG_CLOSED provided that the following conditions are met
     * (other unsuccessful operations will generate audit log events):
     *  - the @a start pointer and the pointer it locates are not NULL
     *  - the initial @ref ivt location is not NULL
     *  - the @ref ivt @ref hdr (given in the @a hdr field) is valid
     *  - the final @ref ivt location (given by the @a self field) is not NULL 
     *  - any loader callback completed successfully,
     *
     * @retval entry field from @ref ivt on other ICs if all operations
     * completed without failure (even if warnings were generated),
     *
     * @retval NULL otherwise.
     */
  T_HAB_ENTRY (*authenticate_image)(uint8 cid, uint32 ivt_offset, void** start, uint32* bytes, T_HAB_LDR_CALLBACK loader);

    /** Execute a boot configuration script.
     * @ingroup run_dcd
     *
     * @par Purpose
     *
     * This function configures the IC based upon a @ref dcd table.  It is
     * intended for use by post-ROM boot stage components, via the @ref rvt.
     * This function may be invoked as often as required for each boot stage.
     *
     * @ifrom It is also intended for use by the boot ROM, both directly via
     * hab_rvt.run_dcd() and indirectly via hab_rvt.authenticate_image().
     * @endrom
     * 
     * The difference between the configuration functionality in this function
     * and hab_rvt.run_csf() arises because the @ref dcd table is not
     * authenticated prior to running the commands.  Hence, there is a more
     * limited range of commands allowed, and a limited range of parameters to
     * allowed commands.
     * 
     * @par Operation 
     *
     * This function performs the following operations:
     * - Checks the @ref hdr for compatibility and consistency
     * - Makes an internal copy of the @ref dcd table
     * - Executes the commands in sequence from the internal copy of the @ref
     * dcd
     *
     * If any failure occurs, an audit event is logged and all remaining
     * operations are abandoned.
     *
     * @param[in] dcd   Address of the @ref dcd.
     *
     * @warning It is the responsibility of the caller to ensure that the @a
     * dcd parameter points to a valid memory location.
     *
     * @warning The @ref dcd must be authenticated by a subsequent @ref csf
     * command prior to launching the next boot image, in order to avoid
     * unauthorised configurations which may subvert secure operation.
     * Although the content of the next boot stage's CSF may be out of scope
     * for the hab_rvt.run_dcd() caller, it is possible to enforce this
     * constraint by using hab_rvt.assert() to ensure that both the DCD and
     * any pointers used to locate it have been authenticated.
     *
     * @warning Each invocation of hab_rvt.run_dcd() must occur between a pair
     * of hab_rvt.entry() and hab_rvt.exit() calls, although multiple
     * hab_rvt.run_dcd() calls (and other HAB calls) may be made in one
     * bracket.  This constraint applies whether hab_rvt.run_dcd() is
     * successful or not: a subsequent call to hab_rvt.exit() is required
     * prior to launching the authenticated image or switching to another boot
     * target.
     *
     * @post Many commands may cause side-effects. See the @ref dcd
     * documentation.
     *
     * @post If a failure or warning occurs within a command handler, an audit
     * event is logged with the offending command, copied from the DCD.  The
     * status and reason logged are described in the relevant command
     * documentation.
     *
     * @post For other failures or warning, the status logged is:
     *   - #HAB_WARNING, with further reasons:
     *     - #HAB_UNS_COMMAND: unsupported command encountered, where DCD
     *     version and HAB library version differ
     *   - #HAB_FAILURE, with further reasons:
     *     - #HAB_INV_ADDRESS: NULL @a dcd parameter
     *     - #HAB_INV_CALL: hab_rvt.entry() not run successfully prior to call
     *     - #HAB_INV_COMMAND: command not allowed in DCD
     *     - #HAB_UNS_COMMAND: unrecognised command encountered, where DCD
     *     version and HAB library version match
     *     - #HAB_INV_DCD: DCD malformed or too large
     *     - #HAB_INV_DCD: DCD version number is less than HAB library version
     * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
     * although unsuccessful operations will still generate audit log events,
     *
     * @retval #HAB_SUCCESS on other ICs if all commands completed
     * without failure (even if warnings were generated),
     *
     * @retval #HAB_FAILURE otherwise.
     */
  T_HAB_STATUS (*runDCD)(const uint8* dcd);

    /** Execute an authentication script.
     * @ingroup run_csf
     *
     * @par Purpose
     *
     * This function authenticates SW images and configures the IC based upon
     * a @ref csf.  It is intended for use by post-ROM boot stage components,
     * via the @ref rvt.  This function may be invoked as often as required
     * for each boot stage.
     * 
     * @ifrom It is also available for use by the boot ROM via hab_rvt.run_csf,
     * although it is anticipated that the boot ROM will mostly call this
     * function indirectly via hab_rvt.authenticate_image(). @endrom
     *
     * @par Operation 
     *
     * This function performs the following operations:
     * - Checks the @ref hdr for compatibility and consistency
     * - Makes an internal copy of the @ref csf
     * - Executes the commands in sequence from the internal copy of the @ref
     * csf
     *
     * The internal copy of the @ref csf is authenticated by an explicit
     * command in the sequence.  Prior to authentication, a limited set of
     * commands is available to:
     * - Install a Super-Root key (unless previously installed)
     * - Install a CSF key (unless previously installed)
     * - Specify any variable configuration items
     * - Authenticate the CSF
     *
     * Subsequent to CSF authentication, the full set of commands is available.
     *
     * If any failure occurs, an audit event is logged and all remaining
     * operations are abandoned.
     *
     * @param[in] csf   Address of the @ref csf.
     *
     * @param[in] cid Caller ID, used to identify which SW issued this call.
     *
     * @remark Caller ID may be bound to signatures verified using keys
     * installed with #HAB_CMD_INS_KEY_CID flag. See @ref cmd_ins_key and @ref
     * bnd_obj for details.
     *
     * @warning It is the responsibility of the caller to ensure that the @a
     * csf parameter points to a valid memory location.
     *
     * @warning Each invocation of hab_rvt.run_csf() must occur between a pair
     * of hab_rvt.entry() and hab_rvt.exit() calls, although multiple
     * hab_rvt.run_csf() calls (and other HAB calls) may be made in one
     * bracket.  This constraint applies whether hab_rvt.run_csf() is
     * successful or not: a subsequent call to hab_rvt.exit() is required
     * prior to launching the authenticated image or switching to another boot
     * target.
     *
     * @post Many commands may cause side-effects. See the @ref csf
     * documentation.  In particular, note that keys installed by the @ref csf
     * remain available for use in subsequent operations.
     *
     * @post If a failure or warning occurs within a command handler, an audit
     * event is logged with the offending command, copied from the CSF.  The
     * status and reason logged are described in the relevant command
     * documentation.
     *
     * @post For other failures or warning, the status logged is:
     *   - #HAB_WARNING, with further reasons:
     *     - #HAB_UNS_COMMAND: unsupported command encountered, where CSF
     *     version and HAB library version differ
     *   - #HAB_FAILURE, with further reasons:
     *     - #HAB_INV_ADDRESS: NULL @a csf parameter
     *     - #HAB_INV_CALL: hab_rvt.entry() not run successfully prior to call
     *     - #HAB_INV_COMMAND: command not allowed prior to CSF authentication
     *     - #HAB_UNS_COMMAND: unrecognised command encountered, where CSF
     *     version and HAB library version match
     *     - #HAB_INV_CSF: CSF not authenticated
     *     - #HAB_INV_CSF: CSF malformed or too large
     *     - #HAB_INV_CSF: CSF version number is less than HAB library version
     *
     * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
     * although unsuccessful operations will still generate audit log events,
     *
     * @retval #HAB_SUCCESS on other ICs if all commands completed
     * without failure (even if warnings were generated),
     *
     * @retval #HAB_FAILURE otherwise.
     */
  T_HAB_STATUS (*runCSF)(const uint8* csf, uint8 cid);

    /** Test an assertion against the audit log.
     * @ingroup assert
     *
     * @par Purpose
     *
     * This function allows the audit log to be interrogated.  It is intended
     * for use by post-ROM boot stage components, via the @ref rvt, to
     * determine the state of authentication operations.  This function may be
     * invoked as often as required for each boot stage.
     *
     * @ifrom It is also available for use by the boot ROM, both directly via
     * hab_rvt.assert() and indirectly via hab_rvt.authenticate_image().
     * @endrom
     *
     * @par Operation 
     *
     * This function checks the required assertion as detailed below.
     *
     * @param[in] type Assertion type.
     *
     * @param[in] data Assertion data.
     *
     * @param[in] count Data size or count.
     *
     * @par Memory block authentication:
     * For #HAB_ASSERT_BLOCK assertion type, hab_rvt.assert() checks that the
     * given memory block has been authenticated after running a CSF. The
     * parameters are interpreted as follows:
     *
     * @par
     * - @a data: memory block starting address
     * - @a count: memory block size (in bytes)
     *
     * @par 
     *
     * A simple interpretation of "memory block has been authenticated" is
     * taken, such that the given block must lie wholly within a single
     * contiguous block authenticated while running a CSF.  A given memory
     * block covered by the union of several neighboring or overlapping
     * authenticated blocks could fail the test with this interpretation, but
     * it is assumed that such cases will not arise in practice.
     *
     * @post If the assertion fails, an audit event is logged with status
     * #HAB_FAILURE and reason #HAB_INV_ASSERTION, together with the call
     * parameters.  See the @ref evt record documentation for details.
     *
     * @post For successful commands, no audit event is logged.
     *
     * @retval #HAB_SUCCESS on an IC not configured as #HAB_CFG_CLOSED,
     * although unsuccessful operations will still generate audit log events,
     *
     * @retval #HAB_SUCCESS on other ICs if the assertion is confirmed
     *
     * @retval #HAB_FAILURE otherwise
     */
  T_HAB_STATUS (*habAssert)(T_HAB_ASSERT type, const void* data, uint32 count);

    /** Report an event from the audit log.
     * @ingroup event
     *
     * @par Purpose
     *
     * This function allows the audit log to be interrogated.  It is intended
     * for use by post-ROM boot stage components, via the @ref rvt, to
     * determine the state of authentication operations.  This function may
     * be called outside an hab_rvt.entry() / hab_rvt.exit() pair.
     * 
     * @ifrom It is also available for use by the boot ROM, where it may be
     * used to report boot failures as part of a tethered boot
     * protocol. @endrom
     *
     * @par Operation 
     *
     * This function performs the following operations:
     * - Scans the audit log for a matching event
     * - Copies the required details to the output parameters (if found)
     *
     * @param[in] status Status level of required event.
     *
     * @param[in] index Index of required event at given status level.
     *
     * @param[out] event @ref evt record.
     *
     * @param[in,out] bytes Size of @a event buffer on entry, size of event
     * record on exit.
     *
     * @remark Use @a status = #HAB_STS_ANY to match any logged event,
     * regardless of the status value logged.
     *
     * @remark Use @a index = 0 to return the first matching event, @a index =
     * 1 to return the second matching event, and so on.
     *
     * @remark The data logged with each event is context-dependent.  Refer to
     * @ref evt record documentation.
     *
     * @warning Parameter @a bytes may not be NULL.
     *
     * @warning If the @a event buffer is a NULL pointer or too small to fit
     * the event record, the required size is written to @a bytes, but no 
     * part of the event record is copied to the output buffer.
     *
     * @retval #HAB_SUCCESS if the required event is found, and the event
     * record is copied to the output buffer.
     * 
     * @retval #HAB_SUCCESS if the required event is found and @a event buffer 
     * passed is a NULL pointer.
     *
     * @retval #HAB_FAILURE otherwise
     */
  T_HAB_STATUS (*reportEvent)(T_HAB_STATUS status, uint32 index, uint8* event, uint32* bytes);

    /** Report security status.
     * @ingroup status
     *
     * @par Purpose
     *
     * This function reports the security configuration and state of the IC as
     * well as searching the audit log to determine the status of the boot
     * process.  It is intended for use by post-ROM boot stage components, via
     * the @ref rvt.  This function may be called outside an
     * hab_rvt.entry() / hab_rvt.exit() pair.
     * 
     * @ifrom It is also available for use by the boot ROM, and should be used
     * rather than the HAL function hab_hal_read_sec_cfg(). @endrom
     *
     * @par Operation 
     *
     * This function reads the fuses which indicate the security
     * configuration.  The fusemap varies by IC, and should be taken from the
     * @ref ref_rug.  It also uses the @ref shw state machine, if present and
     * enabled, to report on the security state.
     *
     * @param[out] config Security configuration, NULL if not required
     *
     * @param[out] state Security state, NULL if not required
     *
     * @remark If no @ref shw state machine is present and enabled, the state
     * #HAB_STATE_NONE will be output.
     *
     * @retval #HAB_SUCCESS if no warning or failure audit events have been
     * logged.
     *
     * @retval #HAB_WARNING otherwise, if only warning events have been logged.
     *
     * @retval #HAB_FAILURE otherwise
     */
  T_HAB_STATUS (*reportStatus)(T_HAB_CFG* config, T_HAB_STATE* state);

    /** Enter failsafe boot mode.
     * @ingroup safe
     *
     * @par Purpose
     *
     * This function provides a safe path when image authentication has failed
     * and all possible boot paths have been exhausted.  It is intended for
     * use by post-ROM boot stage components, via the @ref rvt.
     * 
     * @ifrom It is also available for use by the boot ROM via
     * hab_rvt.failsafe(). @endrom
     *
     * @par Operation 
     *
     * The precise details of this function vary by IC and core, and should be
     * taken from @ref ref_rug.
     *
     * @warning This function does not return.
     *
     * @remark Since this function does not return, it implicitly performs the
     * functionality of hab_rvt.exit() in order to ensure an appropriate
     * configuration of the @ref shw plugins.
     *
     * @remark Two typical implementations are:
     * - a low-level provisioning protocol in which an image is downloaded to
     * RAM from an external host, authenticated and launched.  The downloaded
     * image may communicate with tools on the external host to report the
     * reasons for boot failure, and may re-provision the end-product with
     * authentic boot images.
     * - a failsafe boot mode which does not allow execution to leave the ROM
     * until the IC is reset.
     */
  void (*failsafe)(void);
  T_HAB_ENTRY (*authenticate_image_no_dcd)(uint8 cid, uint32 ivt_offset, void** start, uint32* bytes, T_HAB_LDR_CALLBACK loader);
  uint32 (*get_version)(void);
  T_HAB_STATUS (*authenticate_container)(uint8 cid, uint32 ivt_offset, void** start, uint32* bytes, T_HAB_LDR_CALLBACK loader, uint32 srkmask, int skip_dcd);
}T_HAB_RVT;


/*! @ref ivt structure
 * @ingroup ivt
 *
 * @par Format
 *
 * An @ref ivt consists of a @ref hdr followed by a list of addresses as
 * described further below.
 *
 * @warning The @a entry address may not be NULL.
 *
 * @warning On an IC not configured as #HAB_CFG_CLOSED, the
 * @a csf address may be NULL.  If it is not NULL, the @ref csf will be
 * processed, but any failures should be non-fatal.
 *
 * @warning On an IC configured as #HAB_CFG_CLOSED, the @a
 * csf address may not be NULL, and @ref csf failures are typically fatal.
 *
 * @remark The Boot Data located using the @a boot_data field is interpreted
 * by the HAB caller in a boot-mode specific manner.  This may be used by the
 * boot ROM as to determine the load address and boot device configuration for
 * images loaded from block devices (see @ref ref_rug for details).
 *
 * @remark All addresses given in the IVT, including the Boot Data (if
 * present) are those for the final load location. 
 *
 * @anchor ila
 *
 * @par Initial load addresses
 *
 * The @a self field is used to calculate addresses in boot modes where an
 * initial portion of the image is loaded to an initial location.  In such
 * cases, the IVT, Boot Data (if present) and DCD (if present) are used in
 * configuring the IC and loading the full image to its final location.  Only
 * the IVT, Boot Data (if present) and DCD (if present) are required to be
 * within the initial image portion.
 *
 * The method for calculating an initial load address for the DCD is
 * illustrated in the following C fragment.  Similar calculations apply to
 * other fields.
 *
@verbatim
        hab_ivt_t* ivt_initial = <initial IVT load address>;
        const void* dcd_initial = ivt_initial->dcd;
        if (ivt_initial->dcd != NULL)
            dcd_initial = (const uint8_t*)ivt_initial 
                          + (ivt_initial->dcd - ivt_initial->self)
@endverbatim
 */
typedef struct
{
  /* Header with tag = HAB_TAG_IVT, length and HAB version fields */
  T_HAB_HDR hdr;
  /* Absolute address of the first instruction to execute from the image */
  T_HAB_ENTRY entry;
  /* Reserved in this version of HAB: should be NULL. */
  const void* reserved1;
  /* Absolute address of the image's device configuration data (DCD).
   * May be NULL.
   */
  const void* dcd;
  /* Absolute address of the Boot Data.
   * May be NULL, but not interpreted any further by HAB
   */
  const void* boot_data;
  /* Absolute address of the image vector table (IVT). */
  const void* self;
  /* Absolute address of the image CSF. */
  const void* csf;
  /* Reserved in this version of HAB: should be zero. */
  uint32 reserved2;
}T_HAB_IVT;

typedef struct
{
  uint32 start;            /**< Start address of the image */
  uint32 size;             /**< Size of the image */
  uint32 plugin;           /**< Plugin flag */
  uint32 __align;
}T_HAB_BOOT_DATA;

typedef struct
{
   /* Header with tag = HAB_TAG_CSF, length and HAB version fields */
   T_HAB_HDR hdr;
   T_HAB_CMD cmds[0];
}T_HAB_CSF;


#define __ivt_sect __attribute__((section(".ivt")))
#define __csf_sect __attribute__((section(".csf")))
#define __boot_data_sect __attribute__((section(".boot_data")))
#define __dcd_data_sect __attribute__((section(".dcd_data")))
#define __dcd_wr_cmd_sect __attribute__((section(".dcd_wr_cmd")))
#define __dcd_hdr_sect __attribute__((section(".dcd_hdr")))

#endif /* HAB_H */

