#!/bin/csh
set rundate=BGAOL_tests_`date "+%m%d%H%M%Y.%S"`
echo "Test of BGAOL " > ${rundate}_cur
foreach file (*.in)
  echo $file >> ${rundate}_cur
  ../bgaol < $file >> ${rundate}_cur
end
diff -bu ${rundate}_cur bgaol_tests_orig > bgaol_tests.diff
cat bgaol_tests.diff
