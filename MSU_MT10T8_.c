#include "MSU_MT10T8.h"
#define size_of_bin			    8		    // 8 character cell for binary number.
#define size_of_dec			    10		    // 10 character cell for decimal number.
#define size_of_hex			    8		    // 8 character cell for hexadecimal number.
#define max_positive_value	    0x80000000
#define negative_value_msk	    0x7FFFFFFF  // Negative value extraction mask.
#define min_negative_dec_value	999999999   // -999999999 - the minimum value that can be displayed on the 10 character cell LCI.
#define low_tetrad_msk	        0x0F        // Low tetrad extraction mask.
#define dec_num_sys_base        10          // The base of the decimal number system.

static	void    MSU_MT10T8_init_LCI();
static	void    MSU_MT10T8_out_segment_code(uint8_t *pointer);
static	void    MSU_MT10T8_convert_string_to_segment_code(uint8_t *pointer);
static	void    MSU_MT10T8_convert_bin_to_string(uint8_t *pointer);
static	void    MSU_MT10T8_convert_dec_to_string(uint32_t *pointer);
static	void    MSU_MT10T8_convert_hex_to_string(uint32_t *pointer);

static  MSU_MT10T8_init_t   *pointer_to_pins;
static  uint8_t             *pointer_to_WR2, *pointer_to_input_data, *pointer_to_output_data;
static  uint8_t             char_string[MT10T8_dimention];
static  uint8_t             segment_code[MT10T8_dimention];
static  uint8_t             sign, i;
static  uint32_t            data1, data2;


uint8_t		*MSU_MT10T8(void *pointer, MT10T8_purpos_t purpose)
{
    sign = ' ';                                                                 // by default sata is unsign.
    switch  (purpose)
    {
        case init_pins  :
            pointer_to_pins = pointer;
            pointer_to_WR2 = (uint8_t*)(((MSU_MT10T8_init_t*)pointer)->WR2);
            break;
        case choise_LCI  :
            pointer_to_WR2 = pointer;
            break;
        case init_LCI  :
            MSU_MT10T8_init_LCI();
            for (i=0; i<MT10T8_dimention; i++)
                segment_code[i] = 0;
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case out_segment_code  :
            MSU_MT10T8_out_segment_code((uint8_t*)pointer);
            break;
        case out_string  :
            MSU_MT10T8_convert_string_to_segment_code((uint8_t*)pointer);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_string  :
            MSU_MT10T8_convert_string_to_segment_code((uint8_t*)pointer);
            return &segment_code[0];
            break;
        case out_bin  :
            MSU_MT10T8_convert_bin_to_string((uint8_t*) pointer);
            MSU_MT10T8_convert_string_to_segment_code(&char_string[0]);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_bin  :
            MSU_MT10T8_convert_bin_to_string((uint8_t*) pointer);
            return &char_string[0];
            break;
        case out_unsign_dec  :
            MSU_MT10T8_convert_dec_to_string((uint32_t*)pointer);
            MSU_MT10T8_convert_string_to_segment_code(&char_string[0]);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_unsign_dec  :
            MSU_MT10T8_convert_dec_to_string((uint32_t*)pointer);
            return &char_string[0];
            break;
        case out_sign_dec  :
            sign = '-';
            MSU_MT10T8_convert_dec_to_string((uint32_t*)pointer);
            MSU_MT10T8_convert_string_to_segment_code(&char_string[0]);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_sign_dec  :
            sign = '-';
            MSU_MT10T8_convert_dec_to_string((uint32_t*)pointer);
            return &char_string[0];
            break;
        case out_unsign_hex  :
            MSU_MT10T8_convert_hex_to_string((uint32_t*)pointer);
            MSU_MT10T8_convert_string_to_segment_code(&char_string[0]);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_unsign_hex  :
            MSU_MT10T8_convert_hex_to_string((uint32_t*)pointer);
            return &char_string[0];
            break;
        case out_sign_hex  :
            sign = '-';
            MSU_MT10T8_convert_hex_to_string((uint32_t*)pointer);
            MSU_MT10T8_convert_string_to_segment_code(&char_string[0]);
            MSU_MT10T8_out_segment_code(&segment_code[0]);
            break;
        case back_sign_hex  :
            sign = '-';
            MSU_MT10T8_convert_hex_to_string((uint32_t*)pointer);
            return &char_string[0];
            break;
    }   // End of switch purpose.
return &segment_code[0];
}   // End of MSU_MT10T8.

void    MSU_MT10T8_init_LCI()       // Send signals to LCI for unlock.
{
    *(pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer_to_WR2) = 0;          // WR2 = 0. LCI enable.
    *(pointer_to_pins->D0) = 1;     // DB0 = 1.
    *(pointer_to_pins->D1) = 1;     // DB1 = 1.
    *(pointer_to_pins->D2) = 1;     // DB2 = 1.
    *(pointer_to_pins->D3) = 1;     // DB3 = 1.
    *(pointer_to_pins->A0) = 0;     // A0 = 0.  DB3-DB0 = 0xF - LCI register address.
    *(pointer_to_pins->WR1) = 1;    // WR1 = 1. Send register address 0xF to LCI.
    *(pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer_to_pins->D1) = 0;     // DB1 = 0.
    *(pointer_to_pins->D2) = 0;     // DB2 = 0.
    *(pointer_to_pins->D3) = 0;     // DB3 = 0.
    *(pointer_to_pins->A0) = 1;     // A0 = 1.  DB3-DB0 = 0x1 - data.
    *(pointer_to_pins->WR1) = 1;    // WR1 = 1, Send data 0x1 to LCI register 0xF for LCI unlock.
    *(pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer_to_WR2) = 1;          // WR2 = 1. LCI disable.
    return;
}   // End of MSU_MT10T8_init_LCI.
      
void    MSU_MT10T8_out_segment_code(uint8_t* pointer)
{
    *(pointer_to_pins->WR1) = 0;        // WR1 = 0. Don't send signals to LCI.
    *(pointer_to_WR2) = 0;              // WR2 = 0. LCI enable.
    *(pointer_to_pins->D0) = 0;         // DB0 = 0.
    *(pointer_to_pins->D1) = 0;         // DB1 = 0.
    *(pointer_to_pins->D2) = 0;         // DB2 = 0.
    *(pointer_to_pins->D3) = 0;         // DB3 = 0.
    *(pointer_to_pins->A0) = 0;         // A0 = 0.  DB3-DB0 = 0x0 - LCI register address.
    *(pointer_to_pins->WR1) = 1;        // WR1 = 1. Send register address 0x0 to LCI.
    *(pointer_to_pins->WR1) = 0;        // WR1 = 0. Don't send signals to LCI.
    *(pointer_to_pins->A0) = 1;         // A0 = 1.  DB3-DB0 = 0x1 - data.
    for (pointer_to_input_data = pointer + MT10T8_dimention-1; pointer_to_input_data > pointer - 1; pointer_to_input_data--)
    {
        *(pointer_to_pins->D0) = (*pointer_to_input_data) & 0x01;           // DB0.
        *(pointer_to_pins->D1) = (*pointer_to_input_data >> 1) & 0x01;      // DB1.
        *(pointer_to_pins->D2) = (*pointer_to_input_data >> 2) & 0x01;      // DB2.
        *(pointer_to_pins->D3) = (*pointer_to_input_data >> 3) & 0x01;      // DB3.
        *(pointer_to_pins->WR1) = 1;                                        // WR1 = 1. Send low tetrad data to LCI register.
        *(pointer_to_pins->WR1) = 0;                                        // WR1 = 0. Don't send signals to LCI.
        *(pointer_to_pins->D0) = (*pointer_to_input_data >> 4) & 0x01;      // DB0.
        *(pointer_to_pins->D1) = (*pointer_to_input_data >> 5) & 0x01;      // DB1.
        *(pointer_to_pins->D2) = (*pointer_to_input_data >> 6) & 0x01;      // DB2.
        *(pointer_to_pins->D3) = (*pointer_to_input_data >> 7) & 0x01;      // DB3.
        *(pointer_to_pins->WR1) = 1;                                        // WR1 = 1. Send higt tetrad data to LCI register. Increment LCI register address automatically.
        *(pointer_to_pins->WR1) = 0;                                        // WR1 = 0. Don't send signals to LCI.
    }
    *(pointer_to_WR2) = 1;                                                  // WR2 = 1. LCI disable.
    return;
}   // End of MT10T8_out_segment_code.

void    MSU_MT10T8_convert_string_to_segment_code(uint8_t* pointer)
{
	pointer_to_input_data = pointer;
    for (pointer_to_output_data = &segment_code[0]; pointer_to_output_data < (&segment_code[0] + MT10T8_dimention); pointer_to_output_data++)
    {
        switch  (*pointer_to_input_data)
        {
            case 48  :
            	*pointer_to_output_data = 0xEE;   // 0
                break;
            case 49  :
            	*pointer_to_output_data = 0x60;   // 1
                break;
            case 50  :
            	*pointer_to_output_data = 0x2F;   // 2
                break;
            case 51  :
            	*pointer_to_output_data = 0x6D;   // 3
                break;
            case 52  :
            	*pointer_to_output_data = 0xE1;   // 4
                break;
            case 53  :
            	*pointer_to_output_data = 0xCD;   // 5
                break;
            case 54  :
            	*pointer_to_output_data = 0xCF;   // 6
                break;
            case 55  :
            	*pointer_to_output_data = 0x68;   // 7
                break;
            case 56  :
            	*pointer_to_output_data = 0xEF;   // 8
                break;
            case 57  :
            	*pointer_to_output_data = 0xED;   // 9
                break;
            case 65  :
            	*pointer_to_output_data = 0xEB;   // A
                break;
            case 98  :
            	*pointer_to_output_data = 0xC7;   // b
                break;
            case 99  :
            	*pointer_to_output_data = 0x07;   // c
                break;
            case 100 :
            	*pointer_to_output_data = 0x67;   // d
                break;
            case 69  :
            	*pointer_to_output_data = 0x8F;   // E
                break;
            case 70  :
            	*pointer_to_output_data = 0x8B;   // F
                break;
            case 45  :
            	*pointer_to_output_data = 0x01;   // -
                break;
            case 42  :
            	*pointer_to_output_data = 0xA9;   // *
                break;
            case 74  :
            	*pointer_to_output_data = 0x6E;   // J
                break;
            case 71  :
            	*pointer_to_output_data = 0xCE;   // G
                break;
            case 104 :
            	*pointer_to_output_data = 0xC3;   // h
                break;
            case 76  :
            	*pointer_to_output_data = 0x86;   // L
                break;
            case 110 :
            	*pointer_to_output_data = 0x43;   // n
                break;
            case 111 :
            	*pointer_to_output_data = 0x47;   // o
                break;
            case 80  :
            	*pointer_to_output_data = 0xAB;   // P
                break;
            case 114 :
            	*pointer_to_output_data = 0x03;   // r
                break;
            case 85  :
            	*pointer_to_output_data = 0xE6;   // U
                break;
            case 88  :
            	*pointer_to_output_data = 0xE3;   // X
                break;
            case 89  :
            	*pointer_to_output_data = 0xE5;   // Y
                break;
            case 61  :
            	*pointer_to_output_data = 0x05;   // =
                break;
            case 95  :
            	*pointer_to_output_data = 0x04;   // _
                break;
            case 33  :
            	*pointer_to_output_data = 0x70;   // !
                break;
            case 94  :
            	*pointer_to_output_data = 0x41;   // ^
                break;
            default  :
            	*pointer_to_output_data = 0;      // Spase
                break;
        }   // End of switch.
        pointer_to_input_data++;
    }   // End of for.
}   // End of for MSU_MT10T8_convert_string_to_segment_code

void    MSU_MT10T8_convert_bin_to_string(uint8_t *pointer)
{
    data1 = (uint32_t)(*pointer);
    for (pointer_to_output_data = &char_string[0]; pointer_to_output_data < (&char_string[0] + size_of_bin); pointer_to_output_data++)
    {
    	*pointer_to_output_data = '0' + (((uint8_t)(data1)) & 0x01);
        data1 >>= 1;
    }
    char_string[8] = ' ';                                           // Replace insignificant zeros by space.
    char_string[9] = 'b';                                           // Set bin sign.
    return;
}   // End of MSU_MT10T8_convert_bin_to_string

void    MSU_MT10T8_convert_dec_to_string(uint32_t *pointer)
{
    data1 = *pointer;
    if ((sign == '-') && (data1 > max_positive_value))              // If sign == '-' then data is sing. If data1 > max_positive_value then data is negative.
        if (data1 > max_positive_value + min_negative_dec_value)    // If value < -999999999, then 
            data1 = min_negative_dec_value;                         // limit value by -999999999,
            else                                                    // else
                data1 &= negative_value_msk;                        // extract negative value.
        else
            sign = ' ';                                             // If data1 < max_positive_value then data is positive and sign is absent.

    pointer_to_output_data = &char_string[0];
    *pointer_to_output_data = '0';
    while ((pointer_to_output_data < &char_string[0] + size_of_dec) && (data1 > 0))
    {
        data2 = data1;
        data1 = (data1 / dec_num_sys_base);
        *pointer_to_output_data = '0' + (int8_t)(data2 - data1 * dec_num_sys_base);
        pointer_to_output_data++;
    }   // End of while
    if (pointer_to_output_data < &char_string[0] + size_of_dec)
        *pointer_to_output_data = sign;
    pointer_to_output_data++;
    while (pointer_to_output_data < &char_string[0] + size_of_dec)  // Replace insignificant zeros by space.
    {
        *pointer_to_output_data = ' ';
        pointer_to_output_data++;
    }   // End of while
    return;
}   // End of MSU_MT10T8_convert_dec_to_string

void    MSU_MT10T8_convert_hex_to_string(uint32_t *pointer)
{
    data1 = *pointer;
    if ((sign == '-') && (data1 > max_positive_value))              // If sign == '-' then data is sing.  If data1 > max_positive_value then data is negative.
        data1 &= negative_value_msk;                                // Extract negative value.
        else
            sign = ' ';                                             // If data1 > max_positive_value then data is positive and sign is absent.
    pointer_to_output_data = &char_string[0];
    *pointer_to_output_data = '0';
    while ((pointer_to_output_data < (&char_string[0] + size_of_hex)) && (data1 > 0))
    {
        switch ((uint8_t)(data1 & low_tetrad_msk))
        {
            case 0x0F   :
                *pointer_to_output_data = 'F';
                break;
            case 0x0E   :
                *pointer_to_output_data = 'E';
                break;
            case 0x0D   :
                *pointer_to_output_data = 'd';
                break;
            case 0x0C   :
                *pointer_to_output_data = 'c';
                break;
            case 0x0B   :
                *pointer_to_output_data = 'b';
                break;
            case 0x0A   :
                *pointer_to_output_data = 'A';
                break;
            default     :
                *pointer_to_output_data = '0' + ((uint8_t)(data1 & low_tetrad_msk));
        }   // End of switch.
        data1 >>= 4;
        pointer_to_output_data++;
    }   // End of while.
    *pointer_to_output_data = sign;
    pointer_to_output_data++;
    while (pointer_to_output_data < (&char_string[0] + MT10T8_dimention - 1))         // Replace insignificant zeros by space.
    {
        *pointer_to_output_data = ' ';
        pointer_to_output_data++;
    }   // End of while.
    *pointer_to_output_data = 'h';                                                    // Set hex sign.
	return;
}   // End of MSU_MT10T8_convert_hex_to_string

