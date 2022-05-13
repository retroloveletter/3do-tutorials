#include "stdio.h"
#include "displayutils.h"
#include "celutils.h"

ScreenContext *sc;
Item sport;
Item vbl;

uint32 color;
GrafCon gcon;

void init(void)
{
	OpenGraphicsFolio();

	sc = (ScreenContext*) AllocMem(sizeof(ScreenContext), MEMTYPE_ANY);

	CreateBasicDisplay(sc, DI_TYPE_DEFAULT, 2);
	sc->sc_CurrentScreen = 0;

	sport = GetVRAMIOReq();
	vbl = GetVBLIOReq();

	color = 0; /* MakeRGB15(31, 0, 0); */
}

int main(int argc, char *argv[])
{
	init();

	while (TRUE)
	{
		SetVRAMPages(sport, sc->sc_Bitmaps[sc->sc_CurrentScreen]->bm_Buffer, color, sc->sc_NumBitmapPages, -1);

		gcon.gc_PenX = 10;
		gcon.gc_PenY = 10;
		DrawText8(&gcon, sc->sc_BitmapItems[sc->sc_CurrentScreen], "Hello World");

		DisplayScreen(sc->sc_ScreenItems[sc->sc_CurrentScreen], 0);

		sc->sc_CurrentScreen = 1 - sc->sc_CurrentScreen;

		WaitVBL(vbl, 1);
	}

	return(0);
}
