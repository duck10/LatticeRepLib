#ifndef REDUCETRANSFORMSTEST_H
#define REDUCETRANSFORMSTEST_H

#include "MatG6.h"

// Function to test ReduceWithTransforms with random cells
int TestReduceWithTransformsRandom(const size_t numTests);

// Function to test specific test cases
void TestSpecificCases();

// Function to test cells with challenging properties
void TestChallengingCells();

// Main test function that runs all the tests
int TestReduceTransforms(bool runRandomTests = true);

// Function declarations from our test modules
void TestMatrixCorrespondence(const MatG6& matG6, const Matrix_3x3& mat3D, const std::string& matrixName, const G6& testCell);
void TestAllMatrixPairs();
void TestSignVariations();
void ExploreMatrixSigns(const MatG6& matG6, const Matrix_3x3& baseMatrix, const std::string& matrixName, const G6& testCell);
void ExploreKeyMatrices();
void DeriveCorrect3DMatrices();
void TestSingleCell();



#endif // REDUCETRANSFORMSTEST_H
