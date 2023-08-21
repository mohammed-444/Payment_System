#include"server.h"
float fbalance;
uint8_t line=0;
/*----------------------------------------------------------------------------------------------------------------*/

void server_side_accounts_database(void){
	/*arrays to restore the orginal database*/
	uint8_t name[5][25]={"Mohammed Alaa AbdElkader","Hesham Gamal Abd-Elhady","Anas Mohammed Fazy","Mohamed Mansor AbdElmagd","Ahmed Gamil Mohammed"};
	float balance[5]={25000,15000,20000,10000,5000};
	uint8_t primaryAccountNumber[5][20]={"0123456789123456789","1123456789123456789","2123456789123456789","3123456789123456789","4123456789123456789"};

	/*open a new file for it */
	FILE* accounts_ptr;
	accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","w");
	/*the stored format in the database for the five customers*/
	for (int i = 0; i <5; ++i) {fprintf(accounts_ptr,"name:%s\t\tPAN:%s\t\tbalance:%f\n",name[i],primaryAccountNumber[i],balance[i]);}
	fclose(accounts_ptr);

}
/*----------------------------------------------------------------------------------------------------------------*/

void server_side_transactions_database(void){

	/*open a new file for it */
	FILE* transactions_ptr;

	transactions_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/transactions_database.txt","w");

	fclose(transactions_ptr);
}
/*----------------------------------------------------------------------------------------------------------------*/

EN_transStat_t recieveTransactionData(ST_transaction_t* transData){

	if(isValidAccount(&(transData->cardHolderData))==ACCOUNT_NOT_FOUND){
		return DECLINED_STOLEN_CARD;
	}
	else if(isAmountAvailable(&(transData->terminalData)) == LOW_BALANCE){
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if(getTransaction(transactionSequenceNumber,transData)==SAVING_FAILED){
		return INTERNAL_SERVER_ERROR;
	}
	else{
		/*the next block to modifiy the database file */
/*---------------------------------------------------------*/
		FILE* accounts_ptr,*temp_ptr;
		uint8_t linedata[100];
		uint8_t _[100];
		uint8_t first_name[100];
		uint8_t second_name[100];
		uint8_t third_name[100];
		uint8_t PAN[100];


		/*making a temp file to make a new copy of the data and put that back in the main text file */
		temp_ptr= fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/temp_file.txt","w+");
		accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","r+");

		/*copy every row and stop only in the new appended copy
		 the line variable is changed in the isValidAccount function */
		for (int var = 0; var < line; ++var) {
			fgets((char*)linedata,100,accounts_ptr);
			fputs((char*)linedata,temp_ptr);
		}
		/*read the wanted row */
		fgets((char*)linedata,100,accounts_ptr);
		/*parsing the string row to collect the needed data*/
		 sscanf((char*)linedata,"%[^:] %[:] %[A-Za-z] %[A-Za-z] %[A-Za-z] %[a-zA-Z] %[:] %[0-9] %[a-zA-Z] %[:] %[0-9] ",_,_,first_name,second_name,third_name,_,_,PAN,_,_,_);
		 /*writing the new row to the temp file*/
		 fprintf(temp_ptr,"name:%s %s %s \tPAN:%s\t\tbalance:%f\n",first_name,second_name,third_name,PAN,(fbalance- transData->terminalData.transAmount));
		 /*then copy the rest of the file to the temp file*/
		 while(fgets((char*)linedata,100,accounts_ptr)){fputs((char*)linedata,temp_ptr);}

		fclose(temp_ptr);
		fclose(accounts_ptr);

		/*open the files again but in different modes*/
		temp_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/temp_file.txt","r+");
		accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","w+");
		/*copy every thing back to the main file which contain the new changes */
		 while(fgets((char*)linedata,100,temp_ptr)){fputs((char*)linedata,accounts_ptr);}
		fclose(temp_ptr);
		fclose(accounts_ptr);

/*---------------------------------------------------------*/
		return APPROVED;
	}

}

/*----------------------------------------------------------------------------------------------------------------*/
/* this function takes the PAN and search through the database for it */
EN_serverError_t isValidAccount(ST_cardData_t* cardData){

	uint8_t line_data[100];
	uint8_t PAN[20];
	uint8_t flag=1;

	FILE* accounts_ptr;

	accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","r");

	/*read every row and parse it to collect the PAN */
	while(fgets((char*)line_data,100,accounts_ptr)){
		/*parsing the string note the value of the PAN takes the last  read data */
		sscanf((char*)line_data,"%[A-Za-z: -\t] %[0-9]",PAN,PAN);
		/*compare the two PANs and of equal return 0  the flage of we find it to stop the other path*/
		if(!strcmp( (char*)PAN,(char*)ST_cardData.primaryAccountNumber) ){flag=0;break;}
	}
	fclose(accounts_ptr);

	if(flag){return ACCOUNT_NOT_FOUND;}
	return OK_s;

}

/*----------------------------------------------------------------------------------------------------------------*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData){

		uint8_t line_data[100];
		uint8_t PAN[20];
		uint8_t balance[100];



		FILE* accounts_ptr;
		accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","r");

		/*read every row and parse it to collect the PAN */
		while(fgets((char*)line_data,100,accounts_ptr)){
			/*parsing the string note the value of the PAN takes the last  read data */
			sscanf((char*)line_data,"%[A-Za-z: -\t] %[0-9]",PAN,PAN);
			/*compare the two PANs and of equal return 0  the flage of we find it to stop the other path*/
			if(!strcmp((char*)PAN,(char*)ST_cardData.primaryAccountNumber) ){break;}
			line++;/*the final value for this indecate the line number*/
		}
		fclose(accounts_ptr);



		accounts_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/accounts_database.txt","r");
		/*this loop for reaching the wanted line */
		for (int i = 0; i <= line; ++i) {fgets((char*)line_data,100,accounts_ptr);}
		/*parsing the string to collect the balance note the value of the balance takes the last  read data */
		sscanf((char*)line_data,"%[A-Za-z: -\t] %[0-9] %[A-Za-z: -\t] %[0-9]",balance,balance,balance,balance);
		fclose(accounts_ptr);

		/*cast the string number to a float*/
		sscanf((char*)balance, "%f", &fbalance);
		if(termData->transAmount > fbalance){return LOW_BALANCE;}

		return OK_s;

}




/*----------------------------------------------------------------------------------------------------------------*/
/*take the transaction data and store it in the database*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData){

	transData->transactionSequenceNumber=transactionSequenceNumber;
	/*to store the transaction state error type*/
	if(isCardExpired(&ST_cardData,&(transData->terminalData))== EXPIRED_CARD ){transData->transState= EXPIRED_CARD;}
	else if(isBelowMaxAmount(&(transData->terminalData))==EXCEED_MAX_AMOUNT) {transData->transState= EXCEED_MAX_AMOUNT;}
	else{transData->transState= OK_t;}




	FILE* transactions_ptr;
	transactions_ptr=fopen("E:/Fwd Embedded system professional track/Payment_System_eclipse/Payment_System_project/source/DataBase/transactions_database.txt","a");
	/*make sure the file is open*/
	if (transactions_ptr==NULL ){return SAVING_FAILED;}

	/*the stored format in the database for the transactions*/
	fprintf((char*)transactions_ptr,"transactionSequenceNumber:%d\t Card holder name:%s\t PAN:%s\t cardExpirationDate:%s\t transaction amount:%f\t transaction date:%s\t transState:%d\n",
			transData->transactionSequenceNumber,transData->cardHolderData.cardHolderName, transData->cardHolderData.primaryAccountNumber,
			transData->cardHolderData.cardExpirationDate,transData->terminalData.transAmount,transData->terminalData.transactionDate,transData->transState);

	transactionSequenceNumber++;/*wanted variable in the descreption */

	fclose(transactions_ptr);
	if (transData->transState!= OK_t){return SAVING_FAILED;}
	return OK_s;

}


