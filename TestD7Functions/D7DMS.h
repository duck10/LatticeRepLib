#ifndef D7DMS_H
#define D7DMS_H

#include <vector>

std::vector< void(*)(const double[], double[])> GetDMSFunctions();
static void D7DMS1(const double d[], double dd[]);
static void D7DMS2(const double d[], double dd[]);
static void D7DMS3(const double d[], double dd[]);
static void D7DMS4(const double d[], double dd[]);
static void D7DMS5(const double d[], double dd[]);
static void D7DMS6(const double d[], double dd[]);
static void D7DMS7(const double d[], double dd[]);
static void D7DMS8(const double d[], double dd[]);
static void D7DMS9(const double d[], double dd[]);

#endif  // D7DMS_H