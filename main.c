#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <conio.h>
#include <stdint.h>

#include "com_port.h"

void print_buffer_hex(uint8_t * p_buffer, DWORD len)
{
    DWORD i;

    //printf("\n");
    for (i=0;i<len;i++)
    {
        printf("0x%02x ",p_buffer[i]);
    }
}

int main()
{
    char lp_vcom_1[6], lp_vcom_2[6];
    HANDLE h_vcom1, h_vcom2;
    uint8_t * p_buffer;
    p_buffer=malloc(1);
    DWORD real_len;
    int last_printed_port_num=0;

    printf("\nFree COM port monitor - ComMon");
    com_port_list();

    printf("\nPlease choose the first virtual or physical COM port\n");
    com_port_select(lp_vcom_1);
    h_vcom1=com_port_open(lp_vcom_1);
    com_port_open_error_check(h_vcom1,TRUE);
    com_port_params(h_vcom1);
    com_port_flush(h_vcom1);

    printf("\nPlease choose the second virtual or physical COM port\n");
    com_port_select(lp_vcom_2);
    h_vcom2=com_port_open(lp_vcom_2);
    com_port_open_error_check(h_vcom2,TRUE);
    com_port_params(h_vcom2);
    com_port_flush(h_vcom2);

    while(1)
    {
        real_len=com_port_read(h_vcom1,p_buffer,1);
        if (real_len)
        {
            com_port_write(h_vcom2,p_buffer,real_len);
            if (last_printed_port_num!=1)
            {
                printf("\n\ndata from %s\n",lp_vcom_1);
            }
            print_buffer_hex(p_buffer,real_len);
            last_printed_port_num=1;
        }
        fflush(stdout);


        real_len=com_port_read(h_vcom2,p_buffer,1);
        if (real_len)
        {
            com_port_write(h_vcom1,p_buffer,real_len);
            if (last_printed_port_num!=2)
            {
                printf("\n\ndata from %s\n",lp_vcom_2);
            }
            print_buffer_hex(p_buffer,real_len);
            last_printed_port_num=2;
        }
        fflush(stdout);
    }

    com_port_close(h_vcom1);
    com_port_close(h_vcom2);
    return 0;
}
