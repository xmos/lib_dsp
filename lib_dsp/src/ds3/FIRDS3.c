// Copyright (c) 2016, XMOS Ltd, All rights reserved
// ===========================================================================
// ===========================================================================
//	
// File: FIRDS3.c
//
// FIRDS3 functions implementation file 
//
// ===========================================================================
// ===========================================================================


// ===========================================================================
//
// Includes
//
// ===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// FIRDS3 includes
#include "dsp_ds3.h"
#include "fir_inner_loop_asm.h"


// ===========================================================================
//
// Defines
//
// ===========================================================================


// State init value
#define		FIRDS3_STATE_INIT						0




// ===========================================================================
//
// Variables
//
// ===========================================================================

int					iFirDS3Coefs[FIRDS3_N_COEFS]	 = {
	#include FIRDS3_COEFS_FILE	
};

// ===========================================================================
//
// Local Functions prototypes
//
// ===========================================================================



// ===========================================================================
//
// Functions implementations
//
// ===========================================================================

// ==================================================================== //
// Function:		FIRDS3_init											//
// Arguments:		FIRDS3Ctrl_t 	*psFIRDS3Ctrl: Ctrl strct.			//
// Return values:	FIRDS3_NO_ERROR on success							//
//					FIRDS3_ERROR on failure								//
// Description:		Inits the FIRDS3 									//
// ==================================================================== //
FIRDS3ReturnCodes_t				FIRDS3_init(FIRDS3Ctrl_t* psFIRDS3Ctrl)
{
	// Check that delay line base has been set
	if(psFIRDS3Ctrl->piDelayB == 0)
		return FIRDS3_ERROR;


	// Setup from FIRDS2 descriptor
	psFIRDS3Ctrl->uiDelayL			= FIRDS3_N_COEFS<<1;									// Double length for circular buffer simulation
	psFIRDS3Ctrl->piDelayW			= psFIRDS3Ctrl->piDelayB + FIRDS3_N_COEFS;	
	psFIRDS3Ctrl->uiDelayO			= FIRDS3_N_COEFS;
	psFIRDS3Ctrl->uiNLoops			= FIRDS3_N_COEFS>>1;									// Due to 2 x 32bits read for coefs per inner loop
	psFIRDS3Ctrl->uiNCoefs			= FIRDS3_N_COEFS;
	psFIRDS3Ctrl->piCoefs			= iFirDS3Coefs;

	// Sync the FIRDS3
	if(FIRDS3_sync(psFIRDS3Ctrl) != FIRDS3_NO_ERROR)
		return FIRDS3_ERROR;

	return FIRDS3_NO_ERROR;
}


// ==================================================================== //
// Function:		FIRDS3_sync											//
// Arguments:		FIRDS3Ctrl_t 	*psFIRDS3Ctrl: Ctrl strct.			//
// Return values:	FIRDS3_NO_ERROR on success							//
//					FIRDS3_ERROR on failure								//
// Description:		Syncs the FIRDS3 									//
// ==================================================================== //
FIRDS3ReturnCodes_t				FIRDS3_sync(FIRDS3Ctrl_t* psFIRDS3Ctrl)
{
	unsigned int		ui;

	// Set delay line index back to base
	psFIRDS3Ctrl->piDelayI	= psFIRDS3Ctrl->piDelayB;

	// Clear delay line
	for(ui = 0; ui < psFIRDS3Ctrl->uiDelayL; ui++)
		psFIRDS3Ctrl->piDelayB[ui]	= FIRDS3_STATE_INIT;

	return FIRDS3_NO_ERROR;
}


// ==================================================================== //
// Function:		FIRDS3_proc											//
// Arguments:		FIRDS3Ctrl_t 	*psFIRDS3Ctrl: Ctrl strct.			//
// Return values:	FIRDS3_NO_ERROR on success							//
//					FIRDS3_ERROR on failure								//
// Description:		Processes the FIRDS3								//
// ==================================================================== //
FIRDS3ReturnCodes_t				FIRDS3_proc(FIRDS3Ctrl_t* psFIRDS3Ctrl)
{
	int*			piData;
	int*			piCoefs;
	int				iData0;
	__int64_t			i64Acc;

	// Get three new data samples to delay line (double write for circular buffer simulation)
	iData0					= *psFIRDS3Ctrl->piIn;
	*psFIRDS3Ctrl->piDelayI									= iData0;
	*(psFIRDS3Ctrl->piDelayI + psFIRDS3Ctrl->uiDelayO)		= iData0;

	iData0					= *(psFIRDS3Ctrl->piIn + 1);
	*(psFIRDS3Ctrl->piDelayI + 1)							= iData0;
	*(psFIRDS3Ctrl->piDelayI + psFIRDS3Ctrl->uiDelayO + 1)	= iData0;

	iData0					= *(psFIRDS3Ctrl->piIn + 2);
	*(psFIRDS3Ctrl->piDelayI + 2)							= iData0;
	*(psFIRDS3Ctrl->piDelayI + psFIRDS3Ctrl->uiDelayO + 2)	= iData0;

	// Step delay with circular simulation (will also rewrite to control structure for next round)
	// Note as delay line length is a multiple of 3 (because filter coefficients length is a multiple of 3)
	// we only have to do the wrap around check after having written 3 input samples
	psFIRDS3Ctrl->piDelayI				+= 3;
	if(psFIRDS3Ctrl->piDelayI >= psFIRDS3Ctrl->piDelayW)						
		psFIRDS3Ctrl->piDelayI		= psFIRDS3Ctrl->piDelayB;	
		
	// Clear accumulator and set access pointers
	piData					= psFIRDS3Ctrl->piDelayI;
	piCoefs					= psFIRDS3Ctrl->piCoefs;
	i64Acc					= 0;
    if ((unsigned)piData & 0b0100) fir_inner_loop_asm_odd(piData, piCoefs, &iData0, psFIRDS3Ctrl->uiNLoops);
    else fir_inner_loop_asm(piData, piCoefs, &iData0, psFIRDS3Ctrl->uiNLoops);

	// Write output
	*psFIRDS3Ctrl->piOut		= iData0;	

	return FIRDS3_NO_ERROR;
}

