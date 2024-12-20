#include "contact.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validationMainMenu();
void displayMainMenu();
void displayPersonMenu();
int validationPersonMenu();


int main(int argc, char* argv[]) {
	PersonList *personList=createPersonList();
	if (!personList) {
		return 0;
	}
	EmailToPersonMap* emailToPersonMap = createMap();
	if (!emailToPersonMap) {
		return 0;
	}

	Person *newPerson = createNewPerson("Abu","Dula");
	addContactToPerson(newPerson, CT_PHONE, "9022212222", false);
	addContactToPerson(newPerson, CT_EMAIL, "111@qq.com", false);
	int result = addPersonToList(personList, newPerson);
	addToEmailMap(emailToPersonMap, "111@qq.com", newPerson);

	Person* newPerson1 = createNewPerson("Mike", "Chen");
	addContactToPerson(newPerson1, CT_PHONE, "9023334444", false);
	addContactToPerson(newPerson1, CT_EMAIL, "222@qq.com", false);
	addContactToPerson(newPerson1, CT_ADDRESS, "33 Bristol Road", false);
	int result1=addPersonToList(personList,newPerson1);
	addToEmailMap(emailToPersonMap, "222@qq.com", newPerson1);


	printf("Person Contact Management System\n");

	displayMainMenu();
	int choice=validationMainMenu();

	while(choice >= 1 && choice <= 7){
		if (choice == 1) {
			int result = createNewPersonToList(personList, emailToPersonMap);
			if (result == 1) {
				printf("New person has been added successfully!\n");
				printf("Print person list:\n");
				printPersonList(stdout, personList);
				printf("Next\n");
			}
		}
		else if (choice == 2) {
			printf("Please enter the first name: ");
			char name[50];
			scanf_s("%s", name, (unsigned)_countof(name));
			Person* p = searchPersonByName(personList, name);
			if (!p) {
				printf("Not found!\n");
				
			}
			else {
				printf("Person with first name: %s found!\n", name);
				printPerson(stdout, p, false);
				displayPersonMenu();
				int choice_p = validationPersonMenu();
				while (choice_p >= 1 && choice_p <= 5) {
					if (choice_p == 1) {
						printPerson(stdout, p, false);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 2) {
						inputContact(p, emailToPersonMap);
						printf("Update contact successfully!\n");
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 3) {
						editContact(p, emailToPersonMap);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 4) {
						deleteContact(p, emailToPersonMap);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 5) {
						char* deletedFirstName = getFirstName(p);
						deletePersonandList(personList, emailToPersonMap, p);
						printf("People has been deletedly!\n");
						printPersonList(stdout, personList);
						break;
					}
				}				
			}
		}
		else if (choice == 3) {
			printf("Please enter the email: ");
			char emailToSearch[50];
			scanf_s("%s", emailToSearch, (unsigned)_countof(emailToSearch));
			Person* p = findPersonByEmail(emailToPersonMap, emailToSearch);
			if (!p) {
				printf("Not found!\n");
			}
			else {
				printf("Person with email: %s found!\n", emailToSearch);
				printPerson(stdout, p, false);
				displayPersonMenu();
				int choice_p = validationPersonMenu();
				while (choice_p >= 1 && choice_p <= 5) {
					if (choice_p == 1) {
						printPerson(stdout, p, false);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 2) {
						inputContact(p, emailToPersonMap);
						printf("Update contact successfully!\n");
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 3) {
						editContact(p, emailToPersonMap);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 4) {
						deleteContact(p, emailToPersonMap);
						displayPersonMenu();
						choice_p = validationPersonMenu();
					}
					else if (choice_p == 5) {
						char* deletedFirstName = getFirstName(p);
						deletePersonandList(personList, emailToPersonMap, p);
						printf("People has beed deletedly!\n");
						break;
					}
				}
			}
		}
		else if (choice == 4) {
			clearAll(personList, emailToPersonMap);
			printf("Person List cleared!\n");
		}
		else if (choice == 5) {
			FILE* outfile = fopen("person_list.csv", "w");  
			if (!outfile) {
				printf("Error opening file for writing.\n");
			}
			else {
				;
				int result5 = serialize_person_list_to_csv(personList, "person_list.csv");
				fclose(outfile);  
				if (result5 == 1) {
					printf("Serialization Successfully!\n");
				}
				else {
					printf("Serialization failed.\n");
				}
			}
		}
		else if (choice == 6) {
			//void deserialize_persons(FILE* file, PersonList* person_list, EmailToPersonMap* map)
			FILE* infile = fopen("input.csv", "r");
			if (!infile) {
				perror("Failed to open file");
				return;
			}
			deserialize_persons(infile, personList, emailToPersonMap);
			fclose(infile);
			printPersonList(stdout, personList);
			printf("Deserialization Successfully!\n");
		}
		else {
			/*clearPersonList(personList);
			printf("Finished clear!\n");
			free(personList);
			personList = NULL;
			printf("Finished free 1!\n");
			free(emailToPersonMap);
			emailToPersonMap = NULL;
			printf("Finished free 2!\n");*/
			//for (int i = 0; i < getNumPersons(personList); ++i) {
			///*	destroyPerson(&(getPerson(personList,i)));*/
			//	Person* person = getPerson(personList, i); // Retrieve the person pointer
			//	destroyPerson(&person); //
			//}
			//free(personList);
			deleteAll(personList, emailToPersonMap);
			/*printf("free personlist.\n");
			
			deleteEmailToPersonList(emailToPersonMap);
			printf("free email to person list.\n");*/
			printf("Bye bye!\n");
			return;
		}
		displayMainMenu();
		choice = validationMainMenu();
	}
	return 0;
}

int validationMainMenu() {

	int choice;
	while (1) { 
		printf("Please enter your choice(1-7):");


		if (scanf_s("%d", &choice) == 1 && choice >= 1 && choice <= 7) {
			break; 
		}
		else {
			printf("Invalid input. Please enter an integer between 1 and 7.\n");
			while (getchar() != '\n'); 
		}
	}
	return choice;
}

int validationPersonMenu() {

	int choice;
	while (1) {
		printf("Please enter your choice(1-6):");


		if (scanf_s("%d", &choice) == 1 && choice >= 1 && choice <= 6) {
			break;
		}
		else {
			printf("Invalid input. Please enter an integer between 1 and 6.\n");
			while (getchar() != '\n');
		}
	}
	return choice;
}

void displayMainMenu() {
	printf("\n");
	printf("Main Menu\n");
	printf("1. Create a new person\n");
	printf("2. Search for a person by name\n");
	printf("3. Search for a person by email address\n");
	printf("4. Clear Person list\n");
	printf("5. Save Person list to file \n");
	printf("6. Load Person list from file\n");
	printf("7. Exit\n");
}
void displayPersonMenu() {
	printf("\n");
	printf("Selected Person Options : \n");
	printf("1.View Person contact info\n");
	printf("2.Add new contact info\n");
	printf("3.Edit existing contact info\n");
	printf("4.Delete existing contact info\n");
	printf("5.Delete Person\n");
	printf("6.Return to main menu \n");
}