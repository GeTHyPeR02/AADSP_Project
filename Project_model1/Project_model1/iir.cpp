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

double second_order_IIR(double input, double* coefficients, double* x_history, double* y_history) {
	double output = 0;
	double* x = x_history;
	double* y = y_history;

	output += *(coefficients++) * input;  /* A0 * x(n)   */
	output += *(coefficients++) * *(x++); /* A1 * x(n-1) */
	output += *(coefficients++) * *(x); /* A2 * x(n-2) */
	coefficients++;
	output -= *(coefficients++) * *(y++); /* B1 * y(n-1) */
	output -= *(coefficients++) * *(y); /* B2 * y(n-2) */

	*y = *y_history;       /* y(n-2) = y(n-1) */
	*y_history = output;   /* y(n-1) = y(n)   */
	*x = *x_history;       /* x(n-2) = x(n-1) */
	*x_history = input;    /* x(n-1) = x(n)   */


	return output;
}