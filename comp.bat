set PATH=E:\Programmation\Psx\PSYQ\BIN
set PSYQ_PATH=E:\Programmation\Psx\PSYQ\BIN

set prog=
set address=$80100000
::set param=-DCDROM_RELEASE
set lib=..\MyPsxLib

echo off
cls
ccpsx -I%lib%\. %param% -O2 -Xo%address% %prog%.c %lib%\BSdec.c %lib%\DataManager.c %lib%\System.c %lib%\Movie.c -o%prog%.cpe,,%prog%.map
pause

cpe2x %prog%.cpe
del %prog%.psx
del %prog%.cpe
ren %prog%.exe %prog%.PSX
