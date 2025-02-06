#include "stdio.h"
#include <stdlib.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "ihm/display.h"
#include "ihm/switch.h"
#include "file/wav.h"
#include "effects/effect_control.h"

#define NUM_EFFECTS 8
Uint32 cnt = 0;
int currentEffect = 0;

void delay_ms(Uint32 milliseconds)
{
    volatile Uint32 scs;
    for (scs = 0; scs < milliseconds * 1000; scs++)
        ;
}

void execEffect(FILE *fp, int effect)
{
    printf("Executing effect: %d\n", effect);
    show_effect(effect, 3);
    delay_ms(4000);
}

void main(void)
{
    EZDSP5502_init();
    printf("Reading WAV...\n");
    const char *filename = "..\\data\\audio.wav";
    const char *filename_out = "..\\data\\audio_out.wav";

    FILE *fpIn = open_wav_file(filename);
    show_effect(currentEffect, 1);

    while (1)
    {
        int buttonStatus = check_switch();

        if (buttonStatus == 0)
        {
            show_effect(currentEffect, 2);
            FILE *fpOut = create_file(filename_out);

            Uint8 waveHeader[44];
            fread(waveHeader, sizeof(Uint8), 44, fpIn);
            fwrite(waveHeader, sizeof(Uint8), 44, fpOut);

            apply_effect(fpIn, fpOut, currentEffect, &cnt);

            wHeader(waveHeader, 8000, 8000, cnt << 1);
            rewind(fpOut);
            fwrite(waveHeader, sizeof(Int8), 44, fpOut);

            fclose(fpOut);
            printf("Effect %d applied and saved.\n", currentEffect);
            show_effect(currentEffect, 3);

            while (1)
            {
                buttonStatus = check_switch();

                if (buttonStatus == 0)
                {
                    show_effect(currentEffect, 4);
                    delay_ms(4000);
                    show_effect(currentEffect, 3);
                }
                else if (buttonStatus == 1)
                {
                    printf("Returning to effect selection...\n");

                    show_effect(currentEffect, 1);
                    break;
                }
            }
        }

        if (buttonStatus == 1)
        {
            currentEffect = (currentEffect + 1) % NUM_EFFECTS;
            show_effect(currentEffect, 1);
        }
    }


}

void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes)
{
    Int32 t;

    t = bytes;
    w[40] = (t >> 0) & 0xff;
    w[41] = (t >> 8) & 0xff;
    w[42] = (t >> 16) & 0xff;
    w[43] = (t >> 24) & 0xff;

    t += 36;
    w[4] = (t >> 0) & 0xff;
    w[5] = (t >> 8) & 0xff;
    w[6] = (t >> 16) & 0xff;
    w[7] = (t >> 24) & 0xff;

    t = w[24] | (w[25] << 8) | ((Int32)w[26] << 16) | ((Int32)w[27] << 24);
    t = (Int32)((float)t * f2 / f1);
    w[24] = (t >> 0) & 0xff;
    w[25] = (t >> 8) & 0xff;
    w[26] = (t >> 16) & 0xff;
    w[27] = (t >> 24) & 0xff;

    t = w[28] | (w[29] << 8) | ((Int32)w[30] << 16) | ((Int32)w[31] << 24);
    t = (Int32)((float)t * f2 / f1);
    w[28] = (t >> 0) & 0xff;
    w[29] = (t >> 8) & 0xff;
    w[30] = (t >> 16) & 0xff;
    w[31] = (t >> 24) & 0xff;

    return;
}
