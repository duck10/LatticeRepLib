@echo off
REM Test instructions for Unified Lattice Matcher
echo Creating test input sequences...

REM Test 1: Simple direct match
echo ; Test 1: Simple direct match > test1_input.txt
echo ; Copy and paste these lines when prompted: >> test1_input.txt
echo DETAILS true >> test1_input.txt
echo DISTANCETHRESHOLD 5.0 >> test1_input.txt
echo P 10.0 10.1 10.2 90.0 90.1 90.2 >> test1_input.txt
echo P 10.05 10.15 10.25 90.05 90.15 90.25 >> test1_input.txt
echo end >> test1_input.txt

REM Test 2: Niggli reduction
echo ; Test 2: Niggli reduction needed > test2_input.txt
echo ; Copy and paste these lines when prompted: >> test2_input.txt
echo DETAILS true >> test2_input.txt
echo USENIGGLI true >> test2_input.txt
echo DISTANCETHRESHOLD 1.0 >> test2_input.txt
echo P 8.0 9.0 10.0 80.0 85.0 95.0 >> test2_input.txt
echo P 8.1 8.9 10.1 100.0 95.0 85.0 >> test2_input.txt
echo end >> test2_input.txt

REM Test 3: B4Matcher test
echo ; Test 3: B4Matcher test > test3_input.txt
echo ; Copy and paste these lines when prompted: >> test3_input.txt
echo DETAILS true >> test3_input.txt
echo MAXMATRICES 1500 >> test3_input.txt
echo COMBINATIONDEPTH 3 >> test3_input.txt
echo DISTANCETHRESHOLD 10.0 >> test3_input.txt
echo P 12.5 8.7 15.2 110.5 95.8 88.2 >> test3_input.txt
echo P 15.2 12.5 8.7 88.2 110.5 95.8 >> test3_input.txt
echo end >> test3_input.txt

REM Test 4: Progressive guided cloud
echo ; Test 4: Progressive guided cloud > test4_input.txt
echo ; Copy and paste these lines when prompted: >> test4_input.txt
echo DETAILS true >> test4_input.txt
echo TESTSTRATEGY GUIDED >> test4_input.txt
echo CLOUDRADIUS 2.5 >> test4_input.txt
echo CLOUDSTEPS 50 >> test4_input.txt
echo MAXITERATIONS 100 >> test4_input.txt
echo DISTANCETHRESHOLD 0.5 >> test4_input.txt
echo P 6.2 7.8 9.4 103.2 87.6 91.4 >> test4_input.txt
echo P 9.4 6.2 7.8 91.4 103.2 87.6 >> test4_input.txt
echo end >> test4_input.txt

REM Test 5: Bidirectional search
echo ; Test 5: Bidirectional search > test5_input.txt
echo ; Copy and paste these lines when prompted: >> test5_input.txt
echo DETAILS true >> test5_input.txt
echo TESTBIDIRECTIONAL true >> test5_input.txt
echo MAXITERATIONS 50 >> test5_input.txt
echo DISTANCETHRESHOLD 2.0 >> test5_input.txt
echo P 11.2 13.7 9.8 125.6 67.3 102.8 >> test5_input.txt
echo P 13.7 9.8 11.2 102.8 125.6 67.3 >> test5_input.txt
echo end >> test5_input.txt

REM Test 6: 90-degree challenge
echo ; Test 6: 90-degree singularity > test6_input.txt
echo ; Copy and paste these lines when prompted: >> test6_input.txt
echo DETAILS true >> test6_input.txt
echo DISTANCETHRESHOLD 1.0 >> test6_input.txt
echo P 8.5 8.5 12.0 89.5 90.5 90.0 >> test6_input.txt
echo P 8.5 8.5 12.0 90.5 89.5 90.0 >> test6_input.txt
echo end >> test6_input.txt

REM Test 7: Algorithm comparison
echo ; Test 7: Algorithm comparison > test7_input.txt
echo ; Copy and paste these lines when prompted: >> test7_input.txt
echo DETAILS true >> test7_input.txt
echo COMPARESTRATEGIES true >> test7_input.txt
echo TESTALL true >> test7_input.txt
echo DISTANCETHRESHOLD 5.0 >> test7_input.txt
echo P 7.2 8.9 11.6 78.4 95.2 103.7 >> test7_input.txt
echo P 11.6 7.2 8.9 103.7 78.4 95.2 >> test7_input.txt
echo end >> test7_input.txt

REM Test 8: Large distance case
echo ; Test 8: Large distance case > test8_input.txt
echo ; Copy and paste these lines when prompted: >> test8_input.txt
echo DETAILS true >> test8_input.txt
echo MAXMATRICES 3000 >> test8_input.txt
echo COMBINATIONDEPTH 4 >> test8_input.txt
echo MAXCOEFFICIENT 3 >> test8_input.txt
echo DISTANCETHRESHOLD 20.0 >> test8_input.txt
echo P 5.8 9.2 14.6 115.3 72.8 98.4 >> test8_input.txt
echo P 14.6 5.8 9.2 98.4 115.3 72.8 >> test8_input.txt
echo end >> test8_input.txt

echo.
echo Test input files created:
echo   test1_input.txt - Simple direct match
echo   test2_input.txt - Niggli reduction test  
echo   test3_input.txt - B4Matcher test
echo   test4_input.txt - Progressive guided cloud
echo   test5_input.txt - Bidirectional search
echo   test6_input.txt - 90-degree singularity
echo   test7_input.txt - Algorithm comparison
echo   test8_input.txt - Large distance case
echo.
echo TO RUN TESTS:
echo 1. Type: transformer
echo 2. When prompted, copy and paste the contents of a test file
echo 3. Watch the output
echo.
echo Example:
echo   transformer
echo   [paste contents of test1_input.txt]
echo.
echo Each test file contains the exact input to type/paste.

pause