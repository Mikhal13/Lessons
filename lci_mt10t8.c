#include "lci_mt10t8.h"

#define SIZE_OF_BIN			        8		    // 8 character cell for binary number.
#define SIZE_OF_DEC			        10		    // 10 character cell for decimal number.
#define SIZE_OF_HEX			        8		    // 8 character cell for hexadecimal number.
#define MAX_POSITIVE_VALUE	        0x80000000
#define NEGATIVE_VALUE_MSK	        0x7FFFFFFF  // Negative value extraction mask.
#define MIN_NEGATIVE_DEC_VALUE	    999999999   // -999999999 - the minimum value that can be displayed on the 10 character cell LCI.
#define LOW_TETRAD_MSK	            0x0F        // Low tetrad extraction mask.
#define DEC_NUM_SYS_BASE            10          // The base of the decimal number system.

#define SEGMENT_CODE_0              0xEE
#define SEGMENT_CODE_1              0x60
#define SEGMENT_CODE_2              0x2F
#define SEGMENT_CODE_3              0x6D
#define SEGMENT_CODE_4              0xE1
#define SEGMENT_CODE_5              0xCD
#define SEGMENT_CODE_6              0xCF
#define SEGMENT_CODE_7              0x68
#define SEGMENT_CODE_8              0xEF
#define SEGMENT_CODE_9              0xED
#define SEGMENT_CODE_A              0xEB
#define SEGMENT_CODE_b              0xC7
#define SEGMENT_CODE_c              0x07
#define SEGMENT_CODE_d              0x67
#define SEGMENT_CODE_E              0x8F
#define SEGMENT_CODE_F              0x8B
#define SEGMENT_CODE_MINUS          0x01
#define SEGMENT_CODE_DEGREE         0xA9
#define SEGMENT_CODE_J              0x6E
#define SEGMENT_CODE_G              0xCE
#define SEGMENT_CODE_h              0xC3
#define SEGMENT_CODE_L              0x86
#define SEGMENT_CODE_n              0x43
#define SEGMENT_CODE_o              0x47
#define SEGMENT_CODE_P              0xAB
#define SEGMENT_CODE_r              0x03
#define SEGMENT_CODE_U              0xE6
#define SEGMENT_CODE_X              0xE3
#define SEGMENT_CODE_Y              0xE5
#define SEGMENT_CODE_EQUAL          0x05
#define SEGMENT_CODE_UNDERLINE      0x04
#define SEGMENT_CODE_EXCLAMATION    0x70
#define SEGMENT_CODE_ROOF           0x41
#define SEGMENT_CODE_SPASE          0x00

typedef  struct
{
    lci_mt10t8_bus_t    *pt_pins;
    uint8_t             *pt_WR2;
    void                *pt_data;
    uint8_t             char_string[LCI_MT10T8_DIMENTION];
    uint8_t             segment_code[LCI_MT10T8_DIMENTION];
    uint8_t             sign;
} data_fild_t;

static	void        lci_mt10t8_init_lci(data_fild_t *pt_data);
static	void        lci_mt10t8_out_segment_code(data_fild_t *pt_data);
static	uint8_t     *lci_mt10t8_convert_string_to_segment_code(data_fild_t *pt_data);
static	uint8_t		*lci_mt10t8_convert_bin_to_string(data_fild_t *pt_data);
static	uint8_t		*lci_mt10t8_convert_dec_to_string(data_fild_t *pt_data);
static	uint8_t		*lci_mt10t8_convert_hex_to_string(data_fild_t *pt_data);

uint8_t		*lci_mt10t8(void *pt_data, lci_mt10t8_purpos_t purpose)
{
	static data_fild_t     data_fild;
    uint8_t         i;

    data_fild.sign = ' ';                                           // by default sata is unsign.
    data_fild.pt_data = pt_data;
    switch  (purpose)
    {
        case init_pins  :
            data_fild.pt_pins = pt_data;
            data_fild.pt_WR2 = data_fild.pt_pins->pt_WR2;
            break;
        case choise_lci  :
            data_fild.pt_WR2 = pt_data;
            break;
        case init_lci  :
            lci_mt10t8_init_lci(&data_fild);
            for (i=0; i<LCI_MT10T8_DIMENTION; i++)
                data_fild.segment_code[i] = 0;
            data_fild.pt_data = &(data_fild.segment_code[0]);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case out_segment_code  :
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case out_string  :
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_string  :
            return lci_mt10t8_convert_string_to_segment_code(&data_fild);
            break;
        case out_bin  :
            data_fild.pt_data = lci_mt10t8_convert_bin_to_string(&data_fild);
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_bin  :
            return lci_mt10t8_convert_bin_to_string(&data_fild);
            break;
        case out_unsign_dec  :
            data_fild.pt_data = lci_mt10t8_convert_dec_to_string(&data_fild);
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_unsign_dec  :
            return  lci_mt10t8_convert_dec_to_string(&data_fild);
            break;
        case out_sign_dec  :
            data_fild.sign = '-';
            data_fild.pt_data = lci_mt10t8_convert_dec_to_string(&data_fild);
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_sign_dec  :
            data_fild.sign = '-';
            return  lci_mt10t8_convert_dec_to_string(&data_fild);
            break;
        case out_unsign_hex  :
            data_fild.pt_data = lci_mt10t8_convert_hex_to_string(&data_fild);
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_unsign_hex  :
            return  lci_mt10t8_convert_hex_to_string(&data_fild);
            break;
        case out_sign_hex  :
            data_fild.sign = '-';
            data_fild.pt_data = lci_mt10t8_convert_hex_to_string(&data_fild);
            data_fild.pt_data = lci_mt10t8_convert_string_to_segment_code(&data_fild);
            lci_mt10t8_out_segment_code(&data_fild);
            break;
        case back_sign_hex  :
            data_fild.sign = '-';
            return  lci_mt10t8_convert_hex_to_string(&data_fild);
            break;
    }   // End of switch purpose.
return &(data_fild.segment_code[0]);
}   // End of lci_mt10t8.

void    lci_mt10t8_init_lci(data_fild_t *pt_data)       // Send signals to lci for unlock.
{
    *(pt_data->pt_pins->pt_WR1) = 0;        // pt_WR1 = 0. Don't send signals to lci.
    *(pt_data->pt_WR2) = 0;                 // WR2 = 0. lci enable.
    *(pt_data->pt_pins->pt_D0) = 1;      // DB0 = 1.
    *(pt_data->pt_pins->pt_D1) = 1;         // DB1 = 1.
    *(pt_data->pt_pins->pt_D2) = 1;         // DB2 = 1.
    *(pt_data->pt_pins->pt_D3) = 1;         // DB3 = 1.
    *(pt_data->pt_pins->pt_A0) = 0;         // pt_A0 = 0.  DB3-DB0 = 0xF - lci register address.
    *(pt_data->pt_pins->pt_WR1) = 1;        // pt_WR1 = 1. Send register address 0xF to lci.
    *(pt_data->pt_pins->pt_WR1) = 0;        // pt_WR1 = 0. Don't send signals to lci.
    *(pt_data->pt_pins->pt_D1) = 0;         // DB1 = 0.
    *(pt_data->pt_pins->pt_D2) = 0;         // DB2 = 0.
    *(pt_data->pt_pins->pt_D3) = 0;         // DB3 = 0.
    *(pt_data->pt_pins->pt_A0) = 1;         // pt_A0 = 1.  DB3-DB0 = 0x1 - data.
    *(pt_data->pt_pins->pt_WR1) = 1;        // pt_WR1 = 1, Send data 0x1 to lci register 0xF for lci unlock.
    *(pt_data->pt_pins->pt_WR1) = 0;        // pt_WR1 = 0. Don't send signals to lci.
    *(pt_data->pt_WR2) = 1;                 // WR2 = 1. lci disable.
    return;
}   // End of lci_mt10t8_init_lci.
      
void    lci_mt10t8_out_segment_code(data_fild_t *pt_data)
{
    uint8_t     *pt_data_internal;

    *(pt_data->pt_pins->pt_WR1) = 0;        // pt_WR1 = 0. Don't send signals to lci.
    *(pt_data->pt_WR2) = 0;                 // WR2 = 0. lci enable.
    *(pt_data->pt_pins->pt_D0) = 0;      // DB0 = 0.
    *(pt_data->pt_pins->pt_D1) = 0;         // DB1 = 0.
    *(pt_data->pt_pins->pt_D2) = 0;         // DB2 = 0.
    *(pt_data->pt_pins->pt_D3) = 0;         // DB3 = 0.
    *(pt_data->pt_pins->pt_A0) = 0;         // pt_A0 = 0.  DB3-DB0 = 0x0 - lci register address.
    *(pt_data->pt_pins->pt_WR1) = 1;        // pt_WR1 = 1. Send register address 0x0 to lci.
    *(pt_data->pt_pins->pt_WR1) = 0;        // pt_WR1 = 0. Don't send signals to lci.
    *(pt_data->pt_pins->pt_A0) = 1;         // pt_A0 = 1.  DB3-DB0 = 0x1 - data.
    for (pt_data_internal = (uint8_t*)(pt_data->pt_data) + LCI_MT10T8_DIMENTION-1; pt_data_internal > (uint8_t*)(pt_data->pt_data) - 1; pt_data_internal--)
    {
        *(pt_data->pt_pins->pt_D0) = (*pt_data_internal) & 0x01;         // DB0.
        *(pt_data->pt_pins->pt_D1) = ((*pt_data_internal) >> 1) & 0x01;     // DB1.
        *(pt_data->pt_pins->pt_D2) = ((*pt_data_internal) >> 2) & 0x01;     // DB2.
        *(pt_data->pt_pins->pt_D3) = ((*pt_data_internal) >> 3) & 0x01;     // DB3.
        *(pt_data->pt_pins->pt_WR1) = 1;                                    // pt_WR1 = 1. Send low tetrad data to lci register.
        *(pt_data->pt_pins->pt_WR1) = 0;                                    // pt_WR1 = 0. Don't send signals to lci.
        *(pt_data->pt_pins->pt_D0) = ((*pt_data_internal) >> 4) & 0x01;  // DB0.
        *(pt_data->pt_pins->pt_D1) = ((*pt_data_internal) >> 5) & 0x01;     // DB1.
        *(pt_data->pt_pins->pt_D2) = ((*pt_data_internal) >> 6) & 0x01;     // DB2.
        *(pt_data->pt_pins->pt_D3) = ((*pt_data_internal) >> 7) & 0x01;     // DB3.
        *(pt_data->pt_pins->pt_WR1) = 1;                                    // pt_WR1 = 1. Send higt tetrad data to lci register. Increment lci register address automatically.
        *(pt_data->pt_pins->pt_WR1) = 0;                                    // pt_WR1 = 0. Don't send signals to lci.
    }
    *(pt_data->pt_WR2) = 1;                                                 // WR2 = 1. lci disable.
    return;
}   // End of lci_mt10t8_out_segment_code.

uint8_t     *lci_mt10t8_convert_string_to_segment_code(data_fild_t *pt_data)
{
    uint8_t     i;

    for (i = 0; i < LCI_MT10T8_DIMENTION; i++)
    {
        switch  (*((uint8_t*)pt_data->pt_data + i))
        {
            case '0' :
            	pt_data->segment_code[i] = SEGMENT_CODE_0;   // 0
                break;
            case '1' :
            	pt_data->segment_code[i] = SEGMENT_CODE_1;   // 1
                break;
            case '2' :
            	pt_data->segment_code[i] = SEGMENT_CODE_2;   // 2
                break;
            case '3' :
            	pt_data->segment_code[i] = SEGMENT_CODE_3;   // 3
                break;
            case '4' :
            	pt_data->segment_code[i] = SEGMENT_CODE_4;   // 4
                break;
            case '5' :
            	pt_data->segment_code[i] = SEGMENT_CODE_5;   // 5
                break;
            case '6' :
            	pt_data->segment_code[i] = SEGMENT_CODE_6;   // 6
                break;
            case '7' :
            	pt_data->segment_code[i] = SEGMENT_CODE_7;   // 7
                break;
            case '8' :
            	pt_data->segment_code[i] = SEGMENT_CODE_8;   // 8
                break;
            case '9' :
            	pt_data->segment_code[i] = SEGMENT_CODE_9;   // 9
                break;
            case 'A' :
            	pt_data->segment_code[i] = SEGMENT_CODE_A;   // A
                break;
            case 'b' :
            	pt_data->segment_code[i] = SEGMENT_CODE_b;   // b
                break;
            case 'c' :
            	pt_data->segment_code[i] = SEGMENT_CODE_c;   // c
                break;
            case 'd' :
            	pt_data->segment_code[i] = SEGMENT_CODE_d;   // d
                break;
            case 'E' :
            	pt_data->segment_code[i] = SEGMENT_CODE_E;   // E
                break;
            case 'F' :
            	pt_data->segment_code[i] = SEGMENT_CODE_F;   // F
                break;
            case '-' :
            	pt_data->segment_code[i] = SEGMENT_CODE_MINUS;   // -
                break;
            case '*' :
            	pt_data->segment_code[i] = SEGMENT_CODE_DEGREE;   // *
                break;
            case 'J' :
            	pt_data->segment_code[i] = SEGMENT_CODE_J;   // J
                break;
            case 'G' :
            	pt_data->segment_code[i] = SEGMENT_CODE_G;   // G
                break;
            case 'h' :
            	pt_data->segment_code[i] = SEGMENT_CODE_h;   // h
                break;
            case 'L' :
            	pt_data->segment_code[i] = SEGMENT_CODE_L;   // L
                break;
            case 'n' :
            	pt_data->segment_code[i] = SEGMENT_CODE_n;   // n
                break;
            case 'o' :
            	pt_data->segment_code[i] = SEGMENT_CODE_o;   // o
                break;
            case 'P' :
            	pt_data->segment_code[i] = SEGMENT_CODE_P;   // P
                break;
            case 'r' :
            	pt_data->segment_code[i] = SEGMENT_CODE_r;   // r
                break;
            case 'U' :
            	pt_data->segment_code[i] = SEGMENT_CODE_U;   // U
                break;
            case 'X' :
            	pt_data->segment_code[i] = SEGMENT_CODE_X;   // X
                break;
            case 'Y' :
            	pt_data->segment_code[i] = SEGMENT_CODE_Y;   // Y
                break;
            case '=' :
            	pt_data->segment_code[i] = SEGMENT_CODE_EQUAL;   // =
                break;
            case '_' :
            	pt_data->segment_code[i] = SEGMENT_CODE_UNDERLINE;   // _
                break;
            case '!' :
            	pt_data->segment_code[i] = SEGMENT_CODE_EXCLAMATION;   // !
                break;
            case '^' :
            	pt_data->segment_code[i] = SEGMENT_CODE_ROOF;   // ^
                break;
            default  :
            	pt_data->segment_code[i] = SEGMENT_CODE_SPASE;      // Spase
                break;
        }   // End of switch.
    }   // End of for.
    return &(pt_data->segment_code[0]);
}   // End of lci_mt10t8_convert_string_to_segment_code

uint8_t     *lci_mt10t8_convert_bin_to_string(data_fild_t *pt_data)
{
    uint8_t     i;

    for (i = 0; i < SIZE_OF_BIN; i++)
    pt_data->char_string[i] = '0' + (((*((uint8_t*)(pt_data->pt_data))) >> i) & 0x01);
    (pt_data->char_string[8]) = ' ';                                // Replace insignificant zeros by space.
    (pt_data->char_string[9]) = 'b';                                // Set bin sign.
    return &(pt_data->char_string[0]);
}   // End of lci_mt10t8_convert_bin_to_string

uint8_t     *lci_mt10t8_convert_dec_to_string(data_fild_t *pt_data)
{
    uint8_t     *pt_output_data;
    uint32_t    data1, data2;
    uint8_t     sign;

    data1 = *((uint32_t*)(pt_data->pt_data));
    sign = pt_data->sign;
    if ((sign == '-') && (data1 > MAX_POSITIVE_VALUE))              // If sign == '-' then data is sing. If data1 > MAX_POSITIVE_VALUE then data is negative.
        if (data1 > MAX_POSITIVE_VALUE + MIN_NEGATIVE_DEC_VALUE)    // If value < -999999999, then 
            data1 = MIN_NEGATIVE_DEC_VALUE;                         // limit value by -999999999,
        else                                                        // else
            data1 &= NEGATIVE_VALUE_MSK;                            // extract negative value.
    else
            sign = ' ';                                             // If data1 < MAX_POSITIVE_VALUE then data is positive and sign is absent.

    pt_output_data = &(pt_data->char_string[0]);
    *pt_output_data = '0';
    while ((pt_output_data < &(pt_data->char_string[0]) + SIZE_OF_DEC) && (data1 > 0))
    {
        data2 = data1;
        data1 = (data1 / DEC_NUM_SYS_BASE);
        *pt_output_data = '0' + (int8_t)(data2 - data1 * DEC_NUM_SYS_BASE);
        pt_output_data++;
    }   // End of while
    if (pt_output_data < &(pt_data->char_string[0]) + SIZE_OF_DEC)
        *pt_output_data = sign;
    pt_output_data++;
    while (pt_output_data < &(pt_data->char_string[0]) + SIZE_OF_DEC)  // Replace insignificant zeros by space.
    {
        *pt_output_data = ' ';
        pt_output_data++;
    }   // End of while
    return &(pt_data->char_string[0]);
}   // End of lci_mt10t8_convert_dec_to_string

uint8_t     *lci_mt10t8_convert_hex_to_string(data_fild_t *pt_data)
{
    uint8_t     *pt_output_data;
    uint32_t    data1;
    uint8_t     sign;

    data1 = *((uint32_t*)(pt_data->pt_data));
    sign = pt_data->sign;
    if ((sign == '-') && (data1 > MAX_POSITIVE_VALUE))              // If sign == '-' then data is sing.  If data1 > max_positive_value then data is negative.
        data1 &= NEGATIVE_VALUE_MSK;                                // Extract negative value.
    else
        sign = ' ';                                                 // If data1 > max_positive_value then data is positive and sign is absent.
    pt_output_data = &(pt_data->char_string[0]);
    *pt_output_data = '0';
    while ((pt_output_data < &pt_data->char_string[0] + SIZE_OF_HEX) && (data1 > 0))
    {
        switch ((uint8_t)(data1 & LOW_TETRAD_MSK))
        {
            case 0x0F   :
                *pt_output_data = 'F';
                break;
            case 0x0E   :
                *pt_output_data = 'E';
                break;
            case 0x0D   :
                *pt_output_data = 'd';
                break;
            case 0x0C   :
                *pt_output_data = 'c';
                break;
            case 0x0B   :
                *pt_output_data = 'b';
                break;
            case 0x0A   :
                *pt_output_data = 'A';
                break;
            default     :
                *pt_output_data = '0' + ((uint8_t)(data1 & LOW_TETRAD_MSK));
        }   // End of switch.
        data1 >>= 4;
        pt_output_data++;
    }   // End of while.
    *pt_output_data = sign;
    pt_output_data++;
    while (pt_output_data < (&(pt_data->char_string[0]) + LCI_MT10T8_DIMENTION - 1))    // Replace insignificant zeros by space.
    {
        *pt_output_data = ' ';
        pt_output_data++;
    }   // End of while.
    *pt_output_data = 'h';                                                              // Set hex sign.
	return  &(pt_data->char_string[0]);
}   // End of lci_mt10t8_convert_hex_to_string

