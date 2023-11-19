#pragma once
#include "common.h"

DSPfract second_order_IIR(DSPfract input, DSPfract* coefficients, DSPfract* x_history, DSPfract* y_history);