/*
 * fxViewNC.cpp
 *
 *  Created on: 5 maj 2018
 *      Author: SF
 */

#include "fxViewNC.hpp"
#include "string.h"

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

/*
 * FxSettingViewNC
 */

FxSettingViewNC::FxSettingViewNC(std::string newValueName, std::string newInitName, int newWindowWidth, int newWindowHeight, int newWindowPosX, int newWindowPosY):
		windowWidth(newWindowWidth  ),
		windowHeight(newWindowHeight ),
		windowPosX(newWindowPosX   ),
		windowPosY(newWindowPosY   ),
		valueName(newValueName),
		value(newInitName),
		window(newwin(newWindowHeight, newWindowWidth, newWindowPosY, newWindowPosX))
{
	box(window.get(), 0, 0);

	//print setting name
	print_in_middle(window.get(), 2, 0, windowWidth, (char*)valueName.c_str(), COLOR_PAIR(1));

	//print setting init value
	print_in_middle(window.get(), 4, 0, windowWidth, (char*)value.c_str(), COLOR_PAIR(1));

	wrefresh(window.get());
}

FxSettingViewNC::~FxSettingViewNC(){
	delwin(window.get());
	refresh();
}

void FxSettingViewNC::updateValue(std::string newValueName){
	//print setting init value
	value = newValueName;
	print_in_middle(window.get(), 4, 0, windowWidth, (char*)value.c_str(), COLOR_PAIR(1));

	wrefresh(window.get());
}

/*
 * FxInfoViewNC
 */

FxInfoViewNC::FxInfoViewNC(std::string newFxname, int newColorScheme,int newWindowWidth, int newWindowHeight, int newWindowPosX, int newWindowPosY):
		windowWidth(newWindowWidth  ),
		windowHeight(newWindowHeight ),
		windowPosX(newWindowPosX   ),
		windowPosY(newWindowPosY   ),
		fxName(newFxname),
		colorScheme(newColorScheme),
		window(newwin(newWindowHeight, newWindowWidth, newWindowPosY, newWindowPosX))
{
	box(window.get(), 0, 0);

	//print setting name
	print_in_middle(window.get(), 2, 0, windowWidth, (char*)fxName.c_str(), colorScheme);

	wrefresh(window.get());
}

FxInfoViewNC::~FxInfoViewNC(){
	delwin(window.get());
	refresh();
}

/*
 * FxViewNC
 */

FxViewNC::FxViewNC(){
//	settingsBoxs.fill(nullptr);

	fxInfoBox.reset(new FxInfoViewNC("Simple overdrive", 1, 60, 5, 20, 4));
	settingsBoxs[0].reset(new FxSettingViewNC("gain", "10%", 20, 6, 20, 13));
	settingsBoxs[1].reset(new FxSettingViewNC("gain", "10%", 20, 6, 40, 13));
	settingsBoxs[2].reset(new FxSettingViewNC("gain", "10%", 20, 6, 20, 13));
	settingsBoxs[3].reset(new FxSettingViewNC("gain", "10%", 20, 6, 60, 13));
	settingsBoxs[4].reset(new FxSettingViewNC("gain", "10%", 20, 6, 20, 19));
//	settingsBoxs[5].reset(new FxSettingViewNC("gain", "10%", 20, 6, 40, 19));
//	settingsBoxs[6].reset(new FxSettingViewNC("gain", "10%", 20, 6, 60, 19));

}

FxViewNC::~FxViewNC(){

}

void FxViewNC::refresh(){
	redrawwin(fxInfoBox->window.get());
	wnoutrefresh(fxInfoBox->window.get());


	for(auto &settingBox : settingsBoxs){
		if(settingBox != nullptr){
			redrawwin(settingBox->window.get());
			wnoutrefresh(settingBox->window.get());
		}else {
			break;
		}
	}
	doupdate();
}
