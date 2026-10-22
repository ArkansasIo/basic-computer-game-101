@echo off
if exist build\Release\basicos.exe (
  build\Release\basicos.exe
) else if exist build\basicos.exe (
  build\basicos.exe
) else (
  echo Run build.bat first.
)
