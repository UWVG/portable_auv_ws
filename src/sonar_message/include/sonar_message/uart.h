#ifndef __UART_H_
#define __UART_H_

int uart_open(const char *dev, int a, int speed, char b, int c);
int uart_write(char *s, int len);
int uart_read(char *s,int maxlen);
void uart_close();
#endif