## Recreation of a simple backdoor written in C to understand the basics of remote code execution. 
This simple backdoor written in C uses win socks libraries to send data to a server. Obviously, windows flags it because it is very easy to detect.
<br>
The server to control the backdoor can be executed both on windows and linux.<br>
It can be used to test remote code execution on LAN networks, and you need to set your server IP in the backdoor.c file. <br>

This backdoor also includes a keylogger and a function to set the program for auto run when the computer is turned on, with the editing of registry keys.

If you want to compile .c files for windows, you need to compile them including the library of windows sockets. Here is the guide to do this with GCC_
```
gcc nameFile.c -o nameFile -lws2_32
```
and if you need to compile the backdoor, use the flag **"-mwindows"** to hide the console window.
```
gcc nameFile.c -o nameFile -lws2_32 -mwindows
```

<br><br>Good luck!
<br><br>
Use this only for good purposes, and not to harm people/things.
