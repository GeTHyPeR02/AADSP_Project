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

	output += coefficients[0] * input;        /* A0 * x(n)   */
	output += coefficients[1] * x_history[0]; /* A1 * x(n-1) */
	output += coefficients[2] * x_history[1]; /* A2 * x(n-2) */
	output -= coefficients[4] * y_history[0]; /* B1 * y(n-1) */
	output -= coefficients[5] * y_history[1]; /* B2 * y(n-2) */

	y_history[1] = y_history[0];    /* y(n-2) = y(n-1) */
	y_history[0] = output;          /* y(n-1) = y(n)   */
	x_history[1] = x_history[0];    /* x(n-2) = x(n-1) */
	x_history[0] = input;           /* x(n-1) = x(n)   */

	return output;
}