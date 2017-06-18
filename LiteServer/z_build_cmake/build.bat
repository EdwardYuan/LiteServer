@echo off
for /d %%a in (*) do (
	set curDir = %%a
	@echo %%a
	cd %%a
	mkdir build
	cd build
	cmake ../
	cd ..
	cd ..
)

pause