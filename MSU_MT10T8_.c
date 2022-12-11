#include "MSU_MT10T8.h"

#define size_of_bin			        8		    // 8 character cell for binary number.
#define size_of_dec			        10		    // 10 character cell for decimal number.
#define size_of_hex			        8		    // 8 character cell for hexadecimal number.
#define max_positive_value	        0x80000000
#define negative_value_msk	        0x7FFFFFFF  // Negative value extraction mask.
#define min_negative_dec_value	    999999999   // -999999999 - the minimum value that can be displayed on the 10 character cell LCI.
#define low_tetrad_msk	            0x0F        // Low tetrad extraction mask.
#define dec_num_sys_base            10          // The base of the decimal number system.

#define segment_code_0              0xEE
#define segment_code_1              0x60
#define segment_code_2              0x2F
#define segment_code_3              0x6D
#define segment_code_4              0xE1
#define segment_code_5              0xCD
#define segment_code_6              0xCF
#define segment_code_7              0x68
#define segment_code_8              0xEF
#define segment_code_9              0xED
#define A_segment_code              0xEB
#define b_segment_code              0xC7
#define c_segment_code              0x07
#define d_segment_code              0x67
#define E_segment_code              0x8F
#define F_segment_code              0x8B
#define minus_segment_code          0x01
#define degree_segment_code         0xA9
#define J_segment_code              0x6E
#define G_segment_code              0xCE
#define h_segment_code              0xC3
#define L_segment_code              0x86
#define n_segment_code              0x43
#define o_segment_code              0x47
#define P_segment_code              0xAB
#define r_segment_code              0x03
#define U_segment_code              0xE6
#define X_segment_code              0xE3
#define Y_segment_code              0xE5
#define equal_segment_code          0x05
#define underline_segment_code      0x04
#define exclamation_segment_code    0x70
#define roof_segment_code           0x41
#define spase_segment_code          0x00

typedef  struct
{
    MSU_MT10T8_init_t   *pointer_to_pins;
    uint8_t             *pointer_to_WR2;
    void                *pointer_to_data;
    uint8_t             char_string[MT10T8_dimention];
    uint8_t             segment_code[MT10T8_dimention];
    uint8_t             sign;
} data_fild_t;

static	void        MSU_MT10T8_init_LCI(data_fild_t *pointer);
static	void        MSU_MT10T8_out_segment_code(data_fild_t *pointer);
static	uint8_t     *MSU_MT10T8_convert_string_to_segment_code(data_fild_t *pointer);
static	uint8_t		*MSU_MT10T8_convert_bin_to_string(data_fild_t *pointer);
static	uint8_t		*MSU_MT10T8_convert_dec_to_string(data_fild_t *pointer);
static	uint8_t		*MSU_MT10T8_convert_hex_to_string(data_fild_t *pointer);

uint8_t		*MSU_MT10T8(void *pointer, const MT10T8_purpos_t purpose)
{
	static data_fild_t     data_fild;
    uint8_t         i;

    data_fild.sign = ' ';                                           // by default sata is unsign.
    switch  (purpose)
    {
        case init_pins  :
            data_fild.pointer_to_pins = pointer;
            data_fild.pointer_to_WR2 = data_fild.pointer_to_pins->WR2;
            break;
        case choise_LCI  :
            data_fild.pointer_to_WR2 = pointer;
            break;
        case init_LCI  :
            MSU_MT10T8_init_LCI(&data_fild);
            for (i=0; i<MT10T8_dimention; i++)
                data_fild.segment_code[i] = 0;
            data_fild.pointer_to_data = &(data_fild.segment_code[0]);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case out_segment_code  :
            data_fild.pointer_to_data = pointer;
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case out_string  :
            data_fild.pointer_to_data = pointer;
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_string  :
            data_fild.pointer_to_data = pointer;
            return MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            break;
        case out_bin  :
            data_fild.pointer_to_data = pointer;
            data_fild.pointer_to_data = MSU_MT10T8_convert_bin_to_string(&data_fild);
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_bin  :
            data_fild.pointer_to_data = pointer;
            return MSU_MT10T8_convert_bin_to_string(&data_fild);
            break;
        case out_unsign_dec  :
            data_fild.pointer_to_data = pointer;
            data_fild.pointer_to_data = MSU_MT10T8_convert_dec_to_string(&data_fild);
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_unsign_dec  :
            data_fild.pointer_to_data = pointer;
            return  MSU_MT10T8_convert_dec_to_string(&data_fild);
            break;
        case out_sign_dec  :
            data_fild.pointer_to_data = pointer;
            data_fild.sign = '-';
            data_fild.pointer_to_data = MSU_MT10T8_convert_dec_to_string(&data_fild);
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_sign_dec  :
            data_fild.pointer_to_data = pointer;
            data_fild.sign = '-';
            return  MSU_MT10T8_convert_dec_to_string(&data_fild);
            break;
        case out_unsign_hex  :
            data_fild.pointer_to_data = pointer;
            data_fild.pointer_to_data = MSU_MT10T8_convert_hex_to_string(&data_fild);
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_unsign_hex  :
            data_fild.pointer_to_data = pointer;
            return  MSU_MT10T8_convert_hex_to_string(&data_fild);
            break;
        case out_sign_hex  :
            data_fild.pointer_to_data = pointer;
            data_fild.sign = '-';
            data_fild.pointer_to_data = MSU_MT10T8_convert_hex_to_string(&data_fild);
            data_fild.pointer_to_data = MSU_MT10T8_convert_string_to_segment_code(&data_fild);
            MSU_MT10T8_out_segment_code(&data_fild);
            break;
        case back_sign_hex  :
            data_fild.pointer_to_data = pointer;
            data_fild.sign = '-';
            return  MSU_MT10T8_convert_hex_to_string(&data_fild);
            break;
    }   // End of switch purpose.
return &(data_fild.segment_code[0]);
}   // End of MSU_MT10T8.

void    MSU_MT10T8_init_LCI(data_fild_t *pointer)       // Send signals to LCI for unlock.
{
    *(pointer->pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer->pointer_to_WR2) = 0;          // WR2 = 0. LCI enable.
    *(pointer->pointer_to_pins->D0) = 1;     // DB0 = 1.
    *(pointer->pointer_to_pins->D1) = 1;     // DB1 = 1.
    *(pointer->pointer_to_pins->D2) = 1;     // DB2 = 1.
    *(pointer->pointer_to_pins->D3) = 1;     // DB3 = 1.
    *(pointer->pointer_to_pins->A0) = 0;     // A0 = 0.  DB3-DB0 = 0xF - LCI register address.
    *(pointer->pointer_to_pins->WR1) = 1;    // WR1 = 1. Send register address 0xF to LCI.
    *(pointer->pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer->pointer_to_pins->D1) = 0;     // DB1 = 0.
    *(pointer->pointer_to_pins->D2) = 0;     // DB2 = 0.
    *(pointer->pointer_to_pins->D3) = 0;     // DB3 = 0.
    *(pointer->pointer_to_pins->A0) = 1;     // A0 = 1.  DB3-DB0 = 0x1 - data.
    *(pointer->pointer_to_pins->WR1) = 1;    // WR1 = 1, Send data 0x1 to LCI register 0xF for LCI unlock.
    *(pointer->pointer_to_pins->WR1) = 0;    // WR1 = 0. Don't send signals to LCI.
    *(pointer->pointer_to_WR2) = 1;          // WR2 = 1. LCI disable.
    return;
}   // End of MSU_MT10T8_init_LCI.
      
void    MSU_MT10T8_out_segment_code(data_fild_t *pointer)
{
    uint8_t     *pointer_to_data;

    *(pointer->pointer_to_pins->WR1) = 0;        // WR1 = 0. Don't send signals to LCI.
    *(pointer->pointer_to_WR2) = 0;              // WR2 = 0. LCI enable.
    *(pointer->pointer_to_pins->D0) = 0;         // DB0 = 0.
    *(pointer->pointer_to_pins->D1) = 0;         // DB1 = 0.
    *(pointer->pointer_to_pins->D2) = 0;         // DB2 = 0.
    *(pointer->pointer_to_pins->D3) = 0;         // DB3 = 0.
    *(pointer->pointer_to_pins->A0) = 0;         // A0 = 0.  DB3-DB0 = 0x0 - LCI register address.
    *(pointer->pointer_to_pins->WR1) = 1;        // WR1 = 1. Send register address 0x0 to LCI.
    *(pointer->pointer_to_pins->WR1) = 0;        // WR1 = 0. Don't send signals to LCI.
    *(pointer->pointer_to_pins->A0) = 1;         // A0 = 1.  DB3-DB0 = 0x1 - data.
    for (pointer_to_data = (uint8_t*)(pointer->pointer_to_data) + MT10T8_dimention-1; pointer_to_data > (uint8_t*)(pointer->pointer_to_data) - 1; pointer_to_data--)
    {
        *(pointer->pointer_to_pins->D0) = (*pointer_to_data) & 0x01;            // DB0.
        *(pointer->pointer_to_pins->D1) = ((*pointer_to_data) >> 1) & 0x01;     // DB1.
        *(pointer->pointer_to_pins->D2) = ((*pointer_to_data) >> 2) & 0x01;     // DB2.
        *(pointer->pointer_to_pins->D3) = ((*pointer_to_data) >> 3) & 0x01;     // DB3.
        *(pointer->pointer_to_pins->WR1) = 1;                                   // WR1 = 1. Send low tetrad data to LCI register.
        *(pointer->pointer_to_pins->WR1) = 0;                                   // WR1 = 0. Don't send signals to LCI.
        *(pointer->pointer_to_pins->D0) = ((*pointer_to_data) >> 4) & 0x01;     // DB0.
        *(pointer->pointer_to_pins->D1) = ((*pointer_to_data) >> 5) & 0x01;     // DB1.
        *(pointer->pointer_to_pins->D2) = ((*pointer_to_data) >> 6) & 0x01;     // DB2.
        *(pointer->pointer_to_pins->D3) = ((*pointer_to_data) >> 7) & 0x01;     // DB3.
        *(pointer->pointer_to_pins->WR1) = 1;                                   // WR1 = 1. Send higt tetrad data to LCI register. Increment LCI register address automatically.
        *(pointer->pointer_to_pins->WR1) = 0;                                   // WR1 = 0. Don't send signals to LCI.
    }
    *(pointer->pointer_to_WR2) = 1;                                             // WR2 = 1. LCI disable.
    return;
}   // End of MT10T8_out_segment_code.

uint8_t     *MSU_MT10T8_convert_string_to_segment_code(data_fild_t *pointer)
{
    uint8_t     i;

    for (i = 0; i < MT10T8_dimention; i++)
    {
        switch  (*((uint8_t*)pointer->pointer_to_data + i))
        {
            case 48  :
            	pointer->segment_code[i] = segment_code_0;   // 0
                break;
            case 49  :
            	pointer->segment_code[i] = segment_code_1;   // 1
                break;
            case 50  :
            	pointer->segment_code[i] = segment_code_2;   // 2
                break;
            case 51  :
            	pointer->segment_code[i] = segment_code_3;   // 3
                break;
            case 52  :
            	pointer->segment_code[i] = segment_code_4;   // 4
                break;
            case 53  :
            	pointer->segment_code[i] = segment_code_5;   // 5
                break;
            case 54  :
            	pointer->segment_code[i] = segment_code_6;   // 6
                break;
            case 55  :
            	pointer->segment_code[i] = segment_code_7;   // 7
                break;
            case 56  :
            	pointer->segment_code[i] = segment_code_8;   // 8
                break;
            case 57  :
            	pointer->segment_code[i] = segment_code_9;   // 9
                break;
            case 65  :
            	pointer->segment_code[i] = A_segment_code;   // A
                break;
            case 98  :
            	pointer->segment_code[i] = b_segment_code;   // b
                break;
            case 99  :
            	pointer->segment_code[i] = c_segment_code;   // c
                break;
            case 100 :
            	pointer->segment_code[i] = d_segment_code;   // d
                break;
            case 69  :
            	pointer->segment_code[i] = E_segment_code;   // E
                break;
            case 70  :
            	pointer->segment_code[i] = F_segment_code;   // F
                break;
            case 45  :
            	pointer->segment_code[i] = minus_segment_code;   // -
                break;
            case 42  :
            	pointer->segment_code[i] = degree_segment_code;   // *
                break;
            case 74  :
            	pointer->segment_code[i] = J_segment_code;   // J
                break;
            case 71  :
            	pointer->segment_code[i] = G_segment_code;   // G
                break;
            case 104 :
            	pointer->segment_code[i] = h_segment_code;   // h
                break;
            case 76  :
            	pointer->segment_code[i] = L_segment_code;   // L
                break;
            case 110 :
            	pointer->segment_code[i] = n_segment_code;   // n
                break;
            case 111 :
            	pointer->segment_code[i] = o_segment_code;   // o
                break;
            case 80  :
            	pointer->segment_code[i] = P_segment_code;   // P
                break;
            case 114 :
            	pointer->segment_code[i] = r_segment_code;   // r
                break;
            case 85  :
            	pointer->segment_code[i] = U_segment_code;   // U
                break;
            case 88  :
            	pointer->segment_code[i] = X_segment_code;   // X
                break;
            case 89  :
            	pointer->segment_code[i] = Y_segment_code;   // Y
                break;
            case 61  :
            	pointer->segment_code[i] = equal_segment_code;   // =
                break;
            case 95  :
            	pointer->segment_code[i] = underline_segment_code;   // _
                break;
            case 33  :
            	pointer->segment_code[i] = exclamation_segment_code;   // !
                break;
            case 94  :
            	pointer->segment_code[i] = roof_segment_code;   // ^
                break;
            default  :
            	pointer->segment_code[i] = spase_segment_code;      // Spase
                break;
        }   // End of switch.
    }   // End of for.
    return &(pointer->segment_code[0]);
}   // End of for MSU_MT10T8_convert_string_to_segment_code

uint8_t     *MSU_MT10T8_convert_bin_to_string(data_fild_t *pointer)
{
    uint8_t     i;

    for (i = 0; i < size_of_bin; i++)
    pointer->char_string[i] = '0' + (((*((uint8_t*)(pointer->pointer_to_data))) >> i) & 0x01);
    (pointer->char_string[8]) = ' ';                                // Replace insignificant zeros by space.
    (pointer->char_string[9]) = 'b';                                // Set bin sign.
    return &(pointer->char_string[0]);
}   // End of MSU_MT10T8_convert_bin_to_string

uint8_t     *MSU_MT10T8_convert_dec_to_string(data_fild_t *pointer)
{
    uint8_t     *pointer_to_output_data;
    uint32_t    data1, data2;
    uint8_t     sign;

    data1 = *((uint32_t*)(pointer->pointer_to_data));
    sign = pointer->sign;
    if ((sign == '-') && (data1 > max_positive_value))              // If sign == '-' then data is sing. If data1 > max_positive_value then data is negative.
        if (data1 > max_positive_value + min_negative_dec_value)    // If value < -999999999, then 
            data1 = min_negative_dec_value;                         // limit value by -999999999,
            else                                                    // else
                data1 &= negative_value_msk;                        // extract negative value.
        else
            sign = ' ';                                             // If data1 < max_positive_value then data is positive and sign is absent.

    pointer_to_output_data = &(pointer->char_string[0]);
    *pointer_to_output_data = '0';
    while ((pointer_to_output_data < &(pointer->char_string[0]) + size_of_dec) && (data1 > 0))
    {
        data2 = data1;
        data1 = (data1 / dec_num_sys_base);
        *pointer_to_output_data = '0' + (int8_t)(data2 - data1 * dec_num_sys_base);
        pointer_to_output_data++;
    }   // End of while
    if (pointer_to_output_data < &(pointer->char_string[0]) + size_of_dec)
        *pointer_to_output_data = sign;
    pointer_to_output_data++;
    while (pointer_to_output_data < &(pointer->char_string[0]) + size_of_dec)  // Replace insignificant zeros by space.
    {
        *pointer_to_output_data = ' ';
        pointer_to_output_data++;
    }   // End of while
    return &(pointer->char_string[0]);
}   // End of MSU_MT10T8_convert_dec_to_string

uint8_t     *MSU_MT10T8_convert_hex_to_string(data_fild_t *pointer)
{
    uint8_t     *pointer_to_output_data;
    uint32_t    data1;
    uint8_t     sign;

    data1 = *((uint32_t*)(pointer->pointer_to_data));
    sign = pointer->sign;
    if ((sign == '-') && (data1 > max_positive_value))              // If sign == '-' then data is sing.  If data1 > max_positive_value then data is negative.
        data1 &= negative_value_msk;                                // Extract negative value.
        else
            sign = ' ';                                             // If data1 > max_positive_value then data is positive and sign is absent.
    pointer_to_output_data = &(pointer->char_string[0]);
    *pointer_to_output_data = '0';
    while ((pointer_to_output_data < &pointer->char_string[0] + size_of_hex) && (data1 > 0))
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
    while (pointer_to_output_data < (&(pointer->char_string[0]) + MT10T8_dimention - 1))         // Replace insignificant zeros by space.
    {
        *pointer_to_output_data = ' ';
        pointer_to_output_data++;
    }   // End of while.
    *pointer_to_output_data = 'h';                                                    // Set hex sign.
	return  &(pointer->char_string[0]);
}   // End of MSU_MT10T8_convert_hex_to_string

