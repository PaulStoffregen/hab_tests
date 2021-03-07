#ifndef REG_H
#define REG_H


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define __REG32(addr) (*(volatile unsigned int*)(volatile void*)((volatile unsigned char*)0UL + (addr)))

#define __REG16(addr) (*(volatile unsigned short*)(volatile void*)((volatile unsigned char*)0UL + (addr)))

#define __REG8(addr) (*(volatile unsigned char*)(volatile void*)((volatile unsigned char*)0UL + (addr)))


#define REG32_GET_BY_ADDR(addr) __REG32(addr)
#define REG16_GET_BY_ADDR(addr) __REG16(addr)
#define REG8_GET_BY_ADDR(addr)  __REG8(addr)

#define REG32_SET_BY_ADDR(arg, addr) __REG32(addr) |= (arg)
#define REG16_SET_BY_ADDR(arg, addr) __REG16(addr) |= (arg)
#define REG8_SET_BY_ADDR(arg, addr)  __REG8(addr) |= (arg)

#define REG32_CLR_BY_ADDR(arg, addr) __REG32(addr) &= ~(arg)
#define REG16_CLR_BY_ADDR(arg, addr) __REG16(addr) &= ~(arg)
#define REG8_CLR_BY_ADDR(arg, addr)  __REG8(addr) &= ~(arg)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG32_RD_ADDR(arg, addr) ((arg) = __REG32(addr))

#define REG16_RD_ADDR(arg, addr) ((arg) = __REG16(addr))

#define REG8_RD_ADDR(arg, addr)  ((arg) = __REG8(addr))


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG32_WR_ADDR(arg, addr) (__REG32(addr) = (arg))

#define REG16_WR_ADDR(arg, addr) (__REG16(addr) = (arg))

#define REG8_WR_ADDR(arg, addr)  (__REG8(addr) = (arg))


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG32_RD_BASE_OFFS(arg, base, offs) ((arg) = __REG32((base) + (offs)))

#define REG16_RD_BASE_OFFS(arg, base, offs) ((arg) = __REG16((base) + (offs)))

#define REG8_RD_BASE_OFFS(arg, base, offs)  ((arg) = __REG8((base) + (offs)))


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG32_WR_BASE_OFFS(arg, base, offs) (__REG32((base) + (offs)) = (arg))

#define REG16_WR_BASE_OFFS(arg, base, offs) (__REG16((base) + (offs)) = (arg))

#define REG8_WR_BASE_OFFS(arg, base, offs)  (__REG8((base) + (offs)) = (arg))


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG_RDBW(arg, reg, lsb, width) ( (arg) = ( (reg) >> (lsb) ) & ((1U << (width)) - 1U) )


/*!
 ******************************************************************************
 * @def REG_RD(arg, reg, field)
 ******************************************************************************
 * @brief Preprocessor macro reading a specific field from a register
 *
 * @par Description:
 *
 *
 * @param arg [OUT] - The storage where to put the value. This can be any
 *   variable.
 * @param reg [IN] - The register to be read from. This can be any variable.
 * @param field [IN] - Definition of the field to be read defined by a comma 
 *   separated pair of values defining start bit and field width in bits
 *
 * @return none
 *
 ******************************************************************************
 */

#define REG32_RDBF(arg, reg, bf) ( REG_RDBW((arg), (reg), bf) )
#define REG32_RDBF_ADDR(arg, addr, bf) ( REG_RDBW((arg), __REG32(addr), bf) )
#define REG32_RDBF_BASE_OFFS(arg, base, offs, bf) ( REG_RDBW((arg), __REG32((base) + (offs)), bf) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define REG_WRBW(arg, reg, lsb, width) ( (reg) = ( (reg) & ~( ((1U << (width)) - 1U) << (lsb) ) ) | ( (arg) << (lsb) ) )


/*!
 ******************************************************************************
 * @def REG_WR(arg, reg, bf)
 ******************************************************************************
 * @brief Preprocessor macro writing a specific field in a register
 *
 * @par Description:
 *
 *
 * @param arg [IN] - The storage where to read the value from. This can be any
 *   variable.
 * @param reg [OUT] - The register to write the value to. This can be any
 *   variable.
 * @param bf [IN] - Definition of the field to be read defined by a comma 
 *   separated pair of values defining start bit and field width in bits
 *
 * @return none
 *
 ******************************************************************************
 */

#define REG32_WRBF(arg, reg, bf) ( REG_WRBW((arg), (reg), bf) )
#define REG32_WRBF_ADDR(arg, addr, bf) ( REG_WRBW((arg), __REG32(addr), bf) )
#define REG32_WRBF_BASE_OFFS(arg, base, offs, bf) ( REG_WRBW((arg), __REG32((base) + (offs)), bf) )


#define REG_SETBW(arg, reg, lsb, width) ( (reg) |= ( (arg) << (lsb) ) )

#define REG32_SETBF(arg, reg, bf) ( REG_SETBW((arg), (reg), bf) )
#define REG32_SETBF_ADDR(arg, addr, bf) ( REG_SETBW((arg), __REG32(addr), bf) )
#define REG32_SETBF_BASE_OFFS(arg, base, offs, bf) ( REG_SETBW((arg), __REG32((base) + (offs)), bf) )


#define REG_CLRBW(arg, reg, lsb, width) ( (reg) &= ~( (arg) << (lsb) ) )

#define REG32_CLRBF(arg, reg, bf) ( REG_CLRBW((arg), (reg), bf) )
#define REG32_CLRBF_ADDR(arg, addr, bf) ( REG_CLRBW((arg), __REG32(addr), bf) )
#define REG32_CLRBF_BASE_OFFS(arg, base, offs, bf) ( REG_CLRBW((arg), __REG32((base) + (offs)), bf) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define _REG_CMP(arg, reg, pos, width) ( (arg) == (( (reg) >> (pos) ) & ((1 << (width)) - 1)) )


/*!
 ******************************************************************************
 * @def REG_CMP(arg, reg, field)
 ******************************************************************************
 * @brief Preprocessor macro comparing a specific field in a register
 *
 * @par Description:
 *
 *
 * @param arg [IN] - The storage where to read the value from. This can be any
 *   variable.
 * @param reg [OUT] - The register to write the value to. This can be any
 *   variable.
 * @param field [IN] - Definition of the field to be read defined by a comma 
 *   separated pair of values defining start bit and field width in bits
 *
 * @return none
 *
 ******************************************************************************
 */

#define REG32_CMP(arg, reg, field) ( _REG_CMP((arg), (reg), field) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BF_LSB_WIDTH(lsb, width) lsb, width


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BW_MASK(lsb, width) (((1 << (width)) - 1) << lsb)


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BW_WIDTH_MASK(lsb, width) ((1 << (width)) - 1)


/*!
 ******************************************************************************
 * @def BF_MASK(bf)
 ******************************************************************************
 * @brief Preprocessor macro creating a bitmask from a bit-field definition
 *
 * @par Description:
 *
 *
 * @param bf [IN] - Definition of the bit-field defined by a comma separated
 *   pair of values defining lsb and width of the bit-field in bits
 *
 * @return The bitmask defined by the bit-field definition
 *
 ******************************************************************************
 */

#define BF_MASK(bf) ( BW_MASK(bf) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BF_WIDTH_MASK(bf) ( BW_WIDTH_MASK(bf) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BW_LSB(lsb, width) (lsb)


/*!
 ******************************************************************************
 * @def BF_LSB(field)
 ******************************************************************************
 * @brief Preprocessor macro extracting the lsb bit-position from a
 *   bit-field definition
 *
 * @par Description:
 *
 *
 * @param bf [IN] - Definition of the bit field, defined by a comma separated
 *   pair of values defining lsb and width of the bit-field in bits
 *
 * @return The bit index defining the lsb
 *
 ******************************************************************************
 */

#define BF_LSB(bf) ( BW_LSB(bf) )


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

#define BW_WIDTH(lsb, width) ( width )


/*!
 ******************************************************************************
 * @def BF_WIDTH(field)
 ******************************************************************************
 * @brief Preprocessor macro extracting the width in bits from a
 *   bit-field definition
 *
 * @par Description:
 *
 *
 * @param bf [IN] - Definition of the bit field, defined by a comma separated
 *   pair of values defining lsb and width of the bit-field in bits
 *
 * @return The width of the bit-field
 *
 ******************************************************************************
 */

#define BF_WIDTH(bf) ( BW_WIDTH(bf) )


#define BW_SET(arg, lsb, width) ( ( (arg) & BW_WIDTH_MASK(lsb, width) ) << (lsb) )

#define BF_SET(arg, bf) BW_SET(arg, bf)


#define BW_GET(arg, lsb, width) ( ( (arg) >> (lsb) ) & BW_WIDTH_MASK(lsb, width) )

#define BF_GET(arg, bf) BW_GET(arg, bf)

#endif /* REG_H */

