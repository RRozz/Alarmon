// Connecting_with_SQLConnect.cpp
// compile with: user32.lib odbc32.lib
#include <windows.h>
#include "Shlwapi.h"
#include <string>
#include <sstream>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int checkForFile(char *fname){
	int exists = 0;
	exists = PathFileExistsA(fname);
	return exists;
}
int zgetExponent(int xint, int zExponent){
	//
	int zint = 0;
	int zxint = 0;
	zint = xint;
	for(zxint = 1;zxint < zExponent;zxint++){
		zint *= xint;
	}
	return zint;
}
class MyEventReceiver : public IEventReceiver
{
public:
    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
		float wheel;
        SMouseState() : LeftButtonDown(false), wheel(7) { }
    } MouseState;
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_RMOUSE_PRESSED_DOWN:
                MouseState.RightButtonDown = true;
                break;

            case EMIE_RMOUSE_LEFT_UP:
                MouseState.RightButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

			case EMIE_MOUSE_WHEEL:
				MouseState.wheel += event.MouseInput.Wheel;
				break;

            default:
                // We won't use the wheel
                break;
            }
        }
		//if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			//KeyIsDown[event.MouseInput.ButtonStates] = event.MouseInput.isLeftPressed;
		//if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			//KeyIsDown[event.MouseInput.ButtonStates] = event.MouseInput.Event;

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

	virtual bool ControlKeyCheck(int keyint) const{
		return KeyIsDown[keyint];
	}
    
    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
class xfpslimiter{
	//
	int MaxFPS;
	int MaxSDLFPS;
	int FPS;
public:
	int nowTime;
	xfpslimiter();
	void sdlwait();
	irr::ITimer *timer;
};
xfpslimiter::xfpslimiter(){
	//
	MaxFPS = 30;
	MaxSDLFPS = 30;
	FPS = 0;
	nowTime = 0;
}
void xfpslimiter::sdlwait(){
	if(timer->getTime() - nowTime < 1000/MaxFPS){
		FPS = timer->getTime() - nowTime;
		if(FPS == 0){
			Sleep(1000/MaxSDLFPS);
		}else{
			Sleep(int((int((1000/FPS)))/MaxSDLFPS));
		}
	}
}
int intconvchar(std::string xchar){
	//
	char zchar = xchar[0];
	//zchar = xchar.data();
	switch(zchar){
		case '0':{
			return 0;
			break;
				 }
		case '1':{
			return 1;
			break;
				 }
		case '2':{
			return 2;
			break;
				 }
		case '3':{
			return 3;
				 }
		case '4':{
			return 4;
			break;
				 }
		case '5':{
			return 5;
			break;
				 }
		case '6':{
			return 6;
			break;
				 }
		case '7':{
			return 7;
			break;
				 }
		case '8':{
			return 8;
			break;
				 }
		case '9':{
			return 9;
			break;
				 }
		default:{
			return 0;
				}
	}
}
int intconvstr(std::string xchar){
	//
	if(xchar.length() < 1){
		return 0;
	}
	int xint = 0;
	std::string xstring = "";
	int xlength = xchar.length();
	for(int i=0;i<xlength;i++){
		xstring = xchar[((xlength - 1) - i)];
		xint += (intconvchar(xstring) * (i > 0 ? (zgetExponent(10, i)) : (1)));
	}
	if(xchar[0] == '-'){
		//xint -= 2;
		/**
		float xint2 = 0;;
		xint2 = xint;
		xint2 *= -1;
		return xint2;**/
		int xint2 = 0;
		xint2 -= xint;
		return xint2;
	}
	return xint;
}
std::wstring zgetintstr2(int xint){
	wostringstream osstream;
	osstream << xint;
	return osstream.str();
}
std::string zgetreststr(std::string xstring, int xint){
	//
	std::string zxstring = "";
	int zxint = 0;
	zxint = xstring.length();
	for(int xint2 = xint;xint2<zxint;xint2++){
		zxstring += xstring[xint2];
	}
	return zxstring;
}
std::string zgetstrto(std::string xstring, int xint, char tochar){
	//
	std::string zxstring = "";
	for(int xint2 = xint;xint2<xstring.length();xint2++){
		if(xstring[xint2] == tochar){
			break;
		}else{
			zxstring += xstring[xint2];
		}
	}
	return zxstring;
}
std::wstring zgetstrto2(std::wstring xstring, int xint, wchar_t tochar){
	//
	std::wstring zxstring = L"";
	for(int xint2 = xint;xint2<xstring.length();xint2++){
		if(xstring[xint2] == tochar){
			break;
		}else{
			zxstring += xstring[xint2];
		}
	}
	return zxstring;
}
std::string zgetintstr(int xint){
	ostringstream osstream;
	osstream << xint;
	return osstream.str();
}
char xconvchar(wchar_t xchar){
	//
	char zchar = int(xchar);
	return zchar;
}
wchar_t zconvchar(char xchar){
	//
	wchar_t zchar = int(xchar);
	return zchar;
}
std::string xconvstr(std::wstring xstring){
	//
	int xlength = xstring.length();
	std::string zstring = "";
	for(int xint = 0;xint < xlength;xint++){
		zstring += xconvchar(xstring[xint]);
	}
	return zstring;
}
std::wstring zconvstr(std::string xstring){
	//
	int xlength = xstring.length();
	std::wstring zstring = L"";
	for(int xint = 0;xint < xlength;xint++){
		zstring += zconvchar(xstring[xint]);
	}
	return zstring;
}
int timetoint(int hour, int min, int sec){
	int xint = 0;
	xint += sec;
	xint += (min * 60);
	xint += (hour * 3600);
	return xint;
}
std::string timetostr(SYSTEMTIME time){
	std::string str;
	str += zgetintstr(time.wHour);
	str += " : ";
	if(time.wMinute < 10)str += "0";
	str += zgetintstr(time.wMinute);
	return str;
}