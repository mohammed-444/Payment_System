#include "terminal.h"

/*As the function takes the time from the pc there is no need to check for error*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData){


	time_t now=time(NULL);/*built in function to take the time from the pc*/

	struct tm *cur_time = localtime(&now);/*cast the time to readable built in struct */

	strftime(termData->transactionDate,11,"%d/%m/%Y", cur_time);/*cast the stuct to the needed format*/


	return OK_t;
}
/*------------------------------------------------------------------------------------------------------*/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData){

/*As we need only to compar the month and the year we will compare 4 char the two digit year then the month*/
	for(char i=3,j=8,count=0 ;count<4 ;i++,j++,count++){
		if(count==2){   i=0;     j=3;}	/* to jump to the month location */
		if(cardData->cardExpirationDate[i] < termData->transactionDate[j]){return EXPIRED_CARD;}/*if the expired date is smaller then return EXPIRED_CARD*/
		else if(cardData->cardExpirationDate[i] == termData->transactionDate[j]){continue;}/* if it is equal then check the other*/
		else{break;}/* if it is bigger no need to check the other*/
	}

	return OK_t;
}


/*------------------------------------------------------------------------------------------------------*/


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData){

	printf("Enter the Transaction amount\n");
	scanf("%f",&(termData->transAmount));

	/*If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT*/
	if(termData->transAmount<=0){return INVALID_AMOUNT;}

	return OK_t;
}
/*------------------------------------------------------------------------------------------------------*/

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData){
	/*If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT*/
	if(termData->transAmount > termData->maxTransAmount){return EXCEED_MAX_AMOUNT;}
	else{return OK_t;}
}
/*------------------------------------------------------------------------------------------------------*/

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData){

	printf("Enter the max Transaction amount\n");
	scanf("%f",&(termData->maxTransAmount));

	/*If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error*/
	if(termData->maxTransAmount<=0){return INVALID_MAX_AMOUNT;}

	return OK_t;

}
