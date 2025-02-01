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
extern void applyEffect(int effect);
extern void execEffect();
Int16 switchEffect(int currentEffect)
{

    EZDSP5502_I2CGPIO_configLine(SW0, IN);
    EZDSP5502_I2CGPIO_configLine(SW1, IN);

    show_effect(currentEffect, 0);

    while (1)
    {

        if (!EZDSP5502_I2CGPIO_readLine(SW0))
        {

            applyEffect(currentEffect);
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
