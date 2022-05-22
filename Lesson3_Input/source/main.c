#include "stdio.h"
#include "displayutils.h"
#include "celutils.h"
#include "controlpad.h"
#include "event.h"

#define FRACBITS_16 16
#define CEL_VEL_16 (4 << FRACBITS_16)
#define CONT_BTN_MASK (ControlLeft|ControlRight|ControlUp|ControlDown)

ScreenContext *sc;
Item sport;
Item vbl;
uint32 buttons;
uint32 prevButtons;
ControlPadEventData eventData;

CCB *ccb;

void init(void)
{
	OpenGraphicsFolio();

	sc = (ScreenContext*) AllocMem(sizeof(ScreenContext), MEMTYPE_ANY);

	CreateBasicDisplay(sc, DI_TYPE_DEFAULT, 2);
	sc->sc_CurrentScreen = 0;

	sport = GetVRAMIOReq();
	vbl = GetVBLIOReq();

	ccb = LoadCel("Graphics/sun", MEMTYPE_CEL);

	ccb->ccb_XPos = 144 << FRACBITS_16;
	ccb->ccb_YPos = 104 << FRACBITS_16;
	ccb->ccb_Flags |= CCB_LAST;

	/* InitControlPad(1); */

	InitEventUtility(1, 0, LC_Observer);
}

int main(int argc, char *argv[])
{
	init();

	while (TRUE)
	{
		/* Update */

		/* DoControlPad(1, &buttons, CONT_BTN_MASK); */
		GetControlPad(1, FALSE, &eventData);
		buttons = eventData.cped_ButtonBits;

		if ((buttons & ControlLeft)  /*&& !(prevButtons & ControlLeft)*/)
		{
			ccb->ccb_XPos -= CEL_VEL_16;
		}
		else if (buttons & ControlRight)
		{
			ccb->ccb_XPos += CEL_VEL_16;
		}

		if (buttons & ControlUp)
		{
			ccb->ccb_YPos -= CEL_VEL_16;
		}
		else if (buttons & ControlDown)
		{
			ccb->ccb_YPos += CEL_VEL_16;
		}

		/* Draw */

		SetVRAMPages(sport, sc->sc_Bitmaps[sc->sc_CurrentScreen]->bm_Buffer, 0, sc->sc_NumBitmapPages, -1);

		DrawCels(sc->sc_BitmapItems[sc->sc_CurrentScreen], ccb);

		DisplayScreen(sc->sc_ScreenItems[sc->sc_CurrentScreen], 0);

		sc->sc_CurrentScreen = 1 - sc->sc_CurrentScreen;

		prevButtons = buttons;

		WaitVBL(vbl, 1);
	}

	return(0);
}
