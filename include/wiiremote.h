/************************************************************
* Copyright (C) 2006-2008 Masahiko SAWAI All Rights Reserved. 
************************************************************/
#ifndef WIIREMOTE_H
#define WIIREMOTE_H

/** \mainpage
* This is a libwiiremote API document.
* <p>
* libwiiremote is a multi-platform C library for communicating
* with the Nintendo Wii Remote controler. 
* </p>
* @see wiiremote.h
**/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> /* size_t */


#define WRMT_MAX_DEVICES 32

#define WRMT_MASK_BUTTON_TWO    0x0001
#define WRMT_MASK_BUTTON_ONE    0x0002
#define WRMT_MASK_BUTTON_B      0x0004
#define WRMT_MASK_BUTTON_A      0x0008
#define WRMT_MASK_BUTTON_MINUS  0x0010
#define WRMT_MASK_BUTTON_HOME   0x0080
#define WRMT_MASK_BUTTON_LEFT   0x0100
#define WRMT_MASK_BUTTON_RIGHT  0x0200
#define WRMT_MASK_BUTTON_DOWN   0x0400
#define WRMT_MASK_BUTTON_UP     0x0800
#define WRMT_MASK_BUTTON_PLUS   0x1000
#define WRMT_MASK_BUTTONS (\
	WRMT_MASK_BUTTON_TWO|WRMT_MASK_BUTTON_ONE| \
	WRMT_MASK_BUTTON_B|WRMT_MASK_BUTTON_A| \
	WRMT_MASK_BUTTON_MINUS|WRMT_MASK_BUTTON_HOME| \
	WRMT_MASK_BUTTON_LEFT|WRMT_MASK_BUTTON_RIGHT| \
	WRMT_MASK_BUTTON_DOWN|WRMT_MASK_BUTTON_UP| \
	WRMT_MASK_BUTTON_PLUS)

#define WRMT_MASK_LED_1    0x0001
#define WRMT_MASK_LED_2    0x0002
#define WRMT_MASK_LED_3    0x0004
#define WRMT_MASK_LED_4    0x0008
#define WRMT_MASK_LEDS (\
	WRMT_MASK_LED_1|WRMT_MASK_LED_2|\
	WRMT_MASK_LED_3|WRMT_MASK_LED_4)

/**
* speaker sample rate id
* 11 (0x0B) = 4200 Hz
* 12 (0x0C) = 3920 Hz
* 13 (0x0D) = 3640 Hz
* 14 (0x0E) = 3360 Hz
* 15 (0x0F) = 3080 Hz 
**/
#define WRMT_SPEAKER_SAMPLE_RATE_4200 0x0B
#define WRMT_SPEAKER_SAMPLE_RATE_3920 0x0C
#define WRMT_SPEAKER_SAMPLE_RATE_3640 0x0D
#define WRMT_SPEAKER_SAMPLE_RATE_3360 0x0E
#define WRMT_SPEAKER_SAMPLE_RATE_3080 0x0F


/**
* type definitions 
**/

/**
* Function Type ID
*
* @see WRMT_WiiRemote_IsEnabled(), WRMT_WiiRemote_SetEnabled()
**/
typedef enum 
{
	WRMT_FUNCTION_CONTINUOUS = 0,
	WRMT_FUNCTION_MOTION,
	WRMT_FUNCTION_IR,
	WRMT_FUNCTION_SPEAKER,
	WRMT_NUMBER_OF_FUNCTIONS,
} WRMT_FunctionType;

/**
* Data Type ID
*
* @see WRMT_WiiRemote_GetState(), WRMT_WiiRemote_SetState()
**/
typedef enum 
{
	/* read write data */
	WRMT_DATA_FORCE_FEEDBACK = 0,
	WRMT_DATA_LEDS,
	WRMT_DATA_SPEAKER_FORMAT,
	WRMT_DATA_SPEAKER_VOLUME,
	WRMT_DATA_SPEAKER_SAMPLE_RATE,
	/* read only data */
	WRMT_DATA_BUTTONS,
	WRMT_DATA_MOTION_X,
	WRMT_DATA_MOTION_Y,
	WRMT_DATA_MOTION_Z,
	WRMT_DATA_IR1_FOUND,
	WRMT_DATA_IR1_SIZE,
	WRMT_DATA_IR1_X,
	WRMT_DATA_IR1_Y,
	WRMT_DATA_IR2_FOUND,
	WRMT_DATA_IR2_SIZE,
	WRMT_DATA_IR2_X,
	WRMT_DATA_IR2_Y,
	WRMT_DATA_IR_FOUND,
	WRMT_DATA_IR_SIZE,
	WRMT_DATA_IR_X,
	WRMT_DATA_IR_Y,
	WRMT_DATA_BATTERY_LEVEL, /* 0 - 200(?) */
	WRMT_NUMBER_OF_DATA_TYPE,
} WRMT_DataType;

typedef enum 
{
	WRMT_IO_TIMEOUT = -2,
	WRMT_IO_ERROR = -1,
	WRMT_IO_SUCCESS = 0,
} WRMT_IOReturn;

struct wrmt_wiiremote;
typedef struct wrmt_wiiremote WRMT_WiiRemote;

/**
* function declarations
**/

/* WRMT_DECLSPEC for win32 */
#ifndef WRMT_DECLSPEC
# if defined(WIN32) && (!defined(__GNUC__)) && (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x500))
#  ifdef LIBWIIREMOTE_EXPORTS
#   define WRMT_DECLSPEC __declspec(dllexport)
#  else
#   define WRMT_DECLSPEC __declspec(dllimport)
#  endif
# endif
#endif

/* WRMT_DECLSPEC for others */
#ifndef WRMT_DECLSPEC
# define WRMT_DECLSPEC
#endif


/**
* Set Error.
**/
extern
WRMT_DECLSPEC
void
WRMT_SetError(const char *message);

/**
* Get Error.
**/
extern
WRMT_DECLSPEC
char *
WRMT_GetError();

/**
* Initialize this library system. 
* It is necessary to call this function before
* the call of any other functions. 
**/
extern
WRMT_DECLSPEC
int
WRMT_Init();

/**
* Quit library system.
* You should call this function before stop the program. 
**/
extern
WRMT_DECLSPEC
void
WRMT_Quit();

/**
* Sleep.
* Pause for <code>ms</code> milliseconds.
*
* @param ms  the sleeping time specified by the millisecond. 
**/
extern
WRMT_DECLSPEC
void
WRMT_Sleep(int ms);

/**
* Update all opened Wii Remote devices.
*
* @return WRMT_IO_SUCCESS or WRMT_IO_ERROR
* @see WRMT_WiiRemote_GetState()
**/
extern
WRMT_DECLSPEC
int
WRMT_UpdateAll();

/**
* Checks whether there is input data from the Wii Remote device.
* The state is updated if there is data. 
*
* @return WRMT_IO_ERROR, WRMT_IO_TIMEOUT, or updated device index.
* @see WRMT_WiiRemote_GetState()
**/
extern
WRMT_DECLSPEC
int
WRMT_Poll();

/**
* Get the number of Wii Remote devices.
*
* @return the number of Wii Remote devices.
**/
extern
WRMT_DECLSPEC
int
WRMT_GetNumWiiRemote();

/**
* Get the Wii Remote device at specified index.
* If an illegal value is passed to <code>device_index</code>,
* the result is undefined. 
*
* @param  device_index  the index of device.
*                       a valid range is 0 to WRMT_GetNumWiiRemote()-1.
* @return the reference of WRMT_WiiRemote.
**/
extern
WRMT_DECLSPEC
WRMT_WiiRemote *
WRMT_GetWiiRemoteAt(int device_index);

/**
* Open connection to Wii Remote
*
* @param self  the reference of WRMT_WiiRemote. 
* @return  WRMT_IO_SUCCESS or WRMT_IO_ERROR
**/
extern
WRMT_DECLSPEC
WRMT_IOReturn
WRMT_WiiRemote_Open(WRMT_WiiRemote *self);

/**
* Checks whether the connection to Wii Remote has opened. 
*
* @param self  the reference of WRMT_WiiRemote. 
* @return  return 1 if the device is opened, or 0.
**/
extern
WRMT_DECLSPEC
int
WRMT_WiiRemote_IsOpened(WRMT_WiiRemote *self);

/**
* Close connection to Wii Remote.
*
* @param self  the reference of WRMT_WiiRemote. 
**/
extern
WRMT_DECLSPEC
void
WRMT_WiiRemote_Close(WRMT_WiiRemote *self);

/**
* Checks whether the specified function is enabled. 
* If an illegal value is passed to <code>function_type</code>,
* the result is undefined. 
*
* @param self  the reference of WRMT_WiiRemote. 
* @param function_type  the type of function.
* @return  enable : 1, disable : 0
**/
extern
WRMT_DECLSPEC
int 
WRMT_WiiRemote_IsEnabled(WRMT_WiiRemote *self,
	WRMT_FunctionType function_type);

/**
* Enable or disable the specified function. 
* When WRMT_WiiRemote_Update() is called, the function is actually enabled. 
* If an illegal value is passed to <code>function_type</code>,
* the result is undefined. 
*
* @param self  the reference of WRMT_WiiRemote. 
* @param function_type  the type of function.
* @param value  enable : 1, disable : 0
* @see WRMT_WiiRemote_Update()
**/
extern
WRMT_DECLSPEC
void
WRMT_WiiRemote_SetEnabled(WRMT_WiiRemote *self,
	WRMT_FunctionType function_type, int value);

/**
* Get device state from Wii Remote.
* If an illegal value is passed to <code>data_type</code>,
* the result is undefined. 
*
* @param self  the reference of WRMT_WiiRemote. 
* @param data_type  the type of data.
* @return  the various values by the <code>data_type</code>. 
**/
extern
WRMT_DECLSPEC
int 
WRMT_WiiRemote_GetState(WRMT_WiiRemote *self, WRMT_DataType data_type);

/**
* Sets the state of the Wii Remote device. 
* When WRMT_WiiRemote_Update() is called, the value is actually 
* transmitted. 
* If an illegal value is passed to <code>data_type</code>,
* the result is undefined. 
*
* @param self       the reference of WRMT_WiiRemote. 
* @param data_type  the type of data.
* @param value      the various values by the <code>data_type</code>. 
* @see WRMT_WiiRemote_Update()
**/
extern
WRMT_DECLSPEC
void
WRMT_WiiRemote_SetState(WRMT_WiiRemote *self, WRMT_DataType data_type,
	int value);

/**
* Update the state of the Wii Remote device. 
*
* @param self       the reference of WRMT_WiiRemote. 
* @return  WRMT_IO_SUCCESS or WRMT_IO_ERROR
* @see WRMT_WiiRemote_SetEnabled()
* @see WRMT_WiiRemote_SetState()
**/
extern
WRMT_DECLSPEC
WRMT_IOReturn
WRMT_WiiRemote_Update(WRMT_WiiRemote *self);

/**
* Writes data to the specified memory area. 
* @param self       the reference of WRMT_WiiRemote. 
* @param address    
* @param data       
* @param data_size  
**/
extern
WRMT_DECLSPEC
WRMT_IOReturn
WRMT_WiiRemote_WriteDataToMemory(WRMT_WiiRemote *self,
	int address, const unsigned char *data, size_t data_size);

/**
* Play sound.
* @param self            the reference of WRMT_WiiRemote. 
* @param sound_data      the pointer of sound data.
* @param sound_data_size the size of sound data.
**/
extern
WRMT_DECLSPEC
WRMT_IOReturn
WRMT_WiiRemote_PlaySound(WRMT_WiiRemote *self,
	const unsigned char *sound_data, size_t sound_data_size);

#ifdef __cplusplus
}
#endif

#endif /* WIIREMOTE_H */

