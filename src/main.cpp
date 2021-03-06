/*
 * main.cpp
 *
 *  Created on: 23 mar 2018
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

#include "config.hpp"
#include "audio.hpp"
#include "effect.hpp"
#include "controller.hpp"

#include "keyboard.hpp"
#include "encoder.hpp"

#include "effectsList.hpp"

#ifdef GTK_UI
	#include "mainGtkView.hpp"
	#include "adapter.hpp"
#else
	#include "nCoursesKeyboard.hpp"
	#include "ncAdapter.hpp"
#endif
/**
 * @brief Main procedure start fx list, init threads etc.
 * @param argc
 * @param argv
 * @return
 */
int main( int argc, char * argv[] )
{
#ifdef ROTARY_ENCODER
	std::unique_ptr<IDetector> controller(new Encoder());
#else
	#ifdef GTK_UI
	std::unique_ptr<IDetector> controller(new Keyboard(argc, argv));
	#else
	std::unique_ptr<IDetector> controller(new NCourseKeyboard());
	#endif
#endif

	std::unique_ptr<FXList> fxList(new FXList());

	/*
	 * Init effects list
	 */
//	fxList->addFX(new PlaybackFx(controller.get()));
//	fxList->addFX(new Delay(controller.get()));
	fxList->addFX(new Fuzz(controller.get()));
	fxList->addFX(new ToneStackEq(controller.get()));
//	fxList->addFX(new Flanger(controller.get()));
//	fxList->addFX(new Chorus(controller.get()));
//	fxList->addFX(new Vibrato(controller.get()));
//	fxList->addFX(new Clipper(controller.get()));

	std::unique_ptr<Audio> input(new Audio(fxList.get()));
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	std::unique_ptr<ViewGtk> view(new ViewGtk(argc, argv));
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	#ifdef GTK_UI
		std::unique_ptr<Adapter> adapter(new Adapter(fxList.get(), controller.get(), view->getFxGtkList(), view->getFxGtkView(), argc, argv));
	#else
		std::unique_ptr<NcAdapter> adapter(new NcAdapter(fxList.get(), controller.get()));
	#endif
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	std::thread guiThread(&ViewGtk::poolForView, view.get());
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	std::thread controllerInputThread(&IDetector::pollForEvents, controller.get());
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	#ifdef GTK_UI
	std::thread handleUserInputThread(&Adapter::handleUserInput, adapter.get());
	#else
	std::thread handleUserInputThread(&NcAdapter::handleUserInput, adapter.get());
	#endif
	std::this_thread::sleep_for (std::chrono::milliseconds(500));

	/*
	 * endless loop
	 */
	while(1){
		std::this_thread::sleep_for(std::chrono::seconds(60 * 60));
	}

	guiThread.~thread();
	controllerInputThread.~thread();
	handleUserInputThread.~thread();

	view.reset();
	input.reset();
	fxList.reset();
	controller.reset();
    return 0;
}
