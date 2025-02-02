/*
 * switch.c
 *
 *  Created on: Jan 20, 2025
 *      Author: jonhb
 */

#include "stdio.h"
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "switch.h"
#include "display.h"

#define NUM_EFFECTS 8
extern void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt);
extern void execEffect();
extern FILE *create_file(const char *filename);
extern void write_header(FILE *fpIn, FILE *fpOut);
extern void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);
extern void ajust_header(FILE *fpOut, Uint32 cnt);

const char *filename_out = "..\\data\\audio_out.wav";

Int16 switchEffect(int currentEffect, FILE *fpIn)
{

	EZDSP5502_I2CGPIO_configLine(SW0, IN);
	EZDSP5502_I2CGPIO_configLine(SW1, IN);

	show_effect(currentEffect, 0);

	while (1)
	{

		if (!EZDSP5502_I2CGPIO_readLine(SW0))
		{
			FILE *fpOut = create_file(filename_out);
			show_effect(currentEffect, 1);
			apply_effect(fpIn, fpOut, currentEffect, cnt);
			show_effect(currentEffect, 3);

			while (1)
			{

				if (!EZDSP5502_I2CGPIO_readLine(SW0))
				{
					while (!EZDSP5502_I2CGPIO_readLine(SW0))
						;
					execEffect(currentEffect);

					show_effect(currentEffect, 3);
				}
				else if (!EZDSP5502_I2CGPIO_readLine(SW1))
				{

					while (!EZDSP5502_I2CGPIO_readLine(SW1))
						;
					show_effect(currentEffect, 0);
					break;
				}
			}
		}

		if (!EZDSP5502_I2CGPIO_readLine(SW1))
		{

			currentEffect = (currentEffect + 1) % NUM_EFFECTS;
			show_effect(currentEffect, 0);
			while (!EZDSP5502_I2CGPIO_readLine(SW1))
				;
		}
	}
}
