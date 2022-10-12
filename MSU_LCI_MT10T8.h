#define LCI_MT10T8_size   10                                                                                // Размер ЖКИ. 10 знакомест.

typedef struct	                					                                                        // Структура данных для ЖКИ.
{
    GPIO_TypeDef*       WR2_Port;                                                                           // Указатель на порт включения (размер 4 байта).
    uint16_t            WR2_Pin;                                                                            // Номер пина включения (2 байта).
    uint8_t             flag;                                                                               // Флаг операции.
    uint32_t		    data32;							                                                    // Число в 32-х битном формате.
    uint8_t             string[LCI_MT10T8_size];                                                            // Строка данных. Старший бит зажигает сегмент h (точка).
} MSU_MT10T8_Struct_TypeDef;                                                                                // Тип структуры данных для ЖКИ.

void        MSU_MT10T8_Operation(MSU_MT10T8_Struct_TypeDef*);                                               // Операции с конвертацией данных и выводом на экран ЖКИ.
void        MSU_MT10T8_LCI_connect(GPIO_TypeDef*, uint16_t);                                                // Присоединение ЖКИ к управляющим сигналам.
void        MSU_MT10T8_LCI_disconnect(GPIO_TypeDef*, uint16_t);                                             // Отсоединение ЖКИ от управляющих сигналов.
void        MSU_MT10T8_Send_sumbol_address_to_LCI(uint8_t);                                                 // Отправка адреса знакоместа в ЖКИ.
void        MSU_MT10T8_Send_sumbol_segment_code_to_LCI(uint8_t);                                            // Отправка сегментного кода в знакоместо ЖКИ.
void        MSU_MT10T8_Init_LCI(void);                                                                      // Инициализация ЖКИ.
void        MSU_MT10T8_Clear_LCI(void);                                                                     // Очистка ЖКИ.
void        MSU_MT10T8_Clear_segment_code_string(uint8_t*);                                                 // Очистка строки сегментного кода.
void        MSU_MT10T8_Out_segment_code_string_to_LCI(uint8_t*);                                            // Вывод строки сегментного кода в ЖКИ.
uint8_t     MSU_MT10T8_Convert_data_sumbol_to_segment_code_sumbol(uint8_t);                                 // Конвертация элемента данных в элемент сегментного кода.
void        MSU_MT10T8_Convert_data_string_to_segment_code_string(uint8_t*, uint8_t*);                      // Конвертация строки данных в строку сегментного кода.
void        MSU_MT10T8_Convert_unsing_data_to_h_data_string(uint32_t, uint8_t*);                            // Конвертация беззнакового числа в шестнадцатиричном формате в строку данных.
void        MSU_MT10T8_Convert_unsing_data_to_d_data_string(uint32_t, uint8_t*);                            // Конвертация беззнакового числа в десятичном формате в строку данных.
void        MSU_MT10T8_Convert_unsing_data_to_b_data_string(uint8_t, uint8_t*);                             // Конвертация беззнакового числа в двоичном формате в строку данных.
void        MSU_MT10T8_Convert_sing_data_to_h_data_string(uint32_t, uint8_t*);                              // Конвертация знакового числа в шестнадцатиричном формате в строку данных.
void        MSU_MT10T8_Convert_sing_data_to_d_data_string(uint32_t, uint8_t*);                              // Конвертация знакового числа в десятичном формате в строку данных.
