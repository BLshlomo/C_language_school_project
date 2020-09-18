#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "shlomo.h"

/*
Ask Meny: 
- About adding a new NULL node just for saving because i already use add to last
*/

// atexit pointer
//pCls_t *school = NULL; 
//int size = 0;

void exit_failed_alc();
void save_file(void * School, void * Size);
void ext(void * School, void * Size);
void create_school(void * School, void * Size);
void load_file(void * School, void * Size);
int select_class(int size);
void wraper_force_school(pCls_t ** school, int * size);
void wraper_stn(void * Cls, void * addOrDelChar);
void wraper_grade (void * Cls, void * addOrDel);
static void _PrintMenu();
void gotoxy(int x, int y);
void gui_display();
void menu_func(pCls_t ** school, int * size);

/*
void wraper_ext() {
ext(&school, &size);
}
*/

Menu_item menu_array[] = {
	{ "\t    MENU	\n\n", NULL },
	{ "\t 1. Add student \n", wraper_stn, select_class, 'a' },
	{ "\t 2. Add grade \n", wraper_grade, select_class, 'a' },
	{ "\t 3. Remove student \n", wraper_stn, select_class, 'd' },
	{ "\t 4. Remove grade \n", wraper_grade, select_class, 'd' },
	{ "\t 5. Show class grades \n", lprnt_cls, select_class },
	{ "\t 6. Show student grades \n", lprnt_stn_grds, select_class },
	{ "\t 7. Add class \n", create_school },
	{ "\t 8. Save school to file \n", save_file },
	{ "\t 9. Load school from file \n", load_file },
	{ "\t 10. Exit \n", ext },
	{ NULL	, NULL }
};

int main() {
	pCls_t *school = NULL; 
	int size = 0;
	menu_func(&school, &size);
	//atexit(wraper_ext);
	if (school)
		ext((void *)&school, &size);
	system("pause");
	return 0;
}

void exit_failed_alc() {
	printf("Failed allocation \n");
	system("pause");
	exit(1);
}

void save_file(void * School, void * Size) {
	int ln, tr = 1, fl = 0, size = *(int*)Size;
	pCls_t *cls;
	pStn_t stn;
	pGrd_t grd;
	FILE * f = fopen("Shlomo.bin", "wb");
	if (!f) {
		printf("Cant edit the file \n");
		return;
	}
	if (!((cls = *(pCls_t**)School) && (*cls)->type_t == CLS)) {
		printf("School dosent exist \n");
		return;
	}
	fwrite(&size, sizeof(int), 1, f);
	while (*cls) {
		stn = (*cls)->student;
		fwrite(&(*cls)->clsNum, sizeof((*cls)->clsNum), 2, f);
		while (stn) {
			grd = stn->to_grade;
			ln = strlen(stn->name) + 1;
			fwrite(&ln, sizeof(int), 1, f);
			fwrite(stn->name, sizeof(*stn->name), ln, f);
			while (grd) {
				fwrite(&tr, sizeof(int), 1, f);
				fwrite(grd, sizeof(grd->grade), 2, f);
				grd = grd->next;
			}
			fwrite(&fl, sizeof(int), 1, f);
			stn = stn->next;
		}
		cls++;
	}
	fclose(f);
}

void ext(void * School, void * Size) {
	int *size = (int*)Size;
	pCls_t **school = (pCls_t**)School, *cls = *school;
	pStn_t stn, sDel;
	pGrd_t grd, gDel;
	if (!cls) return;
	printf("Would you like to save to file first (y/n)? ");
	if (yOn() == 'y') save_file(school, size);
	while (*cls) {
		stn = sDel = (*cls)->student;
		while (stn) {
			grd = gDel = stn->to_grade;
			while (grd) {
				grd = grd->next;
				free(gDel);
				gDel = grd; 
			}
			stn = stn->next;
			free(sDel->name);
			free(sDel);
			sDel = stn;
		}
		free(*cls++);
	}
	free(*school);
	*school = NULL;
	*size = 0;
}

void create_school(void * School, void * Size) {
	pCls_t *cls, **school = (pCls_t**)School;
	int c = 1, *size = (int*)Size;
	if (school && *school) {
		cls = (pCls_t *)realloc(*school, (++*size + 1) * sizeof(pCls_t));
		if (!cls) {
			printf("Alocation failed \n");
			return;
		}
		*school = cls;
		*(*school + *size) = NULL;
		*(*school + (*size - 1)) = make_cls(*size);
	}
	else {
		printf("Enter amount of classes: ");
		while (*size < 1)
			*size = create_num();
		*school = cls = (pCls_t *)calloc(*size + 1, sizeof(pCls_t));
		if (!school) exit_failed_alc();
		while (c <= *size) {
			*cls++ = make_cls(c++);
		}
	}
}

void load_file(void * School, void * Size) {
	int ln, flag = 0, *size = (int*)Size, i;
	pCls_t * cls, **school = (pCls_t**)School;
	pStn_t nNode, stn;
	pGrd_t nGrd, grd;
	FILE * f = fopen("Shlomo.bin", "rb");
	if (!f) {
		printf("File not found in the app directory \n");
		system("pause");
		exit(1);
	}
	if (*school && (**school)->type_t == CLS) {
		printf("Overwrite existing data ? ");
		if (yOn() == 'n') return;
		ext(school, size);
	}
	fread(size, sizeof(int), 1, f);
	if (*size < 1) { 
		printf("Error \n"); 
		return; 
	}
	*school = cls = (pCls_t *)calloc(*size + 1, sizeof(pCls_t));
	if (!*school) exit_failed_alc();
	while (cls != *school + *size) {
		*cls = (pCls_t)calloc(sizeof(Cls_t), 1);
		if (!*cls) exit_failed_alc();
		(*cls)->type_t = CLS;
		fread(&(*cls)->clsNum, sizeof((*cls)->clsNum), 2, f);
		for (i = 0; i < (*cls)->amountOfStudents; i++) {
			nNode = (pStn_t)calloc(sizeof(Stn_t), 1);
			if (!nNode) exit_failed_alc();
			fread(&ln, sizeof(int), 1, f);
			nNode->name = (char*)calloc(ln, sizeof(char));
			fread(nNode->name, sizeof(*stn->name), ln, f);
			fread(&flag, sizeof(int), 1, f);
			while (flag) {
				nGrd = (pGrd_t)calloc(sizeof(Grd_t), 1);
				if (!nGrd) exit_failed_alc();
				fread(nGrd, sizeof(nGrd->grade), 2, f);
				if (!nNode->to_grade)
					nNode->to_grade = grd = nGrd;
				else {
					grd->next = nGrd;
					grd = grd->next;
				}
				fread(&flag, sizeof(int), 1, f);
			}
			if (!(*cls)->student)
				(*cls)->student = stn = nNode;
			else {
				stn->next = nNode;
				stn = stn->next;
			}
		}
		cls++;
	}
	fclose(f);
}

int select_class(int size) {
	int cls = 0;
	printf("Select class (1 - %d): ", size);
	while (cls < 1 || cls > size)
		cls = create_num();
	return cls - 1;
}

void wraper_force_school(pCls_t ** school, int * size) {
	char answer = 0;
	printf("\t 1. Create class \n"
		"\t 2. Load from file \n"
		"\t 3. Cancel \n"
		"Select number: ");
	while (answer < '1' || answer > '3') {
		flushall();
		scanf(" %c", &answer);
	}
	switch (answer) {
	case '1': create_school((void *)school, size); break;
	case '2': load_file((void *)school, size); break;
	case '3': system("pause"); exit (0);
	}
}

void wraper_stn(void * Cls, void * addOrDelChar) {
	pCls_t cls = (pCls_t)Cls;
	char flag = *(char*)addOrDelChar;
	if (!(cls && flag)) { printf("School is missing \n"); return; }
	if (cls->type_t != CLS) { printf("Init error \n"); return; }
	switch (flag) {
	case 'a': if (!add_student(&cls->student)) cls->amountOfStudents++; break;
	case 'd': if (!del_stn(&cls->student)) cls->amountOfStudents--; break;
	default: printf("Init error \n");
	}

}

void wraper_grade (void * Cls, void * addOrDel) {
	pCls_t cls = (pCls_t)Cls;
	char slct = 0, flag = *(char*)addOrDel;
	int func = 1;
	if (!(cls && flag)) { printf("School is missing \n"); return; }
	if (cls->type_t != CLS) { printf("Init error \n"); return; }
	if (!cls->student) { printf("Add student first \n"); return; }
	printf("\t 1. For class \n\t 2. For student \n\t 3. Cancel \nSelect number: ");
	while (slct < '1' || slct > '3') {
		flushall();
		scanf("%c", &slct);
	}
	while (func == 1) {
		switch (slct - 48) {
		case 1: switch (flag) {
		case 'a': func = add_lsn(cls->student); break;
		case 'd': func = del_lsn(cls->student); break;
				}
				break;
		case 2: switch (flag) {
		case 'a': func = add_stn_lsn(cls->student); break;
		case 'd': func = del_stn_lsn(cls->student); break;
				}
				break;
		case 3: return;
		}
		if (func == 1 && again() == 'n')
			break;
	}
}

static void _PrintMenu() {
	pMenu_item pMenuItem = menu_array;
	while (pMenuItem->description != NULL) {
		printf("%s", pMenuItem->description);
		pMenuItem++;
	}
}

void gotoxy(int x, int y){ // using microsoft include windows.h
	COORD coord; // coordinates
	coord.X = x; coord.Y = y; // X and Y coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void gui_display() {
	static int flag = 0;
	int menLen; // raws;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE  hConsole;
	COORD coord;
	DWORD count;
	menLen = sizeof(menu_array) / sizeof(menu_array[0]);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	SetConsoleTextAttribute(hConsole, 1134);
	//raws = csbi.srWindow.Bottom - csbi.srWindow.Top - 1;
	if (csbi.dwCursorPosition.Y >= csbi.srWindow.Bottom) {
		coord.X = 0;
		coord.Y = csbi.srWindow.Top + 1; //csbi.dwCursorPosition.Y - raws;
		FillConsoleOutputCharacter(hConsole, ' ', (menLen + 1)* csbi.dwSize.X, coord, &count);
		gotoxy(coord.X, coord.Y);
		_PrintMenu();
		gotoxy(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	else if (!flag) {
		_PrintMenu();
		flag = 1;
	}
	SetConsoleTextAttribute(hConsole, csbi.wAttributes);
}

void menu_func(pCls_t ** school, int * size) {
	int menu = 0;
	do {
		gui_display();
		menu = 0;
		printf("\nSelect an option: ");
		while (menu < 1 || menu > 10)
			menu = create_num();
		if (menu < 7) {
			if (!(school && *school))
				wraper_force_school(school, size);
			menu_array[menu].pfunc(*(*school + menu_array[menu].pNum(*size)), &menu_array[menu].flag);
		}
		else 
			menu_array[menu].pfunc((void *)school, size);
	} while(menu > 0 && menu < 10);
}

