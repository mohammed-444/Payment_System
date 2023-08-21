#include "application.h"

ST_cardData_t ST_cardData;
ST_terminalData_t ST_terminalData;
ST_transaction_t ST_transData;
uint32_t transactionSequenceNumber=0;

void CardHolder_acquisition(ST_cardData_t* cardData){
	getCardHolderName(cardData);
	getCardExpiryDate(cardData);
	getCardPAN(cardData);
}

void TerminalData_acquisition(ST_terminalData_t* terminalData){
	getTransactionDate(terminalData);

	getTransactionAmount(terminalData);

	setMaxAmount(terminalData);

}

void TransactionData_acquisition(ST_transaction_t* ST_transData){
	ST_transData->cardHolderData=ST_cardData;
	ST_transData->terminalData=ST_terminalData;
}



void appStart(void){
	uint8_t flag;
	EN_transStat_t error;


//	server_side_accounts_database();
//	server_side_transactions_database();

	printf("you have 3 try:\n---------------------------------\n");
	for (int i = 3; i > 0; --i) {
		flag=1;
		CardHolder_acquisition(&ST_cardData);
		TerminalData_acquisition(&ST_terminalData);
		TransactionData_acquisition(&ST_transData);

		error=recieveTransactionData(&ST_transData);

		if(error == DECLINED_STOLEN_CARD){
			printf("**********DECLINED_STOLEN_CARD ERROR**********\n\n\n");
			flag=0;
		}
		else if(ST_transData.transState == EXPIRED_CARD){
			printf("**********EXPIRED_CARD ERROR**********\n\n\n");
			flag=0;
		}
		else if(error == DECLINED_INSUFFECIENT_FUND){
			printf("**********DECLINED_INSUFFECIENT_FUND ERROR**********\n\n\n");
			flag=0;
		}
		else if(ST_transData.transState == EXCEED_MAX_AMOUNT){
			printf("**********EXCEED_MAX_AMOUNT ERROR**********\n\n\n");
			flag=0;
		}
		else if(error == INTERNAL_SERVER_ERROR){
			printf("**********INTERNAL_SERVER_ERROR ERROR**********\n\n\n");
			flag=0;
		}

		if(flag){printf("**********Successful transaction**********\n\n\n");break;}

		else{printf("try to enter the data again you have %d try\n",i-1);fflush(stdin);}
		fflush(stdout);

	}

}


int main() {

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	uint8_t flag =1;
while(flag){

	fflush(stdout);
	fflush(stdin);
	appStart();
	printf("if you want to continue 1 to stop enter 0:\n");
	scanf("%d",&flag);
}
	return(0);
}
