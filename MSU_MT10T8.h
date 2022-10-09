/* MSU_MT10M8

    Функции инициализируют и управляют ЖКИ МТ10М8.
    Данные передаются через структуру, содержащую указатель на порт активации одного из ЖКИ, номер пина и массив из 10 байт, в котором находится 8-и сегментный код.
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
    Если установлен старший бит флага операции, то вывод осуществять в не в ЖКИ, а в MT10T8_Data.Data8[i].
     
    Пример использования:

    int8_t	i;
	MSU_MT10T8_Struct_TypeDef		LCI_Data;		// Объявление структуры данных для ЖКИ.

      LCI_Data.WR2_Port = MT10T8_WR2_GPIO_Port;		// Заполнение структуры именем управляющего порта, сгенерированного в CUBE32IDE, если порту присваивалось пользовательское имя MT10T8_WR2.
      LCI_Data.WR2_Pin = MT10T8_WR2_Pin;			// Заполнение структуры номером управляющего пина, сгенерированного в CUBE32IDE, если порту присваивалось пользовательское имя MT10T8_WR2.
      LCI_Data.Flag  =  0;                          // Операция инициализации.
      for (i=0; i<10; i++)
	  {
		  LCI_Data.Data[i] = i;				        // Заполнение структуры данными для вывода. Поле LCI_Data.Data[i] получает значения от 0 до 9.
		  LCI_Data.Data[i] |= 0x80;			        // Дополнение к каждому знакоместу точки (по необходимости).
	  }
      MSU_MT10T8(&LCI_Data);					    // Инициализация ЖКИ.
	  LCI_Data.Flag  =  2;                          // Операция вывода поля с предварительной конвертацией в восьмисегментный код.
      MSU_MT10T8(&LCI_Data);				        // Вывод данных на ЖКИ. На экране появится надпись: 9.8.7.6.5.4.3.2.1.0.
      LCI_Data.Flag  =  1;                          // Операция вывода поля Data ЖКИ.
	  MSU_MT10T8(&LCI_Data);					    // Вывод данных на ЖКИ. На экране зажгутся сегменты, указанные в поле LCI_Data.Data[i].
*/

typedef struct	                					// Структура данных для ЖКИ.
{
    GPIO_TypeDef*   WR2_Port;                       // Указатель на порт включения (размер 4 байта).
    uint16_t        WR2_Pin;                        // Номер пина включения (2 байта).
    int8_t          Flag;                           // Флаг операции.
    uint32_t		Data32;							// Число в 32-х битном формате.
    int8_t          Data8[10];                      // Массив данных. Старший бит зажигает сегмент h (точка).
} MSU_MT10T8_Struct_TypeDef;                        // Тип структуры данных для ЖКИ.

void    MSU_MT10T8_F0(MSU_MT10T8_Struct_TypeDef*);
void    MSU_MT10T8_F1(MSU_MT10T8_Struct_TypeDef*);
void    MSU_MT10T8_F2(MSU_MT10T8_Struct_TypeDef*);
void    MSU_MT10T8_F3(MSU_MT10T8_Struct_TypeDef*);
void    MSU_MT10T8_F4(MSU_MT10T8_Struct_TypeDef*);
void    MSU_MT10T8_F6(MSU_MT10T8_Struct_TypeDef*);

// Основная функция работы с ЖКИ. Начало.
void    MSU_MT10T8(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
	MSU_MT10T8_Struct_TypeDef	MSU_MT10T8_Struct;		// Внутренняя структура.
    int8_t      Sign = 17;                              // Знак. По умолчанию положительный (пустое место).
    int8_t      i;                                      // Счетчик.

    MSU_MT10T8_Struct = *MT10T8_Struct;                 // Перенос структуры во внутреннюю область.

    switch  ((MSU_MT10T8_Struct.Flag)&0x7F)
    {
    case 0  :   MSU_MT10T8_F0(&MSU_MT10T8_Struct);              // провести инициализацию.
                for (i=0; i<10; i++)
                {
                    MSU_MT10T8_Struct.Data8[i] = 0;             // Очистка поля .Data8[10].
                }
                MSU_MT10T8_F1(&MSU_MT10T8_Struct);              // Вывод поля .Data8[10] ЖКИ.
                
    case 1  :   MSU_MT10T8_F1(&MSU_MT10T8_Struct);              // вывод поля .Data8[10] ЖКИ.
                break;
    case 2  :   MSU_MT10T8_F2(&MSU_MT10T8_Struct);              // провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ.
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)    // если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;         // отослать данные в исходную структуру,
                }   else                                        // иначе
                    {
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);      // вывод поля .Data8[10] ЖКИ.
                    }
                break;
    case 3  :   MSU_MT10T8_F3(&MSU_MT10T8_Struct);              // провести конвертацию младшего байта .Data32 в битовое поле .Data8[10].
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)    // Если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;         // отослать данные в исходную структуру,
                }   else                                        // иначе
                    {
                        MSU_MT10T8_F2(&MSU_MT10T8_Struct);      // провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ. 
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);      // Вывод поля .Data8[10] ЖКИ.
                    }
                break;
    case 4  :   MSU_MT10T8_F4(&MSU_MT10T8_Struct);              // Провести конвертацию беззнакового .Data32 в шестнадцатиричное поле .Data8[10].
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)    // Если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;         // отослать данные в исходную структуру,
                }   else                                        // иначе
                    {
                        MSU_MT10T8_F2(&MSU_MT10T8_Struct);      // провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ. 
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);      // Вывод поля .Data8[10] ЖКИ.
                    }
                break;
    case 5  :   if (MSU_MT10T8_Struct.Data32 > 0x80000000)
    			{
    				Sign = 16;    								// Определить знак числа.
    				MSU_MT10T8_Struct.Data32 &= 0x7FFFFFFF;     // Исключить знак числа из значения.
    			}
                MSU_MT10T8_F4(&MSU_MT10T8_Struct);              // Провести конвертацию беззнакового .Data32 в шестнадцатиричное поле .Data8[10].
                i = 7;
                while ((i>0)&&(MSU_MT10T8_Struct.Data8[i] == 17))   // Искать первый ненулевой символ.
                {
                    i--;
                }
                MSU_MT10T8_Struct.Data8[i+1] = Sign;            // Установить знак перед первой значащей цифрой.
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)    // Если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;         // отослать данные в исходную структуру,
                }   else                                        // иначе
                    {
                        MSU_MT10T8_F2(&MSU_MT10T8_Struct);      // провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ. 
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);      // Вывод поля .Data8[10] ЖКИ.
                    }
                break;
    case 6  :   MSU_MT10T8_F6(&MSU_MT10T8_Struct);              // Провести конвертацию беззнакового .Data32 в десятичное поле .Data8[10].
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)    // Если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;         // отослать данные в исходную структуру,
                }   else                                        // иначе
                    {
                        MSU_MT10T8_F2(&MSU_MT10T8_Struct);      // провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ. 
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);      // Вывод поля .Data8[10] ЖКИ.
                    }
                break;
    case 7  :   if (MSU_MT10T8_Struct.Data32 > 0x80000000)
				{
					Sign = 16;    								// Определить знак числа.
					MSU_MT10T8_Struct.Data32 &= 0x7FFFFFFF;     // Исключить знак числа из значения.
                	if (MSU_MT10T8_Struct.Data32 > 999999999)   // Если отрицательное число превысит разрядность ЖКИ, то
                	{
                		MSU_MT10T8_Struct.Data32 = 999999999;   // установить максимальную величину числа.
                	}
                }
                MSU_MT10T8_F6(&MSU_MT10T8_Struct);             	// Провести конвертацию беззнакового .Data32 в десятичное поле .Data8[10].
                i = 9;
                while ((i>0)&&(MSU_MT10T8_Struct.Data8[i] == 17))   // Искать первый ненулевой символ.
                {
                    i--;
                }
                if (i < 9)                                   	// Если число занимает не все знакоместа, то
                {
                    MSU_MT10T8_Struct.Data8[i+1] = Sign;      	// Установить знак перед первой значащей цифрой.
                }
                if (((MSU_MT10T8_Struct.Flag)&0x80) == 0x80)  	// Если установлен старший бит флага операции, то
                {
                    *MT10T8_Struct = MSU_MT10T8_Struct;        	// отослать данные в исходную структуру,
                }   else                                      	// иначе
                    {
                        MSU_MT10T8_F2(&MSU_MT10T8_Struct);     	// провести конвертацию .Data8[10] в восьми-сегментный код ЖКИ.
                        MSU_MT10T8_F1(&MSU_MT10T8_Struct);     	// Вывод поля .Data8[10] ЖКИ.
                    }
                break;                
    }
    return;
}
// Основная функция работы с ЖКИ. Конец

// Функция инициализации. Начало.
void   MSU_MT10T8_F0(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                // WR1 = 0. Сброс тактирующего сигнала.
    HAL_GPIO_WritePin(MT10T8_Struct->WR2_Port, MT10T8_Struct->WR2_Pin, GPIO_PIN_RESET);     // WR2 = 0, подключение соответствующего ЖКИ.
    HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, GPIO_PIN_SET);                  // DB0 = 1.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, GPIO_PIN_SET);                  // DB1 = 1.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, GPIO_PIN_SET);                  // DB2 = 1.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, GPIO_PIN_SET);                  // DB3 = 1.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_RESET);                  // A0 = 0,  режим формирование адреса.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                  // WR1 = 1, запись адреса 0x0F в ЖКИ.
    HAL_Delay(1);                                                                           //          Задержка.    
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                // WR1 = 0.
    HAL_Delay(1);                                                                           //          Задержка.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, GPIO_PIN_RESET);                // DB1 = 0.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, GPIO_PIN_RESET);                // DB2 = 0.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, GPIO_PIN_RESET);                // DB3 = 0.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_SET);                    // A0 = 1,  формировани данных.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                  // WR1 = 1, запись данных 0x01 в ЖКИ (по адресу 0x0F) для разблокировки.
    HAL_Delay(1);                                                                           // 			Задержка.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                // WR1 = 0. Сброс тактирующего сигнала.
    HAL_Delay(1);                                                                           //          Задержка.
    HAL_GPIO_WritePin(MT10T8_Struct->WR2_Port, MT10T8_Struct->WR2_Pin, GPIO_PIN_SET);       // WR2 = 1, отключение соответствующего ЖКИ.
    return;                                                                            		// Возвращение.
}
// Функция инициализации. Конец.

// Функция вывода поля Data8[i] ЖКИ. Начало.
void   MSU_MT10T8_F1(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
    int8_t  i;                                                                                                                  // Счетчик.

    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                                    // WR1 = 0. Сброс тактирующего сигнала.
    HAL_GPIO_WritePin(MT10T8_Struct->WR2_Port, MT10T8_Struct->WR2_Pin, GPIO_PIN_RESET);                                         // WR2 = 0, подключение соответствующего ЖКИ.
    HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, GPIO_PIN_RESET);                                                    // DB0 = 0.
    HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, GPIO_PIN_RESET);                                                    // DB1 = 0.
    HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, GPIO_PIN_RESET);                                                    // DB2 = 0.
    HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, GPIO_PIN_RESET);                                                    // DB3 = 0.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_RESET);                                                      // A0 = 0,  режим формирование адреса.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                                      // WR1 = 1, запись адреса 0x00 в ЖКИ.
    HAL_Delay(1);                                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                                    // WR1 = 0.
    HAL_Delay(1);                                                                                                               //          Задержка.
    HAL_GPIO_WritePin(MT10T8_A0_GPIO_Port, MT10T8_A0_Pin, GPIO_PIN_SET);                                                        // A0 = 1,  формировани данных (начиная с адреса 0x00).
    for (i=10; i>0; i--)                                                                                                        // Вывод данных поля в ЖКИ.
    {
        HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, (GPIO_PinState) (MT10T8_Struct->Data8[i-1] & 0x01));            // DB0.
        HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>1 ) & 0x01));     // DB1.
        HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>2 ) & 0x01));     // DB2.
        HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>3 ) & 0x01));     // DB3.
        HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                                  // WR1 = 1, запись данных младшей тетрады.
        HAL_Delay(1);                                                                                                           //          Задержка.
        HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                                // WR1 = 0.
        HAL_Delay(1);                                                                                                           //          Задержка.
        HAL_GPIO_WritePin(MT10T8_DB0_GPIO_Port, MT10T8_DB0_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>4 ) & 0x01));     // DB0.
        HAL_GPIO_WritePin(MT10T8_DB1_GPIO_Port, MT10T8_DB1_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>5 ) & 0x01));     // DB1.
        HAL_GPIO_WritePin(MT10T8_DB2_GPIO_Port, MT10T8_DB2_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>6 ) & 0x01));     // DB2.
        HAL_GPIO_WritePin(MT10T8_DB3_GPIO_Port, MT10T8_DB3_Pin, (GPIO_PinState) ((MT10T8_Struct->Data8[i-1] >>7 ) & 0x01));     // DB3.
        HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_SET);                                                  // WR1 = 1, запись данных старшей тетрады.
        HAL_Delay(1);                                                                                                           //			Задержка.
        HAL_GPIO_WritePin(MT10T8_WR1_GPIO_Port, MT10T8_WR1_Pin, GPIO_PIN_RESET);                                                // WR1 = 0.
        HAL_Delay(1);                                                                                                           //			Задержка.
    }
    HAL_GPIO_WritePin(MT10T8_Struct->WR2_Port, MT10T8_Struct->WR2_Pin, GPIO_PIN_SET);                                           // WR2 = 1, отключение соответствующего ЖКИ.
    return;                                                                                                                		// Возвращение.
}
// Функция вывода поля Data8[i] ЖКИ. Конец.

// Функция конвертации .Data8[10] в восьми-сегментный код ЖКИ. Начало.
void   MSU_MT10T8_F2(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
    int8_t  i;                                                                                                                  // Счетчик.
	#define	TS 34                                                                                                            	// Размер массива знаков восьмисегментного кода ЖКИ.
// Таблица констант сегментного кода.
    static	int8_t	LCI_Table[TS] = {0xEE, 0x60, 0x2F, 0x6D, 0xE1, 0xCD, 0xCF, 0x68, 0xEF, 0xED, 0xEB, 0xC7, 0x07, 0x67, 0x8F, 0x8B, 0x01, 0x00, 0xA9, 0x6E, 0xCE, 0xC3, 0x86, 0x43, 0x47, 0xAB, 0x03, 0xE6, 0xE3, 0xE5, 0x05, 0x04, 0x70, 0x41};
//  Номер символа.                   0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    31    33
//  Символ.                          0     1     2     3     4     5     6     7     8     9     A     b     c     d     E     F     -           *     J     G     h     L     n     o     P     r     U     Х     Y     =     _     !     ^

    for (i=0; i<10; i++)                                                                                                        // Конвертация данных из исходной структуры, расположенной по указателю MT10T8_Data
    {                                                                                                                           // в данные сегментного кода в служебной структуре MSU_MT10T8_Data.
        if (MT10T8_Struct->Data8[i]>(TS-1))
        {
            MT10T8_Struct->Data8[i] = 17;                                                                                       // Несуществующий элемент конвертируется в пустой знак.
        }
        MT10T8_Struct->Data8[i] = ((LCI_Table[((MT10T8_Struct->Data8[i])& 0x7F)])|(((MT10T8_Struct->Data8[i]) & 0x80) >> 3));	// Старший бит - зажигание сегмента h (точка).
    }                                                                                                               	        // Зажигание сегмента h (точка) при наличии старшего бита.
    return;                                                                                                                     // Возвращение.
}
// Функция конвертации .Data8[10] в восьми-сегментный код ЖКИ. Конец.

// Функция конвертации младшего байта .Data32 в битовое поле .Data8[10]. Начало.
void    MSU_MT10T8_F3(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)                 
{
    int8_t  i;                                                                          // Счетчик.

        for (i=0; i<8; i++)
        {
            MT10T8_Struct->Data8[i] = (int8_t)((MT10T8_Struct->Data32 >> i) & 0x01);
        }
        MT10T8_Struct->Data8[8] = 17;                                                   // Пустой знак.
        MT10T8_Struct->Data8[9] = 11;                                                   // Знак "b".
    return;                                                                        		// Возвращение.
}
// Функция конвертации младшего байта .Data32 в битовое поле .Data8[10]. Конец.

// Функция конвертации беззнакового .Data32 в шестнадцатиричное поле .Data8[10]. Начало.
void   MSU_MT10T8_F4(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
    uint32_t     Data32;                                                // Промежуточная.
    int8_t      i;                                                      // Счетчик.

        Data32 = MT10T8_Struct->Data32;
        for (i=1; i<9; i++)
        {
        	MT10T8_Struct->Data8[i] = 17;								// Очистка поля пустыми символами.
        }
        i = 0;
        MT10T8_Struct->Data8[0] = 0;                                    // Младший элемент поля .Data8[10] равен 0.
        while ((i < 8) && (Data32 > 0))                                 // Пока элементы преобразования не закончатся , или не станут равными незначащим нулям.
        {
            MT10T8_Struct->Data8[i] = (((int8_t) (Data32)) & 0x0F);
            Data32 = (Data32 >> 4);
            i++;
        }

        MT10T8_Struct->Data8[9] = 21;                                   // Код "h" в старшем поле .Data8[10].
    return;                                                        		// Возвращение.
}
// Функция конвертации беззнакового .Data32 в шестнадцатиричное поле .Data8[10]. Конец.

// Функция конвертации беззнакового .Data32 в десятичное поле .Data8[10]. Начало.
void   MSU_MT10T8_F6(MSU_MT10T8_Struct_TypeDef* MT10T8_Struct)
{
    uint32_t     Data32_1, Data32_2;                                        // Промежуточные.
    int8_t      i;                                                          // Счетчик.

        Data32_1 = MT10T8_Struct->Data32;
        for (i=1; i<10; i++)
        {
        	MT10T8_Struct->Data8[i] = 17;									// Очистка поля пустыми символами.
        }
        i = 0;
        MT10T8_Struct->Data8[0] = 0;                                        // Младший элемент поля .Data8[10] равен 0.
        while ((i < 10) && (Data32_1 > 0))                                  // Пока элементы преобразования не закончатся , или не станут равными незначащим нулям.
        {
            Data32_2 = Data32_1;
            Data32_1 = (Data32_1 / 10);
            MT10T8_Struct->Data8[i] = (int8_t) (Data32_2 - Data32_1*10);    // Остаток от деления на 10.
            i++;
        }
     return;                                                                // Возвращение.
}
// Функция конвертации беззнакового .Data32 в десятичное поле .Data8[10]. Конец.
