#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	uint8_t size;

	printf("Enter the card holder name:\n");

	gets((char*)cardData->cardHolderName);
	fflush(stdin);
	size=strlen((char*)cardData->cardHolderName);/*calculate the length of the string*/

	/*Card holder name is 24 characters string max and 20 min.If the cardholder name is NULL*/
	if((size<20)||(size>24)||cardData->cardHolderName=="NULL"){return WRONG_NAME;}

	return OK;
}
/*----------------------------------------------------------------------------------------------------*/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	uint8_t size;

	printf("Enter the card expiration date  format MM/YY:\n");
	gets((char*)cardData->cardExpirationDate);
	size=strlen((char*)cardData->cardExpirationDate);/*calculate the length of the string*/

	/*Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25". If the card expiry date is NULL*/
	if((size!=5)||(cardData->cardExpirationDate=="NULL")||(cardData->cardExpirationDate[2]!='/')){return WRONG_EXP_DATE;}

	return OK;
}
/*----------------------------------------------------------------------------------------------------*/

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	uint8_t size;

	printf("Enter the card PAN:\n");
	gets((char*)cardData->primaryAccountNumber);
	size=strlen((char*)cardData->primaryAccountNumber);/*calculate the length of the string*/

	/*PAN is 20 characters alphanumeric only string 19 character max, and 16 character min. If the PAN is NULL*/
	if((size>19)||(size<16)||(cardData->primaryAccountNumber=="NULL")){return WRONG_PAN;}

	return OK;
}
