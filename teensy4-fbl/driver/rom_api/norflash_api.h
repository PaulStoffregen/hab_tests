#ifndef NORFLASH_API_H
#define NORFLASH_API_H

/*!
 * @brief FlexSPI LUT Sequence structure
 */

typedef struct
{
  uint8  seqNum; //!< Sequence Number, valid number: 1-16
  uint8  seqId;  //!< Sequence Index, valid number: 0-15
  uint16 reserved;
}T_FLEXSPI_LUT_SEQ;


typedef struct
{
  uint8 time100ps;  // Data valid time, in terms of 100ps
  uint8 delayCells; // Data valid time, in terms of delay cells
}T_FLEXSPI_DLL_TIME;


/*!
 * @brief FlexSPI Memory Configuration Block
 */

typedef struct
{
  uint32 tag;               //!< [0x000-0x003] Tag, fixed value 0x42464346UL
  uint32 version;           //!< [0x004-0x007] Version,[31:24] -'V', [23:16] - Major, [15:8] - Minor, [7:0] - bugfix
  uint32 reserved0;         //!< [0x008-0x00b] Reserved for future use
  uint8  readSampleClkSrc;   //!< [0x00c-0x00c] Read Sample Clock Source, valid value: 0/1/3
  uint8  csHoldTime;         //!< [0x00d-0x00d] CS hold time, default value: 3
  uint8  csSetupTime;        //!< [0x00e-0x00e] CS setup time, default value: 3
  uint8  columnAddressWidth; //!< [0x00f-0x00f] Column Address with, for HyperBus protocol, it is fixed to 3, For
  //! Serial NAND, need to refer to datasheet
  uint8  deviceModeCfgEnable; //!< [0x010-0x010] Device Mode Configure enable flag, 1 - Enable, 0 - Disable
  uint8  deviceModeType; //!< [0x011-0x011] Specify the configuration command type:Quad Enable, DPI/QPI/OPI switch,
  //! Generic configuration, etc.
  uint16 waitTimeCfgCommands; //!< [0x012-0x013] Wait time for all configuration commands, unit: 100us, Used for
  //! DPI/QPI/OPI switch or reset command
  T_FLEXSPI_LUT_SEQ deviceModeSeq; //!< [0x014-0x017] Device mode sequence info, [7:0] - LUT sequence id, [15:8] - LUt
  //! sequence number, [31:16] Reserved
  uint32 deviceModeArg;    //!< [0x018-0x01b] Argument/Parameter for device configuration
  uint8  configCmdEnable;   //!< [0x01c-0x01c] Configure command Enable Flag, 1 - Enable, 0 - Disable
  uint8  configModeType[3]; //!< [0x01d-0x01f] Configure Mode Type, similar as deviceModeTpe
  T_FLEXSPI_LUT_SEQ cfgCmdSeqs[3]; //!< [0x020-0x02b] Sequence info for Device Configuration command, similar as deviceModeSeq
  uint32 reserved1;   //!< [0x02c-0x02f] Reserved for future use
  uint32 configCmdArgs[3];     //!< [0x030-0x03b] Arguments/Parameters for device Configuration commands
  uint32 reserved2;            //!< [0x03c-0x03f] Reserved for future use
  uint32 controllerMiscOption; //!< [0x040-0x043] Controller Misc Options, see Misc feature bit definitions for more
  //! details
  uint8  deviceType;    //!< [0x044-0x044] Device Type:  See Flash Type Definition for more details
  uint8  sflashPadType; //!< [0x045-0x045] Serial Flash Pad Type: 1 - Single, 2 - Dual, 4 - Quad, 8 - Octal
  uint8  serialClkFreq; //!< [0x046-0x046] Serial Flash Frequencey, device specific definitions, See System Boot
  //! Chapter for more details
  uint8  lutCustomSeqEnable; //!< [0x047-0x047] LUT customization Enable, it is required if the program/erase cannot
  //! be done using 1 LUT sequence, currently, only applicable to HyperFLASH
  uint32 reserved3[2];               //!< [0x048-0x04f] Reserved for future use
  uint32 sflashA1Size;               //!< [0x050-0x053] Size of Flash connected to A1
  uint32 sflashA2Size;               //!< [0x054-0x057] Size of Flash connected to A2
  uint32 sflashB1Size;               //!< [0x058-0x05b] Size of Flash connected to B1
  uint32 sflashB2Size;               //!< [0x05c-0x05f] Size of Flash connected to B2
  uint32 csPadSettingOverride;       //!< [0x060-0x063] CS pad setting override value
  uint32 sclkPadSettingOverride;     //!< [0x064-0x067] SCK pad setting override value
  uint32 dataPadSettingOverride;     //!< [0x068-0x06b] data pad setting override value
  uint32 dqsPadSettingOverride;      //!< [0x06c-0x06f] DQS pad setting override value
  uint32 timeoutInMs;                //!< [0x070-0x073] Timeout threshold for read status command
  uint32 commandInterval;            //!< [0x074-0x077] CS deselect interval between two commands
  T_FLEXSPI_DLL_TIME dataValidTime[2]; //!< [0x078-0x07b] CLK edge to data valid time for PORT A and PORT B
  uint16 busyOffset;                 //!< [0x07c-0x07d] Busy offset, valid value: 0-31
  uint16 busyBitPolarity; //!< [0x07e-0x07f] Busy flag polarity, 0 - busy flag is 1 when flash device is busy, 1 -
  //! busy flag is 0 when flash device is busy
  uint32 lookupTable[64];           //!< [0x080-0x17f] Lookup table holds Flash command sequences
  T_FLEXSPI_LUT_SEQ lutCustomSeq[12]; //!< [0x180-0x1af] Customizable LUT Sequences
  uint32 reserved4[4];              //!< [0x1b0-0x1bf] Reserved for future use
} T_FLEXSPI_MEM_CFG;


/*
 * Serial NOR Configuration Option
 */
typedef struct
{
  union
  {
    struct
    {
      uint32 maxFreq : 4;          //!< Maximum supported Frequency
      uint32 miscMode : 4;         //!< miscellaneous mode
      uint32 quadModeSetting : 4;  //!< Quad mode setting
      uint32 cmdPads : 4;          //!< Command pads
      uint32 queryPads : 4;        //!< SFDP read pads
      uint32 deviceType : 4;       //!< Device type
      uint32 optionSize : 4;       //!< Option size, in terms of uint32_t, size = (option_size + 1) * 4
      uint32 tag : 4;              //!< Tag, must be 0x0E
    } B;
    uint32 U;
  }option0;
  union
  {
    struct
    {
      uint32 dummyCycles : 8;     //!< Dummy cycles before read
      uint32 statusOverride : 8;  //!< Override status register value during device mode configuration
      uint32 pinmuxGroup : 4;     //!< The pinmux group selection
      uint32 dqsPinmuxGroup : 4;  //!< The DQS Pinmux Group Selection
      uint32 driveStrength : 4;   //!< The Drive Strength of FlexSPI Pads
      uint32 flashConnection : 4; //!< Flash connection option: 0 - Single Flash connected to port A, 1 -
      //! Parallel mode, 2 - Single Flash connected to Port B
    } B;
    uint32 U;
  }option1;
} T_SER_NOR_ONFIG_OPTION;

typedef enum FLEXSPI_OPC
{
    FLEXSPI_OPC_eCMD, //!< FlexSPI operation: Only command, both TX and
    //! RX buffer are ignored.
    FLEXSPI_OPC_CFG, //!< FlexSPI operation: Configure device mode, the
    //! TX FIFO size is fixed in LUT.
    FLEXSPI_OPC_WRITE, //!< FlexSPI operation: Write,  only TX buffer is
    //! effective
    FLEXSPI_OPC_READ, //!< FlexSPI operation: Read, only Rx Buffer is
    //! effective.
}T_FLEXSI_OPC;

/*!
 * @brief FlexSPI Transfer Context
 */
typedef struct
{
  uint32 opc;                  //!< FlexSPI operation
  uint32 baseAddress;          //!< FlexSPI operation base address
  uint32 seqId;                //!< Sequence Id
  uint32 seqNum;               //!< Sequence Number
  uint32 parallelModeEna;      //!< Is a parallel transfer
  uint32 *txBuffer;            //!< Tx buffer
  uint32 txSize;               //!< Tx size in bytes
  uint32 *rxBuffer;            //!< Rx buffer
  uint32 rxSize;               //!< Rx size in bytes
}T_FLEXSPI_XFER;


typedef struct
{
  T_FLEXSPI_MEM_CFG memCfg;     //!< Common memory configuration info via FlexSPI
  uint32 pageSize;              //!< Page size of Serial NOR
  uint32 sectorSize;            //!< Sector size of Serial NOR
  uint8  ipcmdSerialClkFreq;    //!< Clock frequency for IP command
  uint8  isUniformBlockSize;    //!< Sector/Block size is the same
  uint8  isDataOrderSwapped;    //!< Data order (D0, D1, D2, D3) is swapped (D1,D0, D3, D2)
  uint8  reserved0[1];          //!< Reserved for future use
  uint8  serialNorType;         //!< Serial NOR Flash type: 0/1/2/3
  uint8  needExitNoCmdMode;     //!< Need to exit NoCmd mode before other IP command
  uint8  halfClkForNonReadCmd;  //!< Half the Serial Clock for non-read command: true/false
  uint8  needRestoreNoCmdMode;  //!< Need to Restore NoCmd mode after IP commmand execution
  uint32 blockSize;             //!< Block size
  uint32 reserve2[11];          //!< Reserved for future use
}T_FLEXSPI_NOR_CFG;


typedef struct
{
  const uint32 version;
  int (*init)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg);
  int (*writePage)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg, uint32 dstAddr, const uint8* pageBuf);
  int (*eraseAll)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg);
  int (*erase)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg, uint32 startAddr, uint32 lenInBytes);
  int (*read)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg, uint8* dstBuf, uint32 startAddr, uint32 lenInBytes);
  int (*clearCache)(uint32 devID);
  int (*transferData)(uint32 devID, T_FLEXSPI_XFER* xfer);
  int (*updateLut)(uint32 devID, uint32 seqIdx, const uint32* lutBase, uint32 seqNum);
  int (*getConfig)(uint32 devID, T_FLEXSPI_NOR_CFG* devCfg, T_SER_NOR_ONFIG_OPTION* option);
}T_NORFLASH_API;

#endif /* NORFLASH_API_H */

