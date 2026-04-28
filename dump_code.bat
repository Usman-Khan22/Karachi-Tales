@echo off

set OUTPUT=dump.txt

echo Dumping all .cpp and .h files into %OUTPUT%...
echo. > %OUTPUT%

for %%f in (*.cpp *.h) do (
    echo ===== %%f ===== >> %OUTPUT%
    type "%%f" >> %OUTPUT%
    echo. >> %OUTPUT%
)

echo Done!
pause
