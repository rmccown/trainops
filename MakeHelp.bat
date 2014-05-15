@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TRAINOPS.HPJ. >"hlp\TrainOps.hm"
echo. >>"hlp\TrainOps.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\TrainOps.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\TrainOps.hm"
echo. >>"hlp\TrainOps.hm"
echo // Prompts (IDP_*) >>"hlp\TrainOps.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\TrainOps.hm"
echo. >>"hlp\TrainOps.hm"
echo // Resources (IDR_*) >>"hlp\TrainOps.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\TrainOps.hm"
echo. >>"hlp\TrainOps.hm"
echo // Dialogs (IDD_*) >>"hlp\TrainOps.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\TrainOps.hm"
echo. >>"hlp\TrainOps.hm"
echo // Frame Controls (IDW_*) >>"hlp\TrainOps.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\TrainOps.hm"
REM -- Make help for Project TRAINOPS


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\TrainOps.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\TrainOps.hlp" goto :Error
if not exist "hlp\TrainOps.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\TrainOps.hlp" Debug
if exist Debug\nul copy "hlp\TrainOps.cnt" Debug
if exist Release\nul copy "hlp\TrainOps.hlp" Release
if exist Release\nul copy "hlp\TrainOps.cnt" Release
echo.
goto :done

:Error
echo hlp\TrainOps.hpj(1) : error: Problem encountered creating help file

:done
echo.
