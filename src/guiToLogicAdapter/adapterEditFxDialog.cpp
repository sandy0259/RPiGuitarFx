/*
 * adapterEditFxDialog.cpp
 *
 *  Created on: 21 kwi 2018
 *      Author: SF
 */

#include "adapterEditFxDialog.hpp"

AdapterEditFxDialog::AdapterEditFxDialog(int argc, char *argv[], IDetector *newUserInput, FXList *newFxList, int *newSelectedFxNum):
	userInput(newUserInput),
	fxList(newFxList),
	selectedFxNum(newSelectedFxNum),
	fxDialog(new DialogWindowFxList(argc, argv)),
	selectedOptionFxListDialog(0),
	addFxDialog(new AdapterAddFxDialog(argc, argv, newUserInput, newFxList, newSelectedFxNum)),
	moveFxDialog(new AdapterMoveFxDialog(argc, argv, newUserInput, newFxList, newSelectedFxNum))
	{
}

void AdapterEditFxDialog::addToSelectedOptionFxListDialog(int diff){
	selectedOptionFxListDialog += diff;
	if(selectedOptionFxListDialog < 0){
		selectedOptionFxListDialog = 0;

	}else if (selectedOptionFxListDialog > fxDialog->buttons.size() - 1){
		selectedOptionFxListDialog = fxDialog->buttons.size() - 1;
	}
}


void AdapterEditFxDialog::handleEditFxDialog(){

	int *pot1 = userInput->getInputHandler(ControllerInput::pot1);
	int *btn1 = userInput->getInputHandler(ControllerInput::btn1);
	if(*btn1){
		std::this_thread::sleep_for (std::chrono::milliseconds(800));
		if(*btn1){ //check if button is pressed through given time
			fxDialog->showDialog();
			fxDialog->markButton(selectedOptionFxListDialog);

			while(*btn1){ //wait to stop press button
				std::this_thread::sleep_for (std::chrono::milliseconds(100));
			}

			while(!*btn1){ //stop when button pressed
				std::this_thread::sleep_for (std::chrono::milliseconds(100));

				if(*pot1 != 0){
					addToSelectedOptionFxListDialog(*pot1);
					fxDialog->markButton(selectedOptionFxListDialog);
					*pot1 = 0;
					}
				}

			while(*btn1){ //wait to stop press button
				std::this_thread::sleep_for (std::chrono::milliseconds(100));
			}

		fxDialog->hideDialog();
		//button stop pressed
			switch (selectedOptionFxListDialog) {

				case DialogWindowBtn::cancelBtn:
					break;

				case DialogWindowBtn::addNextBtn:
					addFxDialog->handleEditFxDialog();
					break;

				case DialogWindowBtn::movelBtn:
					moveFxDialog->handleMoveFxDialog();
					break;

				case DialogWindowBtn::deletelBtn:

					if(fxList->getCurrentFXList()->size() == 1){
						fxList->addFX(FxFactory::createFx(0, userInput));
					}

					fxList->removeFX(*selectedFxNum);

					*selectedFxNum = *selectedFxNum - 1;
					if (*selectedFxNum < 0){
						*selectedFxNum = 0;
					}
					break;

				default:
					break;
			}

			*btn1 = 0; //because without this selected dialog window can show again
			selectedOptionFxListDialog = 0;
		}
	}
}
