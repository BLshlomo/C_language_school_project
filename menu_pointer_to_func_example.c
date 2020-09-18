#include<stdio.h>


typedef struct _menue_item{
	char *description;
	void(*pfunc)(void);
}menue_item, *pmenue_item;

void func1();
void func2();
void func3();
void func4();
void func5();
static void _PrintMenu();

menue_item menue_array[] = {
	{ "MENUE	Please enter Your choice\n\n", NULL },
	{ "\t 1 to func1\n", func1 },
	{ "\t 2 to func2\n", func2 },
	{ "\t 3 to func3\n", func3 },
	{ "\t 4 to func4\n", func4 },
	{ "\t 5 to exit \n", func5 },
	{ NULL	, NULL }
};


int main(){
	int choise;
	while(1){
		system("cls");
		_PrintMenu();
		scanf("%d", &choise);
		if (menue_array[choise].pfunc){
			menue_array[choise].pfunc();
		}
		system("pause");
	}
	system("pause");
	return 0;
}
static void _PrintMenu()
{
	pmenue_item pMenuItem = menue_array;
	while (pMenuItem->description != NULL)
	{
		printf("%s", pMenuItem->description);
		pMenuItem++;
	}
}

void func1(){ printf("i'm in func 1\n"); }
void func2(){ printf("i'm in func 2\n"); }
void func3(){ printf("i'm in func 3\n"); }
void func4(){ printf("i'm in func 4\n"); }
void func5(){ 
	char ch; 
	printf("Are you sure? : ");
	scanf(" %c", &ch); 
	if (ch == 'y' || ch == 'Y') 
		exit(0); 
}
