#pragma once

#define LOG_ON

#include <stdint.h>
#include <stdbool.h>

#if defined (__AVR__) || defined (__AVR_ARCH__)

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>

#endif

#if defined (__linux) || defined (__linux__) || defined (__gnu_linux__) \
|| defined (__unix) || defined (__unix__) \
|| defined (__x86_64) || defined (__x86_64__)

#include <stddef.h>

#define PROGMEM
#define EEMEM
#define PSTR(x) x

#define eeprom_is_ready() 1
#define eeprom_busy_wait() do {} while (!eeprom_is_ready())
uint8_t eeprom_read_byte (const uint8_t *__p);
uint16_t eeprom_read_word (const uint16_t *__p);
uint32_t eeprom_read_dword (const uint32_t *__p);
float eeprom_read_float (const float *__p);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_write_byte (uint8_t *__p, uint8_t __value);
void eeprom_write_word (uint16_t *__p, uint16_t __value);
void eeprom_write_dword (uint32_t *__p, uint32_t __value);
void eeprom_write_float (float *__p, float __value);
void eeprom_write_block (const void *__src, void *__dst, size_t __n);
void eeprom_update_byte (uint8_t *__p, uint8_t __value);
void eeprom_update_word (uint16_t *__p, uint16_t __value);
void eeprom_update_dword (uint32_t *__p, uint32_t __value);
void eeprom_update_float (float *__p, float __value);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

#define _EEPUT(addr, val)   eeprom_write_byte ((uint8_t *)(addr), (uint8_t)(val))
#define __EEPUT(addr, val)  eeprom_write_byte ((uint8_t *)(addr), (uint8_t)(val))
#define _EEGET(var, addr)  (var) = eeprom_read_byte ((const uint8_t *)(addr))
#define __EEGET(var, addr) (var) = eeprom_read_byte ((const uint8_t *)(addr))

#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

char     pgm_read_byte(const char *_address_short);
// uint8_t  pgm_read_byte(const uint8_t *_address_short);
uint16_t pgm_read_word(const uint16_t *_address_short);
uint32_t pgm_read_dword(const uint32_t *_address_short);


#define memchr_P        memchr
#define memcmp_P        memcmp
#define memccpy_P       memccpy
#define memcpy_P        memcpy
#define memmem_P        memmem
#define memrchr_P       memrchr
#define strcat_P        strcat
#define strchr_P        strchr
#define strchrnul_P     strchrnul
#define strcmp_P        strcmp
#define strcpy_P        strcpy
#define strcasecmp_P    strcasecmp
#define strcasestr_P    strcasestr
#define strcspn_P       strcspn
#define strlcat_P       strlcat
#define strlcpy_P       strlcpy
#define strnlen_P       strnlen
#define strncmp_P       strncmp
#define strncasecmp_P   strncasecmp
#define strncat_P       strncat
#define strncpy_P       strncpy
#define strpbrk_P       strpbrk
#define strrchr_P       strrchr
#define strsep_P        strsep
#define strspn_P        strspn
#define strstr_P        strstr
#define strtok_P        strtok
#define strtok_rP       strtok
#define strlen_PF       strlen
#define strnlen_PF      strnlen
#define memcpy_PF       memcpy
#define strcpy_PF       strcpy
#define strncpy_PF      strncpy
#define strcat_PF       strcat
#define strlcat_PF      strlcat
#define strncat_PF      strncat
#define strcmp_PF       strcmp
#define strncmp_PF      strncmp
#define strcasecmp_PF   strcasecmp
#define strncasecmp_PF  strncasecmp
#define strstr_PF       strstr
#define strlcpy_PF      strlcpy
#define memcmp_PF       memcmp
#define strlen_P        strlen
// --------------------------
#define vfprintf_P  vfprintf
#define printf_P    printf
#define sprintf_P   sprintf
#define snprintf_P  snprintf
#define vsprintf_P  vsprintf
#define vsnprintf_P vsnprintf
#define fprintf_P   fprintf
#define fputs_P     fputs
#define puts_P      puts
#define fscanf_P    fscanf
#define scanf_P     scanf
#define sscanf_P    sscanf
// --------------------------
#define sei()
#define cli()

#endif

void init();
uint32_t millis();
uint32_t uptime_sec();
uint32_t unixtime_get();
void unixtime_set(uint32_t unixtime);
bool elapsed_1sec();
bool elapsed_100ms();
void mcu_lcd_write4b(uint8_t value);
void mcu_lcd_insert(const uint8_t rs, const uint8_t value);

void Print_Space_N(const uint8_t size);
void HexDump(const void* data, const uint16_t size);

#define LCD stderr

void __gsm_on_off(bool on);

#ifdef LOG_ON
#define LOGGING(x) printf_P("\n%s:%d %s():\n", __FILE__, __LINE__, __func__); x
#else
#define LOGGING(x)
#endif

