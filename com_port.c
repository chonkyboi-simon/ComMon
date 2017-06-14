#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <conio.h>
#include <stdint.h>

#include "com_port.h"

void com_port_select(LPSTR com_port_name)
{
    //char port_name[6];//allocate 5 characters for COM port name + 1 terminator
    printf("Please input COM port name(e.g. COM1 or COM13):");
    scanf("%s",com_port_name);
    //memcpy(com_port_name,port_name,6);
}

HANDLE com_port_open(LPSTR com_port_name)
{
    char com_port_name_syntaxed[10];//allocate 9 characters for syntaxed COM port name + 1 terminator
    DWORD  accessdirection =GENERIC_READ | GENERIC_WRITE;
    sprintf(com_port_name_syntaxed,"\\\\.\\%s",com_port_name);
    //printf("checking port %s\n",com_port_name_syntaxed);
    HANDLE hSerial = CreateFile(com_port_name_syntaxed, accessdirection, 0, 0, OPEN_EXISTING, 0, 0);
    return hSerial;
}

void com_port_close(HANDLE hSerial)
{
    CloseHandle(hSerial);
}

void com_port_list()
{
	HANDLE hSerial;
	int i;
	char com_port_name[6];//allocate 5 characters for COM port name + 1 terminator
	printf("Detecting available serial ports (maximum port number 99)...\n");
	for (i=1;i<100;i++)
	{
		sprintf(com_port_name,"COM%d",i);
		hSerial=com_port_open(com_port_name);
		if(hSerial==INVALID_HANDLE_VALUE)
        {
            if(GetLastError()==ERROR_FILE_NOT_FOUND)
            {
                //printf("\nError:Serial Port does not exist.");
                //printf("Sub Error: ERROR_FILE_NOT_FOUND");
            }
            //printf("\nSerial port communication error.\nSub Error: INVALID_HANDLE_VALUE");
            //printf("Detecting port %s\n...",port_name,);
        }
        else
        {
            printf("%s is available.\n",com_port_name);
			com_port_close(hSerial);
        }
	}
}

int com_port_open_error_check(HANDLE hSerial,BOOLEAN prompt_enable)
{
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (prompt_enable==TRUE)
        {
            printf("Error: Could not create handle for specified COM port.\n");
            printf("Press any key to continue.\n");
            while(kbhit()!=1);
            getch();
        }
        return 1;
        //call GetLastError(); to gain more information
    }
    else
    {
        if (prompt_enable==TRUE)
        {
            printf("Successfully created handle for specified COM port.\n");
            printf("Press any key to continue.\n");
            while(kbhit()!=1);
            getch();
        }
        return 0;
    }
}

int com_port_params(HANDLE hSerial)
{
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error: Could not get the state of secified handle.\n");
        return 1;
         //could not get the state of the comport
    }
    dcbSerialParams.BaudRate=   115200;
    dcbSerialParams.ByteSize=   8;
    dcbSerialParams.StopBits=   ONESTOPBIT;
    dcbSerialParams.Parity=     NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error: Could not set COM port parameters.\n");
        return 1;
         //analyse error
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Error: Could not set COM port timeout.\n");
        return 1;
        //handle error
    }

    return 0;
}


DWORD com_port_write(HANDLE hSerial, uint8_t * data, int length)
{

	DWORD dwBytesWrite = 0;
	if(!WriteFile(hSerial, data, length, &dwBytesWrite, NULL)){
		//printLastError();
		//handle error
	}
	return dwBytesWrite;

}

DWORD com_port_read(HANDLE hSerial, uint8_t * data, int length)
{
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, data, length, &dwBytesRead, NULL)){
        //handle error
    }
    return dwBytesRead;
}

void com_port_flush(HANDLE hSerial)
{
    uint8_t buffer;
    DWORD cnt=0;
    while(1)
	{
		if(com_port_read(hSerial,&buffer,1)==0)
		{
		    printf("COM port buffer is empty\n");
            break;
		}
		else
        {
            cnt++;
        }
	}
	printf("COM port flushed: %lu characters\n",cnt);
	//return cnt;
}
