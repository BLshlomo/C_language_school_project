#define NAME 100
#define CYAN FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define CLS 234

typedef unsigned char uchar;
typedef int data_item;

typedef struct _menu_item {
	char * description;
	void(*pfunc)(void *, void *);
	int (*pNum)(int);
	char flag;
} Menu_item, *pMenu_item;

typedef struct Grade {
	data_item lsn, grade;
	struct Grade *next;
} Grd_t, *pGrd_t;

typedef struct Student {
	char * name;
	struct Student *next;
	pGrd_t to_grade;
} Stn_t, *pStn_t; 

typedef struct Cls {
	uchar type_t;
	data_item clsNum, amountOfStudents;
	pStn_t student;
} Cls_t, *pCls_t;

int create_num();

char yOn();

char again();

int is_str_equall(char * name, char * string);

void name_print(char * name);

pCls_t make_cls(data_item data);

pStn_t make_student(char name[]);

pGrd_t make_grade(int gradeId, int grd );

int add_first_student(pStn_t *head, char name[]);

int add_first_grade(pGrd_t *head, int gradeId, int grd);

int add_last_grade(pGrd_t *head, pGrd_t last, int gradeId, int grd);

int find_stn(pStn_t * stn);

int add_student(pStn_t *head);

int change_grd(pStn_t stn, pGrd_t * last, int lsn, int grd);

int add_lsn(pStn_t stn);

int add_stn_lsn(pStn_t stn);

int del_stn(pStn_t * head);

int del_lsn(pStn_t stn);

int del_stn_lsn(pStn_t stn);

void lprnt_cls(void * Cls, void * null);

void lprnt_stn_grds(void * Cls, void * null);
