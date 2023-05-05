# UDP Image Receiver

This is a simple C program that receives an image file over a UDP connection using Winsock2. It demonstrates the basic usage of Winsock library to create a UDP server socket, wait for incoming data packets, and save the received image data to a file.

## Dependencies

This program requires the following dependencies:

- [Winsock2](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)
- Windows operating system (tested on Windows 10)

## Usage

1. Make sure you have a compatible C compiler installed on your system (e.g., [Visual Studio](https://visualstudio.microsoft.com/downloads/)).
2. Clone this repository to your local machine.
3. Compile the program with your C compiler.
4. Run the compiled executable.

## Important Notes

- This program listens for incoming data packets on the local IP address `127.0.0.1` and port `80`. Make sure the UDP client that sends the image data is configured to send the data to the same IP address and port.
- This program assumes that the image data is sent in fixed-size packets of 1024 bytes. If the image data is sent in a different packet size, modify the `PACKETSIZE` macro at the top of the source code accordingly.
- This program does not handle any error correction or data integrity checks. It is intended as a simple demonstration of receiving raw data over UDP using Winsock2.
- This program assumes that the image file can fit into memory. For large files, it may be necessary to read and write the file data in smaller chunks to avoid memory issues.
