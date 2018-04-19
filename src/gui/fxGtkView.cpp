/*
 * fxGtkView.cpp
 *
 *  Created on: 12 kwi 2018
 *      Author: SF
 */

#include "fxGtkView.hpp"

FxGtkView::FxGtkView():
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10),
	gridFxValues(Gtk::manage(new Gtk::Grid())),
	fxNameFrame(Gtk::manage(new Gtk::Frame())),
	fxNameLabel(Gtk::manage(new Gtk::Label()))
{
	gridFxValues->set_row_spacing(40);
	gridFxValues->set_column_spacing(200);

	//fxNameFrame->set_shadow_type(Gtk::SHADOW_IN);
	//fxNameFrame->override_background_color(Gdk::RGBA("green"));

	fxNameFrame->add(*fxNameLabel);
	pack_start(*fxNameFrame, Gtk::PACK_EXPAND_PADDING);
	pack_start(*gridFxValues, Gtk::PACK_EXPAND_PADDING);

	/*
	 * fill setting grid
	 */

	//FxSetting *set1, *set2, *set3, *set4, *set5, *set6;
	fxSettings[0].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));
	fxSettings[1].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));
	fxSettings[2].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));
	fxSettings[3].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));
	fxSettings[4].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));
	fxSettings[5].reset(Gtk::manage(new FxSetting("gain", 0.0, 0.0, 100.0, 0.1)));

	gridFxValues->attach(*fxSettings[0].get(), 0, 0, 2, 1);
	gridFxValues->attach(*fxSettings[1].get(), 1, 0, 2, 1);
	gridFxValues->attach(*fxSettings[2].get(), 2, 0, 2, 1);
	gridFxValues->attach(*fxSettings[3].get(), 0, 1, 2, 1);
	gridFxValues->attach(*fxSettings[4].get(), 1, 1, 2, 1);
	gridFxValues->attach(*fxSettings[5].get(), 2, 1, 2, 1);
}

FxGtkView::~FxGtkView() {}

/*
 * FxSetting
 */

FxGtkView::FxSetting::FxSetting(std::string valueName, double initVal, double min, double max, double stepIncrement):
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10),
	fxValueLabel(Gtk::manage(new Gtk::Label(valueName))),
	fxValueScale(Gtk::manage(new Gtk::Scale(Gtk::Adjustment::create(initVal, min, max, stepIncrement, 10, 0))))
{
	pack_start(*fxValueLabel, Gtk::PACK_EXPAND_WIDGET);
	pack_start(*fxValueScale, Gtk::PACK_EXPAND_WIDGET);
}

void FxGtkView::FxSetting::setFxSetting(std::string valueName, double initVal, double min, double max, double stepIncrement)
{
	fxValueLabel->set_text(valueName);
	fxValueScale = Gtk::manage(new Gtk::Scale(Gtk::Adjustment::create(initVal, min, max, stepIncrement, 10, 0)));
}

FxGtkView::FxSetting::~FxSetting() {}
