/* MSU_MT10M8

    Функции управляют конвертацией данных и выводом в ЖКИ МТ10М8.
    Данные передаются через структуру, содержащую указатель на порт активации одного из ЖКИ, номер пина, флаг операции, значение и массив из 10 байт, в котором находится 10-и сегментный код.
    В программе CUBE32IDE надо присвоить выходам управления ЖКИ определённые имена для генерации соответствующих имён, использующихся в функциях:

    Пользовательское          Сгенерированное           Сгенерированный
         имя                    имя порта                 номер пина

       MT10T8_A0            MT10T8_A0_GPIO_Port         MT10T8_A0_Pin
       MT10T8_DB0           MT10T8_DB0_GPIO_Port        MT10T8_DB0_Pin
       MT10T8_DB1           MT10T8_DB1_GPIO_Port        MT10T8_DB1_Pin
       MT10T8_DB2           MT10T8_DB2_GPIO_Port        MT10T8_DB2_Pin
       MT10T8_DB3           MT10T8_DB3_GPIO_Port        MT10T8_DB3_Pin
       MT10T8_DB4           MT10T8_DB4_GPIO_Port        MT10T8_DB4_Pin
       MT10T8_WR1           MT10T8_WR1_GPIO_Port        MT10T8_WR1_Pin

    Данные передаются через указатель на структуру типа MSU_MT10T8_Struct_TypeDef. Значение флага указывает на тип операции:
     0 - инициализация.
     1 - вывод поля Data8[i] ЖКИ.
     2 - вывод поля с предварительной конвертацией в восьмисегментный код.
     3 - вывод битового поля, содержащегося в младшем байте MT10T8_Data.Data32.
     4 - вывод беззнакового числа, содержащегося в MT10T8_Data.Data32 в шестнадцатиричном формате.
     5 - вывод знакового числа, содержащегося в MT10T8_Data.Data32 в шестнадцатиричном формате.
     6 - вывод беззнакового числа, содержащегося в MT10T8_Data.Data32 в десятичном формате.
     7 - вывод знакового числа, содержащегося в MT10T8_Data.Data32 в десятичном формате.
     8 - очистка ЖКИ.
    Если установлен старший бит флага операции, то вывод осуществять в не в ЖКИ, а в MT10T8_Data.Data8[i].
     
    Пример использования:

    int8_t	i;
	MSU_MT10T8_Struct_TypeDef		LCI_Data;		// Объявление структуры данных для ЖКИ.

      LCI_Data.WR2_Port = MT10T8_WR2_GPIO_Port;		// Заполнение структуры именем управляющего порта, сгенерированного в CUBE32IDE, если порту присваивалось пользовательское имя MT10T8_WR2.
      LCI_Data.WR2_Pin = MT10T8_WR2_Pin;			// Заполнение структуры номером управляющего пина, сгенерированного в CUBE32IDE, если порту присваивалось пользовательское имя MT10T8_WR2.
      LCI_Data.flag  =  0;                          // Операция инициализации.
      for (i=0; i<10; i++)
	  {
		  LCI_Data.Data[i] = i;				        // Заполнение структуры данными для вывода. Поле LCI_Data.Data[i] получает значения от 0 до 9.
		  LCI_Data.Data[i] |= 0x80;			        // Дополнение к каждому знакоместу точки (по необходимости).
	  }
      MSU_MT10T8_Operation(&LCI_Data);				// Инициализация ЖКИ.
	  LCI_Data.flag  =  2;                          // Операция вывода поля с предварительной конвертацией в восьмисегментный код.
      MSU_MT10T8_Operation(&LCI_Data);				// Вывод данных на ЖКИ. На экране появится надпись: 9.8.7.6.5.4.3.2.1.0.
      LCI_Data.flag  =  1;                          // Операция вывода поля Data ЖКИ.
	  MSU_MT10T8_Operation(&LCI_Data);				// Вывод данных на ЖКИ. На экране зажгутся сегменты, указанные в поле LCI_Data.Data[i].
*/

#include "MSU_LCI_MT10T8.h"

// Основная функция работы с ЖКИ. Начало.
void    MSU_MT10T8_Operation(MSU_MT10T8_Struct_TypeDef* pt_struct)      											// Операции с конвертацией данных и выводом на экран ЖКИ.
{
    uint16_t    local_counter_1;
	uint8_t		data_string[LCI_MT10T8_size];                   												    // Строка данных.

    MSU_MT10T8_LCI_connect(pt_struct->WR2_Port, pt_struct->WR2_Pin);                                                // Присоединение ЖКИ к управляющим сигналам.
    switch  ((pt_struct->flag)&0x7F)
    {
    case 0  :   MSU_MT10T8_Init_LCI();                                                                              // Инициализация ЖКИ.
                MSU_MT10T8_Clear_LCI();                                                                             // Очистка ЖКИ.
                break;

    case 1  :   MSU_MT10T8_Out_segment_code_string_to_LCI(&(pt_struct->string[0]));                                 // Вывод строки сегментного кода в ЖКИ.
                break;

    case 2  :   MSU_MT10T8_Convert_data_string_to_segment_code_string(&(pt_struct->string[0]), &(data_string[0]));  // Конвертация строки данных в строку сегментного кода.
                if (((pt_struct->flag)&0x80) == 0x80)                                                               // если установлен старший бит флага операции, то
                    for (local_counter_1 = 0; local_counter_1 < LCI_MT10T8_size; local_counter_1++)
                        pt_struct->string[local_counter_1] = data_string[local_counter_1];                          // отослать данные в исходную структуру,
                    else                                                                                            // иначе
                        MSU_MT10T8_Out_segment_code_string_to_LCI(&(data_string[0]));                               // Вывод строки сегментного кода в ЖКИ.
                break;

    case 3  :   MSU_MT10T8_Convert_unsing_data_to_b_data_string((uint8_t)(pt_struct->data32), &(data_string[0]));   // Конвертация беззнакового числа в двоичном формате в строку данных.
                 break;

    case 4  :   MSU_MT10T8_Convert_unsing_data_to_h_data_string(pt_struct->data32, &(data_string[0]));              // Конвертация беззнакового числа в шестнадцатиричном формате в строку данных.
                break;

    case 5  :   MSU_MT10T8_Convert_sing_data_to_h_data_string(pt_struct->data32, &(data_string[0]));                // Конвертация знакового числа в шестнадцатиричном формате в строку данных.
                break;

    case 6  :   MSU_MT10T8_Convert_unsing_data_to_d_data_string(pt_struct->data32, &(data_string[0]));              // Конвертация беззнакового числа в десятичном формате в строку данных.
                break;

    case 7  :   MSU_MT10T8_Convert_sing_data_to_d_data_string(pt_struct->data32, &(data_string[0]));                // Конвертация знакового числа в десятичном формате в строку данных.
                break;

    case 8  :   MSU_MT10T8_Clear_LCI();                                                                             // Очистка ЖКИ.
    			break;
    }

    switch  ((pt_struct->flag)&0x7F)
    {
    case 3  :   case 4  :   case 5  :   case 6  :   case 7  :                                                           // Если производили конвертацию числа, то
                if (((pt_struct->flag)&0x80) == 0x80)                                                                   // если установлен старший бит флага операции, то
                    for (local_counter_1 = 0; local_counter_1 < LCI_MT10T8_size; local_counter_1++)
                        pt_struct->string[local_counter_1] = data_string[local_counter_1];                              // отослать данные в исходную структуру,
                    else                                                                                                // иначе
                    {
                        MSU_MT10T8_Convert_data_string_to_segment_code_string(&(data_string[0]), &(data_string[0]));    // Конвертация строки данных в строку сегментного кода.
                        MSU_MT10T8_Out_segment_code_string_to_LCI(&(data_string[0]));                                   // Вывод строки сегментного кода в ЖКИ.
                    }
                break;
    }

    MSU_MT10T8_LCI_disconnect(pt_struct->WR2_Port, pt_struct->WR2_Pin);                                         // Отсоединение ЖКИ от управляющих сигналов.
    return;
}
// Основная функция работы с ЖКИ. Конец.

void    MSU_MT10T8_LCI_connect(GPIO_TypeDef* WR2_Port, uint16_t WR2_Pin)            // Присоединение ЖКИ к управляющим сигналам.
{
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);        // WR1 = 0. Сброс тактирующего сигнала.
    HAL_GPIO_WritePin(WR2_Port, WR2_Pin, GPIO_PIN_RESET);                           // WR2 = 0, подключение соответствующего ЖКИ.
    return;
}

void    MSU_MT10T8_LCI_disconnect(GPIO_TypeDef* WR2_Port, uint16_t WR2_Pin)         // Отсоединение ЖКИ от управляющих сигналов.
{
    HAL_GPIO_WritePin(WR2_Port, WR2_Pin, GPIO_PIN_SET);                             // WR2 = 1, отключение соответствующего ЖКИ.
    return;
}

void    MSU_MT10T8_Send_sumbol_address_to_LCI(uint8_t sumbol_address)                                           // Отправка адреса в ЖКИ.
{
    HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, (GPIO_PinState) (sumbol_address & 0x01));           // DB0 = 0.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, (GPIO_PinState) ((sumbol_address >> 1) & 0x01));    // DB1 = 0.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, (GPIO_PinState) ((sumbol_address >> 2) & 0x01));    // DB2 = 0.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, (GPIO_PinState) ((sumbol_address >> 3) & 0x01));    // DB3 = 0.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_RESET);                                      // A0 = 0,  режим формирование адреса.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                      // WR1 = 1, запись адреса 0x00 в ЖКИ.
    HAL_Delay(1);                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                    // WR1 = 0.
    HAL_Delay(1);                                                                                               //          Задержка.
    return;
}

void    MSU_MT10T8_Send_sumbol_segment_code_to_LCI(uint8_t segment_code)                                        // Отправка тетрады сегментного кода в ЖКИ.
{
    HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, (GPIO_PinState) (segment_code & 0x01));             // DB0.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, (GPIO_PinState) ((segment_code >>1) & 0x01));       // DB1.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, (GPIO_PinState) ((segment_code >>2) & 0x01));       // DB2.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, (GPIO_PinState) ((segment_code >>3) & 0x01));       // DB3.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_SET);                                      	// A0 = 1,  режим формирование данных.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                      // WR1 = 1, запись данных младшей тетрады.
    HAL_Delay(1);                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                    // WR1 = 0.
    HAL_Delay(1);                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, (GPIO_PinState) ((segment_code >>4) & 0x01));       // DB0.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, (GPIO_PinState) ((segment_code >>5) & 0x01));       // DB1.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, (GPIO_PinState) ((segment_code >>6) & 0x01));       // DB2.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, (GPIO_PinState) ((segment_code >>7) & 0x01));       // DB3.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_SET);                                      	// A0 = 1,  режим формирование данных.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                      // WR1 = 1, запись данных младшей тетрады.
    HAL_Delay(1);                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                    // WR1 = 0.
    HAL_Delay(1);                                                                                               //          Задержка.
    return;
}

void    MSU_MT10T8_Init_LCI(void)                                                       // Инициализация ЖКИ.
{
    MSU_MT10T8_Send_sumbol_address_to_LCI(0x0F);                                        // Отправка в ЖКИ адреса "0x0F".
    MSU_MT10T8_Send_sumbol_segment_code_to_LCI(0x11);                                   // Отправка в ЖКИ младшей тетрады "0x01".
}

void    MSU_MT10T8_Clear_LCI(void)                                                      // Очистка ЖКИ.
{
    uint16_t     local_counter_1;

    MSU_MT10T8_Send_sumbol_address_to_LCI(0);
    for (local_counter_1 = 0; local_counter_1 < LCI_MT10T8_size; local_counter_1++)
    	MSU_MT10T8_Send_sumbol_segment_code_to_LCI(0);
    return;
}

void    MSU_MT10T8_Clear_segment_code_string(uint8_t* pt_output_string)                 // Очистка строки сегментного кода.
{
    uint16_t    local_counter_1;                                                        // Счетчик.

    for (local_counter_1 = 1; local_counter_1 < LCI_MT10T8_size; local_counter_1++)
        *(pt_output_string + local_counter_1) = 17;					                    // Очистка поля пустыми символами.
    return;
}


void    MSU_MT10T8_Out_segment_code_string_to_LCI(uint8_t* pt_output_string)                        // Вывод строки сегментного кода в ЖКИ.
{
    uint16_t     local_counter_1;

    MSU_MT10T8_Send_sumbol_address_to_LCI(0);
    for (local_counter_1 = LCI_MT10T8_size; local_counter_1 > 0; local_counter_1--)
        MSU_MT10T8_Send_sumbol_segment_code_to_LCI(*(pt_output_string + (local_counter_1 - 1)));    // Отправка сегментного кода в знакоместо ЖКИ.
    return;
}

uint8_t MSU_MT10T8_Convert_data_sumbol_to_segment_code_sumbol(uint8_t argument)             // Конвертация элемента данных в элемент сегментного кода.
{
    uint8_t      return_meaning = 17;
    #define	TS 34                                                                           // Размер массива знаков восьмисегментного кода ЖКИ.
// Таблица констант сегментного кода.
    static	int8_t	LCI_Table[TS] = {0xEE, 0x60, 0x2F, 0x6D, 0xE1, 0xCD, 0xCF, 0x68, 0xEF, 0xED, 0xEB, 0xC7, 0x07, 0x67, 0x8F, 0x8B, 0x01, 0x00, 0xA9, 0x6E, 0xCE, 0xC3, 0x86, 0x43, 0x47, 0xAB, 0x03, 0xE6, 0xE3, 0xE5, 0x05, 0x04, 0x70, 0x41};
//  Номер символа.                   0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    31    33
//  Символ.                          0     1     2     3     4     5     6     7     8     9     A     b     c     d     E     F     -           *     J     G     h     L     n     o     P     r     U     Х     Y     =     _     !     ^

    if (argument < TS)                                                                	    // Несуществующий элемент конвертируется в пустой знак.
    	return_meaning = ((LCI_Table[((argument)& 0x7F)])|(((argument) & 0x80) >> 3));      // Старший бит - зажигание сегмента h (точка).
    return return_meaning;
}

void    MSU_MT10T8_Convert_data_string_to_segment_code_string(uint8_t* pt_input_string, uint8_t* pt_output_string)  // Конвертация строки данных в строку сегментного кода.
{
    uint16_t    local_counter_1;                                                                                    // Счетчик.

    for (local_counter_1 = 0; local_counter_1 < LCI_MT10T8_size; local_counter_1++)
        *(pt_output_string + local_counter_1) = MSU_MT10T8_Convert_data_sumbol_to_segment_code_sumbol(*(pt_input_string + local_counter_1));
    return;
}

void    MSU_MT10T8_Convert_unsing_data_to_h_data_string(uint32_t unsing_data, uint8_t* pt_data_string)  // Конвертация беззнакового числа в шестнадцатиричном формате в строку данных.
{
    uint16_t    local_counter_1;                                                                        // Счетчик.
    uint32_t    data32;                                                                                 // Промежуточная.

    data32 = unsing_data;
    MSU_MT10T8_Clear_segment_code_string(pt_data_string);
    local_counter_1 = 0;
    *pt_data_string = 0;
    while ((local_counter_1 < 8) && (data32 > 0))                                                       // Пока элементы преобразования не закончатся , или не станут равными незначащим нулям.
    {
        *(pt_data_string + local_counter_1) = (((uint8_t) (data32)) & 0x0F);                            // Очередной элемент поля *pt_h_data_string равен значению очередной тетрады числа unsing_data.
        data32 = (data32 >> 4);
        local_counter_1++;
    }
    *(pt_data_string + (LCI_MT10T8_size - 1)) = 21;                                                     // Код "h" в старшем элементе поля *h_data_string.
    return;
}

void    MSU_MT10T8_Convert_unsing_data_to_d_data_string(uint32_t unsing_data, uint8_t* pt_data_string)  // Конвертация беззнакового числа в десятичном формате в строку данных.
{
    uint16_t    local_counter_1;                                                                        // Счетчик.
    uint32_t    data32_1, data32_2;                                                                     // Промежуточная.

    data32_1 = unsing_data;
    MSU_MT10T8_Clear_segment_code_string(pt_data_string);
    local_counter_1 = 0;
    *pt_data_string = 0;
    while ((local_counter_1 < LCI_MT10T8_size) && (data32_1 > 0))
    {
        data32_2 = data32_1;
        data32_1 = (data32_1 / 10);
        *(pt_data_string + local_counter_1) = (uint8_t) (data32_2 - data32_1*10);                       // Очередной элемент поля *pt_d_data_string равен значению очередной декады числа unsing_data.
        local_counter_1++;
    }
    return;
}

void    MSU_MT10T8_Convert_unsing_data_to_b_data_string(uint8_t unsing_data, uint8_t* pt_data_string)   // Конвертация беззнакового числа в двоичном формате в строку данных.
{
    uint16_t    local_counter_1;                                                                        // Счетчик.

    MSU_MT10T8_Clear_segment_code_string(pt_data_string);
    for (local_counter_1 = 0; local_counter_1 < 8; local_counter_1++)
        *(pt_data_string + local_counter_1) = ((unsing_data >> local_counter_1) & 0x01);                // Очередной элемент поля *pt_b_data_string равен значению очередного бита числа unsing_data.
    *(pt_data_string + (LCI_MT10T8_size - 1)) = 11;                                                     // Код "b" в старшем элементе поля *b_data_string.
    return;
}

void     MSU_MT10T8_Convert_sing_data_to_h_data_string(uint32_t sing_data, uint8_t* pt_data_string)     // Конвертация знакового числа в шестнадцатиричном формате в строку данных.
{
    uint8_t     local_sign = 17;                                                                        // Знак. По умолчанию положительный (пустое место).
    uint16_t    local_counter_1;                                                                        // Счетчик.
    uint32_t    data32;                                                                                 // Промежуточная.

    data32 = sing_data;
    if (data32 > 0x80000000)                                                                            // Если число отрицательное.
    {
        local_sign = 16;    								                                            // Определить знак числа.
        data32 &= 0x7FFFFFFF;                                                                           // Исключить знак числа из значения.
    }
    MSU_MT10T8_Convert_unsing_data_to_h_data_string(data32, pt_data_string);                            // Конвертация беззнакового числа в шестнадцатиричном формате в строку данных.
    local_counter_1 = 7;
    while ((local_counter_1 > 0)&&(*(pt_data_string + local_counter_1) == 17))                          // Искать первый ненулевой символ.
        local_counter_1--;
    *(pt_data_string + (local_counter_1 + 1)) = local_sign;                                             // Установить знак перед первой значащей цифрой.
    return;
}

void     MSU_MT10T8_Convert_sing_data_to_d_data_string(uint32_t sing_data, uint8_t* pt_data_string)     // Конвертация знакового числа в десятичном формате в строку данных.
{
    uint8_t     local_sign = 17;                                                                        // Знак. По умолчанию положительный (пустое место).
    uint16_t    local_counter_1;                                                                        // Счетчик.
    uint32_t    data32;                                                                                 // Промежуточная.

    data32 = sing_data;
    if (data32 > 0x80000000)                                                                            // Если число отрицательное.
    {
        local_sign = 16;    								                                            // Определить знак числа.
        data32 &= 0x7FFFFFFF;                                                                           // Исключить знак числа из значения.
        if (data32 > 999999999)   // Если отрицательное число превысит разрядность ЖК�?, то
            data32 = 999999999;   // установить максимальную величину числа.
    }
    MSU_MT10T8_Convert_unsing_data_to_d_data_string(data32, pt_data_string);                            // Конвертация беззнакового числа в десятичном формате в строку данных.
    local_counter_1 = LCI_MT10T8_size - 1;
                while ((local_counter_1 > 0)&&(*(pt_data_string + local_counter_1) == 17))              // Искать первый ненулевой символ.
                    local_counter_1--;
                 if (local_counter_1 < LCI_MT10T8_size - 1)                                             // Если число занимает не все знакоместа, то
                    *(pt_data_string + local_counter_1+1) = local_sign;      	                        // Установить знак перед первой значащей цифрой.
    return;
}
