#include "contact.h"
#include "csv.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/// <summary>
/// A struct to hold contact information.
/// </summary>
typedef struct contact_s {
	ContactType type;
	char* contact;
} Contact;

ContactType getType(Contact* contact) {
	return contact->type;
}

void setType(Contact* contact, ContactType newType) {
	contact->type = newType;
}

char* getContact(Contact* contact) {
	return contact->contact;
}

void setContact(Contact* contact, char* newContact) {
	contact->contact = newContact;
}

/// <summary>
/// A struct to hold information about a person,
/// including a list of contacts. Note that the
/// Contacts are stored as VALUE TYPES in an array.
/// </summary>
typedef struct person_s {
	char first_name[MAX_CHARACTERS_IN_NAME + 1];
	char last_name[MAX_CHARACTERS_IN_NAME + 1];
	Contact contacts[MAX_CONTACTS_PER_PERSON];
	int num_contacts;
	int primary_contact_index;
	int address_contact_index;
} Person;

char* getFirstName(Person* p) {
	return p->first_name;
}

void setPrimaryContact(Person* person, int index) {
	person->primary_contact_index = index;
}

int getNumContacts(Person* person) {
	return person->num_contacts;
}

Contact* getContacts(Person* person,int index) {
	return &(person->contacts[index]);
}

void plusPlusNumcontacts(Person* person) {
	person->num_contacts++;
}

int getIndexofContact(Person* person, char* contact) {
	for (int i = 0;i < person->num_contacts ;i++) {
		//printf("%s\n", &(person->contacts->contact[i]));
		if (strcmp(&(person->contacts->contact[i]), contact) == 0) {
			return i;
		}
	}
	return -1;
}

//  create a person list contain Max_persons
typedef struct person_list_s {
	Person* persons[MAX_PERSONS];
	int num_persons;
} PersonList;

int getNumPersons(PersonList* personList) {
	return personList->num_persons;
}

void MinusNumPersons(PersonList* personList) {
	personList->num_persons--;
}

char* getPersonFirstName(PersonList* personList, int index) {
	return personList->persons[index]->first_name;
}

Person* getPerson(PersonList* personList, int index) {
	return personList->persons[index];
}

void setPerson(PersonList* personList, int index,Person* person) {
	personList->persons[index] = person;
}

// define email person node
typedef struct email_map_entry_s {
	char email[MAX_CHARACTERS_IN_NAME];
	Person* person;  // Pointer to the corresponding person
} EmailMapEntry;

// define email person map
typedef struct email_to_person_map_s {
	EmailMapEntry entries[MAX_PERSONS];
	int size;  // Number of entries in the map
} EmailToPersonMap;

///Initialize the Map
void initializeEmailMap(EmailToPersonMap* map) {
	if (!map) return;
	map->size = 0;  // Initialize with no entries
}

//find person by email
Person* findPersonByEmail(EmailToPersonMap* map, const char* email) {
	if (!map || !email) return NULL;

	for (int i = 0; i < map->size; i++) {
		if (strcmp(map->entries[i].email, email) == 0) {
			return map->entries[i].person;  // Found the person
		}
	}
	return NULL;  // Not found
}

//Add Email to the Map
void addToEmailMap(EmailToPersonMap* map, const char* email, Person* person) {
	if (!map || map->size >= MAX_PERSONS || !email || !person) return;

	// Add the email and person to the map
	strcpy_s(map->entries[map->size].email, sizeof(map->entries[map->size].email), email);
	map->entries[map->size].person = person;

	map->size++;
}

void updateEmailMap(EmailToPersonMap* map, const char* oldEmail, const char* newEmail) {
	if (!map || !newEmail || !oldEmail)
	{
		return;
	}
	for (int i = 0;i < map->size;i++) {
		if (strcmp(map->entries[i].email,oldEmail)==0) {

			if (strlen(newEmail) >= sizeof(map->entries[i].email)) {
				printf("Error: The new email exceeds the maximum allowed length.\n");
				return;
			}
			strncpy_s(map->entries[i].email, sizeof(map->entries[i].email), newEmail, sizeof(map->entries[i].email) - 1);
			map->entries[i].email[sizeof(map->entries[i].email) - 1] = '\0';
			return;
		}
	}
	return;
}

void deleteEmailMapbyEmail(EmailToPersonMap* map, const char* email) {
	if (!map || !email) return;
	for (int i = 0;i < map->size;i++) {
		if (map->entries[i].email == email) {
			for (int j = i; j < map->size - 1; ++j) {
				strcpy_s(map->entries[j].email, sizeof(map->entries[j].email), map->entries[j + 1].email);
				map->entries[j].person = map->entries[j + 1].person;
			}
			(map->size)--;
			break;
		}
	}
}

//initialize personlist

void initializePersonList(PersonList* list) {
	if (!list) return;
	list->num_persons = 0;
	for (int i = 0; i < MAX_PERSONS; ++i) {
		list->persons[i] = NULL;
	}
}

// Create and return a new PersonList
PersonList* createPersonList() {
	// Allocate memory for the list
	PersonList* newList = (PersonList*)malloc(sizeof(PersonList));
	if (!newList) {
		printf("Failed to allocate memory for PersonList.\n");
		return NULL;
	}

	// Initialize the new list
	initializePersonList(newList);

	return newList;
}

EmailToPersonMap* createMap() {
	EmailToPersonMap* newMap = (EmailToPersonMap*)malloc(sizeof(EmailToPersonMap));
	if (!newMap) {
		printf("Failed to allocate memory for EmailToPersonMap.\n");
		return NULL;
	}

	// Initialize the new list
	initializeEmailMap(newMap);

	return newMap;
}

// add person to person list
int addPersonToList(PersonList* list, Person* person) {
	if (!list || !person || list->num_persons >= MAX_PERSONS) {
		return 0; // Failure
	}
	list->persons[list->num_persons++] = person;
	return 1; // Success
}

// print all person in list
void printPersonList(FILE* out, PersonList* list) {
	if (!list) return;
	for (int i = 0; i < list->num_persons; ++i) {
		fprintf(out, "Person #%d:\n", i + 1);
		printPerson(out, list->persons[i], false); // Example: LastNameFirst = false
		fprintf(out, "\n");
	}
}

// clear all person in list
void clearPersonList(PersonList* list) {
	if (!list) return;
	for (int i = 0; i < list->num_persons; ++i) {
		destroyPerson(&(list->persons[i]));
	}
	list->num_persons = 0;
	return;
}

void deleteAll(PersonList* list, EmailToPersonMap* map) {
	if (!list || !map) return;
	for (int i = 0; i < list->num_persons; ++i) {
		for (int j = 0; j < map->size; ++j) {
			if (list->persons[i] == map->entries[i].person) {
				destroyPerson(&(list->persons[i]));
				list->persons[i] = NULL;
				map->entries[i].person = NULL;
			}
		}
	}
	free(list);
	free(map);
	list = NULL;
	map = NULL;
}

void clearAll(PersonList* list, EmailToPersonMap* map) {
	if (!list || !map) return;
	for (int i = 0; i < list->num_persons; ++i) {
		for (int j = 0; j < map->size; ++j) {
			if (list->persons[i] == map->entries[i].person) {
				destroyPerson(&(list->persons[i]));
				list->persons[i] = NULL;
				map->entries[i].person = NULL;
			}
		}
	}
	list->num_persons = 0;
	map->size = 0;
}

void clearEmailToPersonList(EmailToPersonMap* map) {
	if (!map) {
		return;
	}
	for (int i = 0; i < map->size; i++) {
		if (map->entries[i].person) {
			destroyPerson(&(map->entries[i].person));  // Free the dynamically allocated memory
			map->entries[i].person = NULL;  // Set the pointer to NULL after freeing
		}
	}
	map->size = 0;
}

/// <summary>
/// A very simple find and replace that replaces characters in a string on
/// a one-for-one basis.
/// </summary>
/// <param name="src">The string we want to alter.</param>
/// <param name="max_count">The maximum length of the string, used to ensure we stay in bounds</param>
/// <param name="findme">The character we are looking to replace</param>
/// <param name="replacewithme">The character that we want to replace findme with</param>
void replace_character_in_string(char* src, unsigned max_count, char findme, char replacewithme) {
	for (unsigned i = 0; i < strnlen_s(src, max_count); ++i) {
		if (src[i] == findme) {
			src[i] = replacewithme;
		}
	}
}

/// <summary>
/// Utility function for convert an emum type into a human readable string.
/// </summary>
/// <param name="ct">The contact type enum.</param>
/// <returns>A human readable string corresponding to the ContactType. Returns "Unknown" for 
/// invalid Contact Types</returns>
char const* contactTypeToString(ContactType ct) {
	//Use static variables here to ensure that the setup code only happens once.
	static bool initialized = false;
	static char const* enumStrings[CT_COUNT + 1];
	if (!initialized) {
		enumStrings[CT_NONE] = "None";
		enumStrings[CT_PHONE] = "Phone";
		enumStrings[CT_CELL] = "Cell";
		enumStrings[CT_EMAIL] = "Email";
		enumStrings[CT_ADDRESS] = "Address";
		enumStrings[CT_UNKNOWN] = "Unknown";
		enumStrings[CT_COUNT] = "Invalid";
		initialized = true;
	}
	//Check that the Contact Type is valid.
	if (ct < CT_NONE || ct >= CT_COUNT) {
		return enumStrings[CT_COUNT];
	}
	return enumStrings[ct];
}

ContactType stringToContactType(char const *string) {
	if (strcmp(string, "Phone") == 0) {
		return CT_PHONE;
	}
	else if (strcmp(string, "Cell") == 0) {
		return CT_CELL;
	}
	else if (strcmp(string, "Email") == 0) {
		return CT_EMAIL;
	}
	else if (strcmp(string, "Address") == 0) {
		return CT_ADDRESS;
	}
	else {
		return CT_UNKNOWN;
	}
}

/// <summary>
/// Create a new person dynamically on the heap, including the allocation
/// of space for 10 contacts (but not the contact strings - those need
/// to be allocated for each contact separately).
/// A person must have a valid first name and last name (that is, the
/// parameters for first_name and last_name must be non-null and non-empty).
/// </summary>
/// <param name="first_name">The first name of the person</param>
/// <param name="last_name">The last name of the person</param>
/// <returns>An initialized person if successful, otherwise NULL</returns>
Person* createNewPerson(char const* first_name, char const* last_name) {

	if (!first_name || strlen(first_name) == 0 ||
		!last_name || strlen(last_name) == 0
		) {
		//Error, empty first name or null strings not allowed
		return NULL;
	}
	//Allocate memory for the person
	Person* person = malloc(sizeof(*person));
	if (!person) {
		return NULL;
	}
	//Copy the first and last name into the string. We use strncpy to truncate the strings if they are too long.
	if (strncpy_s(person->first_name, MAX_CHARACTERS_IN_NAME + 1, first_name, MAX_CHARACTERS_IN_NAME) ||
		strncpy_s(person->last_name, MAX_CHARACTERS_IN_NAME + 1, last_name, MAX_CHARACTERS_IN_NAME)) {
		//Error, couldn't copy the strings (maybe they are too long?)
		free(person);
		return NULL;
	}
	//Replace any commas in the names with periods. This will make serializing the data so much easier.
	replace_character_in_string(person->first_name, MAX_CHARACTERS_IN_NAME, ',', '.');
	replace_character_in_string(person->last_name, MAX_CHARACTERS_IN_NAME, ',', '.');
	person->num_contacts = 0;
	person->primary_contact_index = -1;
	person->address_contact_index = -1;
	return person;
}

/// <summary>
/// if person is NULL, return
/// loop clear all contacts the person have
/// clear person
/// </summary>
/// <param name="person"></param>
void destroyPerson(Person** person) {
	//Don't forget to free all the contact info strings!
	if (!person ||!*person) {
		return ;
	}
	for (int i = 0;i <(*person)->num_contacts ;i++) {
		if ((*person)->contacts[i].contact) {
			free((*person)->contacts[i].contact);
			(*person)->contacts[i].contact = NULL;
		}
		
	}
	free(*person);
	*person = NULL;
}

/// <summary>
/// Prints a Contact to a specified file. Call with stdout to print directly to the console.
/// Does not print a newline at the end of the contact. Prints NULL if contact is a NULL pointer.
/// </summary>
/// <param name="out">File to send the output to.</param>
/// <param name="contact">Pointer to the contact you want to print</param>
void printContact(FILE* out, Contact* contact) {
	if (!contact) {
		fprintf(out, "NULL");
		return;
	}
	char const* contact_type_string = contactTypeToString(contact->type);
	fprintf(out, "%s: %s", contact_type_string, contact->contact);
	return;
}

/// <summary>
/// Prints a Person and their contact info to a specified file. Call with stdout to print directly to the console.
/// Prints a newline after the last output.
/// Contact info is printed in the following order:
/// Primary contact info (if any exists)
/// All other non-address contact info (if any exists)
/// Primary address contact info (if any exists)
/// All other address contact info (if any exists)
/// Contacts can be printed with the name as "Firstname Lastname" or as "Lastname, Firstname"
/// </summary>
/// <param name="out">The file to send the output to</param>
/// <param name="person">A pointer to the person whose details you want to print</param>
/// <param name="lastNameFirst">If true, prints contact name as "Lastname, Firstname", otherwise prints as "Firstname Lastname"</param>
void printPerson(FILE* out, Person* person, bool lastNameFirst) {
	if (!person) {
		fprintf(out, "NULL\n");
	}
	else {
		//Print the Person's name
		fprintf(out, "%s%s%s\n", lastNameFirst ? person->last_name : person->first_name, lastNameFirst ? ", " : " ", lastNameFirst ? person->first_name : person->last_name);
		//Print's the persons contact info (if any exist).
		if (person->num_contacts == 0) {
			fprintf(out, "No contacts on file");
		}
		else {
			//Primary contact info first
			if (person->primary_contact_index >= 0) {
				fprintf(out, "Primary ");
				printContact(out, &person->contacts[person->primary_contact_index]);
				fprintf(out, "\n");
			}
			//All other non-address, non-primary contact info
			for (int i = 0; i < person->num_contacts; ++i) {
				if ((i != person->address_contact_index) && (i != person->primary_contact_index) && (person->contacts[i].type != CT_ADDRESS)) {
					fprintf(out, "\t");
					printContact(out, &person->contacts[i]);
					fprintf(out, "\n");
				}
			}
			//Primary address contact info
			if (person->address_contact_index >= 0) {
				fprintf(out, "Primary ");
				printContact(out, &person->contacts[person->address_contact_index]);
				fprintf(out, "\n");
			}
			//All other address contact info
			for (int i = 0; i < person->num_contacts; ++i) {
				if (i != person->address_contact_index && i != person->primary_contact_index && person->contacts[i].type == CT_ADDRESS) {
					fprintf(out, "\t");
					printContact(out, &person->contacts[i]);
					fprintf(out, "\n");
				}
			}
		}
	}
}

/// <summary>
/// Add a new contact to an existing person. You need to specify the type of contact as well as 
/// a string that contains the info (like the email address, or mailing address, or phone number).
/// Currently there is no validation being done on the contact info - no effort is made validate 
/// that the contact info for a cell phone is actually a valid phone number.
/// If the person has no existing non-address contact info, the first contact info added to the person will
/// automatically become the primary contact info, even if the setAsPrimaryContact flag is false. If the
/// person has no existing address contact info, the first address contact info added will 
/// automatically become the primary address info, even if the setAsPrimaryContact flag is false.
/// The function fails if there is no more room in the Person's contact info list.
/// </summary>
/// <param name="person">The person to which you are adding the new contact info</param>
/// <param name="ct">The type of the new contact info, like cell phone or address</param>
/// <param name="contact_info">The string with the actual contact info in it</param>
/// <param name="setAsPrimaryContact">Use this flag to indicate this is the primary contact info.
/// Each person can have at most 1 primary address and 1 non-address primary contact (like a phone
/// number or an email address).</param>
/// <returns>0 on failure, otherwise returns the number of contacts stored for person</returns>
int addContactToPerson(Person* person, ContactType ct, char const* contact_info, bool setAsPrimaryContact) {
	//Need a person
	if (!person) {
		return 0;
	}
	//Person must have enough room
	if (person->num_contacts >= MAX_CONTACTS_PER_PERSON) {
		return 0;
	}
	//Contact type must be valid
	if (ct < CT_NONE || ct >= CT_COUNT) {
		return 0;
	}
	char* contact_str = malloc(strnlen_s(contact_info, MAX_CHARACTERS_IN_CONTACT) + 1);
	//Ran out of memory
	if (!contact_str) {
		return 0;
	}
	//Copy the data into the new contact
	strncpy_s(contact_str, strnlen_s(contact_info, MAX_CHARACTERS_IN_CONTACT) + 1, contact_info, MAX_CHARACTERS_IN_CONTACT);
	person->contacts[person->num_contacts].type = ct;
	person->contacts[person->num_contacts].contact = contact_str;

	//If the set as primary contact flag is set, set this as the primary contact. Behaviour is slightly different
	// if this contact is an address.
	if (ct == CT_ADDRESS) {
		if (setAsPrimaryContact || person->address_contact_index == -1) {
			person->address_contact_index = person->num_contacts;
		}
	}
	else {
		if (setAsPrimaryContact || person->primary_contact_index == -1) {
			person->primary_contact_index = person->num_contacts;
		}
	}

	//Increment number of contacts.
	person->num_contacts++;
	return person->num_contacts;
}

int serializePersonToCSV(FILE* outfile, PersonList* list) {
	if (!outfile || !list) {
		return 0;
	}
	for (int i = 0;i < list->num_persons;i++) {

		Person* person = list->persons[i];

		if (!person) {
			continue;
		}

		if (fprintf(outfile, "\"%s\",\"%s\",", person->first_name, person->last_name) < 0) {
			return 0; // Error during writing
		}

		if (person->primary_contact_index >= 0) {
			Contact* primary_contact = &person->contacts[person->primary_contact_index];
			if (fprintf(outfile, "\"%s\",\"%s\",", contactTypeToString(primary_contact->type), primary_contact->contact) < 0) {
				return 0; // Error during writing
			}
		}
		else {
			if (fprintf(outfile, "\"\",\"\",")) {
				return 0; // Error during writing
			}

		}

		if (person->address_contact_index >= 0) {
			Contact* address_contact = &person->contacts[person->address_contact_index];
			if (fprintf(outfile, "\"%s\",\"%s\"\n", contactTypeToString(address_contact->type), address_contact->contact) < 0) {
				return 0; // Error during writing
			}
		}
		else {
			fprintf(outfile, "\"\",\"\"\n");
		}

		for (int j = 0;j < person->num_contacts;j++) {
			Contact* contact = &person->contacts[j];
			if (!contact) {
				continue;
			}
			const char* contact_type_str = contactTypeToString(contact->type);
			const char* label = "";

			if (j == person->primary_contact_index) {
				label = "Primary";
			}
			else if (j == person->address_contact_index) {
				label = "Address";
			}

			if (fprintf(outfile, "\"%s\",\"%s\",\"%s\"\n", contact_type_str, contact->contact, label) < 0) {
				return 0; // Error during writing
			}

		}

	}



	return 1;
};

Person* deserializePersonFromCSV(FILE* in) {
	if (!in) {
		return NULL;
	}

	char buffer[MAX_CHARACTERS_IN_CONTACT*2];
	char* token;
	char* context = NULL;

	if (!fgets(buffer, sizeof(buffer), in)) {
		return NULL;
	}

	char first_name[MAX_CHARACTERS_IN_NAME + 1] = { 0 }, last_name[MAX_CHARACTERS_IN_NAME + 1] = { 0 };
	char primary_contact_type[50] = { 0 }, primary_contact_info[MAX_CHARACTERS_IN_CONTACT] = { 0 };
	char address_contact_type[50] = { 0 }, address_contact_info[MAX_CHARACTERS_IN_CONTACT] = { 0 };

	token = strtok_s(buffer, ",", &context);
	if (token) {
		strncpy_s(first_name, MAX_CHARACTERS_IN_NAME + 1, token + 1, MAX_CHARACTERS_IN_NAME);
		first_name[strlen(first_name) - 1] = '\0';
	}


	token = strtok_s(NULL, ",", &context);
	if (token) {
		strncpy_s(last_name, MAX_CHARACTERS_IN_NAME + 1, token + 1, MAX_CHARACTERS_IN_NAME);
		last_name[strlen(last_name) - 1] = '\0';
	}


	token = strtok_s(NULL, ",", &context);
	if (token) {
		strncpy_s(primary_contact_type, sizeof(primary_contact_type) , token, MAX_CONTACT_TYPE_LENGTH);
		primary_contact_type[strlen(primary_contact_type - 1)] = '\0';
	}


	token = strtok_s(NULL, ",", &context);
	if(token){
		strncpy_s(primary_contact_info, sizeof(primary_contact_info), token, MAX_CONTACT_INFO_LENGTH);
		primary_contact_info[strlen(primary_contact_info - 1)] = '\0';
	}

	token = strtok_s(NULL, ",", &context);
	if (token) {
		strncpy_s(address_contact_type, sizeof(address_contact_type), token, MAX_CONTACT_TYPE_LENGTH - 1);
		address_contact_type[strlen(address_contact_type - 1)] = '\0';
	}

	token = strtok_s(NULL, ",", &context);
	if (token) {
		strncpy_s(address_contact_info, sizeof(address_contact_info), token, MAX_CONTACT_INFO_LENGTH - 1);
		address_contact_info[strlen(address_contact_info - 1)] = '\0';
	}

	Person* new = createNewPerson(first_name, last_name);
	if (!new) {
		return NULL;
	}

	if(strlen(primary_contact_info)>0){
		ContactType primary_type = stringToContactType(primary_contact_type);
		addContactToPerson(new, primary_type, primary_contact_info, true);
	}

	if (strlen(address_contact_info) > 0) {
		ContactType address_type = stringToContactType(address_contact_type);
		addContactToPerson(new, address_type, address_contact_info, true);
	}


	//parse remaining contacts in the CSV file and add to the Person
	while (fgets(buffer, sizeof(buffer), in)) {
		char contact_type[50] = { 0 };
		char contact_info[MAX_CHARACTERS_IN_CONTACT] = { 0 };
		char primary_flag[20] = { 0 };

		token = strtok_s(buffer, ",", &context);
		if (token) {
			strncpy_s(contact_type, sizeof(contact_type), token , sizeof(contact_type) - 1);
			contact_type[strlen(contact_type) - 1] = '\0';
		}

		token = strtok_s(NULL, ",", &context);
		if (token) {
			strncpy_s(primary_flag, sizeof(primary_flag), token, sizeof(primary_flag) - 1);
			primary_flag[strlen(primary_flag) - 1] = '\0';
		}

		// Determine if this contact is primary or address
		bool setAsPrimary = (strcmp(primary_flag, "Primary") == 0);
		ContactType contactType = stringToContactType(contact_type);
		addContactToPerson(new, contactType, contact_info, setAsPrimary);
	}
	return new;
};




int createNewPersonToList(PersonList* list, EmailToPersonMap* emailToPersonMap) {
	if (!list) {
		return 0;
	}
	char firstName[MAX_CHARACTERS_IN_NAME], lastName[MAX_CHARACTERS_IN_NAME];

	while (getchar() != '\n');

	printf("Input first name: ");
	fgets(firstName, sizeof(firstName), stdin);
	firstName[strcspn(firstName, "\n")] = '\0';

	printf("Input last name: ");
	fgets(lastName, sizeof(lastName), stdin);
	lastName[strcspn(lastName, "\n")] = '\0';

	Person* new = createNewPerson(firstName, lastName);
	inputContact(new, emailToPersonMap);
	return addPersonToList(list, new);
}

//void clearInputBuffer() {
//	int c;
//	while ((c = getchar()) != '\n' && c != EOF) {
//		// Discard characters in the buffer
//	}
//}

void inputContact(Person* person, EmailToPersonMap* emailToPersonMap) {
	if (!person || !emailToPersonMap) {
		return;
	}
	char isContinue = 'y';
	// Check if we can add more contacts
	while (tolower(isContinue) == 'y') {


	if (getNumContacts(person) >= MAX_CONTACTS_PER_PERSON) {
		printf("Maximum number of contacts reached!\n");
		return;
	}

	printf("Enter the contact type (Phone, Cell, Email, Address): ");
	char contactTypeInput[20];
	scanf_s("%s", contactTypeInput, (unsigned)_countof(contactTypeInput));

	printf("Do you want to set as primary contact or address(y or n)? ");
	char isPrimary = 'n';
	bool boolPrimary = false;
	while (1) {
		scanf_s(" %c", &isPrimary, 1); 
		isPrimary = tolower(isPrimary); 
		
		if (isPrimary == 'y' || isPrimary == 'n') {
			if (isPrimary == 'y') {
				boolPrimary = true;
			}
			break; 
		}
		else {
			printf("Invalid input. Please enter 'y' for Yes or 'n' for No: ");
			while (getchar() != '\n'); 
		}
	}

	// Convert input string to ContactType enum

	ContactType contactType = stringToContactType(contactTypeInput);

	// Input the actual contact detail
	//printf("Enter the contact detail (e.g., phone number, email, etc.): ");
	char contactDetailInput[50];

	/*scanf_s("%s", contactDetailInput, (unsigned)_countof(contactDetailInput));*/

	////

	while (getchar() != '\n');

	printf("Enter the contact detail (e.g., phone number, email, or address): ");
	fgets(contactDetailInput, sizeof(contactDetailInput), stdin);

	size_t len = strlen(contactDetailInput);
	if (len > 0 && contactDetailInput[len - 1] == '\n') {
		contactDetailInput[len - 1] = '\0';
	}

	////

	addContactToPerson(person, contactType, contactDetailInput, boolPrimary);

	if (contactType == CT_EMAIL) {
		//addToEmailMap(EmailToPersonMap* map, const char* email, Person* person)
		addToEmailMap(emailToPersonMap, contactDetailInput, person);
	}

	while (1) { 
		printf("Contact added. Do you want to continue input contact? (y or n): ");
		scanf_s(" %c", &isContinue, 1);  
		isContinue = tolower(isContinue);
		if (isContinue == 'y' || isContinue == 'n') {
			break; 
		}
		else {
			printf("Invalid input. Please enter 'y' for Yes or 'n' for No.\n");
			while (getchar() != '\n'); 
		}
	}
}
}

Person* searchPersonByName(PersonList* personList, char const* firstName) {
	if (!personList || !firstName) { 
		return NULL; 
	}
	for (int i = 0;i < getNumPersons(personList);i++) {
		if (strcmp(getPersonFirstName(personList, i) ,firstName)==0) {

			return getPerson(personList, i);
		}
	}
	return NULL;
}

void editContact(Person* person, EmailToPersonMap* emailToPersonMap) {
	if (!person || !emailToPersonMap) {
		return;
	}
	while (getchar() != '\n');
	char neededChangeContact[50];
	printf("Enter the contact detail you want to edit: ");

	fgets(neededChangeContact, sizeof(neededChangeContact), stdin);
	size_t len = strlen(neededChangeContact);
	if (len > 0 && neededChangeContact[len - 1] == '\n') {
		neededChangeContact[len - 1] = '\0';
	}

	for (int i=0; i < person->num_contacts; i++) {

		if (strcmp(person->contacts[i].contact, neededChangeContact) == 0) {


			char updatedContactDetail[50];

			printf("Contact you want to edit found! Enter new contact detail: ");

			fgets(updatedContactDetail, sizeof(updatedContactDetail), stdin);
			size_t len = strlen(updatedContactDetail);
			if (len > 0 && updatedContactDetail[len - 1] == '\n') {
				updatedContactDetail[len - 1] = '\0';
			}


			if (strlen(updatedContactDetail) >= MAX_CHARACTERS_IN_CONTACT) {
				printf("Error: The new contact detail is too long. \n");
				return;
			}

			strcpy_s(person->contacts[i].contact, MAX_CHARACTERS_IN_CONTACT, updatedContactDetail);
			if (person->contacts[i].type == CT_EMAIL) {
				updateEmailMap(emailToPersonMap, neededChangeContact, updatedContactDetail);
			}
			printf("Contact updated successfully!\n");
			return;
		}
	}
	printf("Contact not found!\n");
	return;

}

void deleteContact(Person* person, EmailToPersonMap* emailToPersonMap) {
	if (!person || !emailToPersonMap) {
		return;
	}

	while (getchar() != '\n');
	
	char needtodeleteContact[50];

	printf("Enter the contact detail you want to delete: ");

	fgets(needtodeleteContact, sizeof(needtodeleteContact), stdin);
	size_t len = strlen(needtodeleteContact);
	if (len > 0 && needtodeleteContact[len - 1] == '\n') {
		needtodeleteContact[len - 1] = '\0';
	}


	for (int i = 0;i < person->num_contacts;i++) {
		if (strcmp(person->contacts[i].contact, needtodeleteContact) == 0) {
			free(person->contacts[i].contact);
			person->contacts[i].contact=NULL;
			if (person->contacts[i].type == CT_EMAIL) {
				deleteEmailMapbyEmail(emailToPersonMap, needtodeleteContact);
			}
			for (int j = i; j < person->num_contacts - 1; j++) {
				person->contacts[j] = person->contacts[j + 1];
			}
			if (person->address_contact_index >= i) {
				person->address_contact_index--;
			}
			if (person->primary_contact_index >= i) {
				person->primary_contact_index--;
			}
			(person->num_contacts)--;
			printf("Contact deleted!\n");
			return;
		}
	}
	printf("Contact not found!\n");
	return;

}

void deletePersonandList(PersonList* personList, EmailToPersonMap* map, Person* person) {
	for (int i = 0;i < personList->num_persons;i++) {
		if (personList->persons[i] == person) {
			for (int c = 0; c < person->num_contacts; ++c) {
				if (person->contacts[i].type == CT_EMAIL) {
					deleteEmailMapbyEmail(map, person->contacts[i].contact);
				}
			}
			destroyPerson(&person);
			for (int j = i; j < (personList->num_persons - 1); j++) {
				personList->persons[j] = personList->persons[j+1];
			}
			personList->num_persons--;
			break;
		}
	}
}

// Function to serialize a single person and their contacts into the CSV format
void serialize_person(CSV_BUFFER* buffer, Person* person) {
	// Add primary person details
	append_row(buffer); // Add a row for the person
	//append_field(buffer, buffer->rows - 1); // Add first name
	set_field(buffer->field[buffer->rows - 1][0], person->first_name);

	append_field(buffer, buffer->rows - 1); // Add last name
	set_field(buffer->field[buffer->rows - 1][1], person->last_name);

	if (person->primary_contact_index > -1) {
		append_field(buffer, buffer->rows - 1); // Add primary contact type
		int type = person->contacts[person->primary_contact_index].type;
		if (type == CT_PHONE) {
			set_field(buffer->field[buffer->rows - 1][2], "Phone");
		}
		else if (type == CT_CELL) {
			set_field(buffer->field[buffer->rows - 1][2], "Cell");
		}
		else {
			set_field(buffer->field[buffer->rows - 1][2], "Email");
		}

		append_field(buffer, buffer->rows - 1); // Add primary contact info
		set_field(buffer->field[buffer->rows - 1][3], person->contacts[person->primary_contact_index].contact);
	}

	if (person->address_contact_index > -1) {
		append_field(buffer, buffer->rows - 1); // Add address contact type
		set_field(buffer->field[buffer->rows - 1][4], "Address");

		append_field(buffer, buffer->rows - 1); // Add address contact info
		set_field(buffer->field[buffer->rows - 1][5], person->contacts[person->address_contact_index].contact);
	}
	

	// Add each contact on a new line
	for (int i = 0; i < person->num_contacts; ++i) {
		append_row(buffer); // Add a row for each contact

		// Add contact type
		int type = person->contacts[i].type;
		if (type == CT_PHONE) {
			set_field(buffer->field[buffer->rows - 1][0], "Phone");
		}
		else if (type == CT_CELL) {
			set_field(buffer->field[buffer->rows - 1][0], "Cell");
		}
		else if (type == CT_EMAIL) {
			set_field(buffer->field[buffer->rows - 1][0], "Email");
		}
		else {
			set_field(buffer->field[buffer->rows - 1][0], "Address");
		}

		// Add contact info
		append_field(buffer, buffer->rows - 1);
		set_field(buffer->field[buffer->rows - 1][1], person->contacts[i].contact);

		// Mark as primary or address if applicable
		const char* marker = "";
		if (i == person->primary_contact_index) {
			marker = "PRIMARY";
		}
		else if (i == person->address_contact_index) {
			marker = "ADDRESS";
		}

		// Add marker (PRIMARY/ADDRESS)
		append_field(buffer, buffer->rows - 1);
		set_field(buffer->field[buffer->rows - 1][2], marker);
	}
}

// Function to serialize the entire person list into a CSV file
int serialize_person_list_to_csv(PersonList* person_list, const char* file_name) {
	CSV_BUFFER* buffer = csv_create_buffer();
	if (!buffer) {
		perror("Error creating CSV buffer");
		return 0;
	}

	// Iterate over the person list and serialize each person
	for (int i = 0; i < person_list->num_persons; ++i) {
		serialize_person(buffer, person_list->persons[i]);
	}

	// Save the CSV buffer to the file
	int result = csv_save(file_name, buffer);
	if (result != 0) {
		perror("Error saving CSV file");
	}

	// Destroy the buffer after use
	csv_destroy_buffer(buffer);
	return 1;
}


// Deserialization
void deserialize_persons(FILE* file, PersonList* person_list, EmailToPersonMap *map) {
	if (!file || !person_list || !map) {
		perror("Invalid input!");
		return;
	}

	char line[1024];
	if (!line) {
		perror("Failed to allocate memory");
		return;
	}

	int line_number = 0;
	Person* current_person = NULL;
	char* context;

	while (fgets(line, sizeof(line), file)) { // read a line from file
		line_number++;
		char* token = strtok_s(line, ",",&context);
				
		char* primaryContact = "";
		char* primaryAddress = "";

		if (strcmp(token, "Email") != 0 && strcmp(token, "Phone") != 0 && 
			strcmp(token, "Cell") != 0 && strcmp(token, "Address") != 0 && !token) {
			// Create a new person object for every new person's line
			current_person = (Person*)malloc(sizeof(Person));
			if (!current_person) {
				printf("Memory allocation failed\n");
				return 1;
			}

			memset(current_person, 0, sizeof(Person));

			// Parse the person's details (First Name, Last Name, Primary Contact, Address Contact)
			strncpy_s(current_person->first_name,sizeof(current_person->first_name), token, MAX_CHARACTERS_IN_NAME);
			token = strtok_s(NULL, ",", &context);
			strncpy_s(current_person->last_name,sizeof(current_person->last_name), token, MAX_CHARACTERS_IN_NAME);
			token = strtok_s(NULL, ",", &context);

			// Parse primary contact (Type, Contact Info)
			if (token) {
				token = strtok_s(NULL, ",", &context);
				primaryContact = _strdup(token);
			}
			else {
				current_person->primary_contact_index = -1;
				strtok_s(NULL, ",", &context);  // Skip the next "NULL"
			}

			// Parse address contact (Type, Contact Info)
			token = strtok_s(NULL, ",", &context);
			if (token) {
				token = strtok_s(NULL, ",", &context);
				primaryAddress = _strdup(token);
			}
			else {
				current_person->address_contact_index = -1;
				strtok_s(NULL, ",", &context);  // Skip the next "NULL"
			}

			// Add the person to the list
			addPersonToList(person_list, current_person);
		}
		else {
			// Parse the contact lines
			if (current_person != NULL) {
				ContactType type = stringToContactType(token);
				token = strtok_s(NULL, ",", &context);

				// determine if the current contact is primary or address
				if (primaryContact && strcmp(token, primaryContact) == 0) {
					addContactToPerson(current_person, type, token, true);
				}
				else if (primaryAddress && strcmp(token, primaryAddress) == 0) {
					addContactToPerson(current_person, type, token, true);
				}
				else {
					addContactToPerson(current_person, type, token, false);
				}

				// add to email to person map
				if (type == CT_EMAIL) {
					addToEmailMap(map, token, current_person);
				}
			}
			else {
				printf("Invalid input: empty CSV file.\n");
				break;
			}
		}
	}
}