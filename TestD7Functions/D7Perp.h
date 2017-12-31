#ifndef D7PERP_H
#define D7PERP_H

#include <vector>

std::vector< void(*)(const double[], double[])> GetPerpFunctions();
static void D7perp1(const double d[], double dd[]);
static void D7perp2(const double d[], double dd[]);
static void D7perp3(const double d[], double dd[]);
static void D7perp4(const double d[], double dd[]);
static void D7perp5(const double d[], double dd[]);
static void D7perp6(const double d[], double dd[]);
static void D7perp7(const double d[], double dd[]);
static void D7perp8(const double d[], double dd[]);
static void D7perp9(const double d[], double dd[]);

#endif  // D7PERP_H