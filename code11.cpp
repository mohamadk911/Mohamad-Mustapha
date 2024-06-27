#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

// Logging messages into a file saved_communications to monitor the history of the program

    void logMessage(const std::string& message) {
    std::fstream File("saved_communications.txt", std::ios_base::out);

    if (File.is_open()) {
        File << message << '\n' ;
        File.close();
    } else {
        std::cerr << "File Not Opening."<< std::endl ;
    }
    }

    int main() {
    // This is the main function of the program that will call other functions
    HANDLE serialHandle = CreateFile(TEXT("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serialHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error, Serial Port not Opening." << std::endl;
        return 1;
    }

    // Setting serial port parameters
    DCB serialParams = {0};
    serialParams.DCBlength = sizeof(serialParams);
    if (!GetCommState(serialHandle, &serialParams)) {       //Retrieve the current control settings for the serial port
        std::cerr << "Error: Could not get serial port state." << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }
    serialParams.BaudRate = CBR_19200;    // shows moderate communication speed
    serialParams.ByteSize = 8;           // standard byte size packet
    serialParams.StopBits = ONESTOPBIT; // 1 stop bits 
    serialParams.Parity = NOPARITY;     // simple error checking
    //Configuration of the Serial Port
    if (!SetCommState(serialHandle, &serialParams)) {
        std::cerr << "Serial Port Not Opening." << std::endl;
        CloseHandle(serialHandle);     // close an open object handle
        return 1;
    }

    double first_number, second_number; // gives good precision 
    char operation;
    std::cout << "Enter the first_number: "; // ask the first number from the User
    std::cin >> first_number;
    std::cout << "Enter the operation (+, -, *, /): "; //Ask for the operation from the user
    std::cin >> operation;
    if (operation != '+' && operation != '-' && operation != '*' && operation != '/') {
        std::cerr << "Invalid operation." << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }
    std::cout << "Enter the second_number: "; // ask the first number from the User
    std::cin >> second_number;

    //Transform the numerical values into strings to be sent
    std::string dataToSend = std::to_string(first_number) + "," + operation + "," + std::to_string(second_number) + "\n";
    std::cout << "Sending Values: " << dataToSend << '\n' ;

    //Logging the sent data into the saved communication file.txt
    logMessage("Sent: " + dataToSend);

    //Sending the strings to the serial port
    DWORD bytesWritten;
    if (!WriteFile(serialHandle, dataToSend.c_str(), dataToSend.length(), &bytesWritten, NULL)) {
        std::cerr << "Error Not written to serial port." << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }

    //Reading the result from the serial port as bytes into a buffer
    char buffer[512];
    DWORD bytesRead;
    if (!ReadFile(serialHandle, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "Could not read from serial port." << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }
     // Ensure to be treated as C style String
    buffer[bytesRead] = '\0';

    std::cout << "The Result is: " << buffer << '\n';
    logMessage("Received: " + std::string(buffer));

    // Closing the serial port
    CloseHandle(serialHandle);
    return 0;
}
