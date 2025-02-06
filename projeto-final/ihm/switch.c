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

void init_switch()
{
	EZDSP5502_I2CGPIO_configLine(SW0, IN);
	EZDSP5502_I2CGPIO_configLine(SW1, IN);
}

int check_switch()
{
	if (!EZDSP5502_I2CGPIO_readLine(SW0))
	{
		while (!EZDSP5502_I2CGPIO_readLine(SW0))
			;
		return 0;
	}

	if (!EZDSP5502_I2CGPIO_readLine(SW1))
	{
		while (!EZDSP5502_I2CGPIO_readLine(SW1))
			;
		return 1;
	}

	return -1;
}
