#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 700

#define IS_ERROR 0
#define IS_OK 1

typedef struct
{
	int year;
	int month;
	int day;
}date;

typedef struct
{
	char *longitude;
	char *latitude;
}location;

typedef struct incList
{
	char* area;
	date reported;
	int missing;
	int women;
	int men;
	int children;
	char* cause_of_death;
	char* location_description;
	location coordinates;
	char* url;

	struct incList* next;
}incident;

char* copy_string(char* source) {
	char* copy_str = (char*)malloc(sizeof(char) * (strlen(source) + 1));
	if (copy_str == NULL) {
		printf("malloc error\n");
		exit(1);
	}

	strcpy(copy_str, source);

	return copy_str;
}
void delete_node(incident* node){
	if (node == NULL) return;

	free(node->area);
	free(node->cause_of_death);
	free(node->location_description);

	free(node);
}

void free_list(incident* head) {
	incident* node = head;

	if (head == NULL) return;

	if (head->next == head) {
		delete_node(head);
		return;
	}

	// prepei na kano tin lista grammiki (na spaso ton kiklo)
	head = head->next;

	node->next = NULL; // spaei o kiklos edo

	// diagrafi san nan einai kiliki i lista
	while (head != NULL) {
		node = head->next;

		delete_node(head);

		head = node;
	}
}


incident* get_node_with_data(char* area, char* day, char* month, char* year, char* missing, char* women, char* men, char* children, 
							 char* cause_of_death, char* location_description, char* longitude, char* latitude, char* url) 
{
	incident* newNode = (incident*)malloc(sizeof(incident));
	if (newNode == NULL) {
		printf("malloc error, cannot continue!\n");
		exit(1);
	}

	// apothikefsi dedomenon ston neo komvo
	newNode->area = copy_string(area);
	newNode->reported.day = atoi(day);
	newNode->reported.month = atoi(month);
	newNode->reported.year = atoi(year);
	newNode->missing = atoi(missing);
	newNode->women = atoi(women);
	newNode->children = atoi(children);
	newNode->men = atoi(men);
	newNode->cause_of_death = copy_string(cause_of_death);
	newNode->location_description = copy_string(location_description);
	newNode->coordinates.longitude = copy_string(longitude);
	newNode->coordinates.latitude = copy_string(latitude);
	newNode->url = copy_string(url);

	newNode->next = newNode;

	return newNode;
}

int check_data(char* area, char* day, char* month, char* year, char* missing, char* women, char* men, char* children,
	char* cause_of_death, char* location_description, char* longitude, char* latitude, char* url)
{
	int i, length;
	// den iparxei elegxos gia to area

	// to day prepei na einai dipsifio mesa sta oria
	if (strlen(day) != 2 || atoi(day) < 1 || atoi(day) > 31) {
		printf("Wrong day!\n");
		return IS_ERROR;
	}

	// to month prepei na einai dipsifio mesa sta oria
	if (strlen(month) != 2 || atoi(month) < 1 || atoi(month) > 31) {
		printf("Wrong month!\n");
		return IS_ERROR;
	}

	// to year prepei na einai tatrapsifio mesa sta oria
	if (strlen(year) != 4 || atoi(year) < 1900 || atoi(year) > 2100) {
		printf("Wrong year!\n");
		return IS_ERROR;
	}

	// to missing prepei na einai thetiko
	if (atoi(missing) < 0) {
		printf("Missing can not be negative!\n");
		return IS_ERROR;
	}

	// to idio gia men, children kai women
	if (atoi(women) < 0) {
		printf("Dead women can not be negative!\n");
		return IS_ERROR;
	}

	if (atoi(men) < 0) {
		printf("Dead men can not be negative!\n");
		return IS_ERROR;
	}

	if (atoi(children) < 0) {
		printf("Dead children can not be negative!\n");
		return IS_ERROR;
	}

	// aitia thanatou xoris psifia
	length = strlen(cause_of_death);

	for (i = 0; i < length; i++) {
		if (isdigit(cause_of_death[i]) != 0) {
			printf("Character %c in cause of death is not accepted!", cause_of_death[i]);
			return IS_ERROR;
		}
	}

	// topothesia xoris elegxo

	// sidetagmenes mesa sta oria
	if (atof(latitude) < -90 || atof(latitude) > 90) {
		printf("Wrong value for latitude\n");
		return IS_ERROR;
	}

	if (atof(longitude) < -180 || atof(longitude) > 180) {
		printf("Wrong value for longitude\n");
		return IS_ERROR;
	}

	// den kano elegxo gia to url

	return IS_OK;
}

incident* insert2tail(incident* head, char user_input[SIZE]) {
	incident* newNode = NULL;
	incident* tail = NULL;
	char* area, * day, * month, * year, * missing, * women, * men, * children, * cause_of_death, * location_description, * longitude, * latitude, * url;

	// eksago to kommati tou area apo to user input
	area = strtok(user_input, ";");

	// eksago kommati imeras (sinexizo to kopsimo apo ekei pou eimoun
	day = strtok(NULL, "/");

	// minas
	month = strtok(NULL, "/");

	// etos
	year = strtok(NULL, ";");

	//missing
	missing = strtok(NULL, ";");

	// women
	women = strtok(NULL, ";");

	// men
	men = strtok(NULL, ";");

	children = strtok(NULL, ";");

	cause_of_death = strtok(NULL, ";");

	location_description = strtok(NULL, ";");

	latitude = strtok(NULL, ",");

	longitude = strtok(NULL, ";");

	url = strtok(NULL, ";");

	if (check_data(area, day, month, year, missing, women, men, children, cause_of_death, location_description, longitude, latitude, url) == IS_ERROR) {
		printf("No data added to list\n");
		return head;
	}

	// prepei ta dedomena na boun se enan neo komvo
	newNode = get_node_with_data(area, day, month, year, missing, women, men, children, cause_of_death, location_description, longitude, latitude, url);

	// o neos komvos prepei na bei sto telos tis listas
	// 1. den iparxei kanenas komvoa sti lista
	if (head == NULL) {
		printf("New node is head of list\n");

		// o neos komvos einai i kefali tis listas
		return newNode;
	}
	// 2. i lista exei mono 1 komvo (tin kefali)
	else if (head->next == head) {
		// o neos komvos tha bei meta tin kefali
		head->next = newNode;
		newNode->next = head;

		printf("New node is second node of list\n");

		return head;
	}
	else {
		// iparxoun perissoteroi komboi sti lista
		// prepei na bro ton teleftaio komvo
		tail = head;

		while (tail->next != head) tail = tail->next;

		// o neos komvos bainei sto telos tis listas
		tail->next = newNode;
		newNode->next = head;

		printf("New node is added at the end of list\n");

		return head;
	}
}

int save(incident* head, char filename[SIZE]) {
	incident* node = head;
	FILE* f = fopen(filename, "w");
	if (f == NULL) {
		printf("fopen error\n");
		return IS_ERROR;
	}

	do {
		fprintf(f, "%35s;%02d/%02d/%4d;%2d;%2d;%2d;%2d;%35s;%35s;%18s,%18s;%30s",
			node->area, node->reported.day, node->reported.month, node->reported.year, 
			node->missing, node->women, node->men, node->children,
			node->cause_of_death, node->location_description,
			node->coordinates.latitude, node->coordinates.longitude, node->url);
		node = node->next;
	} while (node != head);

	fclose(f);

	return IS_OK;
}

void print_after_date(incident* head, char* date_str) {
	incident* node = head;
	char* day_str, * month_str, *year_str;
	int d, m, y;

	// prepei na vro imera, mina kai etos tou xristi
	day_str = strtok(date_str, "/");
	d = atoi(day_str);

	month_str = strtok(NULL, "/");
	m = atoi(month_str);

	year_str = strtok(NULL, "/");
	y = atoi(year_str);

	do {
		if (node->reported.year > y || 
			(node->reported.year == y && node->reported.month > m) ||
			(node->reported.year == y && node->reported.month == m && node->reported.day > d))
		{
			printf("%35s;%02d/%02d/%4d;%2d;%2d;%2d;%2d;%35s;%35s;%18s,%18s;%30s\n",
				node->area, node->reported.day, node->reported.month, node->reported.year,
				node->missing, node->women, node->men, node->children,
				node->cause_of_death, node->location_description,
				node->coordinates.latitude, node->coordinates.longitude, node->url);				
		}
		node = node->next;
	} while (node != head);
}

void print_before_date(incident* head, char* date_str) {
	incident* node = head;
	char* day_str, * month_str, * year_str;
	int d, m, y;

	// prepei na vro imera, mina kai etos tou xristi
	day_str = strtok(date_str, "/");
	d = atoi(day_str);

	month_str = strtok(NULL, "/");
	m = atoi(month_str);

	year_str = strtok(NULL, "/");
	y = atoi(year_str);

	do {
		if (node->reported.year < y ||
			(node->reported.year == y && node->reported.month < m) ||
			(node->reported.year == y && node->reported.month == m && node->reported.day < d))
		{
			printf("%35s;%02d/%02d/%4d;%2d;%2d;%2d;%2d;%35s;%35s;%18s,%18s;%30s\n",
				node->area, node->reported.day, node->reported.month, node->reported.year,
				node->missing, node->women, node->men, node->children,
				node->cause_of_death, node->location_description,
				node->coordinates.latitude, node->coordinates.longitude, node->url);
		}
		node = node->next;
	} while (node != head);
}

// o palaioteros einai to head, pou prepei na to sviso
incident* delete_oldest_node(incident* head) {
	incident* tail = NULL;
	incident* after_head = NULL;

	// an einai enas komvos prepei na ton diagrapso
	if (head->next == head) {
		delete_node(head);
		printf("Head of list deleted!\n");
		return NULL;
	}


	// prepei na vro to tail kai ton deftero komvo meta to head
	tail = head;

	while (tail->next != head) {
		tail = tail->next;
	}

	after_head = head->next;

	// diagrafo ton proto komvo
	delete_node(head);

	// o teleftaios prepei na deixnei sto deftero
	tail->next = after_head;

	// tora o defteros komvos einai i nea kefali tis listas
	head = after_head;

	printf("First node deleted\n");

	return head;
}

// o neoteros vrisketai stin oura tis listas, pou prepei na diagrapso
incident* delete_newest_node(incident* head) {
	incident* tail = NULL;
	incident* before_tail = NULL;

	// an einai enas komvos prepei na ton diagrapso
	if (head->next == head) {
		delete_node(head);
		printf("Head of list deleted!\n");
		return NULL;
	}


	// prepei na vro to tail kai ton proigoumeno komvo tou
	tail = head;

	while (tail->next != head) {
		before_tail = tail;
		tail = tail->next;
	}

	// diagrafo ton teleftaio komvo
	delete_node(tail);

	// o proteleftaios prepei na deixnei sto head
	before_tail->next = head;

	printf("Last node deleted\n");

	return head;
}


int main() {
	incident* head = NULL;
	char user_input[SIZE];

	do {
		printf("$> ");
		scanf("%s", user_input);

		if (strcmp(user_input, "newIncident") == 0) {
			fgets(user_input, SIZE, stdin);
			head = insert2tail(head, user_input);
		}
		else if (strcmp(user_input, "save") == 0) {
			scanf("%s", user_input);

			if (head == NULL) {
				printf("List is empty, nothing to save!\n");
			}
			else if (save(head, user_input) == IS_OK){
				printf("List saved to file\n");
			}
			else
			{
				printf("No data is saved!\n");
			}
			
		}
		else if (strcmp(user_input, "printA") == 0) {
			scanf("%s", user_input);

			if (head == NULL) {
				printf("List is empty, nothing to print!\n");
			}
			else {
				print_after_date(head, user_input);
			}
		}
		else if (strcmp(user_input, "printB") == 0) {
			scanf("%s", user_input);

			if (head == NULL) {
				printf("List is empty, nothing to print!\n");
			}
			else {
				print_before_date(head, user_input);
			}
		}
		else if (strcmp(user_input, "deleteN") == 0) {
			
			if (head == NULL) {
				printf("List is empty, nothing to delete!\n");
			}
			else {
				head = delete_newest_node(head);
			}
		}
		else if (strcmp(user_input, "deleteO") == 0) {

			if (head == NULL) {
				printf("List is empty, nothing to delete!\n");
			}
			else {
				head = delete_oldest_node(head);
			}
		}

	} while (strcmp(user_input, "exit") != 0);

	free_list(head);

	return 0;
}
