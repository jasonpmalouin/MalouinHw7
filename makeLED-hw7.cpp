/** Simple On-board led flashing program - written in c++ by derek molly *    simple functional struture for the Exploring BeagleBone book
 *
 *    This program uses usr led 3 and can be executed in five ways:
 *         makeLED on
 *         makeLED off
 *         makeLED flash  (flash at 100ms intervals - on 50ms/off 50ms)
 *         makeLED blink <# of blinks> (blinks LED on and off in 1 second intervals n times)
 *         makeLED status (get the trigger status)
 *
 **Modified by Jason Malouin For CPE-422
 *
 * * Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
 * Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
 * ISBN 9781118935125. Please see the file README.md in the repository root 
 * directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
using namespace std;

#define LED3_PATH "/sys/class/leds/beaglebone:green:usr3"

void writeLED(string filename, string value){
	fstream fs;
	string path(LED3_PATH);
	fs.open((path + filename).c_str(), fstream::out);
	fs << value;
	fs.close();
}

void removeTrigger(){
	writeLED("/trigger", "none");
}

int main(int argc, char* argv[]){

	if(argc<2){
		cout << "Usage is makeLED and one of: " << endl;
		cout << "   on, off, flash, blink, or status" << endl;
		cout << " e.g. makeLED flash" << endl;
		return 2;
	}
	
	string cmd(argv[1]);
	cout << "Starting the makeLED program" << endl;
	cout << "The current LED Path is: " << LED3_PATH << endl;

	// select whether command is on, off, flash or status
	if(cmd=="on"){
		cout << "Turning the LED on" << endl;
		removeTrigger();
		writeLED("/brightness", "1");
	}
	else if (cmd=="off"){
		cout << "Turning the LED off" << endl;
		removeTrigger();
		writeLED("/brightness", "0");
	}
	else if (cmd=="flash"){
		cout << "Flashing the LED" << endl;
		writeLED("/trigger", "timer");
		writeLED("/delay_on", "50");
		writeLED("/delay_off", "50");
	}else if (cmd=="blink")
	{

		if(argc!=3)
		{
			cout << "Invalid Syntax. Use:" << endl;
			cout << "makeLED blink <# of blinks>" << endl;
                	return 2;
		}
		string blinks(argv[2]);
		int n = stoi(blinks);

		removeTrigger();
		for(int i=0;i<n;i++)
		{
			writeLED("/brightness", "1");
			sleep(1);
			writeLED("/brightness", "0");
			cout << "Blink #: " + to_string(i+1) + " / " + blinks << endl;
			sleep(1);
		}
	}
	else if (cmd=="status"){
		// display the current trigger details
		std::fstream fs;
		fs.open( LED3_PATH "/trigger", std::fstream::in);
		string line;
		while(getline(fs,line)) cout << line << endl;
		fs.close();
	}
	else{
		cout << "Invalid command!" << endl;
	}
	cout << "Finished the makeLED Program" << endl;
	return 0;
}
