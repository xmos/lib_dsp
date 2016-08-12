// Copyright (c) 2016, XMOS Ltd, All rights reserved

 
#ifndef _FIROS3_H_
#define _FIROS3_H_

    // ===========================================================================
    //
    // Defines
    //
    // ===========================================================================
    
    // General defines
    // ---------------
    // Filter related

    #define     FIROS3_N_COEFS                          144                     // Number of coefficients must be a multiple of 6
    #define     FIROS3_N_PHASES                         3                       // Number of output phases (3 as OS3 over-sample by 3)

    // This filter has about 20dB attenuation at 8kHz but later cutoff
    //#define     FIROS3_COEFS_FILE                       "FilterData/firos3_144.dat"
    // This filter has more than 60dB attenuation at 8kHz
    #define       FIROS3_COEFS_FILE                       "FilterData/firos3_b_144.dat"
    


    // ===========================================================================
    //
    // TypeDefs
    //
    // ===========================================================================

    // To avoid C type definitions when including this file from assembler
    #ifndef INCLUDE_FROM_ASM

        // FIROS3 Return Codes
        // -------------------
        typedef enum _FIROS3ReturnCodes                                 
        {
            FIROS3_NO_ERROR                         = 0,
            FIROS3_ERROR                            = 1
        } FIROS3ReturnCodes_t;

        // FIROS3 Ctrl
        // -----------
#ifdef __XC__
        typedef struct _FIROS3Ctrl                                      
        {
            int                                     iIn;            // Input data (to be updated every 3 output samples, i.e. when iPhase == 0)
            int                                     iOut;           // Output data (1 sample)
            int                                     iPhase;         // Current output phase (when reaching '0', a new input sample is required)

            int* unsafe                             piDelayB;       // Pointer to delay line base
            unsigned int                            uiDelayL;       // Total length of delay line
            int* unsafe                             piDelayI;       // Pointer to current position in delay line
            int* unsafe                             piDelayW;       // Delay buffer wrap around address (for circular buffer simulation)
            unsigned int                            uiDelayO;       // Delay line offset for second write (for circular buffer simulation)

            unsigned int                            uiNLoops;       // Number of inner loop iterations
            unsigned int                            uiNCoefs;       // Number of coefficients
            int* unsafe                             piCoefs;        // Pointer to coefficients
        } FIROS3Ctrl_t;
#else
        typedef struct _FIROS3Ctrl                                      
        {
            int                                     iIn;            // Input data (to be updated every 3 output samples, i.e. when iPhase == 0)
            int                                     iOut;           // Output data (1 sample)
            int                                     iPhase;         // Current output phase (when reaching '0', a new input sample is required)

            int*                                    piDelayB;       // Pointer to delay line base
            unsigned int                            uiDelayL;       // Total length of delay line
            int*                                    piDelayI;       // Pointer to current position in delay line
            int*                                    piDelayW;       // Delay buffer wrap around address (for circular buffer simulation)
            unsigned int                            uiDelayO;       // Delay line offset for second write (for circular buffer simulation)

            unsigned int                            uiNLoops;       // Number of inner loop iterations
            unsigned int                            uiNCoefs;       // Number of coefficients
            int*                                    piCoefs;        // Pointer to coefficients
        } FIROS3Ctrl_t;
#endif

        
/** This function initialises the oversample by 3 function for a given instance
 *
 *
 *  \param      *psFIROS3Ctrl   OS3 control structure of type FIROS3Ctrl_t
 *  \returns    Error code of type FIRDS3ReturnCodes_t
 */
#ifdef __XC__        
        FIROS3ReturnCodes_t             FIROS3_init(FIROS3Ctrl_t* unsafe psFIROS3Ctrl);
#else
        FIROS3ReturnCodes_t             FIROS3_init(FIROS3Ctrl_t* psFIROS3Ctrl);
#endif  

/** This function clears the oversample by 3 delay line for a given instance
 *
 *
 *  \param      *psFIROS3Ctrl   OS3 control structure of type FIROS3Ctrl_t
 *  \returns    Error code of type FIRDS3ReturnCodes_t
 */
#ifdef __XC__
        FIROS3ReturnCodes_t             FIROS3_sync(FIROS3Ctrl_t* unsafe psFIROS3Ctrl);
#else
        FIROS3ReturnCodes_t             FIROS3_sync(FIROS3Ctrl_t* psFIROS3Ctrl);
#endif

/** This function pushes a single input sample into the filter
 *  It should be called three times for each FIROS3_proc call
 *
 *
 *  \param      *psFIROS3Ctrl   OS3 control structure of type FIROS3Ctrl_t
 *  \returns    Error code of type FIROS3ReturnCodes_t
 */
#ifdef __XC__
        FIROS3ReturnCodes_t             FIROS3_input(FIROS3Ctrl_t* unsafe psFIROS3Ctrl);
#else
        FIROS3ReturnCodes_t             FIROS3_input(FIROS3Ctrl_t* psFIROS3Ctrl);
#endif

/** This function performs the oversampling by 3 and outputs one sample
 *  The input and output buffers are pointed to by members of the psFIROS3Ctrl structure
 *
 *
 *  \param      *psFIROS3Ctrl   OS3 control structure of type FIROS3Ctrl_t
 *  \returns    Error code of type FIROS3ReturnCodes_t
 */
#ifdef __XC__
        FIROS3ReturnCodes_t             FIROS3_proc(FIROS3Ctrl_t* unsafe psFIROS3Ctrl);    
#else
        FIROS3ReturnCodes_t             FIROS3_proc(FIROS3Ctrl_t* psFIROS3Ctrl);    
#endif
    #endif // nINCLUDE_FROM_ASM

#endif // _FIROS3_H_