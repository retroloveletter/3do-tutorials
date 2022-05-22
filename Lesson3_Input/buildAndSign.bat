copy source\LaunchMe CD /Y
copy source\splash\Splash CD /Y

3doiso.exe -in CD -out game.iso

3doEncrypt.exe genromtags game.iso

echo Done

pause