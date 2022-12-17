#include <stdio.h>
#include <windows.h>
#include <termios.h>
#include <string.h>
#include "conio.h"
//#include <xinput.h>
#include <iostream>
//#include <bool.h>
//#define DIRECTINPUT_VERSION 0x0800
//#include <Dinput8.h>
#define BAUDRATE 9600
#define WINVER 0x0500

using namespace std; 
void keyBoardInput(INPUT ip, int ms ,char character,bool held,int once,bool button, bool joystick, bool trigger);
void uartInit(){
	//create the terminal for talking to UART
	HANDLE hSerial;
	cout<<"What buttons would you like to act as a trigger. Max of two"<<endl;
	char first;
	cin>>first;
	cout<<"Next button"<<endl;
	char second;
	cin>>second;
	printf("Please open device manager and look for STM32 STLink Virtual Com port\n");
	printf("Please type 'COM#' with the # being the number next to it.\n");
	char port[100];//Ik HUGE buffer overflow attack possible. just dont overflow pls :)
	scanf("%s",port);
	hSerial = CreateFile(port, GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hSerial==INVALID_HANDLE_VALUE){
		if(GetLastError()==ERROR_FILE_NOT_FOUND){
			printf("SerialPort Does not EXIST\n");
		}		
	}
	DCB dcbSerialParams ={0};
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	if(!GetCommState(hSerial, &dcbSerialParams)){
		printf("Second error\n");
	}
	dcbSerialParams.BaudRate=CBR_9600;
	dcbSerialParams.ByteSize=1;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=0;//NOPARITY;
	if(!SetCommState(hSerial, &dcbSerialParams)){
		printf("Error settig n serial port state");
		CloseHandle(hSerial);
		return;
	}
	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=10;
	timeouts.ReadTotalTimeoutConstant=10;
	timeouts.ReadTotalTimeoutMultiplier=10;
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=10;
	if(!SetCommTimeouts(hSerial, &timeouts)){
		printf("Error occured.\n");
		CloseHandle(hSerial);
		return;
	}
	//CloseHandle(hSerial);	
	//return;
	int replace[10]; //for the two trigger buttons.
	int keyStroke[10];
	switch(first){
		case ('u'):
				replace[0]=8;
				keyStroke[0]=0x16;
				break;
		case ('i'):
				replace[0]=10;
				keyStroke[0]=0x17;
				break;
		case ('o'):
				replace[0]=12;
				keyStroke[0]=0x18;
				break;
		case ('j'):
				replace[0]=14;
				keyStroke[0]=0x24;
				break;
		case ('k'):
				replace[0]=16;
				keyStroke[0]=0x25;
				break;
		case ('l'):
				replace[0]=18;
				keyStroke[0]=0x26;
				break;
		
		
	}
	
	switch(second){
		case ('u'):
				replace[1]=8;
				keyStroke[1]=0x16;
				break;
		case ('i'):
				replace[1]=10;
				keyStroke[1]=0x17;
				break;
		case ('o'):
				replace[1]=12;
				keyStroke[1]=0x18;
				break;
		case ('j'):
				replace[1]=14;
				keyStroke[1]=0x24;
				break;
		case ('k'):
				replace[1]=16;
				keyStroke[1]=0x25;
				break;
		case ('l'):
				replace[1]=18;
				keyStroke[1]=0x26;
				break;
		
	}
	
	
    char szBuff[25]={0};
	DWORD dwBytesRead=0;
	INPUT ip;
	INPUT ip2;
	INPUT ip3;
//	keyBoardInput(INPUT ip, )
	//need to READ BRUH.
	//CloseHandle(hSerial);
	//return;
	int check[10] ={0};
	cout<<"Driver has started, enter '1' to close the driver"<<endl;
	int exit=0;
	bool pressed[30]= {false};
	int once[30]={0};
	once[0]=0;
	while(1){
		if (kbhit()){
			
			if (getch()=='1'){
				CloseHandle(hSerial);
				printf("The port is now closed\n");
				printf("The controller driver is now off.\n");
				return;
			}
			
			
		}
		char input =ReadFile(hSerial,szBuff,19,&dwBytesRead,NULL);
		
		if(input <=0){
			printf("Miss char, turning off\n");
			CloseHandle(hSerial);	
			return;
		}
		else{
			
			//for(int j=0; j<3; j++){
				//if(j==0){
					if((szBuff[0]=='z'||szBuff[replace[0]]=='Z' || szBuff[replace[1]]=='Z') && (once[0]==1 || once[27]==1 || once[28]==1)){
						if(szBuff[replace[0]]=='Z' && once[27]==1){
							pressed[27]=false;
							keyBoardInput(ip,keyStroke[0],keyStroke[0],pressed[27],once[0],false,true,false);
							
							once[27]=0;
							//return;
						}
						if(szBuff[replace[1]]=='Z' && once[28]==1){
							pressed[28]=false;
							keyBoardInput(ip,keyStroke[1],keyStroke[1],pressed[28],once[0],false,true,false);
							once[28]=0;
							//return;
						}
						if(szBuff[0]=='z' && once[0]==1){
							pressed[0]=false;
							keyBoardInput(ip,0x11,0x57,pressed[0],once[0],false,true,false);
							//keyBoardInput(ip,30,58,pressed[0],once[0],false,true,false);
							once[0]=0;
							//return;
						}
						
						
						//keyBoardInput(ip,30,0x57,pressed[0],once[0],false,true,false);
						
					}
					if(szBuff[0]=='w'|| szBuff[replace[0]]==first || szBuff[replace[1]] ==second){
						
						
						if(szBuff[0]=='w'){
							pressed[0]=true;
							keyBoardInput(ip,0x11,0x57,pressed[0],once[0],false,true,false);
							//keyBoardInput(ip,30,58,pressed[0],once[0],false,true,false);
							once[0]=1;
						}
						if(szBuff[replace[0]]==first){
							pressed[27]=true;
							keyBoardInput(ip,keyStroke[0],keyStroke[0],pressed[27],once[27],false,false,true);
							once[27]=1;
							
						}
						if(szBuff[replace[1]]==second){
							pressed[28]=true;
							keyBoardInput(ip,keyStroke[1],keyStroke[1],pressed[28],once[28],false,false,true);
							once[28]=1;
						}
						//szBuff[0]='z';
						
					}
			
			
					if(szBuff[4]=='z' && once[4]==1){
						pressed[4]=false;
						keyBoardInput(ip,30,0x41,pressed[4],once[4],false,true,false);
						once[4]=0;
					}
					if(szBuff[4]=='a' ){
						
						pressed[4]=true;
						
						keyBoardInput(ip,30,0x41,pressed[4],once[4],false,true,false);
						//szBuff[4]='z';
						once[4]=1;
					}
					if(szBuff[2]=='z' && once[2]==1){
						pressed[2]=false;
						keyBoardInput(ip,0x1F,0x53,pressed[2],once[2],false,true,false);
						once[2]=0;
					}
					if(szBuff[2]=='s'){
						
						pressed[2]=true;
						
						keyBoardInput(ip,0x1F,0x53,pressed[2],once[2],false,true,false);
						//szBuff[2]='z';
						once[2]=1;
					}
					if(szBuff[6]=='z' && once[6]==1){
						pressed[6]=false;
						keyBoardInput(ip,0x20,0x44,pressed[6],once[6],false,true,false);
						once[6]=0;
					}
					if(szBuff[6]=='d'){
						
						pressed[6]=true;
						
						keyBoardInput(ip,0x20,0x44,pressed[6],once[6],false,true,false);
						//szBuff[6]='z';
						once[6]=1;
					}
							
				//}
				//if(j==1){
					
					if(szBuff[8]=='u' && check[0]==0 && first!='u' && second!='u'){
						
						//pressed[8]=true;
						keyBoardInput(ip2,0x16,0x55,pressed[8],once[8],true,false,false);
						check[0]=1;
						
						//szBuff[8]='z';
						//once[8]=1;
					}
					else if(szBuff[8]=='Z'){
						check[0]=0;
					}
					
					if(szBuff[10]=='i' && check[1]==0 && first!='i' && second!='i'){
						
						//pressed[10]=true;
						check[1]=1;
						keyBoardInput(ip2,0x17,0x49,pressed[10],once[10],true,false,false);
						//szBuff[10]='z';
						//once[10]=1;
					}
					else if(szBuff[10]=='Z'){
						check[1]=0;
					}
					
					
					if(szBuff[12]=='o' && check[2]==0 && first!='o' && second!='o') {
						
					//	pressed[12]=true;
						if(check[2]==0){
							keyBoardInput(ip2,0x18,0x4F,pressed[12],once[12],true,false,false);
							
						}
						
						check[2]=1;
						//szBuff[12]='z';
						//once[12]=1;
					}
					else if(szBuff[12]=='Z'){
						check[2]=0;
					}
					
					
					
					
				//}
				//if(j==2){
							
				
					if(szBuff[14]=='j' && check[3]==0 && first!='j' && second!='j'){
						
						//pressed[14]=true;
						//make array- if pressed turn to 1
						if(check[3]==0){
							keyBoardInput(ip3,0x24,0x4A,pressed[14],once[14],true,false,false);
							check[3]=1;
						}
						
						//szBuff[14]='z';
						//once[14]=1;
					}
					else if(szBuff[14]=='Z'){
						check[3]=0;
					}
					
					
					
					if(szBuff[16]=='k' && check[4]==0 && first!='k' && second!='k'){
						//cout<<"HIT K"<<endl;
					//	pressed[16]=true;
						check[4]=1;
						keyBoardInput(ip3,0x25,0x4B,pressed[16],once[16],false,false,true);
						//szBuff[16]='z';
						//once[16]=1;
					}else if(szBuff[16]=='Z'){
						check[4]=0;
					}
					
					
					if(szBuff[18]=='l' && check[5]==0 && first!='l' && second!='l'){
						
					//	pressed[18]=true;
						check[5]=1;
						keyBoardInput(ip3,0x26,0x4C,pressed[18],once[18],true,false,false);
						//szBuff[18]='z';
						//once[18]=1;
					}else if(szBuff[18]=='Z'){
						check[5]=0;
					}
							
					
				//}
				
				
			//}
			
			
		}
		
	}	
	CloseHandle(hSerial);	
}
void hexto(char test){
	
	
}

void gamepadTest(){
	//XINPUT_DEVSUBTYPE_ARCADE_STICK;
	//_XINPUT_STATE controller;
	//DWORD test = XInputGetState(1,_XINPUT_STATE.*PXINPUT_STATE);
	//test.dwUserIndex=0x1;
	
	//controller.Gamepad.wButtons |=0x1;
	cout<<"Loop"<<endl;
	tagRAWINPUTHEADER check;
	tagRAWINPUT point;
	check.dwType = 1;
	check.dwSize =  1;
	//check.hDevice=point;
	point.data.keyboard;
	tagRAWINPUTDEVICE work;
	work.usUsagePage=0x5;
	work.usUsage=0x5;
	
//	LPRAWINPUTDEVICE  lpdiKeyboard; 

	//lpdiKeyboard->CreateDevice(GUID_SysKeyboardEm2, &lpdiKeyboard, NULL); 
	return;
}

void keyBoardInput(INPUT ip, int ms ,char character,bool held,int once,bool button, bool joystick, bool trigger){
	//INPUT ip;
	//Sleep(ms);
	ip.type = INPUT_KEYBOARD;
	//KEYEVENTF_SCANCODE
	
	// OG ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
	
	//OG ip.ki.wVk = character;
	ip.ki.wVk=0;
	ip.ki.dwFlags=KEYEVENTF_SCANCODE;
	ip.ki.wScan=ms;
	//int print = 0x1E;//A
	//for(int i=0; i<32; i++){
	
		//OG ip.ki.dwFlags =0;
		if(once==0){
			SendInput(1,&ip,sizeof(INPUT));
		//Sleep(30);
		}
		if(held==false && button==true){
			Sleep(150);
			ip.ki.dwFlags=KEYEVENTF_KEYUP |KEYEVENTF_SCANCODE;
			SendInput(1,&ip,sizeof(INPUT));
			return;
		}
		else if (held==false && joystick==true){
			Sleep(10);
			ip.ki.dwFlags=KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
			SendInput(1,&ip,sizeof(INPUT));
			return;
			
		}
		else if(held==false && trigger==true){
			Sleep(150);
			ip.ki.dwFlags=KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
			SendInput(1,&ip,sizeof(INPUT));
			return;
		}
		//cout<<"Print: "<<print<<endl;
		//print+=1;
	//}//
	
	
	
	
	return;
}


int main(){
	cout<<"\nWelcome to our ECE 3710 Microcontroller Final Project."<<endl;
	cout<<"          "<<"By Tim Goodale and Jon Newman"<<endl;
	cout<<"\n"<<endl;
	cout<<"Controls for the joystick map to the keyboard as\n"<<endl;
	cout<<"       UP\n        W\n LEFT A   D RIGHT\n        S\n       DOWN"<<endl;
	cout<<"Top Row of Buttons are mapped as    U I O"<<endl;
	cout<<"Bottom Row of Buttons are mapped as J K L"<<"\n"<<endl;
	//gamepadTest();
	uartInit();
	//keyBoadInputTest();
	//keyBoardInput();
	return 0; 
}

