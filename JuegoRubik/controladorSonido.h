#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

class ControladorSonido {
public:
	static void reproducirSonido() {
		PlaySound(TEXT("movimiento.wav"), NULL, SND_FILENAME | SND_SYNC);
	}
};

