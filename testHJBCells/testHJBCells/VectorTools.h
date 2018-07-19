
#ifndef VECTORTOOLS_H
#define VECTORTOOLS_H

#include <algorithm>
#include <string>

#include "MatMN.h"
#include "Mat66.h"
#include "VecN.h"

class Mat66;
class G6;

namespace VectorTools {

    static G6 FromString( const std::string& s );


	void CleanVector(VecN& v);
	void CleanVector(G6& v, Mat66& m);


/*
G6to7:matrix(
[1,0,0,0,0,0],
[0,1,0,0,0,0],
[0,0,1,0,0,0],
[1,1,1,1,1,1],
[0,1,1,1,0,0],
[1,0,1,0,1,0],
[1,1,0,0,0,1]);
G7to6:matrix(
[1,0,0,0,0,0,0],
[0,1,0,0,0,0,0],
[0,0,1,0,0,0,0],
[0,-1,-1,0,1,0,0],
[-1,0,-1,0,0,1,0],
[-1,-1,0,0,0,0,1]);
*/

MatMN SetMatrix7To6( void );
MatMN SetMatrix6To7( void );

const MatMN g_m6To7 = VectorTools::SetMatrix6To7();

const MatMN g_m7To6 = SetMatrix7To6();
};
#endif
