#ifndef LOG_H
#define LOG_H

void InitUSART();
void uart_putc(unsigned char);
void uart_puts(char *);

void rs232_log();

#endif // LOG_H