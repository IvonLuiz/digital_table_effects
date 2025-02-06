/*
 * aic3204.h
 *
 *  Created on: 27 de out de 2024
 *      Author: Heverton Reis
 */

#ifndef AIC3204_H_
#define AIC3204_H_

#include "ezdsp5502.h"
#include "ezdsp5502_i2c.h"
#include "ezdsp5502_i2cgpio.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"

Int16 AIC3204_rget(Uint16 regnum, Uint16* regval);
Int16 AIC3204_rset(Uint16 regnum, Uint16 regval);
void initAIC3204(void);

#endif /* AIC3204_H_ */
