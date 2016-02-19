#include "mraa.hpp"
#include "sparkfun/oled/Edison_OLED.h"
#include "sparkfun/gpio/gpio.h"
#include "math.h"

#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>

#include <unistd.h>

#include <array>

#include <cstdio>
#include <memory>

using namespace std;

edOLED oled;

gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

int toint(std::string s);

//screen 10 letters wide

void SetupOLED();
void DrawOLED();

void Scan();

std::string getWifis();
std::string getCmdOutput(const char* cmd);

std::vector<std::string> split(const std::string &text, char sep);

int toint(std::string s)
{
    return atoi(s.c_str());
}

int main() {
	SetupOLED();
	Scan();
	while (1) {
		DrawOLED();
	}
	return 0;
}

void SetupOLED (){
	oled.begin();
	oled.clear(PAGE);
	oled.setFontType(0);
	oled.display();
}

std::vector< std::string > addresses;
std::vector< std::string > channels;
std::vector< std::string > freqs;
std::vector< std::string > qualities;
std::vector< std::string > ssids;

int wifiCount = 0;
int currentWifi = 0;

bool a_pressed = false;
bool b_pressed = false;

void DrawOLED(){
	oled.clear(PAGE);
	if(currentWifi > wifiCount)
		currentWifi = wifiCount - 1;

	if(wifiCount > 0){
		oled.setCursor(0, 0);
		oled.print("CH");
		oled.print(channels[currentWifi].c_str());

		oled.setCursor(49, 0);
		oled.print(currentWifi+1);
		oled.setCursor(54, 0);
		oled.print("/");
		oled.setCursor(59, 0);
		oled.print(wifiCount);

		oled.setCursor(0, 8);
		oled.print(freqs[currentWifi].c_str());
		oled.print("Ghz");


		oled.setCursor(0, 17);
		oled.print(qualities[currentWifi].c_str());


		oled.setCursor(0, 25);
		oled.print(ssids[currentWifi].c_str());

		oled.display();
	}

	if (BUTTON_A.pinRead() == LOW){
		if(!a_pressed){
			a_pressed  = true;
			if(currentWifi < wifiCount-1)
				++currentWifi;
			else
				currentWifi = 0;
	//		cout << "Up: " << currentWifi << " < " << wifiCount << '\n';
		}
	}else{
		a_pressed  = false;
	}

	if (BUTTON_B.pinRead() == LOW){
		if(!b_pressed ){
			b_pressed = true;
			if(currentWifi > 0)
				--currentWifi;
			else
				currentWifi = wifiCount-1;
		//	cout << "Down: " << currentWifi << " > 0" << '\n';
		}
	}else{
		b_pressed  = false;
	}

	if (BUTTON_SELECT.pinRead() == LOW){
		Scan();
	}
}

void Scan(){
	oled.clear(PAGE);
	oled.setCursor(0, 0);
	oled.print("Scanning...");

	oled.display();

	wifiCount = 0;

	addresses.clear();
	channels.clear();
	freqs.clear();
	qualities.clear();
	ssids.clear();

	std::string wifis = getWifis();

	//cout << wifis;

	std::istringstream lines( wifis );
	std::ostringstream indented_lines;

	std::string one_line;

	std::string currentaddress;
	std::string currentchannel;
	std::string currentfreq;
	std::string currentquality;
	std::string currentssid;

	while ( getline( lines, one_line ) ) {
		//cout << one_line;
		if(one_line.find("Address") != std::string::npos){
			if(!currentaddress.empty()){
				addresses.push_back(currentaddress);
				channels.push_back(currentchannel);
				freqs.push_back(currentfreq);
				qualities.push_back(currentquality);
				ssids.push_back(currentssid);

				currentaddress.clear();
				currentchannel.clear();
				currentfreq.clear();
				currentquality.clear();
				currentssid.clear();
				++wifiCount;
			}

			currentaddress = split(one_line, ' ')[4];
			//addresses << currentaddress << '\n';
		}
		if(one_line.find("Channel:") != std::string::npos){
			currentchannel = split(one_line, ':')[1];

		}
		if(one_line.find("Frequency") != std::string::npos){
			currentfreq = split(one_line, ':')[1];
			currentfreq = split(currentfreq, ' ')[0];
		}
		if(one_line.find("Quality") != std::string::npos){
			currentquality = split(one_line, '=')[1];
			currentquality = split(currentquality, ' ')[0];
		}
		if(one_line.find("ESSID") != std::string::npos){
			currentssid = split(one_line, ':')[1];
		}

		indented_lines << ">> " << one_line << '\n';
	}

}

std::string getWifis(){
	std::string display = "";

	std::string wifis = getCmdOutput("iwlist wlan0 scan | grep -E 'Address|Channel|ESSID|Mode|Quality|Authentication'");

	return wifis;
}

std::string getCmdOutput(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
/*
std::string getCmdOutput(const char * mStr){
    std::string result, file;
    FILE* pipe{popen(mStr, "r")};
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        file = buffer;
        result += file.substr(0, file.size() - 1);

    }

    cout << result;

    pclose(pipe);
    return result;
}
*/


std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        std::string temp = text.substr(start, end - start);
        if (temp != "") tokens.push_back(temp);
        start = end + 1;
    }
    std::string temp = text.substr(start);
    if (temp != "") tokens.push_back(temp);
    return tokens;
}
