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

#define NUM_EFFECTS 8
extern void apply_effect(int effect);

Int16 switchEffect(int currentEffect)
{
	// Configurar os botoes como entrada
	EZDSP5502_I2CGPIO_configLine(SW0, IN);
	EZDSP5502_I2CGPIO_configLine(SW1, IN);

	while (1)
	{
		// Verifica se SW1 foi pressionado
		if (!EZDSP5502_I2CGPIO_readLine(SW1))
		{
			currentEffect = (currentEffect + 1) % NUM_EFFECTS;
			apply_effect(currentEffect);
			while (!EZDSP5502_I2CGPIO_readLine(SW1))
				;
		}

		// Verifica se SW0 foi pressionado
		if (!EZDSP5502_I2CGPIO_readLine(SW0))
		{
			currentEffect = (currentEffect - 1 + NUM_EFFECTS) % NUM_EFFECTS;
			apply_effect(currentEffect);
			while (!EZDSP5502_I2CGPIO_readLine(SW0))
				;
		}
	}
}
