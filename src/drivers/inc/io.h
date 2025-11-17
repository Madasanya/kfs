/**
 * @file io.h
 * @brief I/O Port Access Functions
 * 
 * @details
 * This header provides low-level functions for interacting with I/O ports,
 * used for device drivers, or hardware programming on architectures like x86. The functions 
 * allow intercting with ports.
 */

 #ifndef _IO_H_
 #define _IO_H_
 
 #include "inttype.h"
 
 /**
  * @brief Reads a byte from the specified I/O port.
  * 
  * @details
  * This function performs an input operation from the given port address,
  * returning the byte value read. It is equivalent to a standard 'inb'.
  * 
  * @param[in] port The 16-bit I/O port address to read from.
  * @return The 8-bit value read from the port.
  */
 uint8_t md_inb(uint16_t port);
 
 /**
  * @brief Writes a byte to the specified I/O port.
  * 
  * @details
  * This function performs an output operation to the given port address,
  * writing the provided byte value. It is equivalent to a standard 'outb'.
  * 
  * @param[in] port The 16-bit I/O port address to write to.
  * @param[in] byte The 8-bit value to write to the port.
  */
 void md_outb(uint16_t port, uint8_t byte);
 
 /**
  * @brief Performs a short wait for I/O operations to complete.
  * 
  * @details
  * This function introduces a brief delay, often implemented as a NOP or
  * a jump to itself, to allow time for previous I/O operations to settle. It is
  * equivalent to a standard 'io_wait' Useful in scenarios where back-to-back
  * port accesses might otherwise cause issues.
  */
 void md_io_wait(void);
 
 #endif /* _IO_H_ */