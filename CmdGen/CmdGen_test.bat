echo off

REM test generation

REM generate a single aP cell 
echo ; using controls 
CmdGen
type A1
show
count 1
end
echo -----------
REM ; using command line as NOT web
CmdGen 1 A1
show
end
echo -----------
REM ; using command line as web
CmdGen 1 A1 --x
show
end
echo -----------------------------------------------

REM Generate a single F cubic with input from command line
CmdGen 1 cF
show
end
echo -----------
CmdGen 1 cF --x
show
end
echo -----------------------------------------------

REM 
CmdGen
type cF
count 1
show
end
echo -----------

CmdGen --x
type cF
count 1
show
end


echo -----------------------------------------------
CmdGen 1 cF
count 2
type aP
show
end
echo -----------
CmdGen 1 cF --x
count 2
type aP
show
end
echo -----------------------------------------------
CmdGen
type 33
count 3
end
echo -----------------------------------------------



echo on
