#include "stdio.h"
#include <stdlib.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "ihm/display.h"
#include "ihm/switch.h"

#define NUM_EFFECTS 8
Uint32 cnt = 0; // Global sample counter
int currentEffect = 0;

void delay_ms(Uint32 milliseconds)
{
	volatile Uint32 scs;
	for (scs = 0; scs < milliseconds * 1000; scs++)
		;
}

void main(void)
{
	EZDSP5502_init();
	show_effect(currentEffect, 0);

	while (1)
	{
		int buttonStatus = check_switch(); // Read switch input

		if (buttonStatus == 0) // SW0 pressed
		{
			show_effect(currentEffect, 1);
			delay_ms(5000);
			show_effect(currentEffect, 2);

			while (1)
			{
				buttonStatus = check_switch();

				if (buttonStatus == 0)
				{
					show_effect(currentEffect, 3);
					delay_ms(4000);
				}
				else if (buttonStatus == 1)
				{
					printf("Returning to effect selection...\n");
					show_effect(currentEffect, 0);
					break;
				}
			}
		}

		if (buttonStatus == 1) // SW1 pressed
		{
			currentEffect = (currentEffect + 1) % NUM_EFFECTS; // Cycle effects
			show_effect(currentEffect, 0);
		}
	}
}
