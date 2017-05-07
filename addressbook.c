#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//macros for char control functions
#define FAILURE -1
#define MODE_1 1
#define MODE_2 2

//main struct
typedef struct a
{
    int id;
    char firstName[15];
    char lastName[15];
    long double phoneNum;
    char address[100];
} Record;

//function prototypes
//displays menu
void menu(void);
//lists records
void list(void);
//gets info for creating a new contact
void getInfo(void);
//adds new record with taken informations
void add(Record*);
//counts records
int countRecords(void);
//sorts for last names
void selectionSort(Record*);
//contact update menu
void update(void);
//checks a record's existence with given id
int checkExistence(int);
//updates phone number of a specific contact
void updatePhoneNumber(int, long double);
//updates address of a specific contact
void updateAddress(int, char*);
//writes updated version to the text file
void writeToTextFile(Record*);
//delete contact menu
void delete(void);
//looks for given id
void deleteID(int);
//and this deletes all information about given id'd record
void deleteProcess(Record*, int);
//id char control
int checkID(char*, int);
//first name and last name char control
char* checkName(char*);
//phone number char control
long double checkNumber(char*);
//choices in menus char control
int checkChoice(char*, int mode);
//int based power function
int myPow(int, int);
//reads current text file and assign it to an array
void getCurrentList(Record*);
//creates text file at first
void createTextFileIfItDoesNotExist(void);

//record counter...
int recordCounter = 0;

int main()
{
	//creates text file at first if it's not exist
	createTextFileIfItDoesNotExist();
    printf("\t\tADDRESS BOOK\n\n----------------------------");
    //calls menu
    menu();
    return 0;
}

void menu()
{
    char choice[10];
    //infinite loop
    while (1)
    {
        printf("\nPlease enter a valid choice\n----------------------------\n");
        printf("1 -> List All Records\n2 -> Update a Record\n3 -> Create a New Record\n4 -> Delete a record\n5 -> Exit\n");
        //getting a choice for menu
        do
        {
            scanf("%s", choice);
        } while(checkChoice(choice, MODE_1) == FAILURE); /*this part is for char control, choices must be integers between 1 and 5.*/
        //if choice is 1, list all records
        if(!strcmp(choice, "1"))
            list();
        //if choice is 2, update a record
        else if(!strcmp(choice, "2"))
            update();
        //if choice is 3, get info for a new contact
        else if(!strcmp(choice, "3"))
            getInfo();
        //if choice is 4, get info to delete a contact
        else if(!strcmp(choice, "4"))
            delete();
        //if choice is 5, exit.
        else if(!strcmp(choice, "5"))
        {
            printf("Bye !\n");
            return;
        }
    }
}

void list()
{
	//if there are more than 0 contacts..
    if (countRecords())
    {
        printf("%s%12s%12s%14s%14s\n==  ==========  ==========  ============       =======\n", "ID", "First Name", "Last Name", "Phone Number", "Address");
        //create a Record array
        Record r[recordCounter];
        //fill it with the text file's records
        getCurrentList(r);
        int i;
        //sort them for last names
        selectionSort(r);
        //and print all of them on the console screen
        for (i = 0; i < recordCounter; i++)
            printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
    }
}

void getInfo()
{
	//getting inputs as char array in order to check them
    char tID[10], tFirst[15], tLast[15], tNum[20], tAddress[50];
    //creates a Record pointer to write the new record to the text file 
    Record *r = (Record*)malloc(sizeof(Record));
    printf("Enter an ID for new contact: ");
    do
    {
        scanf("%s", tID);
    } while(checkID(tID, MODE_1) == FAILURE); /*char control, ID must be an integer.*/
    //if succeed, new record's id will be return value by checkID function which is an integer converted from a char array
    r -> id = checkID(tID, MODE_1);
    printf("Enter a first name for new contact: ");
    do
    {
        scanf("%s", tFirst);
    } while(!checkName(tFirst)); /*char control, First Name must be full of chars.*/
    //if succeed, new record's first name will be return value by checkName function which is a char array
    //must use strcpy function for string assignment
    strcpy(r -> firstName, checkName(tFirst));
    printf("Enter a last name for new contact: ");
    do
    {
        scanf("%s", tLast);
    } while(!checkName(tLast));/*char control, Last Name must be full of chars.*/
    //if succeed, new record's last name will be return value by checkName function which is a char array
    //must use strcpy function for string assignment
    strcpy(r -> lastName, checkName(tLast));
    printf("Enter phone number of new contact: ");
    do
    {
        scanf("%s", tNum);
    } while(checkNumber(tNum) == FAILURE);/*char control, phone number must be full of digits*/
    //if succeed, new record's phone number will be the return value from checkNumber function which is a long double converted from char array
    r -> phoneNum = checkNumber(tNum);
    //there is no control for address because it can be anything.
    printf("Enter the address of new contact: ");
    scanf("%s", r -> address);
    //adding this new record to the text file
    add(r);
}

void add(Record* r)
{
    FILE* file;
    //if file can be opened with append mode...
    if(file = fopen("records.txt", "a"))
    {
    	//if added contact is the first contact write directly
        if(recordCounter == 0)
            fprintf(file, "%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
        //if it's not the first contact
        else
        	//write it to a new line.
            fprintf(file, "\n%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
        //close file
        fclose(file);
        //success message
        printf("New record added.\n");
        //increment counter	
        recordCounter++;
    }
    //if file cannot be opened, print an error message
    else
        printf("Address Book cannot found !");
}

int countRecords()
{
    recordCounter = 0;
    FILE *file;
    int len;
    //if file can be opened with read mode...
    if (file = fopen("records.txt", "r"))
    {
    	//calculates the length of the text file
        //set cursor to the end of file
        fseek(file, 0, SEEK_END);
        //and calculate it's distance to the beginning of the text file
        len = ftell(file);
        //if it's zero it means address book is empty
        if(len == 0)
        {
            printf("Address Book is Empty !\n");
            return 0;
        }
        //go back to beginning of the text file
        rewind(file);
        //while cursor is not on the end of file...
        while (!feof(file))
        {
        	//get current line's record
            Record *rec = (Record *)malloc(sizeof(Record));
            fscanf(file, "%d %s %s %Lf %s", &rec->id, rec->firstName, rec->lastName, &rec->phoneNum, rec->address);
            //increment after each record is scanned.
            recordCounter++;
        }
        //close file
        fclose(file);
    }
    //error message for not founding the text file
    else
    {
        printf("Address Book Cannot Found !");
        return 0;
    }

    return 1;
}

/*SELECTION SORT ALGORITHM DIRECTLY*/
void selectionSort(Record* r)
{
    int i, j;
    for(i=0; i<recordCounter-1; i++)
    {
        for(j=i+1; j<recordCounter; j++)
        {
        	//if last name of the i th element is bigger lexicographically than j th element, swap them
            if(strcmp(r[i].lastName, r[j].lastName) > 0)
            {
                Record temp = r[i];
                r[i] = r[j];
                r[j] = temp;
            }
            //if their last name's are equal
            else if(strcmp(r[i].lastName, r[j].lastName) == 0)
            {
            	//sort for first names
                if(strcmp(r[i].firstName, r[j].firstName) > 0)
                {
                    Record temp = r[i];
                    r[i] = r[j];
                    r[j] = temp;
                }
            }
        }
    }
}

void update()
{
    int id;
    char tID[10];
    //if there are more than zero contacts
    if (countRecords())
    {
	    printf("Enter id for the record which you want to update: ");
	    //same char control for the id above
	    do
	    {
	        scanf("%s", tID);
	    } while(checkID(tID, MODE_2) == FAILURE);
	    id = checkID(tID, MODE_2);
	    //if this id exists
        if (checkExistence(id))
        {
            char choice[10];
            int cInt;
            long double newPhoneNum;
            char newAddress[100];
            char tPhoneNum[20];
            //display update menu
            printf("\n1 -> Update phone number\n2 -> Update address\nEnter your choice: ");
            do
            {
                scanf("%s", choice);
            } while(checkChoice(choice, MODE_2) == FAILURE); /*char control for update choices, must be an integer between 1 and 2*/
            //after success, assign checkChoice function's return value to the cInt
            cInt = checkChoice(choice, MODE_2);
            switch(cInt)
            {
            	//if 1 is selected
                case 1:
                	//get new phone number
                    printf("Enter new phone number: ");
                    do
                    {
                        scanf("%s", tPhoneNum);
                    } while(checkNumber(tPhoneNum) == FAILURE); /*same char control for phone number*/
                    //assign return value to the long double variable newPhoneNumber
                    newPhoneNum = checkNumber(tPhoneNum);
                    //call update phone number function with given id and new phone number
                    updatePhoneNumber(id, newPhoneNum);
                    break;
                //if 2 is selected
                case 2:
                	//get new address
                    printf("Enter new address: ");
                    //there is no control for address...
                    scanf("%s", newAddress);
                    //call updateAddress function with given id and new address
                    updateAddress(id, newAddress);
                    break;
            }
        }
    }
}

int checkExistence(int id)
{
    FILE *file;
    //if file can be opened with read mode...
    if(file = fopen("records.txt", "r"))
    {
    	//while it's not end of the file...
        while(!feof(file))
        {
        	//there is a temporary Record pointer to check given id'd record
            Record *temp = (Record*)malloc(sizeof(Record));
            fscanf(file, "%d %s %s %Lf %s", &temp -> id, temp -> firstName, temp -> lastName, &temp -> phoneNum, temp -> address);
            //if same id'd record is found, exit from function and return succes(1)
            if(temp -> id == id)
            {
                printf("Record is found !\n");
                printf("%d%12s%12s%14.0Lf\t\t%s\n", temp -> id, temp -> firstName, temp -> lastName, temp -> phoneNum, temp -> address);
                return 1;
            }
        }
        //if record cannot found, return failure(0)
        printf("Record cannot found !\n");
        return 0;
    }
    //if text file cannot be found
    else
    	//print an error message and return failure(0)
        printf("Address Book cannot found !\n");
    return 0;
}

void updatePhoneNumber(int id, long double newNumber)
{
    Record r[recordCounter];
    //fill Record array with the text file
    getCurrentList(r);
    int i;
    //look for updated id
    for(i=0; i<recordCounter; i++)
    {
        if(r[i].id == id)
        {
        	//if it's found, assign new phone number to it's phone number and exit from loop
            r[i].phoneNum = newNumber;
            break;
        }
    }
    //write new array to the text file
    writeToTextFile(r);
    //success message
    printf("Updated successfuly !\n");
    //print updated record
    printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
}

void updateAddress(int id, char *newAddress)
{
    Record r[recordCounter];
    //fill Record array with the text file
    getCurrentList(r);
    int i;
    //look for given id
    for(i=0; i<recordCounter; i++)
    {
    	//if it's found...
        if(r[i].id == id)
        {
        	//assign it's new address to the address field with strcpy function and exit from loop
            strcpy(r[i].address, newAddress);
            break;
        }
    }
    //write new array to the text file
    writeToTextFile(r);
    //success message
    printf("Updated successfuly !\n");
    //print updated record
    printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
}

void writeToTextFile(Record *r)
{
	/*prints a pointer to the text file*/
    FILE *file;
    int i;
    //open file with write mode
    if(file = fopen("records.txt", "w"))
    {
        for(i=0; i<recordCounter; i++)
        {
        	//print first contact directly
            if(i == 0)
                fprintf(file, "%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
            //print others from a new line
            else
                fprintf(file, "\n%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
            //increment pointer
            r++;
        }
        //close file
        fclose(file);
    }
    //it's not possible but if file cannot be opened
    else
        printf("Address Book cannot found !");
}

void delete()
{
    int id;
    char tID[10];
    if(countRecords())
    {
        printf("Enter id for the record which you want to delete: ");
        do
	    {
	        scanf("%s", tID);
	    } while(checkID(tID, MODE_2) == FAILURE); /*char control for id, must be an integer*/
	    //assign function's return value to the integer variable id
	    id = checkID(tID, MODE_2);
	    //call deleteID
        deleteID(id);
    }
}

void deleteID(int id)
{
    Record r[recordCounter];
    //fill array with the current records in the text file
    getCurrentList(r);
    int i;
    //looks for given id
    for(i=0; i<recordCounter; i++)
    {
    	//if given id is found print it and exit from loop
        if(r[i].id == id)
        {
            printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
            break;
        }
    }
    //if given id cannot found
    if(i == recordCounter)
        printf("\nRecord cannot found !\n");
    //if it's found, call deleteProcess function with given array and index(index points the deleted contact because we exit the loop when we find it)
    else
        deleteProcess(r, i);
}

void deleteProcess(Record *r, int index)
{
    int j;
    //create a new array(as pointer)
    Record *newR = (Record*)malloc(sizeof(Record)*(recordCounter-1));
    for(j=0; j<recordCounter; j++)
    {
    	//if j doesn't point the deleted record, assign corresponding index to newR array
    	//do not assign deleted record
        if(j != index)
            newR[j] = *r;
        //increment pointer
        r++;
    }
    //success message
    printf("Deleted successfuly !\n");
    //decrement record counter
    recordCounter--;
    //write new record list to the text file 
    writeToTextFile(newR);
}

/*ID char control function*/
int checkID(char* id, int mode)
{
    int i;
    int val = 0;
    double ctr = 0;
    for(i=0; i<strlen(id); i++)
    {
    	//if there is a non-digit char
        if(*(id+i) < 48 || *(id+i) > 57)
        {
        	//print error message and exit directly from the function
            printf("ID must be an integer. Please enter again: ");
            return FAILURE;
        }
    }
    //convert integer from char
    for(i = strlen(id)-1; i>=0; i--)
    {
        val += (*(id+i)-48) * myPow(10, ctr);
    	ctr++;
    }
    /*MODE_1 IS FOR CREATING A NEW CONTACT, DETECTS ID DUPLICATE*/
    if(mode == MODE_1)
	{
	    Record r[recordCounter];
	    //fill Record array with the current text file
	    getCurrentList(r);
	    //checks for ID existence
	    for(i=0; i<recordCounter; i++)
	    {
	    	//if it's found
	        if(r[i].id == val)
	        {
	        	//print an error message and exit from function
	            printf("This ID already exists. Enter a new one: ");
	            return FAILURE;
	        }
	    }
	}
    //return converted integer value
    return val;
}

char* checkName(char *name)
{
	/*char control for first name and last name, cannot contain any digit*/
    int i;
    for(i=0; i<strlen(name); i++)
    {
        if(name[i] >= 48 && name[i] <= 57)
        {
        	//if there is a digit in first name or last name
        	//print an error message and exit from the function
            printf("First Name/Last Name cannot contain an integer. Please enter again: ");
            return NULL;
        }
    }
    //if succeed, return name which is full of chars
    return name;
}

/*PHONE NUMBER CHAR CHECK FUNCTION*/
long double checkNumber(char *number)
{
    int i, ctr = 0;
    long double val = 0;
    //looks for every indexes of given number char pointer
    for(i=strlen(number)-1; i>=0; i--)
    {
    	//if there is a non-digit char
        if(number[i] < 48 || number[i] > 57)
        {
        	//print an error message and exit from the function directly
            printf("Phone Number must be a number. Please enter again: ");
            return FAILURE;
        }
        //convert given char's value to long double
        val += (number[i]-48)*(long double)myPow(10, ctr);
        //ctr variable is for powers of 10
        ctr++;
    }
    //if number is shorter than 5 digits...
    if(strlen(number) < 5)
    {
    	//print an error message and exit from the function
        printf("Number is too short ! Please enter again: ");
        return FAILURE;
    }
    //return converted long double value
    return val;
}

/*CHOICES IN MENUS' CHAR CONTROL*/
int checkChoice(char* c, int mode)
{
    int i, ctr = 0;
    int val = 0;
    if(strlen(c) > 1)
    {
    	printf("Your choice must be a valid integer. Enter again: ");
    	return FAILURE;
    }
    for(i=strlen(c)-1; i>=0; i--)
    {
    	//mode 1 is for integers between 1 and 5
        if (mode == MODE_1)
        {
        	//if it's not between 1 and 5 or it's not an integer
            if (c[i] < 49 || c[i] > 53)
            {
            	//print an error message and exit from the function
                printf("Your choice must be a valid integer. Enter again: ");
                return FAILURE;
            }
        }
        //mode 2 is for integers between 1 and 2
        else if(mode == MODE_2)
        {
        	//if it's not between 1 and 2 or it's not an integer
            if(c[i] < 49 || c[i] > 50)
            {
            	//print an error message and exit from the function
                printf("Your choice must be a valid integer. Enter again: ");
                return FAILURE;
            }
        }
        //calculate choice, 
        val += (c[i]-48)*myPow(10, ctr);
    }
    //returns converted value, only used for update function(between 1 and 2).
    return val;
}


int myPow(int base, int power)
{
    int i;
    int val = 1;
    for(i=0; i<power; i++)
        //calculates base power power...
        //multiplies with base power times
        val *= base;
    //and returns calculated value
    return val;
}

/*IN ORDER TO FILL RECORD ARRAY WITH CURRENT TEXT FILE*/
void getCurrentList(Record r[recordCounter])
{
    FILE *file;
    int i = 0;
    //if file can be opened in read mode
    if(file = fopen("records.txt", "r"))
    {
    	//til the end of the file
        while(!feof(file))
        {
        	//scan and assign values to the array
            fscanf(file, "%d %s %s %Lf %s", &r[i].id, r[i].firstName, r[i].lastName, &r[i].phoneNum, r[i].address);
            i++;
        }
        //close file
        fclose(file);
    }
    //if file cannot be found
    else
        printf("Address Book cannot found !\n");
}

void createTextFileIfItDoesNotExist()
{
	FILE *str;
	//if file can be opened in read mode it means file exists and close it properly
	if(str = fopen("records.txt", "r"))
		fclose(str);
	else
	{
		//if file cannot be opened in read mode, it means that file does not exist.
		//opening it with write mode creates an empty file named "records.txt"
		str = fopen("records.txt", "w");
		//after creating, close it properly
		fclose(str);
	}
}
