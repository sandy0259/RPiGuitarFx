/*
 * ncAdapterMoveFx.cpp
 *
 *  Created on: 10 maj 2018
 *      Author: SF
 */

/*
RPiGuitarFx
Copyright (C) 2018  SpeedFight

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ncAdapterMoveFx.hpp"

NcAdapterMoveFx::NcAdapterMoveFx(FXList *newFxList, IDetector *newUserInput, int *newSelectedFxNum):
fxList(newFxList),
userInput(newUserInput),
selectedFxNum(newSelectedFxNum),
selectedOption(0),
actualFxList(nullptr)
{
	fillActualFxList();
}

void NcAdapterMoveFx::fillActualFxList(){
	static std::vector<char *> fxListElements;
	void *bin = actualFxList.release();

	fxListElements.clear();
	for(auto fx : *fxList->getCurrentFXList()){
		fxListElements.push_back((char *)fx->getName()->c_str());
	}

	//if shorter than 6 fx then set width to 8
	int width = (fxListElements.size() + 2 > 21) ? 21 : fxListElements.size() + 2;
	width = (width < 9) ? 8 : width;

	actualFxList.reset(new ElementListNC(fxListElements, "Move FX", 20, width, 30, 2));
	actualFxList->panel->showPanel();
	actualFxList->panel->hidePanel(); //workaround
}

void NcAdapterMoveFx::addToSelectedOption(int diff){

	selectedOption += diff;
	if(selectedOption < 0){
		selectedOption = 0;

	}else if (selectedOption > fxList->getCurrentFXList()->size() - 1){
		selectedOption = fxList->getCurrentFXList()->size() -1;
	}
}

void NcAdapterMoveFx::handleMoveFx(){
	int *pot1 = userInput->getInputHandler(ControllerInput::pot1);
	int *btn1 = userInput->getInputHandler(ControllerInput::btn1);

	selectedOption = *selectedFxNum;
	fillActualFxList();

	actualFxList->panel->showPanel();

	actualFxList->listWindow->selectIndex(selectedOption);

	while(!*btn1){ //stop when button pressed
		std::this_thread::sleep_for (std::chrono::milliseconds(50));


		if(*pot1 != 0){
			addToSelectedOption(-(*pot1));

			if(*selectedFxNum == selectedOption){
				*pot1 = 0;
				continue;
			}

			fxList->moveFX(*selectedFxNum, selectedOption);
			*selectedFxNum = selectedOption;

			std::this_thread::sleep_for (std::chrono::milliseconds(20));
			fillActualFxList();
			actualFxList->listWindow->selectIndex(selectedOption);
			*pot1 = 0;
		}
	}

	while(*btn1){ //wait to stop press button
		std::this_thread::sleep_for (std::chrono::milliseconds(50));
	}

	//selectFxInList(selectedOptionAddFxDialog);
	actualFxList->panel->hidePanel();
}
