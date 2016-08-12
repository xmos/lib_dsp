// Copyright (c) 2016, XMOS Ltd, All rights reserved
// ===========================================================================
// ===========================================================================
//	
// File: FIROS3.c
//
// FIROS3 functions implementation file 
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

// FIROS3 includes
#include "dsp_os3.h"

// ===========================================================================
//
// Defines
//
// ===========================================================================


// State init value
#define		FIROS3_STATE_INIT						0




// ===========================================================================
//
// Variables
//
// ===========================================================================

int					iFirOS3Coefs[FIROS3_N_COEFS]	 = {
	#include FIROS3_COEFS_FILE	
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
// Function:		FIROS3_init											//
// Arguments:		FIROS3Ctrl_t 	*psFIROS3Ctrl: Ctrl strct.			//
// Return values:	FIROS3_NO_ERROR on success							//
//					FIROS3_ERROR on failure								//
// Description:		Inits the FIROS3 									//
// ==================================================================== //
FIROS3ReturnCodes_t				FIROS3_init(FIROS3Ctrl_t* psFIROS3Ctrl)
{
	// Check that delay line base has been set
	if(psFIROS3Ctrl->piDelayB == 0)
		return FIROS3_ERROR;


	// Setup from FIROS2 descriptor
	psFIROS3Ctrl->uiDelayL			= (FIROS3_N_COEFS/FIROS3_N_PHASES)<<1;						// Double length for circular buffer simulation. x3 over-sampler, so only 1/3rd of coefs length needed
	psFIROS3Ctrl->piDelayW			= psFIROS3Ctrl->piDelayB + (FIROS3_N_COEFS/FIROS3_N_PHASES);	
	psFIROS3Ctrl->uiDelayO			= (FIROS3_N_COEFS/FIROS3_N_PHASES);
	psFIROS3Ctrl->uiNLoops			= (FIROS3_N_COEFS/FIROS3_N_PHASES)>>1;						// Due to 2 x 32bits read for coefs per inner loop and x3 over-sampler, so only 1/3rd of coefs length needed
	psFIROS3Ctrl->uiNCoefs			= FIROS3_N_COEFS;
	psFIROS3Ctrl->piCoefs			= iFirOS3Coefs;

	// Sync the FIROS3
	if(FIROS3_sync(psFIROS3Ctrl) != FIROS3_NO_ERROR)
		return FIROS3_ERROR;

	return FIROS3_NO_ERROR;
}


// ==================================================================== //
// Function:		FIROS3_sync											//
// Arguments:		FIROS3Ctrl_t 	*psFIROS3Ctrl: Ctrl strct.			//
// Return values:	FIROS3_NO_ERROR on success							//
//					FIROS3_ERROR on failure								//
// Description:		Syncs the FIROS3 									//
// ==================================================================== //
FIROS3ReturnCodes_t				FIROS3_sync(FIROS3Ctrl_t* psFIROS3Ctrl)
{
	unsigned int		ui;

	// Set delay line index back to base
	psFIROS3Ctrl->piDelayI	= psFIROS3Ctrl->piDelayB;

	// Clear delay line
	for(ui = 0; ui < psFIROS3Ctrl->uiDelayL; ui++)
		psFIROS3Ctrl->piDelayB[ui]	= FIROS3_STATE_INIT;

	// Set phase to '0' to indicate that a new input sample is needed
	psFIROS3Ctrl->iPhase = 0;

	return FIROS3_NO_ERROR;
}


// ==================================================================== //
// Function:		FIROS3_input										//
// Arguments:		FIROS3Ctrl_t 	*psFIROS3Ctrl: Ctrl strct.			//
// Return values:	FIROS3_NO_ERROR on success							//
//					FIROS3_ERROR on failure								//
// Description:		Writes new input sample to FIROS3 delay line		//
// ==================================================================== //
FIROS3ReturnCodes_t				FIROS3_input(FIROS3Ctrl_t* psFIROS3Ctrl)
{
	// Write new input sample from control structure to delay line 
	// at both positions for circular buffer simulation
	*psFIROS3Ctrl->piDelayI									= psFIROS3Ctrl->iIn;
	*(psFIROS3Ctrl->piDelayI + psFIROS3Ctrl->uiDelayO)		= psFIROS3Ctrl->iIn;

	// Step delay index with circular buffer simulation 
	psFIROS3Ctrl->piDelayI				+= 1;
	if(psFIROS3Ctrl->piDelayI >= psFIROS3Ctrl->piDelayW)						
		psFIROS3Ctrl->piDelayI		= psFIROS3Ctrl->piDelayB;	

	return FIROS3_NO_ERROR;
}


// ==================================================================== //
// Function:		FIROS3_proc											//
// Arguments:		FIROS3Ctrl_t 	*psFIROS3Ctrl: Ctrl strct.			//
// Return values:	FIROS3_NO_ERROR on success							//
//					FIROS3_ERROR on failure								//
// Description:		Processes the FIROS3								//
// ==================================================================== //
FIROS3ReturnCodes_t				FIROS3_proc(FIROS3Ctrl_t* psFIROS3Ctrl)
{
	int*			piData;
	int*			piCoefs;
	int				iData;
	int				iCoef0, iCoef1;
	__int64_t		i64Acc;
	unsigned		ui;
	
	// Clear accumulator and set data/coefs access pointers
	piData					= psFIROS3Ctrl->piDelayI;
	piCoefs					= psFIROS3Ctrl->piCoefs + psFIROS3Ctrl->iPhase;
	i64Acc					= 0;

    if ((unsigned)piData & 0b0100) fir_inner_loop_asm_odd(piData, piCoefs, &iData, psFIROS3Ctrl->uiNLoops);
    else fir_inner_loop_asm(piData, piCoefs, &iData, psFIROS3Ctrl->uiNLoops);

	// Write output
	psFIROS3Ctrl->iOut		= iData;	

	// Step phase
	psFIROS3Ctrl->iPhase += (FIROS3_N_COEFS/FIROS3_N_PHASES);
	if(psFIROS3Ctrl->iPhase == FIROS3_N_COEFS)
		psFIROS3Ctrl->iPhase = 0;

	return FIROS3_NO_ERROR;
}

