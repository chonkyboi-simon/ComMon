#ifndef COM_PORT_H
    #define COM_PORT_H
    //prompt user to select a com port
    void com_port_select(LPSTR com_port_name);

    //open com port
    HANDLE com_port_open(LPSTR portname);

    //close com port
    void com_port_close(HANDLE hSerial);

    //list available com port
    void com_port_list();

    //call this function to check the return value of com_port_open()
    int com_port_open_error_check(HANDLE hSerial,BOOLEAN prompt_enable);

    //set com port parameters
    int com_port_params(HANDLE hSerial);

    //write data to serial port
    DWORD com_port_write(HANDLE hSerial, uint8_t * data, int length);

    //read data from serial port
    DWORD com_port_read(HANDLE hSerial, uint8_t * data, int length);

    //empty COM port RX buffer
    void com_port_flush(HANDLE hSerial);
#endif // COM_PORT_H



