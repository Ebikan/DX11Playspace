#pragma once

#include "EvieWin.h"

#define __LOC__ __FILE__, __LINE__

#ifdef _MSC_VER
#define VoidCasting_IKnowWhatIAmDoing __pragma(warning(suppress:26490))
#else 
#define VoidCasting_IKnowWhatIAmDoing _Pragma("warning(suppress:26490)")
#endif