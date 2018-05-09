/*
 * mainNCrurses.cpp
 *
 *  Created on: 4 maj 2018
 *      Author: SF
 */

#include "mainNCrurses.hpp"

std::vector<char *> choices2 = {
						"wybor 0",
                        "wybor 1",
                        "wybor 2",
                        "wybor 3",
                        "wybor 4",
						"wybor 5",
						"wybor 6",
						"wybor 7",
						"wybor 8",
                        "Exit"
                  };


TerminalGui::TerminalGui(){
	initscr();
	cbreak();
	noecho();
	curs_set(0); //invisible cursor
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);

	//int windowWidth, int windowHeight, int windowPosX, int windowPosY

	std::unique_ptr<ElementListNC> fxList(new ElementListNC(choices2, "FX list", 20, 24, 0, 1));
	fxList->panel->showPanel();
	fxList->panel->hidePanel();
	fxList->panel->showPanel(); //workaround

	std::unique_ptr<ElementListNC> optionsList(new ElementListNC(choices2, "Options", 20, 21, 30, 2));

	std::unique_ptr<FxViewNC> fxView(new FxViewNC());

	refresh();


	int ch;
	while((ch = getch()) != KEY_F(1))
	{	switch(ch){
			case 'a':
				optionsList->panel->showPanel();
			break;

			case 'b':
				optionsList->panel->hidePanel();
				fxView->refresh();
			break;

			case KEY_DOWN:
				optionsList->listWindow->selectByDiff(1);
			break;

			case KEY_UP:
				optionsList->listWindow->selectByDiff(-1);
			break;
		}
	}

	while(1){

	}
}

TerminalGui::~TerminalGui(){
	endwin();			/* End curses mode		  */
}


