echo off

REM test CmdSella

REM CmdSella for single cell
echo ; using controls 
CmdSella
P 10 20 30 89.9 89.9 89
show
end
echo -----------
REM ; using command line as NOT web
CmdSella 
random 2
show
end
echo -----------
REM ; using command line as web
CmdSella --x
random 2
show
end
echo -----------------------------------------------

REM CmdSella 
CmdSella
prefix test1
show
end
echo -----------

REM 
CmdSella
bockstat 2
blocsize 6
show
end


echo on
