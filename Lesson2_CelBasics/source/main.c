#include "stdio.h"
#include "displayutils.h"
#include "celutils.h"

#define FRACBITS_16 16
#define FRACBITS_20 20

ScreenContext *sc;
Item sport;
Item vbl;

ubyte *bg;
CCB *ccb;

void init(void)
{
	OpenGraphicsFolio();

	sc = (ScreenContext*) AllocMem(sizeof(ScreenContext), MEMTYPE_ANY);

	CreateBasicDisplay(sc, DI_TYPE_DEFAULT, 2);
	sc->sc_CurrentScreen = 0;

	sport = GetVRAMIOReq();
	vbl = GetVBLIOReq();

	bg = (ubyte*) LoadImage("Graphics/bg", NULL, NULL, sc);

	ccb = LoadCel("Graphics/sun", MEMTYPE_CEL);

	/* 16.16 */
	ccb->ccb_XPos = 144 << FRACBITS_16;
	ccb->ccb_YPos = 104 << FRACBITS_16;

	/*
		ccb_HDX 12.20
		ccb_VDY 16.16
		ccb_HDY 12.20
		ccb_VDX 16.16
	*/

	/* Scale
	ccb->ccb_HDX = 2 << FRACBITS_20;
	ccb->ccb_VDY = 2 << FRACBITS_16;
	*/

	/* Rotate */
	ccb->ccb_HDY = 1 << FRACBITS_20;
	ccb->ccb_VDX = -1 << FRACBITS_16;

	ccb->ccb_Flags |= CCB_LAST;
}

int main(int argc, char *argv[])
{
	init();

	while (TRUE)
	{
		CopyVRAMPages(sport, sc->sc_Bitmaps[sc->sc_CurrentScreen]->bm_Buffer, bg, sc->sc_NumBitmapPages, -1);

		DrawCels(sc->sc_BitmapItems[sc->sc_CurrentScreen], ccb);

		DisplayScreen(sc->sc_ScreenItems[sc->sc_CurrentScreen], 0);

		sc->sc_CurrentScreen = 1 - sc->sc_CurrentScreen;

		WaitVBL(vbl, 1);
	}

	return(0);
}
