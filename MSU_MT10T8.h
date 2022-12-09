/*
Examples:
    MSU_MT10T8_init_t   LCI_pins;                                               // LCI bus description structure.
    uint8_t             *LCI2;                                                  // Pointer to LCI2 enable pin.
    uint8_t             *pt_to_str;                                             // Pointer LCI string.
    uint32_t            LCI_data = 0x800000FF;                                  // LCI number data.
    uint8_t             LCI_array[MT10T8_dimention] =/
                        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};     // LCI string data.
    
    
    LCI_pins.A0 = BB_GPIOA_PA0;                                                 // Initialization LCI bus description structure.
    LCI_pins.D0 = BB_GPIOA_PA1;
    LCI_pins.D1 = BB_GPIOA_PA2;
    LCI_pins.D2 = BB_GPIOA_PA3;
    LCI_pins.D3 = BB_GPIOA_PA4;
    LCI_pins.WR1 = BB_GPIOA_PA5;
    LCI_pins.WR2 = BB_GPIOA_PA6;

    MSU_MT10T8(&LCI_pins, init_pins);                                           // Initialization LCI bus.
    LCI2 = BB_GPIOA_PA7;                                                        // Choise second LCI.
    MSU_MT10T8(&LCI_pins, init_LCI);                                            // Initialization LCI bus.
    MSU_MT10T8(&(uint8_t)LCI_data, out_bin);                                    // Output low byte LCI_data to LCI.                 Result: 'b 11111111'.
    MSU_MT10T8(&LCI_data, out_unsign_dec);                                      // Output LCI_data to LCI in unsign decimal format. Result: '2147483903'.
    MSU_MT10T8(&LCI_data, out_sign_dec);                                        // Output LCI_data to LCI in sign decimal format.   Result: '      -255'.
    MSU_MT10T8(&LCI_data, out_unsign_hex);                                      // Output LCI_data to LCI in unsign hex format.     Result: 'h 800000FF'.
    MSU_MT10T8(&LCI_data, out_sign_hex);                                        // Output LCI_data to LCI in unsign hex format.     Result: 'h      -FF'.
    MSU_MT10T8(&LCI_array[0], out_string);                                      // Output LCI_string to LCI.                        Result: '9876543210'.
    pt_to_str = MSU_MT10T8(&LCI_array[0], back_string);                         // Convert string to 8-segment code string and
                                                                                // back pointer to 8-segment code array.            Result: pt_to_str = &(uint8_t)internal_unvisible_variable.
    MSU_MT10T8(pt_to_str, out_segment_code);                                    // Output segment_code_string to LCI.               Result: '9876543210'.
*/

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
