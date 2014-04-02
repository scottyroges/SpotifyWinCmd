#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstring>
using namespace std;

void getQueueNext(){

	stringstream stream;    
	
	string content;
	ifstream myReadFile;
	myReadFile.open("queue.txt");
	if (myReadFile.is_open()) {
		while(!myReadFile.eof()) // To get you all the lines.
		{
			getline(myReadFile,content); // Saves the line in STRING.
			//cout<<content; // Prints our STRING.
		}

	}
	myReadFile.close();
	
	stream << "C:\\Users\\Scott\\Programming\\spotifycmd\\myspotifycmd.exe send " << content;
	system(stream.str().c_str());
}
int main(int argc, char **argv)
{
	string currentTitle;
	getQueueNext();
	while(true){
		HWND window_handle = FindWindow("SpotifyMainWindow", NULL);
		
		if (window_handle == NULL)
		{
			cout << "ERROR" << endl;
			cout << "Can not find spotify, is it running?" << endl;
			return 1;
		}
		
		char buffer[512] = "";
		
		if (GetWindowText(window_handle, buffer, sizeof(buffer)) > 0)
		{
			string title = buffer;
		
			if(currentTitle.compare(title) != 0 && !currentTitle.empty()){
				cout << currentTitle << "\n";
				cout << title << "\n";
				cout << "Song finished\n";
				if(title.compare("Spotify")!=0) {
					if(currentTitle.compare("Spotify") !=0) {
						system("C:\\Users\\Scott\\Programming\\spotifycmd\\myspotifycmd.exe playpause");
					}
				}
				getQueueNext();
					
				
			}
			
			currentTitle = title;
		}
	}
	
}
