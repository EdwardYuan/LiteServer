@echo off
for /d %%a in (*) do (
	set curDir = %%a
	@echo %%a
	cd %%a
	del /s /q build
	rd /s /q build
	cd ..
)

pause