#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "csv.h"


#define MAX_CHARACTERS_IN_NAME 40
#define MAX_CHARACTERS_IN_CONTACT 200
#define MAX_CONTACTS_PER_PERSON 10
#define MAX_CONTACT_TYPE_LENGTH 20
#define MAX_CONTACT_INFO_LENGTH 200
#define MAX_PERSONS 100

/// <summary>
/// Contact type for contact info. Enum to specify the nature of the contact
/// info.
/// </summary>
typedef enum contact_type {
	CT_NONE,
	CT_PHONE,
	CT_CELL,
	CT_EMAIL,
	CT_ADDRESS,
	CT_UNKNOWN,
	CT_COUNT
} ContactType;

//typedefs
typedef struct contact_s Contact;
typedef struct person_s Person;
typedef struct person_list_s PersonList;
typedef struct email_to_person_map_s EmailToPersonMap;

PersonList* createPersonList();
EmailToPersonMap* createMap();

//add person to list
int addPersonToList(PersonList* list, Person* person);


/// <param name="out">File to send the output to.</param>
/// <param name="contact">Pointer to the contact you want to print</param>
Person* createNewPerson(char const* first_name, char const* last_name);

/// <param name="out">The file to send the output to</param>
/// <param name="person">A pointer to the person whose details you want to print</param>
/// <param name="lastNameFirst">If true, prints contact name as "Lastname, Firstname", otherwise prints as "Firstname Lastname"</param>
void printPerson(FILE* out, Person* person, bool lastNameFirst);

void printPersonList(FILE* out, PersonList* list);


/// <returns>0 on failure, otherwise returns the number of contacts stored for person</returns>
int addContactToPerson(Person* person, ContactType ct, char const* contact_info, bool setAsPrimaryContact);


/// <param name="person"></param>
void destroyPerson(Person** person);

//Serialization function prototype
int serializePersonToCSV(FILE* outfile, PersonList* list);

// Deserialization function prototype
Person* deserializePersonFromCSV(FILE* in);

//Add Email to the Map
void addToEmailMap(EmailToPersonMap* map, const char* email, Person* person);

//Initialize the Map
void initializeEmailMap(EmailToPersonMap* map);

//find person by email
Person* findPersonByEmail(EmailToPersonMap* map, const char* email);

ContactType stringToContactType(char const* string);
char const* contactTypeToString(ContactType ct);

int getNumContacts(Person* person);
Contact* getContacts(Person* person, int index);
void plusPlusNumcontacts(Person* person);
ContactType getType(Contact* contact);
char* getContact(Contact* contact);
void setType(Contact* contact, ContactType newType);
void setContact(Contact* contact, char* newContact);
int getNumPersons(PersonList* personList);
char* getPersonFirstName(PersonList* personList, int index);
Person* getPerson(PersonList* personList, int index);
void updateEmailMap(EmailToPersonMap* map, const char* oldEmail, const char* newEmail);
void deleteEmailMapbyEmail(EmailToPersonMap* map, const char* email);
void setPerson(PersonList* personList, int index, Person* person);
void MinusNumPersons(PersonList* personList);
char* getFirstName(Person* p);

int createNewPersonToList(PersonList* list, EmailToPersonMap* emailToPersonMap);
void inputContact(Person* person, EmailToPersonMap* emailToPersonMap);
Person* searchPersonByName(PersonList* personList, char const* firstName);
void editContact(Person* person, EmailToPersonMap* emailToPersonMap);
void deleteContact(Person* person, EmailToPersonMap* emailToPersonMap);
void deletePersonandList(PersonList* personList, EmailToPersonMap* map, Person* person);

void deleteAll(PersonList* list, EmailToPersonMap* map);

void clearPersonList(PersonList* list);

void serialize_person(CSV_BUFFER* buffer, Person* person);
int serialize_person_list_to_csv(PersonList* person_list, const char* file_name);
void deserialize_persons(FILE* file, PersonList* person_list, EmailToPersonMap* map);

void clearEmailToPersonList(EmailToPersonMap* map);
void clearAll(PersonList* list, EmailToPersonMap* map);