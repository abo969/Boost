@echo off

echo start to compile all examples
echo.

echo making multiindexcontainer11.cpp ...
cl /wd 4530 /nologo multiindexcontainer11.cpp
echo.

echo making multiindexcontainer12.cpp ...
cl /wd 4530 /nologo multiindexcontainer12.cpp
echo.

echo making multiindexcontainer13.cpp ...
cl /wd 4530 /nologo multiindexcontainer13.cpp
echo.

del *.obj

echo done. bye.
pause
