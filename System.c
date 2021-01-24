#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <windows.h>
#include <winsock.h>
#include <io.h>

//#define MYSQL_HOST "127.0.0.1"
#define MYSQL_USER "root"
//#define MYSQL_PASSWD "plf,.123"
#define DB_NAME ""

//Student Data
typedef struct student
{
	int id;
	int grade;
	int Class;
	float English;
	float Math;
	float LA;
	float SZ;
	float C;
} Student;
//Account Data
typedef struct login
{
	char name[24];
	char password[20];
	char id[20];
	int type;
} Login;
//End

//����������
int Main_Menu();
int mysqldb_connect(MYSQL *mysql);
int Check_DataBase(MYSQL *mysql, char *DataBase_Name);
int Check_Table(MYSQL *mysql, char *DataBase_Name, char *Table_Name, char *Class_Name);
int Search_Account(MYSQL *mysql, char *Account_Name, char *id);
int Create_Root(MYSQL *mysql);
int Create_Account(MYSQL *mysql, char *Account_Name, char *Account_Id, char *Account_Password, int Permission);
char Account_Login(MYSQL *mysql, char *Account_Name, char *Account_Password, char *p);
int Initialize(MYSQL *mysql);
MYSQL Main_Do();
int SignUp(MYSQL *mysql);
int SignIn(MYSQL *mysql, char *id);
int Create_Student(MYSQL *mysql, char *ID, char *Name, int grade, int class, float English, float Math, float LA, float SZ, float C, char *Password, int Permission);
int Print(MYSQL *mysql, char *id);
int UpdateScore(MYSQL *mysql, char *id);
int AddInf(MYSQL *mysql, int Permission);
int PrintAll(MYSQL *mysql);
int Delete_Student(MYSQL *mysql, char *id);
int Root_Delete(MYSQL *mysql);
int SearchStudent(MYSQL *mysql, char *ID);
int CSVWriten(MYSQL *mysql);
void Root_Menu();
void Admin_Menu();
void Normal_Menu();
int Menu(MYSQL *mysql, int Permission, char *id);
void Root_Do();
void Admin_Do();
void Normal_Do();
//��������

int Main_Menu()																		//���˵�
{
	printf("===============================================================\n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("             Student Score Manage System V1.0                  \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("        1.Sign In                  2.Sign Up                   \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("        3.Exit                                                 \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("                                                               \n");
	printf("===============================================================\n");
	printf("Please Select:");
	return 1;
}

int mysqldb_connect(MYSQL *mysql)														//MYSQL���Ӻ���
{
	char MYSQL_HOST[100] = {'\0'};			//������ַ
	char MYSQL_PASSWD[100] = {'\0'};		//Root����
	printf("Please Server Host:");			//����
	scanf("%s", MYSQL_HOST);
	printf("Please Server Root Password:");
	scanf("%s", MYSQL_PASSWD);
	if (!mysql_real_connect(mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, DB_NAME, 0, NULL, 0))    //��������
	{
		printf("\nFailed to connect:%s\n", mysql_error(mysql));				//ʧ�����������Ϣ
		return -1;															//����-1
	}
	else
	{
		printf("\nConnect successfully!\n");								//�ɹ�������ӳɹ�������1
		return 1;
	}
}

int Check_DataBase(MYSQL *mysql, char *DataBase_Name)					//����ض����ݿ��Ƿ����
{
	MYSQL_RES *result = mysql_list_dbs(mysql, DataBase_Name);			//��ѯ���
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(result)) != NULL)						//����������Ϊ���������
	{
		printf("%s DataBase Connected!\n", DataBase_Name);
	}
	else
	{
		char *sql = "create database ";									//���򴴽��ض����ݿ�
		char dest[100] = {'\0'};
		strcat(dest, sql);
		strcat(dest, DataBase_Name);
		mysql_query(mysql, dest);
		printf("%s DataBase Not Found,It Will Be Created!\n", DataBase_Name);
	}
	return 1;
	mysql_free_result(result);									//�ͷŽ����
}

int Check_Table(MYSQL *mysql, char *DataBase_Name, char *Table_Name, char *Class_Name)
{
	char *tmp = "use ";											//ƴ��MYSQL����
	char Command[100] = {'\0'};
	strcat(Command, tmp);
	strcat(Command, DataBase_Name);
	mysql_query(mysql, Command);
	char *sql = "create table ";
	char dest[100] = {""};
	strcat(dest, sql);
	strcat(dest, Table_Name);
	strcat(dest, "(");
	strcat(dest, Class_Name);
	strcat(dest, ")");
	MYSQL_RES *result = mysql_list_tables(mysql, Table_Name);		//��ѯ�ض����Ƿ����
	MYSQL_ROW row;
	if (((row = mysql_fetch_row(result)) != NULL))
	{
		printf("%s TABLE Connected!\n", Table_Name);
	}
	else
	{
		mysql_query(mysql, Command);								//�������򴴽�
		mysql_query(mysql, dest);
		printf("%s Table Not Found,It Will Be Created!\n", Table_Name);
	}
	return 1;
	mysql_free_result(result);
}
int Search_Account(MYSQL *mysql, char *Account_Name, char *id)   	//ͨ���˻�����ID��ѯ�˻�
{
	int flag;				//״ֵ̬
	char *sql = "Select * from ";		//ƴ��MYSQL����
	char dest[100] = {""};
	strcat(dest, sql);
	strcat(dest, "Account");
	strcat(dest, " where id_name = '");
	strcat(dest, Account_Name);
	strcat(dest, "'");
	char destx[100] = {""};
	char *sqlx = "Select * from ";
	strcat(destx, sqlx);
	strcat(destx, "Account");
	strcat(destx, " where id = '");
	strcat(destx, id);
	strcat(destx, "'");
	mysql_real_query(mysql, dest, strlen(dest));   //ͨ��mysql_real_query��ѯ
	MYSQL_RES *result = mysql_store_result(mysql);
	MYSQL_ROW row;
	int R1 = mysql_num_rows(result);
	mysql_free_result(result);
	mysql_real_query(mysql, destx, strlen(destx));
	result = mysql_store_result(mysql);
	int R2 = mysql_num_rows(result);
	if (R1 == 0 && R2 == 0)					//�������ֵ���������򷵻�0
	{
		flag = 0;
	}
	else
	{
		flag = 1;
	}
	mysql_free_result(result);
	return flag;
}

int Create_Root(MYSQL *mysql)  //����root�˻�
{
	char sql[150] = {'\0'};
	char temp_password[100] = {'\0'};
	printf("Please Root Password:");
	scanf("%s", temp_password);
	sprintf(sql, "insert into Account (id_name,id,password,Permission) values('root','NULL','%s','2')", temp_password); //ͨ��sprintf���ַ�������д������

	if (!mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("root Create Successfully\n");
	}
	else
	{
		printf("root Create Failed!");
	}
	return 1;
}

int Create_Account(MYSQL *mysql, char *Account_Name, char *Account_Id, char *Account_Password, int Permission)    //�����˻�
{
	char sql[150] = {'\0'};
	int flag = -1;
	if (Permission == 1)						//��������Ĺ���Ա�˻�������Ҫ����Root����
	{
		char temp_password[100] = {'\0'};
		printf("Please Root Password:");
		scanf("%s", temp_password);
		mysql_query(mysql, "select * from Account limit 0,1");
		MYSQL_RES *result = mysql_store_result(mysql);
		MYSQL_ROW row;
		row = mysql_fetch_row(result);
		if (strcmp(row[2], temp_password) == 0)
		{
			flag = 1;
		}
	}
	else if (Permission == 0)
	{
		flag = 1;
	}
	if (flag != -1)                    //��֤�������˻�
	{
		sprintf(sql, "insert into Account (id_name,id,password,Permission) values('%s','%s','%s','%d')", Account_Name, Account_Id, Account_Password, Permission);

		if (!mysql_real_query(mysql, sql, strlen(sql)))
		{
			printf("Sign Up Successfully\n");
			return 1;
		}
	}
	else
	{
		printf("Sign Up Failed!");
		return 0;
	}
	mysql_free_result(result);
}

char Account_Login(MYSQL *mysql, char *Account_Name, char *Account_Password, char *p)     //�˻���¼��ͨ��ָ�뷵��ID
{
	char Account_Permission;
	char *sql = "select * from Account where id_name='";   //ƴ��MYSQL����
	char dest[100] = {""};
	strcat(dest, sql);
	strcat(dest, Account_Name);
	strcat(dest, "'");
	mysql_real_query(mysql, dest, strlen(dest));
	MYSQL_RES *result = mysql_store_result(mysql);
	MYSQL_ROW row;
	if (((row = mysql_fetch_row(result)) != NULL) && (strcmp(row[2], Account_Password) == 0))   //��ѯ�˻��Ƿ���ڲ��������
	{
		printf("Sign In Successfully!\n");
		Account_Permission = *row[3];
		strcat(p, row[1]);						//��ID����
		mysql_free_result(result);
		return Account_Permission;				//����Ȩ��
	}
	else
	{
		printf("Sign In Failed!\n");
		mysql_free_result(result);
		return 'X';
	}
	mysql_free_result(result);
}

int Initialize(MYSQL *mysql)         //��ʼ������
{
	int a = Check_DataBase(mysql, "Information_db");       //����
	int b = Check_Table(mysql, "Information_db", "Student", "id VARCHAR(25),name VARCHAR(25),grade INT(11),class INT(11),English FLOAT,Math FLOAT,LA FLOAT,SZ FLOAT,C FLOAT");   	//���ѧ�����ݱ�
	int c = Check_Table(mysql, "Information_db", "Account", "id_name VARCHAR(20),id VARCHAR(25),password VARCHAR(25),Permission INT(11)");			//����û����ݱ�
	int d = Search_Account(mysql, "root", "NULL");			//���Root�˻��Ƿ����
	int e;
	if (d == 0)
	{
		e = Create_Root(mysql);
	}
	else
	{
		e = 1;
	}
	return a && b && c && e ? 1 : 0;			//���ȫ���ɹ��򷵻�1
}

MYSQL Main_Do()										//����������
{
	printf("Initializing.....\n");
	MYSQL mysql;
	mysql_init(&mysql);     //��ʼ������
	if (&mysql == NULL)
	{
		printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
		exit(1);
	}
	while (mysqldb_connect(&mysql) != 1)        //һֱ��������֪���ɹ�Ϊֹ
	{
		printf("Login Faild!\n");
		system("pause");
	}
	if (Initialize(&mysql) == 1)				//�����ʼ������򷵻�1�������ʼ�����
	{
		printf("Initialization Completed.\n");
	}
	else										//����ֱ���˳�
	{
		exit(-1);
	}
	return mysql;
}

int SignUp(MYSQL *mysql)						//ע�ắ��
{
	Login NewAccount;
	printf("Please Type New Account Name:");
	scanf("%s", NewAccount.name);
	printf("Please Type New Account Id:");
	scanf("%s", NewAccount.id);
	if (Search_Account(mysql, NewAccount.name, NewAccount.id) == 0)    //����û�����ID����������˳�
	{
		printf("Please Type New Account Password:");
		scanf("%s", NewAccount.password);
		printf("Please Type New Account Type:");
		scanf("%d", &NewAccount.type);
		if(NewAccount.type==0||NewAccount.type>2||NewAccount.type<0)            //������ѧ������ע��
		{
			return 0;
		}
		if (Create_Account(mysql, NewAccount.name, NewAccount.id, NewAccount.password, NewAccount.type) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int SignIn(MYSQL *mysql, char *id)            //��¼����
{
	Login Login_Account;
	printf("Please Type Account Name:");
	scanf("%s", Login_Account.name);
	printf("Please Type Account Id:");
	scanf("%s", Login_Account.id);
	if (Search_Account(mysql, Login_Account.name, Login_Account.id) == 1)   //����˻����� ����е�¼
	{
		printf("Please Type Account Password:");
		scanf("%s", Login_Account.password);
		char tmp = Account_Login(mysql, Login_Account.name, Login_Account.password, id);        //�鿴����ֵ
		if (tmp == '1')						//����Ȩ��
		{
			return 1;
		}
		else if (tmp == '2')
		{
			return 2;
		}
		else if (tmp == '0')
		{
			return 0;
		}
		else if (tmp == 'X')
		{
			return -1;
		}
	}
}

int Create_Student(MYSQL *mysql, char *ID, char *Name, int grade, int class, float English, float Math, float LA, float SZ, float C, char *Password, int Permission)          //����ѧ�����ڳɼ�����д�룬���û����ﴴ��һ��Ȩ��Ϊ0����ͨ�˻�
{
	char sql[150] = {'\0'};
	char sqlx[150] = {'\0'};
	sprintf(sql, "insert into Student (id,name,grade,class,English,Math,LA,SZ,C) values('%s','%s','%d','%d','%f','%f','%f','%f','%f')", ID, Name, grade, class, English, Math, LA, SZ, C);
	sprintf(sqlx, "insert into Account (id_name,id, password, Permission) values('%s','%s','%s','%d')", Name, ID, Password, Permission);
	if ((!mysql_real_query(mysql, sql, strlen(sql))) && (!mysql_real_query(mysql, sqlx, strlen(sqlx))))
	{
		printf("Add Successfully\n");
		return 1;
	}
}

int Print(MYSQL *mysql, char *id)                     //������˳ɼ�
{
	int flag;
	char *sql = "Select * from ";
	char dest[100] = {""};
	strcat(dest, sql);
	strcat(dest, "Student");
	strcat(dest, " where id = '");
	strcat(dest, id);
	strcat(dest, "'");
	mysql_real_query(mysql, dest, strlen(dest));
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	if (res == NULL)
	{
		return 0;
	}
	else
	{
		while ((row = mysql_fetch_row(res)))
		{
			int n = 8;
			while (n--)
				printf("%s\n", row[8 - n]);
		}
		return 1;
	}
	mysql_free_result(res);
	return 1;
}

int UpdateScore(MYSQL *mysql, char *id)										//ͨ��id���³ɼ�
{
	Student NewInf;
	Print(mysql, id);
	printf("Please Type Student English Socre:");
	scanf("%f", &NewInf.English);
	printf("Please Type Student Math Socre:");
	scanf("%f", &NewInf.Math);
	printf("Please Type Student Linear Algebra Socre:");
	scanf("%f", &NewInf.LA);
	printf("Please Type Student Ideological and Political Socre:");
	scanf("%f", &NewInf.SZ);
	printf("Please Type Student C Language Socre:");
	scanf("%f", &NewInf.C);
	char NewInformation[200];
	sprintf(NewInformation, "UPDATE Student SET English='%f' Math='%f' LA='%f' SZ='%f' C='%f' WHERE id='%s';", NewInf.English, NewInf.Math, NewInf.LA, NewInf.SZ, NewInf.C, id);
	mysql_real_query(mysql, NewInformation, strlen(NewInformation));
	MYSQL_RES *result = mysql_store_result(mysql);
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(result)) != NULL)
	{
		printf("Update Success!\n");
	}
	else
	{
		printf("Update Faild!\n");
	}
}

int AddInf(MYSQL *mysql, int Permission)									//��ȡ��Ҫ��ӵ�ѧ����Ϣ
{
	Login Add_Account;
	Student Add_Student;
	printf("Please Type Added Name:");
	scanf("%s", Add_Account.name);
	printf("Please Type Added Id:");
	scanf("%s", &Add_Account.id);
	if (Search_Account(mysql, Add_Account.name, Add_Account.id) == 0)
	{
		printf("Please Type Account Password:");
		scanf("%s", Add_Account.password);
		printf("Please Type Student Grade:");
		scanf("%d", &Add_Student.grade);
		printf("Please Type Student Class:");
		scanf("%d", &Add_Student.Class);
		printf("Please Type Student English Socre:");
		scanf("%f", &Add_Student.English);
		printf("Please Type Student Math Socre:");
		scanf("%f", &Add_Student.Math);
		printf("Please Type Student Linear Algebra Socre:");
		scanf("%f", &Add_Student.LA);
		printf("Please Type Student Ideological and Political Socre:");
		scanf("%f", &Add_Student.SZ);
		printf("Please Type Student C Language Socre:");
		scanf("%f", &Add_Student.C);
		return Create_Student(mysql, Add_Account.id, Add_Account.name, Add_Student.grade, Add_Student.Class, Add_Student.English, Add_Student.Math, Add_Student.LA, Add_Student.SZ, Add_Student.C, Add_Account.password, 0); //���ú�������
	}
}

int PrintAll(MYSQL *mysql)            //�������ѧ����Ϣ
{
	char query[150] = {'\0'};
	char *sql = "select * from Student";
	strcpy(query, sql);
	mysql_real_query(mysql, query, strlen(query));
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	if (res == NULL)
	{
		return 0;
	}
	else
	{
		//printf("%d \n", mysql_num_rows(res));
		while ((row = mysql_fetch_row(res)))
		{
			int n = 8;
			while (n--)
				printf("%s\n", row[8 - n]);
		}
		return 1;
	}
	mysql_free_result(res);
}

int Delete_Student(MYSQL *mysql, char *id)								//ɾ��ѧ����Ϣ
{
	char NewInformation[200] = {'\0'};
	char DeleteAccount[200] = {'\0'};
	int n = 0;
	float m = 0;
	char *tmp = "";
	sprintf(NewInformation, "UPDATE Student SET id='%s' Class='%d' grade='%d' English='%f' Math='%f' LA='%f' SZ='%f' C='%f' WHERE id='%s'", tmp, n, n, m, m, m, m, m, id);
	sprintf(DeleteAccount, "UPDATE Student SET id='%s' id_name='%s' password='%s' Permission='%d' WHERE id='%s'", tmp, tmp, tmp, n, id);
	if ((!mysql_real_query(mysql, NewInformation, strlen(NewInformation))) && (!mysql_real_query(mysql, DeleteAccount, strlen(DeleteAccount))))
	{
		printf("Delete Successfully\n");
		return 1;
	}
}

int Root_Delete(MYSQL *mysql)							//RootȨ�ޣ�����ɾ���κ�Ȩ��Ϊ0��1���˻�
{
	int Select;
	printf("Please Type Deleted Type:");
	scanf("%d", &Select);
	char NewInformation[200] = {'\0'};
	char DeleteAccount[200] = {'\0'};
	int n = 0;
	float m = 0;
	char *tmp = "";
	if (Select == 1)
	{
		char AccountName[100] = {'\0'};
		printf("Please Account Name:");
		scanf("%s", AccountName);
		sprintf(DeleteAccount, "UPDATE Student SET id='%s' id_name='%s' password='%s' Permission='%d' WHERE id_name='%s'", tmp, tmp, tmp, n, AccountName);
		if (!mysql_real_query(mysql, DeleteAccount, strlen(DeleteAccount)))
		{
			printf("Delete Successfully!\n");
		}
	}
	else if (Select == 0)
	{
		char StudentID[100] = {'\0'};
		printf("Please Student ID:");
		scanf("%s", StudentID);
		Delete_Student(mysql, StudentID);
	}
}

int SearchStudent(MYSQL *mysql, char *ID)								//����ѧ���Ƿ����
{
	int flag;
	char *sql = "Select * from ";
	char dest[100] = {""};
	strcat(dest, sql);
	strcat(dest, "Student");
	strcat(dest, " where id = '");
	strcat(dest, ID);
	strcat(dest, "'");
	mysql_real_query(mysql, dest, strlen(dest));
	MYSQL_RES *result = mysql_store_result(mysql);
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(result)) != NULL)
	{
		int n = 8;
		while (n--)
			printf("%s\n", row[8 - n]);
	}
}

int CSVWriten(MYSQL *mysql)										//���ѧ����Ϣ��csv�ļ�
{
	char query[150] = {'\0'};
	char *sql = "select * from Student";
	strcpy(query, sql);
	mysql_real_query(mysql, query, strlen(query));
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	FILE *fp;
	if (res == NULL)
	{
		return 0;
	}
	else
	{
		//printf("%d \n", mysql_num_rows(res));
		while ((row = mysql_fetch_row(res)))
		{
			fp = fopen(".\\Student.csv", "a+");

			fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]);
		}
		return 1;
	}
	mysql_free_result(res);
}

void Root_Menu()
{
	printf("*************************************************\n");
	printf("                  Root__Menu                     \n");
	printf("*************************************************\n");
	printf("                                                 \n");
	printf("    1.Add Student Information                    \n");
	printf("    2.Print  Student Information                 \n");
	printf("    3.Read   Student Information                 \n"); //csv
	printf("    4.Search Student Information                 \n");
	printf("    5.Update Student Information                 \n");
	printf("    6.Delete Student Information                 \n");
	printf("    7.Add Account                                \n");
	printf("    8.Delete Account                            \n");
	printf("    9.Exit                                       \n");
	printf("*************************************************\n");
	printf("Please Select:");
}

void Admin_Menu()
{
	printf("*************************************************\n");
	printf("                  Admin__Menu                    \n");
	printf("*************************************************\n");
	printf("                                                 \n");
	printf("    1.Add Student Information                    \n");
	printf("    2.Print  Student Information                 \n");
	printf("    3.Read   Student Information                 \n");
	printf("    4.Search Student Information                 \n");
	printf("    5.Update Student Information                 \n");
	printf("    6.Delete Student Information                 \n");
	printf("    7.Exit                                       \n");
	printf("*************************************************\n");
	printf("Please Select:");
}

void Normal_Menu()
{
	printf("*************************************************\n");
	printf("                  Normal__Menu                   \n");
	printf("*************************************************\n");
	printf("      1.Print student Information                \n");
	printf("      2.Exit                                     \n");
	printf("*************************************************\n");
	printf("Please Select:");
}

void Normal_Do(MYSQL *mysql, char *id)				//�˻���������
{
	int Select;
	while (1)
	{
		system("cls");
		Normal_Menu();
		scanf("%d", &Select);
		switch (Select)
		{
		case 1:
			Print(mysql, id);
			break;
		case 2:
			exit(1);
			break;
		}
	}
}

void Admin_Do(MYSQL *mysql)	
{
	int Select;
	int n = 1;
	char id[100];
	while (n)
	{
		system("cls");
		Admin_Menu();
		scanf("%d", &Select);
		switch (Select)
		{
		case 1:
			AddInf(mysql, 0);
			break;
		case 2:
			PrintAll(mysql);
			break;
		case 3:
			CSVWriten(mysql);
			break;
		case 4:
			printf("Please Type ID:");
			scanf("%s", id);
			SearchStudent(mysql, id);
			break;
		case 5:
			printf("Please Type ID:");
			scanf("%s", id);
			UpdateScore(mysql, id);
			break;
		case 6:
			printf("Please Type ID:");
			scanf("%s", id);
			Delete_Student(mysql, id);
			break;
		case 7:
			n = 0;
			break;
		}
	}
}
void Root_Do(MYSQL *mysql)
{
	Login Account;
	int Select;
	int n = 1;
	char id[100];
	while (n)
	{
		system("cls");
		Root_Menu();
		scanf("%d", &Select);
		switch (Select)
		{
		case 1:
			AddInf(mysql, 0);
			break;
		case 2:
			PrintAll(mysql);
			break;
		case 3:
			CSVWriten(mysql);
			break;
		case 4:
			printf("Please Type ID:");
			scanf("%s", id);
			SearchStudent(mysql, id);
			break;
		case 5:
			printf("Please Type ID:");
			scanf("%s", id);
			UpdateScore(mysql, id);
			break;
		case 6:
			printf("Please Type ID:");
			scanf("%s", id);
			Delete_Student(mysql, id);
			break;
		case 7:
			printf("Please Type Account Name:");
			scanf("%s", Account.name);
			printf("Please Type Account ID:");
			scanf("%s", Account.id);
			printf("Please Type Account Password:");
			scanf("%s", Account.password);
			printf("Please Type Account Type:");
			scanf("%d", Account.type);
			Create_Account(mysql, Account.name, Account.id, Account.password, Account.type);
			break;
		case 8:
			Root_Delete(mysql);
			break;
		case 9:
			exit(1);
			break;
		}
	}
}

int Menu(MYSQL *mysql, int Permission, char *id)			//��ͬȨ�����˵���ת
{
	switch (Permission)
	{
	case 0:
		system("cls");
		Normal_Do(mysql, id);
		break;
	case 1:
		system("cls");
		Admin_Do(mysql);
		break;
	case 2:
		system("cls");
		Root_Do(mysql);
		break;
	}
}

int main(void)				
{
	int Login_Select;
	int tmp;
	char id[100] = {'\0'};
	MYSQL mysql = Main_Do();
	system("pause");
	system("cls");
	while (1)
	{
		Main_Menu();
		if (scanf("%d", &Login_Select) == 1)
		{
			switch (Login_Select)
			{
			case 3:
				system("cls");
				exit(1);
				break;
			case 2:
				system("cls");
				SignUp(&mysql);
				break;
			case 1:
				system("cls");
				tmp = SignIn(&mysql, id);   //��¼�ķ���ֵ��Ȩ��ֵ��
				Menu(&mysql, tmp, id);			//��ת
			default:
				break;
			}
		}
	}
	return 0;
}