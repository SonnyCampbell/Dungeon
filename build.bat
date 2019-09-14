@echo off

pushd build
C:/"Program Files (x86)"/"Microsoft Visual Studio"/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x86/cl.exe ^
/EHsc /Zi /Fe: program.exe ^
../src/program.cpp ../src/TMXLoader/TMXLoader.cpp ../src/TMXLoader/TMXMap.cpp ../src/TMXLoader/TMXTile.cpp ^
../src/TMXLoader/TMXTileLayer.cpp ../src/TMXLoader/TMXTileSet.cpp ../src/"Game Components"/Game.cpp ../src/Player.cpp ^
../src/EnemyManager.cpp ../src/LTexture.cpp ../src/Weapon.cpp ../src/AnimatedSprite.cpp ../src/SimpleAI.cpp ^
../src/State/EnemyIdleState.cpp ../src/State/EnemyChaseState.cpp ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt" ^
/I C:/code/Projects/libraries/SDL2-2.0.10/include ^
/I C:/code/Projects/libraries/SDL2_image-2.0.5/include ^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\include" ^
/link /LIBPATH:C:\code\Projects\libraries\SDL2-2.0.10\lib\x86 ^
/LIBPATH:C:\code\Projects\libraries\SDL2_image-2.0.5\lib\x86 ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86" ^
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\lib\x86" ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x86" ^
SDL2.lib SDL2main.lib SDL2_image.lib ^
/SUBSYSTEM:WINDOWS
popd