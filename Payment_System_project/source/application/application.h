#pragma once

#ifndef _application_
#define _application_
#include "../Server/server.h"


void CardHolder_acquisition(ST_cardData_t* ST_cardData);

void TerminalData_acquisition(ST_terminalData_t* ST_terminalData);

void TransactionData_acquisition(ST_transaction_t* ST_transData);

//void error_checking(ST_transaction_t* ST_transData);

void appStart(void);





#endif
