@echo off

pushd build
cl -Zi  ../code/main.c
popd

@REM gcc  -ggdb -Og -w -DDEBUG=1  code/main.c -o build/main
