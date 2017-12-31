
#if _MSC_VER
#pragma once // this is to keep the Visual Studio compiler happy
#endif

#ifndef SUPERFASHHASH_H
#define SUPERFASHHASH_H

#include "stdint.h" /* Replace with <stdint.h> if appropriate */

uint32_t SuperFastHash (const char * data, int len);

#endif
