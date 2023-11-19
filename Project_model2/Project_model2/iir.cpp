/**************************************
 * IIR filtar drugog reda
 *
 * input - ulazni odbirak
 * coefficients - koeficijenti [a0 a1 a2 b0 b2 b2]
 * z_x - memorija za ulazne odbirke (niz duzine 2)
 * z_y - memorija za izlazne odbirke (niz duzine 2)
 *
 * povratna vrednost - izlazni odbirak
 *
 *************************************/
#include "common.h"

DSPfract second_order_IIR(DSPfract input, DSPfract* coefficients, DSPfract* x_history, DSPfract* y_history) {
	DSPfract output = 0;
	DSPfract pom = 0;
	DSPfract* x = x_history;
	DSPfract* y = y_history;
	

	output += *(coefficients++) * input;  /* A0 * x(n)   */
	pom = *(coefficients++) * *(x++); /* A1 * x(n-1) */
	pom = pom << 1;
	output += pom;
	output += *(coefficients++) * *(x); /* A2 * x(n-2) */
	coefficients++;
	pom = *(coefficients++) * *(y++); /* B1 * y(n-1) */
	pom = pom << 1;
	output -= pom;
	output -= *(coefficients++) * *(y); /* B2 * y(n-2) */

	*y = *y_history;       /* y(n-2) = y(n-1) */
	*y_history = output;   /* y(n-1) = y(n)   */
	*x = *x_history;       /* x(n-2) = x(n-1) */
	*x_history = input;    /* x(n-1) = x(n)   */


	return output;
}