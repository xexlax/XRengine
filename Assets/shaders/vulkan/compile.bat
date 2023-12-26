

@echo off
setlocal EnableDelayedExpansion

set "srcDir=src"
set "outputDir=output"
set "glslcPath=C:/VulkanSDK/1.3.261.1/Bin/glslc.exe"

if not exist "%outputDir%" mkdir "%outputDir%"

for %%G in ("%srcDir%\*.vert") do (
    set "shaderName=%%~nG"
    "%glslcPath%" "!srcDir!\!shaderName!.vert" -o "!outputDir!\!shaderName!_vert.spv"
)

for %%G in ("%srcDir%\*.frag") do (
    set "shaderName=%%~nG"
    "%glslcPath%" "!srcDir!\!shaderName!.frag" -o "!outputDir!\!shaderName!_frag.spv"
)

echo Compilation completed.

pause
endlocal