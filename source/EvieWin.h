/******************************************************************************
	
	Evie's Windows Includer. 
	
	Removes all the shit windows loads that I dont need.
	Feel Free to use. 

	Author: Evie Brown

	email: evie@eviebrown.com

******************************************************************************/
#pragma once

// Disable GDI and Window Application Stuff
#define DISABLEALLGDIAPIEXTRA

#define WIN32_LEAN_AND_MEAN

// Don't need GetSystemMetrics() codes
#define NOSYSMETRICS
// Disable extended Icon set. (Standard Icon IDs)
//#define NOICONS
// TEXTMETRIC and associated routines
#define NOTEXTMETRIC
// Remove MIN and MAX macro (super useful)
#define NOMINMAX
// Disable COMM driver routines.
#define NOCOMM            
// Disable Kanji Defines and routines.
#define NOKANJI
// Disable help interface.
#define NOHELP

#ifdef DISABLEALLGDIAPIEXTRA
// Not even using GDI
#define NOGDICAPMASKS
// Not using the WinAPI menus (lite)
#define NOMENUS
// Global GDI disable.
//#define NOGDI
#endif

#include <Windows.h>