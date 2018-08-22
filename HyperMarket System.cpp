#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>       //to use atoi Function to conver Quantity char arrya 
//							to int number to make operations on substraction 
//																														and addtion.
using namespace std;
char testCode[6];       //using it when validation new item code

class item
{
public:
	char code[6];			 // for item code .... Item Master & Item transaction 
	char name[31];			 // for item name .... Item Master
	char quantity[4];		 // for item Quantity  Item Master & Item transaction
	char IssueDate[11];		 //issue date     .... Item Master 
	char ReceiptDate[11];    // Receipt date  .... Item Master
	char trans_date[11];	 // Transaction date   Item Transaction
	char trans_type[8];		 //Sell or Buy transaction .... Item Transaction
	char item_masterRec[62]; //full Size record in Item Master File
	char item_transRec[30];  //Full size record in Item Transaction File

}Item;			             // as an object from the Item Calss.

fstream file, file2;         //just intialization of logical file pointer to deal with the physical files in our sys.

void checkFile(fstream &f, string fileName) // passing it the file address
{
	if (!f)
	{
		cout << "Error Opening the file " << fileName << endl;
		exit(0);
	}

}

bool validateItemCode(char code[])
{
	file2.open("itemMaster.txt", ios::in | ios::beg);
	checkFile(file2, "itemMaster.txt");
	while (!file2.eof())
	{
		file2.getline(Item.code, 6, '$');
		if (file2.eof()) break; // here the EOF mark has been read at the end so 
								//	BREAK (only at the last record).
		file2.getline(Item.item_masterRec, 62, '\n');
		if (strcmp(code, Item.code) == 0)
		{
			file2.close();
			return false;
		}
	}
	file2.close();
	return true;
}

void writeInTransactionFile(char code[6], char transType[8], char quantity[4], char transDate[11])
{
	file2.open("file_Transactions.txt", ios::out | ios::app);
	checkFile(file2, "file_Transactions.txt");
	strcpy_s(Item.item_transRec, code);
	strcat_s(Item.item_transRec, "$");
	strcat_s(Item.item_transRec, transType);
	strcat_s(Item.item_transRec, "$");
	strcat_s(Item.item_transRec, quantity);
	strcat_s(Item.item_transRec, "$");
	strcat_s(Item.item_transRec, transDate);
	int len = strlen(Item.item_transRec);
	int size = sizeof(Item.item_transRec);
	int diff = size - len;
	for (int k = 0; k < diff - 1; k++)
		strcat_s(Item.item_transRec, ".");

	file2 << Item.item_transRec << endl;
	file2.close();
}

void Create_Item()
{

	int numberOfItems;
	file.open("itemMaster.txt", ios::out | ios::app);
	checkFile(file, "itemMaster.txt");
	cout << "How many Items to add " << endl;
	cin >> numberOfItems;
	cin.ignore(); //to clear the bufffer
	for (int i = 0; i < numberOfItems; i++)
	{
		cout << "Enter Item code (<=5)" << endl;
		cin.getline(testCode, 6);

		if (!validateItemCode(testCode))
		{
			cout << "This Code Already Exsist Can't Create Item with This Code .. Sorry" << endl;
			file.close();
			system("pause");
			break;
		}
		strcpy_s(Item.code, testCode);
		// Item.code = testCode;
		//cin.getline(Item.code, 6);
		cout << "Enter Item Name (<=30)" << endl;
		cin.getline(Item.name, 31);
		cout << "Enter Item Quantity (<=3 digits)" << endl;
		cin.getline(Item.quantity, 4);
		cout << "Enter Receipt date in form of (DD/MM/YYYY)" << endl;
		cin.getline(Item.ReceiptDate, 11);
		strcpy_s(Item.item_masterRec, Item.code);
		strcat_s(Item.item_masterRec, "$");
		strcat_s(Item.item_masterRec, Item.name);
		strcat_s(Item.item_masterRec, "$");
		strcat_s(Item.item_masterRec, Item.quantity);
		strcat_s(Item.item_masterRec, "$");
		strcat_s(Item.item_masterRec, "Not issued");
		strcat_s(Item.item_masterRec, "$");
		strcat_s(Item.item_masterRec, Item.ReceiptDate);
		int len = strlen(Item.item_masterRec);
		int size = sizeof(Item.item_masterRec);
		int diff = size - len;
		for (int k = 0; k < diff - 1; k++)
			strcat_s(Item.item_masterRec, ".");
		file << Item.item_masterRec << endl;
		writeInTransactionFile(Item.code, "Receipt", Item.quantity, Item.ReceiptDate);
		cout << "Item has Succussfuly Created " << endl;
		system("pause");

	}
	file.close();
	

}

void Transaction()
{
	cin.ignore();
	item Item[100];
	bool found = false;
	char code[6];
	int i = 0;
	int Index_Of_Wanted_item;
	cout << "Enter the Item code to make transaction on (<=5)" << endl;
	cin.getline(code, 6);
	file.open("itemMaster.txt", ios::in); // opening the itemMaster file as a read .
	checkFile(file, "itemMaster.txt");   // check that there is no problems opening the file.
	while (!file.eof()) // this loop to see if the Item code the user entered is exsist.
	{
		file.getline(Item[i].code, 6, '$');
		if (file.eof()) break; // here the EOF mark has been read at the end so BREAK (only at the last record).
		file.getline(Item[i].name, 31, '$');
		file.getline(Item[i].quantity, 4, '$');
		file.getline(Item[i].IssueDate, 11, '$');
		file.getline(Item[i].ReceiptDate, 11, '.');
		file.getline(Item[i].item_masterRec, 62, '\n');

		if (strcmp(code, Item[i].code) == 0) //check the item code if it the required code.
		{
			Index_Of_Wanted_item = i;
			found = true;
		}
		i++;

	}
	file.close(); // closing the ItemMaster file .
	if (found)
	{
		int temp, temp1, temp2; // to use it when making operations on Item Quantity .
		int transType;
		char Quantity[4];
		char transDate[11];
		file.open("file_Transactions.txt", ios::out | ios::app); // opening the Transaction file as a write to write the transaction
		checkFile(file, "file_Transactions.txt");
		cout << "Enter the Transaction Type" << endl;
		cout << "1 : Issue Transaction " << endl;
		cout << "2 : Receipt Transaction " << endl;
		cin >> transType;
		cin.ignore();
		if (transType == 1) // that mean Issue Transaction.
		{
			cout << "Enter the Issued Quantity (<=3)" << endl;
			cin.getline(Quantity, 4);
			temp1 = atoi(Item[Index_Of_Wanted_item].quantity); // the temp has the new Quantity to put in Item Master file
			temp2 = atoi(Quantity);
			temp = temp1 - temp2;
			if (temp >= 0)
			{
				cout << "Enter the Transaction Date (DD/MM/YYYY) " << endl;
				cin.getline(transDate, 11);
				writeInTransactionFile(code, "Issue", Quantity, transDate);
			}
			else
			{
				cout << "Can't Make this Issue Transaction Not enough Quantity Exisit " << endl;
			}

		}
		else if (transType == 2)
		{
			cout << "Enter the Received Quantity (<=3)" << endl;
			cin.getline(Quantity, 4);
			temp1 = atoi(Item[Index_Of_Wanted_item].quantity); // the temp has the new Quantity to put in Item Master file
			temp2 = atoi(Quantity);
			temp = temp1 + temp2;
			cout << "Enter the Transaction Date (DD/MM/YYYY) " << endl;
			cin.getline(transDate, 11);
			writeInTransactionFile(code, "Receipt", Quantity, transDate);
		}
		else
		{
			cout << "Invalid Input" << endl;
			exit(0);
		}
		file.close(); //closing TransactionFile 

		file.open("itemMaster.txt", ios::out); // opening the itemMaster file in write Mode to modify it after the transaction.
		checkFile(file, "itemMaster.txt");   // check that there is no problems opening the file.

		for (int j = 0; j < i; j++)
		{
			if (j != Index_Of_Wanted_item)
				file << Item[j].code << "$" << Item[j].name << "$" << Item[j].quantity << "$" << Item[j].IssueDate << "$" << Item[j].ReceiptDate << Item[j].item_masterRec << endl;
			else if (j == Index_Of_Wanted_item)
			{
				if (transType == 1)
					file << Item[j].code << "$" << Item[j].name << "$" << temp << "$" << transDate << "$" << Item[j].ReceiptDate << Item[j].item_masterRec << endl;
				else if (transType == 2)
					file << Item[j].code << "$" << Item[j].name << "$" << temp << "$" << Item[j].IssueDate << "$" << transDate << Item[j].item_masterRec << endl;
			}

		}
		file.close();
		cout << "Transaction Successfully Done " << endl;
			system("pause");
		

	}
	else
	{
		cout << "The Item Code Number : " << code << " is not Exist to Make Transaction on " << endl;
	}



}

void DisplayItems()
{
	file.open("itemMaster.txt", ios::in);
	checkFile(file, "itemMaster.txt");
	cout << "\n\nCODE\tQuantity\tLast ISSUE. \t\tLast Receipt.\t\tName" << endl;
	cout << "----\t-------\t\t----------\t\t-------------\t\t----" << endl;
	while (!file.eof())
	{
		file.getline(Item.code, 6, '$');
		if (file.eof()) break; // here the EOF mark has been read at the end so BREAK (only at the last record).
		file.getline(Item.name, 31, '$');
		file.getline(Item.quantity, 4, '$');
		file.getline(Item.IssueDate, 11, '$');
		file.getline(Item.ReceiptDate, 11, '.');
		file.getline(Item.item_masterRec, 62, '\n');
		cout << Item.code << "\t" << Item.quantity << "\t\t" << Item.IssueDate << "\t\t" << Item.ReceiptDate << "\t\t" << Item.name << "\t" << endl << endl;
	}
	cout << "Records Displayed " << endl;
	system("pause");
	file.close();
	
	
}

void search()
{
	cin.ignore();
	char code[7];
	bool found = false;
	cout << "enter the Item Code You want To search for" << endl;
	cin.getline(code, 7);
	file.open("file_Transactions.txt", ios::in);
	checkFile(file, "file_Transactions.txt");
	cout << "\n\nTransaction Type\tQuantity\t\tTransaction Date" << endl;
	cout << "-----------------\t-------\t\t\t----------------" << endl;
	while (!file.eof())
	{

		file.getline(Item.code, 6, '$');
		if (file.eof()) break; // here the EOF mark has been read at the end so BREAK (only at the last record).
		file.getline(Item.trans_type, 8, '$');
		file.getline(Item.quantity, 4, '$');
		file.getline(Item.trans_date, 11, '.');
		file.getline(Item.item_transRec, 30, '\n');
		if (strcmp(code, Item.code) == 0)
		{
			found = true;
			cout << Item.trans_type << "\t\t\t" << Item.quantity << "\t\t\t" << Item.trans_date << endl;
		}
	}
	file.close();
	if (found)
	{
		cout << endl << "Item Transactions Displayed" << endl;
		system("pause");
	}
	else
	{
		cout << "Item not found to Show It's Transactions" << endl;
		system("pause");
	}

}


int main()
{
	//Create_Item();
	//Transaction();
	//DisplayItems();
	//search();
	int c = 5;
	while (c != 0) {
		cout << "\n0 : Exit" << endl;
		cout << "\n1 : Create Item" << endl;
		cout << "\n2 : Make Transaction Operation to an Item" << endl;
		cout << "\n3 : search For Item" << endl;
		cout << "\n4 : Display All Items" << endl;
		cout << "\nChoose an Operation to perform" << endl;
		cin >> c;
		switch (c)
		{
		case 1:
			Create_Item();
			break;
		case 2:
			Transaction();
			break;
		case 3:
			search();
			break;
		case 4:
			DisplayItems();
			break;
		case 0:
			break;
		default:
			cout << "Choose right option" << endl;
		}
	}



	return 0;
}