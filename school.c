#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "school.h"

int create_num() {
	char temp;
	int num = 0;
	flushall();
	while ((temp = getchar()) != ' ' && temp != '\n' && temp != '\r') {
		if (temp >= '0' && temp <= '9')	{
			num *= 10;
			num += temp - '0';
		}
	}
	return num;
}

char yOn() {
	char answer = 0;
	while (!(answer == 'y' || answer == 'Y')) {
		flushall();
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			return 'n';
	}
	return 'y';
}

char again() {
	printf("Again (y/n)?\n");
	return yOn();
}

int is_str_equall(char * name, char * string) {
	char nm, st;
	if (!string)
		return 0;
	while(*name && *string) {
		nm = *name++; 
		st = *string++;
		if (nm >= 'a' && nm <= 'z') 
			nm -= 32;
		if (st >= 'a' && st <= 'z')
			st -= 32;
		if (nm != st)
			return 0;
	}
	if (*name == *string && *string == 0)
		return 1;
	return 0;
}

void name_print(char * name) {
	HANDLE  hConsole;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	SetConsoleTextAttribute(hConsole, CYAN);
	printf("'%s'", name);
	SetConsoleTextAttribute(hConsole, csbi.wAttributes);
}

pCls_t make_cls(data_item data) {
	pCls_t nNode = NULL;
	nNode = (pCls_t)calloc(1, sizeof(Cls_t));
	if (!nNode) {
		printf("Failed allocation");
		return NULL;
	}
	nNode->type_t = CLS;
	nNode->clsNum = data;
	return nNode;
}

pStn_t make_student(char name[]) {
	pStn_t nNode = NULL;
	nNode = (pStn_t)calloc(1, sizeof(Stn_t));
	if (!nNode) {
		printf("Failed allocation \n");
		return NULL;
	}
	nNode->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nNode->name, name);
	return nNode;
}

pGrd_t make_grade(int gradeId, int grd ) {
	pGrd_t nNode = NULL;
	nNode = (pGrd_t)calloc(1, sizeof(Grd_t));
	if (!nNode) {
		printf("Failed allocation \n");
		return NULL;
	}
	nNode->lsn = gradeId;
	nNode->grade = grd;
	return nNode;
}

int add_first_student(pStn_t *head, char name[]) {
	pStn_t nNode = make_student(name);
	if (!nNode) {
		printf("Failed allocation \n");
		return 1;
	}
	if (!head) {
		printf("Missing pointer \n");
		return 2;
	}
	nNode->next = *head;
	*head = nNode;
	return 0;
}

int add_first_grade(pGrd_t *head, int gradeId, int grd) {
	pGrd_t nNode = make_grade(gradeId, grd);
	if (!nNode) {
		printf("Failed allocation \n");
		return 1;
	}
	if (!head) {
		printf("Missing pointer \n");
		return 2;
	}
	nNode->next = *head;
	*head = nNode;
	return 0;
}

int add_last_grade(pGrd_t *head, pGrd_t last, int gradeId, int grd) {
	pGrd_t nNode = make_grade(gradeId, grd);
	if (!nNode) {
		printf("Failed allocation \n");
		return 1;
	}
	if (!last) {
		*head = nNode;
		return 0;
	}
	last->next = nNode;
	return 0;
}

int find_stn(pStn_t * stn) {
	char name[NAME];
	if (!(stn && *stn)) {
		printf("No students \n");
		return 2;
	}
	printf("Student name: ");
	flushall();
	gets(name);
	while (!(is_str_equall(name, (*stn)->name))) {
		if (!(*stn = (*stn)->next)) {
			printf("Name not found \n");
			return 1;
		}
	}
	return 0;
}

int add_student(pStn_t *head) {
	pStn_t ptr;
	char name[100];
	if (!head) {
		printf("No students \n");
		return 2;
	}
	ptr = *head;
	printf("Student name: ");
	flushall();
	gets(name);
	while (ptr) {
		if (is_str_equall(name, ptr->name)) {
			printf("The name exsist \n");
			return 1;
		}
		ptr = ptr->next;
	}
	return add_first_student(head, name);
}

int change_grd(pStn_t stn, pGrd_t * last, int lsn, int grd) {
	pGrd_t prv = stn->to_grade, ptr = stn->to_grade;
	while (ptr && ptr->lsn != lsn) {
		prv = ptr;
		ptr = ptr->next;
	}
	if (ptr) {
		printf("The lesson exist \n");
		printf("Would you like to change it (y/n)? ");
		if (yOn() == 'y') ptr->grade = grd;
		return 1;
	}
	else *last = prv;
	return 0;
}

int add_lsn(pStn_t stn) {
	int lsn, grd, func;
	pGrd_t noUseVar = NULL;
	if (!stn) {
		printf("No students \n");
		return 2;
	}
	printf("Enter lesson ID: ");
	lsn = create_num();
	while (stn) {
		printf("Enter grade for ");
		name_print(stn->name); 
		printf(" (15 - 110): ");
		do { grd = create_num(); } while (grd < 15 || grd > 110);
		if (!(change_grd(stn, &noUseVar, lsn, grd))) {
			if (func = add_first_grade(&stn->to_grade, lsn, grd))
				return func; 
		}
		stn = stn->next;
	}
	return 0;
}

int add_stn_lsn(pStn_t stn) {
	int lsn, grd, func;
	pGrd_t last = NULL;
	if (!stn) {
		printf("No students \n");
		return 2;
	}
	if (func = find_stn(&stn)) return func;
	printf("Enter lesson ID: ");
	lsn = create_num();
	printf("Enter grade (15 - 110): ");
	do { grd = create_num(); } while (grd < 15 || grd > 110);
	if (!(change_grd(stn, &last, lsn, grd)))
		return add_last_grade(&stn->to_grade, last, lsn, grd);
	return 0;
}

int del_stn(pStn_t * head) {
	pStn_t ptr, prv;
	char name[100];
	if (!(head && *head)) {
		printf("No students \n");
		return 2;
	}
	printf("Student name: ");
	flushall();
	gets(name);
	ptr = prv = *head;
	if (is_str_equall(name, (*head)->name)) {
		*head = (*head)->next;
		free(ptr->name);
		free(ptr);
		return 0;
	}
	while (ptr && !is_str_equall(name, ptr->name)) {
		prv = ptr;
		ptr = ptr->next;
	}
	if (ptr) {
		prv->next = ptr->next;
		free(ptr->name);
		free(ptr);
		return 0;
	}
	printf("Name not found \n");
	return 1;
}

int del_lsn(pStn_t stn) {
	pGrd_t ptr, prv;
	int lsn, c = 0;
	if (!stn) {
		printf("No students \n");
		return 2;
	}
	printf("Enter lesson ID: ");
	lsn = create_num();
	while (stn) {
		if ((ptr = stn->to_grade) && ptr->lsn == lsn) {
			stn->to_grade = ptr->next;
			free(ptr);
			c++;
		}
		else {
			while (ptr && lsn != ptr->lsn) {
				prv = ptr;
				ptr = ptr->next;
			}
			if (ptr) {
				prv->next = ptr->next;
				free(ptr);
				c++;
			} 
		}
		stn = stn->next;
	}
	if (c)
		return 0;
	printf("Lesson not found \n");
	return 1;
}

int del_stn_lsn(pStn_t stn) {
	pGrd_t ptr, prv;
	int lsn, func;
	if (!stn) {
		printf("No students \n");
		return 2;
	}
	if (func = find_stn(&stn))
		return func;

	if (!stn->to_grade) {
		printf("Result sheet is empty \n");
		return 2;
	}
	while (1) {
		printf("Enter lesson ID: ");
		lsn = create_num();
		if ((ptr = stn->to_grade) && ptr->lsn == lsn) {
			stn->to_grade = ptr->next;
			free(ptr);
			return 0;
		}
		while (ptr) {
			if (ptr->lsn == lsn) {
				prv->next = ptr->next;
				free(ptr);
				return 0;
			}
			prv = ptr;
			ptr = ptr->next;
		}
		printf("Lesson ID not found \n");
		if(again() == 'n') return 2;
	}
	return 2;
}

void lprnt_cls(void * Cls, void * null) {
	pStn_t stn; pGrd_t ptr; pCls_t cls = (pCls_t)Cls;
	if (!cls) { printf("Create school first \n"); return; }
	if (cls->type_t != CLS) { printf("Init error \n"); return; }
	if (!(stn = cls->student)) { printf("No students \n"); return; }
	printf("\t Class %d, students %d: \n", cls->clsNum, cls->amountOfStudents);
	while (stn) {
		printf("\t");
		name_print(stn->name);
		printf(" grades: \n");
		if (!(ptr = stn->to_grade)) printf("\t Results sheet is empty ");
		while (ptr) {
			printf("Lesson ID - %d: %d\t", ptr->lsn, ptr->grade);
			ptr = ptr->next;
		}
		printf("\n");
		stn = stn->next;
	}
}

void lprnt_stn_grds(void * Cls, void * null) {
	pGrd_t ptr; pStn_t stn;
	if (!Cls) { printf("Create school first \n"); return; }
	if (((pCls_t)Cls)->type_t != CLS) { printf("Init error \n"); return; }
	stn = ((pCls_t)Cls)->student;
	if (find_stn(&stn)) return;
	if (!(ptr = stn->to_grade)) { printf("Results sheet is empty "); return; }
	while (ptr) {
		printf("Lesson ID - %d: %d\t", ptr->lsn, ptr->grade);
		ptr = ptr->next;
	}
	printf("\n");
}
