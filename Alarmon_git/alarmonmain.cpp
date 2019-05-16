#include "irrlicht.h"
#include "windows.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <iostream>
#include <string>
#include <fstream>
#include "extensiveFunctions.cpp"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace z{
};

int main(int argc, char* args[]){
	//Initialize IrrLicht
		MyEventReceiver receiver;
	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, core::dimension2d<u32>(600, 480), 16, false, false, true, &receiver);
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* env = device->getGUIEnvironment();

	//Initialize SDL_Mixewr
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) { cout << "Failed to start mixer" << endl;
	system("pause");
	device->drop();
	return -3;
	}

	//Set background and window title bar and load font
	device->setWindowCaption(L"Alarmon!");
	device->setEventReceiver(&receiver);
	IGUIImage *bg = env->addImage(driver->getTexture("ressystem/bg.png"), vector2d<s32>(0, 0), true);
	bg->setScaleImage(true);
	bg->setMinSize(dimension2du(600, 480));
	bg->setMaxSize(dimension2du(600, 480));
	IGUIFont *font = env->getFont("ressystem/superfont.xml");


	//load the 'timebg' (image on top of which text is displayed) and load the alarm sound (alarm.wav)
	bool titlechange = false;
	Mix_VolumeMusic(20);
	IGUIImage *timebg = env->addImage(driver->getTexture("ressystem/timebg.png"), vector2d<s32>(0, 0));
	timebg->setRelativePosition(position2di((driver->getScreenSize().Width / 2) - (timebg->getImage()->getSize().Width / 2), (driver->getScreenSize().Height / 2) - (timebg->getImage()->getSize().Height / 2)));
	rect<s32> timepos;
	{timepos.UpperLeftCorner.X = timebg->getRelativePosition().UpperLeftCorner.X + 130; timepos.UpperLeftCorner.Y = timebg->getRelativePosition().UpperLeftCorner.Y + 103;
	timepos.LowerRightCorner.X = timepos.UpperLeftCorner.X + 279; timepos.LowerRightCorner.Y = timepos.UpperLeftCorner.Y + 154;}
	Mix_Chunk *alarm1 = Mix_LoadWAV("ressystem/alarm.wav");
	if(!alarm1){
		cout << "Failed to load alarm sound. Please check format and file presence." << endl;
		cout << " Shutting down now..." << endl;
		system("pause");
		device->drop();
		return -2;
	}

	//Create FPS limiter in order to regulate FPS and maintain low cpu-usage (fps set to 30 in xfpslimiter class)
	//then get local time and set systime to local time
	xfpslimiter xfps;
	xfps.timer = device->getTimer();
	SYSTEMTIME systime;
	SYSTEMTIME alarmtime;
	GetLocalTime(&systime);
	GetLocalTime(&alarmtime);
	
	//close if no alarm.txt
	//if alarm.txt, load time and set 'alarmtime' (time of alarm)
	if(!checkForFile("ressystem/alarm.txt")){
		cout << "No alarm time file ('alarm.txt')!" << endl;
		cout << "Shutting down program..." << endl;
		system("pause");
		device->drop();
		return -1;
	}
	{ifstream ifs;
	ifs.open("ressystem/alarm.txt");
	std::string xstr = "";
	getline(ifs, xstr);
	ifs.close();
	std::string str = "";
	str = zgetstrto(xstr, 0, ':');
	alarmtime.wHour = intconvstr(str);
	int xlength = str.length();
	xlength++;
	str = zgetreststr(xstr, xlength);
	alarmtime.wMinute = intconvstr(str);
	}

	//alarm only detects hour and minute, not second, so set second of alarm time to 0
	alarmtime.wSecond = 0;
	cout << "Current time: " << timetostr(systime) << "(" << timetoint(systime.wHour, systime.wMinute, systime.wSecond) << ")" << endl;
	cout << "Alarm time: " << timetostr(alarmtime) << "(" << timetoint(alarmtime.wHour, alarmtime.wMinute, alarmtime.wSecond) << ")" << endl;
	int alarmtimeint = timetoint(alarmtime.wHour, alarmtime.wMinute, alarmtime.wSecond);
	int alarmvolume = 20;
	int alarmloopint = 30;
	int alarmloopmax = 30;
	int alarmloopchange = 300;
	bool alarmtriggered = false;
	bool alarmplaying = false;
	bool alarmon = true;
	if(timetoint(systime.wHour, systime.wMinute, systime.wSecond) > alarmtimeint)alarmtriggered=true;
	bool btns[4] = {false};

	cout << "[Space] = turn alarm off" << endl;
	cout << "[Backspace] = force alarm to play now" << endl;
	cout << "[Escape] = exit program" << endl;

	//start main loop
	while(device->run()){
		xfps.nowTime = xfps.timer->getTime();
		GetLocalTime(&systime);
		if(!alarmtriggered){
			if(timetoint(systime.wHour, systime.wMinute, systime.wSecond) > alarmtimeint){ //get value of time in seconds, if greater than alarm time, alarm starts playing
				alarmplaying = true;
				alarmvolume = 20;
				alarm1->volume = alarmvolume;
				alarmloopint = 30;
				alarmloopmax = 30;
				alarmloopchange = 300;
				Mix_PlayChannel(-1, alarm1, 0);
				alarmtriggered = true;
				cout << "Alarm: TRIGGERED" << endl;
			}
		}else if(alarmplaying){
			if(alarmloopchange == 0){
				if(alarmvolume < 100){
					alarmvolume += 20;
					alarm1->volume = alarmvolume;
				}
				if(alarmloopmax > 10){
					alarmloopmax -= 5;
				}
				alarmloopchange = 300;
			}else{
				alarmloopchange--;
			}
			if(alarmloopint == 0){
				Mix_PlayChannel(-1, alarm1, 0);
				alarmloopint = alarmloopmax;
			}else{
				alarmloopint--;
			}
		}
		if(systime.wHour == 0 && systime.wMinute == 0){
			if(alarmon){
				alarmtriggered = false;
			}else{
				alarmtriggered = true;
			}
		}
		if(receiver.IsKeyDown(KEY_SPACE)){ //key down, on key up: turn alarm off
			if(!btns[0]){
				btns[0] = true;
			}
		}else if(btns[0]){
			btns[0] = false;
			if(alarmplaying){
				alarmplaying = false;
				cout << "Alarm: OFF" << endl;
			}
		}
		if(receiver.IsKeyDown(KEY_ESCAPE)){
			if(!btns[1]){
				btns[1] = true;
			}
		}else if(btns[1]){
			btns[1] = false;
			break;
		}
		if(receiver.IsKeyDown(KEY_BACK)){
			if(!btns[2]){
				btns[2] = true;
			}
		}else if(btns[2]){
			btns[2] = false;
			alarmplaying = true;
			alarmvolume = 20;
			alarm1->volume = alarmvolume;
			alarmloopint = 30;
			alarmloopmax = 30;
			alarmloopchange = 300;
			Mix_PlayChannel(-1, alarm1, 0);
			alarmtriggered = true;
			cout << "Alarm: STARTED (:DECOY)" << endl;
		}

		//Draw cycle: draw bg, draw timebg, draw font stating current time and alarm time
		driver->beginScene(false, false, SColor(255, 255, 255, 255));
		env->drawAll();
		std::wstring time = L"";
		time += zgetintstr2(systime.wHour);
		time += L" : ";
		if(systime.wMinute < 10){
			time += L"0";
		}
		time += zgetintstr2(systime.wMinute);
		font->draw(time.data(), timepos, SColor(255, 255, 255, 53), true, true);
		time = L"\n\n(Alarm: ";
		time += zconvstr(timetostr(alarmtime));
		time += L")";
		font->draw(time.data(), timepos, SColor(255, 255, 92, 43), true, true);
		driver->endScene();
		xfps.sdlwait();
	}
	//drop irrLicht device and close program
	device->drop();
	return 0;
}