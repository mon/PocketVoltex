@echo off
:loop
make init
IF %ERRORLEVEL% EQU 0 (
  echo Success!
  ping -n 5 127.0.0.1 >nul
)
goto loop