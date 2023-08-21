#pragma once

#ifndef _terminal_

#define _terminal_

#include<time.h>
#include "../Card/card.h"
/**************************************
 *         typedefs
**************************************/
typedef unsigned char uint8_t;

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	OK_t, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;


extern ST_terminalData_t ST_terminalData;
/**************************************
 *         prototypes
**************************************/
/*The function will read the current date from your computer and store it into terminal data with the mentioned size and format.*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);

/*This function compares the card expiry date with the transaction date.
If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.*/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData);


/*This function asks for the transaction amount and saves it into terminal data.
If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);

/*This function compares the transaction amount with the terminal max amount.
If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);

/*This function sets the maximum allowed amount into terminal data.
Transaction max amount is a float number.
If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif
