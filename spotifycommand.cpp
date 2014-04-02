/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 *   Copyright (C) 2009 by Mattias Runge <mattias@slacker.se>              *
 *                                                                         *
 *   File: spotify_cmd.cpp                                                 *
 *                                                                         *
 ***************************************************************************/
/*
 *  Inspiration and definitions from:
 *  http://code.google.com/p/pytify/
 *
 *  This project has no affiliation to spotify.com whatsoever!
 *
 *  Intended use is to control Spotify running under Linux with Wine.
 *  
 *  Compile: wineg++ spotify_cmd.cpp -o spotify_cmd.exe
 *  Run example: ./spotify_cmd.exe status
 */

#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

#define VERSION "0.5"

#define APPCOMMAND 0x0319

#define CMD_PLAYPAUSE 917504
#define CMD_MUTE 524288
#define CMD_VOLUMEDOWN 589824
#define CMD_VOLUMEUP 655360
#define CMD_STOP 851968
#define CMD_PREVIOUS 786432
#define CMD_NEXT 720896

using namespace std;

void toClipboard(const string &s){
	OpenClipboard(0);
	EmptyClipboard();	
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size()+1);
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg),s.c_str(),s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT,hg);
	CloseClipboard();
	GlobalFree(hg);
}
	
vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		HWND window_handle = FindWindow("SpotifyMainWindow", NULL);
	
		if (window_handle == NULL)
		{
			cout << "ERROR" << endl;
			cout << "Can not find spotify, is it running?" << endl;
			return 1;
		}
		
		char buffer[512] = "";
		string artistName = "";
		string songName = "";
		
		if (GetWindowText(window_handle, buffer, sizeof(buffer)) > 0)
		{
			string title = buffer;
			
			string::size_type pos1 = title.find('-');
			string::size_type pos2 = title.find(static_cast<char>(-106));
			
			if (pos1 != string::npos && pos2 != string::npos)
			{
				pos1 += 2;
				artistName = title.substr(pos1, pos2 - pos1 - 1);
				songName = title.substr(pos2 + 2);
			}
		}
		
		string command = argv[1];
		
		if (command == "playpause")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_PLAYPAUSE);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "next")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_NEXT);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "prev")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_PREVIOUS);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "stop")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_STOP);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "mute")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_MUTE);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "volup")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_VOLUMEUP);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "voldown")
		{
			SendMessage(window_handle, APPCOMMAND, 0, CMD_VOLUMEDOWN);
			cout << "OK" << endl;
			return 0;
		}
		else if (command == "status")
		{
			if (artistName == "" && songName == "")
			{
				cout << "WARN" << endl;
				cout << "Playback paused" << endl;
			}
			else
			{
				cout << "OK" << endl;
				cout << artistName << endl;
				cout << songName << endl;
			}
			
			return 0;
		}
		
		else if(command == "send")
		{
			string url = argv[2]; 
			
			toClipboard(url);
			
			SetForegroundWindow(window_handle);
			Sleep(100);
			
			// This structure will be used to create the keyboard
			// input event.
			INPUT ip1;
			INPUT ip2;
			INPUT ip3;
		 
			// Set up a generic keyboard event.
			ip1.type = INPUT_KEYBOARD;
			ip1.ki.wScan = 0; // hardware scan code for key
			ip1.ki.time = 0;
			ip1.ki.dwExtraInfo = 0;
			
			ip2.type = INPUT_KEYBOARD;
			ip2.ki.wScan = 0; // hardware scan code for key
			ip2.ki.time = 0;
			ip2.ki.dwExtraInfo = 0;
			
			ip3.type = INPUT_KEYBOARD;
			ip3.ki.wScan = 0; // hardware scan code for key
			ip3.ki.time = 0;
			ip3.ki.dwExtraInfo = 0;
		 
			//click enter for focus
			ip3.ki.dwFlags = 0;
			ip3.ki.wVk = 0x0D;
			
			SendInput(1, &ip3, sizeof(INPUT));
			
			Sleep(50);
			
			ip3.ki.dwFlags = KEYEVENTF_KEYUP;
			
			SendInput(1, &ip3, sizeof(INPUT));
			
			
			// Press the key
			ip1.ki.wVk = 0x11; // virtual-key code 
			ip1.ki.dwFlags = 0; // 0 for key press
			SendInput(1, &ip1, sizeof(INPUT));
			
			ip2.ki.wVk = 0x4C; // virtual-key code 
			ip2.ki.dwFlags = 0; // 0 for key press
			SendInput(1, &ip2, sizeof(INPUT));
		 
			Sleep(50);
			// Release key
			ip1.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
			SendInput(1, &ip1, sizeof(INPUT));
			
			ip2.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
			SendInput(1, &ip2, sizeof(INPUT));
			
			//Paste the url that was copied to clipboard
			ip1.ki.dwFlags = 0;
			ip2.ki.dwFlags = 0;
			ip2.ki.wVk = 0x56;
			
			SendInput(1, &ip1, sizeof(INPUT));
			SendInput(1, &ip2, sizeof(INPUT));
			
			Sleep(50);
			
			ip1.ki.dwFlags = KEYEVENTF_KEYUP;
			ip2.ki.dwFlags = KEYEVENTF_KEYUP;
			
			SendInput(1, &ip1, sizeof(INPUT));
			SendInput(1, &ip2, sizeof(INPUT));
			
			size_t pos = url.find("playlist");
			if(pos!=string::npos){
				ip3.ki.dwFlags = 0;
				SendInput(1, &ip3, sizeof(INPUT));	
				Sleep(50);	
				ip3.ki.dwFlags = KEYEVENTF_KEYUP;	
				SendInput(1, &ip3, sizeof(INPUT));

				/*int downCount = atoi(argv[3]);
				
				//Press tab twice
				
				ip2.ki.wVk = 0x09; // virtual-key code 
				ip2.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip2, sizeof(INPUT));
			 
				Sleep(100);
				// Release key
				ip2.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
				SendInput(1, &ip2, sizeof(INPUT));
				
				//Sleep(200);

				ip2.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip2, sizeof(INPUT));
			 
				Sleep(100);
				// Release key
				ip2.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
				SendInput(1, &ip2, sizeof(INPUT));
				
				//Sleep(200);
				
				//PRESS DOWN KEYS
				for(int x =0 ; x<downCount; x++){
					ip2.ki.wVk = 0x28; // virtual-key code 
					ip2.ki.dwFlags = 0; // 0 for key press
					SendInput(1, &ip2, sizeof(INPUT));
				 
					Sleep(100);
					// Release key
					ip2.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
					SendInput(1, &ip2, sizeof(INPUT));
					//Sleep(200);
				}

			} */
			}
			
			//press enter
			ip3.ki.dwFlags = 0;
			SendInput(1, &ip3, sizeof(INPUT));	
			Sleep(50);	
			ip3.ki.dwFlags = KEYEVENTF_KEYUP;	
			SendInput(1, &ip3, sizeof(INPUT));
			
			return 0;
		}
	}

	cout << "spotify_cmd version " << VERSION << ", copyright by Mattias Runge 2009" << endl;
	cout << "Usage:" << " ./spotify_cmd [playpause|prev|next|stop|mute|volup|voldown|status]" << endl;
	return 0;
}

