#ifndef D7PRJ_H
#define D7PRJ_H

#include <vector>

std::vector< void(*)(const double[], double[])> GetProjectorFunctions();
static void D7prj1(const double d[], double dd[]);
static void D7prj2(const double d[], double dd[]);
static void D7prj3(const double d[], double dd[]);
static void D7prj4(const double d[], double dd[]);
static void D7prj5(const double d[], double dd[]);
static void D7prj6(const double d[], double dd[]);
static void D7prj7(const double d[], double dd[]);
static void D7prj8(const double d[], double dd[]);
static void D7prj9(const double d[], double dd[]);

#endif //  D7PRJ_H