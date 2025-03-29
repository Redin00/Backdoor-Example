## Recreation of a simple backdoor written in C to understand the basics of remote code execution. 
This simple backdoor written in C uses win socks libraries to send data to a server. Obviously, windows flags it because it is very easy to detect.
<br>
The server to control the backdoor can be executed both on windows and linux.<br>
It can be used to test remote code execution on LAN networks, and you need to set your server IP in the backdoor.c file. <br>

If you want to compile .c files for windows, you need to compile them in this way
```
gcc nameFile.c -o nameFile -lws2_32
```

<br><br>Good luck!
<br><br>
Educational purposes only.
