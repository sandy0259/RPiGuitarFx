/*
 * adapter.cpp
 *
 *  Created on: 17 kwi 2018
 *      Author: SF
 */

#include "adapter.hpp"

Adapter::Adapter(FxGtkList *newFxGtkList, FxGtkView *newFxGtkView):
	fxGtkList(newFxGtkList),
	fxGtkView(newFxGtkView)
	{

}

void Adapter::updateFxGuiList(FXList *fxList){
	static Gtk::TreeModel::Row row;
	int id = 1;

	fxGtkList->treeModel->clear();

	for(auto fx : *fxList->getCurrentFXList()){
		row = *(fxGtkList->treeModel->append());
		row[fxGtkList->columnsModel.idColumn] = id++;
		row[fxGtkList->columnsModel.nameColumn] = Glib::ustring(*fx->getName());
	}
}

void Adapter::setNewFxGuiBox(FXList *fxList, int indxOfFxToUpdate){
	auto fx = fxList->getCurrentFXList()->at(indxOfFxToUpdate).get();

	fxGtkView->fxNameLabel->set_text(*fx->getName());

	//set settings in gui
	int num = 0;
	for(auto fxSetting : *(fx->getSettings())){
		auto fxBox = fxGtkView->fxSettings[num++].get();
		fxBox->show();
		fxBox->fxValueLabel->set_text(*fxSetting.getName());
		fxBox->fxValueScale->set_range(fxSetting.getMinValue(), fxSetting.getMaxValue());
		fxBox->fxValueScale->set_value(fxSetting.getValue());
	}

	//hide unused settings in gui
	for(; num <= fxGtkView->fxSettings.size(); ++num){
		fxGtkView->fxSettings[num]->hide();
	}
}

void Adapter::updateFxGuiBox(FXList *fxList, int indxOfFxToUpdate){
	auto fx = fxList->getCurrentFXList()->at(indxOfFxToUpdate).get();

	int num = 0;
	for(auto fxSetting : *(fx->getSettings())){
		fxGtkView->fxSettings[num].get()->fxValueScale->set_value(fxSetting.getValue());
	}
}
