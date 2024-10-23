@echo off
SET /P _file= Podaj nazwe pliku: 
Program.exe -file < "%_file%"
pause