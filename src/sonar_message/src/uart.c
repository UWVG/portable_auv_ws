#include "sonar_message/uart.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static int fd;
int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop){

       struct termios newtio,oldtio;

       if(tcgetattr(fd,&oldtio)!=0){//保存测试现有串口参数设置，在这里如果串口号出错，会有相关的出错信息

            perror("SetupSerial 1");

            return -1;

        }

        bzero(&newtio,sizeof(newtio));//设置字符大小

        newtio.c_cflag |= CLOCAL |CREAD;  //通过位掩码的方式激活本地连接和接受使能选项

        newtio.c_iflag &= ~CSIZE;

        switch(nBits){//设置停止位

        case 7:

                  newtio.c_cflag |= CS7;

                  break;

        case 8:

                  newtio.c_cflag |=CS8;

                  break;

        }

        switch(nEvent){//设置奇偶效验位

        case 'o': //奇数

                   newtio.c_cflag |= PARENB;

                   newtio.c_cflag |=PARODD;

                   newtio.c_iflag |=(INPCK | ISTRIP);

                   break;

        case 'E': //偶数

                   newtio.c_iflag |=(INPCK | ISTRIP);

                   newtio.c_cflag |=PARENB;

                   newtio.c_cflag &= ~PARODD;

                   break;

        case 'N':  //无奇偶效验位

                   newtio.c_cflag &= ~PARENB;

                   break;

        }

        switch(nSpeed){  //设置数据传输率

        case 2400:

                   cfsetispeed(&newtio,B2400);

                   cfsetospeed(&newtio,B2400);

                   break;

        case 4800:

                   cfsetispeed(&newtio,B4800);

                   cfsetospeed(&newtio,B4800);

                   break;

        case 9600:

                   cfsetispeed(&newtio,B9600);

                   cfsetospeed(&newtio,B9600);

                   break;

        case 115200:

                   cfsetispeed(&newtio,B115200);

                   cfsetospeed(&newtio,B115200);

                   break;

        case 460800:

                   cfsetispeed(&newtio,B460800);

                   cfsetospeed(&newtio,B460800);

                   break;

        default:

                   cfsetispeed(&newtio,B9600);

                   cfsetospeed(&newtio,B9600);

                   break;

        }

        if(nStop == 1)    //设置停止位

                   newtio.c_cflag &= ~CSTOPB;

        else if(nStop == 2)

                  newtio.c_cflag |= CSTOPB;

         newtio.c_cc[VTIME] = 0; //设置等待时间和最少的接收字符

         newtio.c_cc[VMIN] = 0;

         tcflush(fd,TCIFLUSH);//处理未接收字符

         if((tcsetattr(fd,TCSANOW,&newtio))!=0){//激活新配置

                  perror("com set error");

                  return -1;

         }

         printf("set done!\n");

         return 0;

}

int uart_open(const char *dev, int a, int speed, char b, int c)
{
    fd = open(dev,O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
    {
        fprintf(stderr, "Error occurs when open uart dev!\n");
        return 1;
    }

    if(set_opt(fd, speed, a, b, c))
    {
        fprintf(stderr, "Error occurs when config uart!");
        return 2;
    }
    return 0;
}

int uart_write(char *s, int len)
{
    if(!fd)
        return 0;
    return write(fd, s, len);
}

int uart_read(char *s,int maxlen)
{
    if(!fd)
        return -1;
    return read(fd, s, maxlen); 
}
         
void uart_close()
{
    close(fd);
}


