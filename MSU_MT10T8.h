#include <stdint.h>
#define MT10T8_dimention    10      // MT10T8 character cell quantity.

typedef enum
{
    init_pins,          // Initialization output pins.              *pointer point to MSU_MT10T8_init_t structure.      Return *result_pointer not used.
    init_LCI,           // Initialization LCI.                      *pointer not used.                                  Return *result_pointer not used.
    choise_LCI,         // Choise LCI.                              *pointer point to WR2 pin bit-band addres.          Return *result_pointer not used.
    out_bin,            // Output uint8_t in binary format.         *pointer point to uint8_t data.                     Return *result_pointer not used.
    out_unsign_dec,     // Output uint32_t in decimal format.       *pointer point to uint32_t data.                    Return *result_pointer not used.
    out_sign_dec,       // Output int32_t in decimal format.        *pointer point to uint32_t data.                    Return *result_pointer not used.
    out_unsign_hex,     // Output uint32_t in hexadecimal format.   *pointer point to uint32_t data.                    Return *result_pointer not used.
    out_sign_hex,       // Output int32_t in hexadecimal format.    *pointer point to uint32_t data.                    Return *result_pointer not used.
    out_string,         // Output char elements.                    *pointer point to uint8_t data array.               Return *result_pointer not used.
    out_segment_code,   // Output 8-segment code.                   *pointer point to uint8_t 8-segment code array.     Return *result_pointer not used.
    back_bin,           // Back uint8_t in binary format.           *pointer point to uint8_t data.                     Return *result_pointer to uint8_t char array.
    back_unsign_dec,    // Back uint32_t in decimal format.         *pointer point to uint32_t data.                    Return *result_pointer to uint8_t char array.
    back_sign_dec,      // Back int32_t in decimal format.          *pointer point to uint32_t data.                    Return *result_pointer to uint8_t char array.
    back_unsign_hex,    // Back uint32_t in hexadecimal format.     *pointer point to uint32_t data.                    Return *result_pointer to uint8_t char array.
    back_sign_hex,      // Back int32_t in hexadecimal format.      *pointer point to uint32_t data.                    Return *result_pointer to uint8_t char array.
    back_string         // Back string.                             *pointer point to uint8_t char array.               Return *result_pointer to uint8_t 8-segment code array.
} MT10T8_purpos_t;      // Function purpose type.

typedef struct
{
	volatile    uint8_t     *A0;    // Bit-band adress A0 pin signal.
	volatile    uint8_t     *D0;    // Bit-band adress D0 pin signal.
	volatile    uint8_t     *D1;    // Bit-band adress D1 pin signal.
	volatile    uint8_t     *D2;    // Bit-band adress D2 pin signal.
	volatile    uint8_t     *D3;    // Bit-band adress D3 pin signal.
	volatile    uint8_t     *WR1;   // Bit-band adress WR1 pin signal.
	volatile    uint8_t     *WR2;   // Bit-band adress WR2 pin signal.
} MSU_MT10T8_init_t;                // LCI-pins structure type.

uint8_t     *MSU_MT10T8(void *pointer, MT10T8_purpos_t purpose);
