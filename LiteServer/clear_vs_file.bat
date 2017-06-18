@echo off

del *.sdf
del /s /q Debug
rd /s /q Debug
del /s /q X64
rd /s /q X64

cd VS
for /d %%a in (*) do (
	set curDir = %%a
	@echo %%a
	cd %%a
	del /s /q Debug
	rd /s /q Debug
	del /s /q X64
	rd /s /q X64
	cd ..
)

pause