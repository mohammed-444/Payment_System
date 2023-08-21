#pragma once

#ifndef _server_
#define _server_

#include "../Terminal/terminal.h"

/**************************************
 *         typedefs
**************************************/
typedef unsigned char uint8_t;
typedef unsigned long uint32_t;


typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_terminalError_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transStat_t;

typedef enum EN_serverError_t
{
	OK_s, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

extern uint32_t transactionSequenceNumber;
/**************************************
 *         prototypes
**************************************/
/*server_side_accounts_database and but some data to it*/
void server_side_accounts_database(void);

/*store to data from the transactions when needed*/
void server_side_transactions_database(void);

/*This function will take all transaction data and validate its data.
It checks the account details and amount availability.
If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND,
if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
It will update the database with the new balance.*/
EN_transStat_t recieveTransactionData(ST_transaction_t* transData);

/*This function will take card data and validate these data.
It checks if the PAN exists or not in the server's database.
If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData);

/*This function will take terminal data and validate these data.
It checks if the transaction's amount is available or not.
If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);



/*This function will take all transaction data into the transactions database.
It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
If transaction can't be saved will return SAVING_FAILED, else will return OK*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif
