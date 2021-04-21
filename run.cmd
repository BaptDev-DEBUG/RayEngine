@echo off
g++ src/Sources/main.cpp -o bin/SRE.exe -I include -I src/Headers -L lib -lsfml-graphics -lsfml-window -lsfml-system
cd bin
SRE.exe
pause