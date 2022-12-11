/*
Examples:
    lci_mt10t8_init_t   lci_pins;                                               // lci bus description structure.
    uint8_t             *lci2;                                                  // Pointer to lci2 enable pin.
    uint8_t             *pt_to_str;                                             // Pointer lci string.
    uint32_t            lci_data = 0x800000FF;                                  // lci number data.
    uint8_t             lci_array[MT10T8_dimention] =/
                        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};     // lci string data.
    
    
    lci_pins.A0 =  PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD4_Pos * 4;                       // Initialization lci bus description structure.
    lci_pins.D0 =  PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD0_Pos * 4;
    lci_pins.D1 =  PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD1_Pos * 4;
    lci_pins.D2 =  PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD2_Pos * 4;
    lci_pins.D3 =  PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD3_Pos * 4;
    lci_pins.WR1 = PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD5_Pos * 4;
    lci_pins.WR2 = PERIPH_BB_BASE + (((uint32_t)&(GPIOA->ODR)) & 0x000FFFFF) * 32 + GPIO_ODR_OD6_Pos * 4;

    lci_mt10t8(&lci_pins, init_pins);                                           // Initialization lci bus.
    lci2 = BB_GPIOA_PA7;                                                        // Choise second lci.
    lci_mt10t8(&lci_pins, init_lci);                                            // Initialization lci bus.
    lci_mt10t8(&(uint8_t)lci_data, out_bin);                                    // Output low byte lci_data to lci.                 Result: 'b 11111111'.
    lci_mt10t8(&lci_data, out_unsign_dec);                                      // Output lci_data to lci in unsign decimal format. Result: '2147483903'.
    lci_mt10t8(&lci_data, out_sign_dec);                                        // Output lci_data to lci in sign decimal format.   Result: '      -255'.
    lci_mt10t8(&lci_data, out_unsign_hex);                                      // Output lci_data to lci in unsign hex format.     Result: 'h 800000FF'.
    lci_mt10t8(&lci_data, out_sign_hex);                                        // Output lci_data to lci in unsign hex format.     Result: 'h      -FF'.
    lci_mt10t8(&lci_array[0], out_string);                                      // Output lci_string to lci.                        Result: '9876543210'.
    pt_to_str = lci_mt10t8(&lci_array[0], back_string);                         // Convert string to 8-segment code string and
                                                                                // back pointer to 8-segment code array.            Result: pt_to_str = &(uint8_t)internal_unvisible_variable.
    lci_mt10t8(pt_to_str, out_segment_code);                                    // Output segment_code_string to lci.               Result: '9876543210'.
*/

#include <stdint.h>
#define LCI_MT10T8_DIMENTION    10      // MT10T8 character cell quantity.

typedef enum
{
    init_pins,          // Initialization output pins.              *pointer point to lci_mt10t8_init_t structure.      Return *result_pointer not used.
    init_lci,           // Initialization lci.                      *pointer not used.                                  Return *result_pointer not used.
    choise_lci,         // Choise lci.                              *pointer point to WR2 pin bit-band addres.          Return *result_pointer not used.
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
} lci_mt10t8_purpos_t;  // Function purpose type.

typedef struct
{
	volatile    uint8_t     *pt_A0;    // Bit-band adress A0 pin signal.
	volatile    uint8_t     *pt_D0;    // Bit-band adress D0 pin signal.
	volatile    uint8_t     *pt_D1;    // Bit-band adress D1 pin signal.
	volatile    uint8_t     *pt_D2;    // Bit-band adress D2 pin signal.
	volatile    uint8_t     *pt_D3;    // Bit-band adress D3 pin signal.
	volatile    uint8_t     *pt_WR1;   // Bit-band adress WR1 pin signal.
	volatile    uint8_t     *pt_WR2;   // Bit-band adress WR2 pin signal.
} lci_mt10t8_bus_t;                // lci-pins structure type.


uint8_t     *lci_mt10t8(void *pt_data, lci_mt10t8_purpos_t purpose);
