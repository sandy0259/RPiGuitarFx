/*
 * toneStackEq.cpp
 *
 *  Created on: 2 maj 2018
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

#include "toneStackEq.hpp"

const std::string ToneStackEq::nameFx("Tone stack EQ");

const float toneStacksModels[][7] = {
	/* for convenience, */
	#define k *1e3
	#define M *1e6
	#define nF *1e-9
	#define pF *1e-12
	/* parameter order is R1 - R4, C1 - C3 */
	/* R1=treble R2=Bass R3=Mid, C1-3 related caps, R4 = parallel resistor */
	{250 k, 1 M, 25 k, 56 k, 250 pF, 20 nF, 20 nF},  /* 59 Bassman 5F6-A */
	{250 k, 1 M, 25 k, 47 k, 600 pF, 20 nF, 20 nF}, /* Mesa Dual Rect. 'Orange' */
	/* Vox -- R3 is fixed (circuit differs anyway) */
	{1 M, 1 M, 20 k, 100 k, 50 pF, 22 nF, 22 nF}, /* 59/86 Vox AC-30 */
	{220 k, 1 M, 22 k, 33 k, 470 pF, 22 nF, 22 nF}, /* 59/81 Jcm-800 Lead 100 2203 */
	#undef k
	#undef M
	#undef nF
	#undef pF
};

void ToneStackEq::process(jack_nframes_t nframes, JackCpp::AudioIO::audioBufVector inBufs, JackCpp::AudioIO::audioBufVector outBufs){

	//TODO rework
	static float c = 2*audioSettings::fs;
	static float lowPot = 0.5;
	static float midPot, highPot, tone;
	static std::array<float,8> reg = {0, 0, 0, 0, 0, 0, 0, 0};

	static std::array<float,4> numCoeff = {0, 0, 0, 0};
	static std::array<float,4> denomCoeff = {0, 0, 0, 0};

	//pot give value from 0 to 100;
	float b = settings.at(0).getValue() / 30.0; //bass
	float m = settings.at(1).getValue() / 30.0;	//mid
	float t = settings.at(2).getValue() / 30.0;	//treble

	unsigned short tonestack = settings.at(3).getValue();	//tonestack parameters
	R1 = toneStacksModels[tonestack][0];
	R2 = toneStacksModels[tonestack][1];
	R3 = toneStacksModels[tonestack][2];
	R4 = toneStacksModels[tonestack][3];

	C1 = toneStacksModels[tonestack][4];
	C2 = toneStacksModels[tonestack][5];
	C3 = toneStacksModels[tonestack][6];


	if(lowPot != b || midPot != m || highPot != t || tone != tonestack ){
		lowPot 	= b;
		midPot 	= m;
		highPot = t;
		tone = tonestack;
		double b1t, b1m, b1l, b1d,
				b2t, b2m2, b2m, b2l, b2lm, b2d,
				b3lm, b3m2, b3m, b3t, b3tm, b3tl,
				a0, a1d, a1m, a1l, a2m, a2lm, a2m2, a2l, a2d,
				a3lm, a3m2, a3m, a3l, a3d;      // intermediate calculations


		//calculate coeffecients
		b1t  = C1*R1;
		b1m  = C3*R3;
		b1l  = C1*R2 + C2*R2;
		b1d  = C1*R3 + C2*R3;
		b2t  = C1*C2*R1*R4 + C1*C3*R1*R4;
		b2m2 = -(C1*C3*R3*R3 + C2*C3*R3*R3);
		b2m  = C1*C3*R1*R3 + C1*C3*R3*R3 + C2*C3*R3*R3;
		b2l  = C1*C2*R1*R2 + C1*C2*R2*R4 + C1*C3*R2*R4;
		b2lm = C1*C3*R2*R3 + C2*C3*R2*R3;
		b2d  = C1*C2*R1*R3 + C1*C2*R3*R4 + C1*C3*R3*R4;
		b3lm = C1*C2*C3*R1*R2*R3 + C1*C2*C3*R2*R3*R4;
		b3m2 = -(C1*C2*C3*R1*R3*R3 + C1*C2*C3*R3*R3*R4);
		b3m  = C1*C2*C3*R1*R3*R3 + C1*C2*C3*R3*R3*R4;
		b3t  = C1*C2*C3*R1*R3*R4;
		b3tm = -b3t;
		b3tl = C1*C2*C3*R1*R2*R4;
		a0   = 1;
		a1d  = C1*R1 + C1*R3 + C2*R3 + C2*R4 + C3*R4;
		a1m  = C3*R3;
		a1l  = C1*R2 + C2*R2;
		a2m  = C1*C3*R1*R3 - C2*C3*R3*R4 + C1*C3*R3*R3 + C2*C3*R3*R3;
		a2lm = C1*C3*R2*R3 + C2*C3*R2*R3;
		a2m2 = -(C1*C3*R3*R3 + C2*C3*R3*R3);
		a2l  = C1*C2*R2*R4 + C1*C2*R1*R2 + C1*C3*R2*R4 + C2*C3*R2*R4;
		a2d  = C1*C2*R1*R4 + C1*C3*R1*R4 + C1*C2*R3*R4
				+ C1*C2*R1*R3 + C1*C3*R3*R4 + C2*C3*R3*R4;
		a3lm = C1*C2*C3*R1*R2*R3 + C1*C2*C3*R2*R3*R4;
		a3m2 = -(C1*C2*C3*R1*R3*R3 + C1*C2*C3*R3*R3*R4);
		a3m  = C1*C2*C3*R3*R3*R4 + C1*C2*C3*R1*R3*R3 - C1*C2*C3*R1*R3*R4;
		a3l  = C1*C2*C3*R1*R2*R4;
		a3d  = C1*C2*C3*R1*R3*R4;

		struct {
				double a1, a2, a3;
				double b1, b2, b3;
		} acoef; // analog coefficients

		// digital coefficients
		double dcoef_a[3 + 1];
		double dcoef_b[3 + 1];

		m = (m - 1) * 3.5;
		m = pow (10, m);

		acoef.a1 = a1d + m*a1m + b*a1l;
		acoef.a2 = m*a2m + b*m*a2lm + m*m*a2m2 + b*a2l + a2d;
		acoef.a3 = b*m*a3lm + m*m*a3m2 + m*a3m + b*a3l + a3d;

		double c2 = c*c, c3 = c2*c;

		acoef.a1 *= c, acoef.a2 *= c2, acoef.a3 *= c3;

		dcoef_a[0] = -1 - acoef.a1 - acoef.a2 - acoef.a3; // sets scale
		dcoef_a[1] = -3 - acoef.a1 + acoef.a2 + 3*acoef.a3;
		dcoef_a[2] = -3 + acoef.a1 + acoef.a2 - 3*acoef.a3;
		dcoef_a[3] = -1 + acoef.a1 - acoef.a2 + acoef.a3;

		acoef.b1 = t*b1t + m*b1m + b*b1l + b1d;
		acoef.b2 = t*b2t + m*m*b2m2 + m*b2m + b*b2l + b*m*b2lm + b2d;
		acoef.b3 = b*m*b3lm + m*m*b3m2 + m*b3m + t*b3t + t*m*b3tm + t*b*b3tl;

		acoef.b1 *= c, acoef.b2 *= c2, acoef.b3 *= c3;

		dcoef_b[0] = - acoef.b1 - acoef.b2 - acoef.b3;
		dcoef_b[1] = - acoef.b1 + acoef.b2 + 3*acoef.b3;
		dcoef_b[2] = acoef.b1 + acoef.b2 - 3*acoef.b3;
		dcoef_b[3] = acoef.b1 - acoef.b2 + acoef.b3;

		double a0i = 1. / dcoef_a[0];
		for (int i=1; i<=3; ++i)
			denomCoeff[i] = dcoef_a[i] * a0i;

		for (int i=0; i<=3; ++i)
			numCoeff[i] = dcoef_b[i] * a0i;

	}


	int N = 3;

	  int j, k;

	  for(j=0; j<=nframes; j++){
			// Shift the register values.
			for(k=N; k>0; k--){
				reg[k] = reg[k-1];
			}

			// The denominator
			reg[0] = inBufs[0][j];
			for(k=1; k<=N; k++){
			  reg[0] -= denomCoeff[k] * reg[k];
			 }

			// The numerator
			float y = 0;
			for(k=0; k<=N; k++) {
			  y += numCoeff[k] * reg[k];
			 }

			outBufs[0][j] = y * 0.98;
		   }



}

ToneStackEq::ToneStackEq(IDetector *newUserInput):
		IFX(newUserInput)
		{
	settings = std::vector<Setting>{
		Setting("low", userInput->getInputHandler(ControllerInput::pot2), 15, 0, 30),
		Setting("mid", userInput->getInputHandler(ControllerInput::pot3), 15, 0, 30),
		Setting("high", userInput->getInputHandler(ControllerInput::pot4), 15, 0, 30),
		Setting("Bassm-Mesa-JCM-T20", userInput->getInputHandler(ControllerInput::pot6), 0, 0, 3)
	};

	/* parameter order is R1 - R4, C1 - C3 */
	/* R1=treble R2=Bass R3=Mid, C1-3 related caps, R4 = parallel resistor */

//	C1 = 250e-12;
//	C2 = 22e-9;
//	C3 = 22e-9;
//	R1 = 250e3; // highR
//	R2 = 1e6; // lowR
//	R3 = 25e3; // midR
//	R4 = 56e3;

	R1 = toneStacksModels[0][0];
	R2 = toneStacksModels[0][1];
	R3 = toneStacksModels[0][2];
	R4 = toneStacksModels[0][3];

	C1 = toneStacksModels[0][4];
	C2 = toneStacksModels[0][5];
	C3 = toneStacksModels[0][6];
}

ToneStackEq::~ToneStackEq(){
}

const std::string *ToneStackEq::getName(){
	return &ToneStackEq::nameFx;
}

std::vector<Setting> *ToneStackEq::getSettings(){

}



