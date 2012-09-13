@echo off

echo start to compile all examples
echo.

echo making multiindexcontainer1.cpp ...
cl /wd 4530 /wd 4715 /nologo multiindexcontainer1.cpp
echo.

echo making multiindexcontainer2.cpp ...
cl /wd 4530 /nologo multiindexcontainer2.cpp
echo.

echo making multiindexcontainer3.cpp ...
cl /wd 4530 /nologo multiindexcontainer3.cpp
echo.

echo making multiindexcontainer4.cpp ...
cl /wd 4530 /nologo multiindexcontainer4.cpp
echo.

echo making multiindexcontainer5.cpp ...
cl /wd 4530 /nologo multiindexcontainer5.cpp
echo.


del *.obj

echo done. bye.
pause
