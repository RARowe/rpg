cl /Iinclude main.c^
    /link /libpath:windows\SDL2-dev\lib\x64\ shell32.lib SDL2.lib SDL2main.lib^
    /libpath:windows\SDL2_image-dev\lib\x64 SDL2_image.lib^
    /libpath:windows\SDL2_ttf-dev\lib\x64 SDL2_ttf.lib^
    /libpath:windows\SDL2_mixer-dev\lib\x64 SDL2_mixer.lib^
    /SUBSYSTEM:WINDOWS