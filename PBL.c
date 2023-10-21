#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

#define FEE_AMOUNT 2.000
#define TRANSACTION 0
#define TRANSACTION_LIMIT 2
#define MAX_NUM_ACCOUNTS 1000
#define MAX_ACCNUMBER_DIGITS 15
#define MAX_PIN_DIGITS 7
#define NAME_OF_TRANSACTION 50

int check;

struct THE_ATM {
	char AccNumber[MAX_ACCNUMBER_DIGITS];
	char Pins[MAX_PIN_DIGITS];
	float Sodu;
};

struct BIEN_LAI {
	char MaThe[MAX_ACCNUMBER_DIGITS];
	int MaLoaiGD;
	char TenLoaiGD[NAME_OF_TRANSACTION];
	float SoTienGD;
	float LePhi;
	float SoDuConLai;
};

int findAcc(struct THE_ATM CARDS[] , char AccNumber[] , int num_of_accounts);

void inputPassword(char admin[MAX_ACCNUMBER_DIGITS] , char passWord[MAX_ACCNUMBER_DIGITS]);

void sort(int l , int r , struct THE_ATM CARDS[]);

void input(int *num_of_accounts , struct THE_ATM CARDS[]);

void transferMoney(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int i);

void withdrawMoney(struct BIEN_LAI BL[] , struct THE_ATM CARDS[]  , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int i);

void checkBalance(struct BIEN_LAI BL[] , struct THE_ATM CARDS[]  , char AccNumber[] , char Pins[] , int SoLanGD[] , int *total_transactions , int i);

void transferMoneyActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int num_of_accounts , int i) ;

void withdrawMoneyActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int num_of_accounts , int i);

void checkBalanceActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , int *total_transactions , int num_of_accounts , int i);
void print(struct BIEN_LAI BL[] , int *total_transactions);

void checkAcc(struct THE_ATM CARDS[] , char AccNumber[] , char Pins[]  , int SoLanGD[] , int i);

void menu(int *selected_option);

void atmScreen(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , int SoLanGD[] , float SoTienGD , int total_transactions , int num_of_accounts , char admin[MAX_ACCNUMBER_DIGITS] , char passWord[MAX_ACCNUMBER_DIGITS]);

int main() 
{
	int num_of_accounts, total_transactions = 0;
	
	struct THE_ATM CARDS[MAX_NUM_ACCOUNTS];
	char admin[MAX_ACCNUMBER_DIGITS] , passWord[MAX_ACCNUMBER_DIGITS];

	input(&num_of_accounts , CARDS);
	inputPassword(admin , passWord);


	struct BIEN_LAI BL[num_of_accounts * TRANSACTION_LIMIT + 1];

	float SoTienGD;
	int SoLanGD[num_of_accounts + 1];
	
	// Khởi tạo số lần GD trong 1 ngày
	for(int i = 1; i <= num_of_accounts; i++) 
		SoLanGD[i] = TRANSACTION;
	
	atmScreen(BL , CARDS , SoLanGD , SoTienGD , total_transactions , num_of_accounts , admin , passWord);
		
	return 0;
}


void sort(int l , int r , struct THE_ATM CARDS[]) {
	char p[MAX_ACCNUMBER_DIGITS];
	strcpy(p , CARDS[(l + r) / 2].AccNumber);
    int i = l , j = r;
    while(i < j) {
        while(strcmp(CARDS[i].AccNumber , p) < 0) i++;
        while(strcmp(CARDS[j].AccNumber , p) > 0) j--;
        if(i <= j) {
            struct THE_ATM tam = CARDS[i];
            CARDS[i] = CARDS[j];
            CARDS[j] = tam;
            i++;
            j--;
        }
    }
    if(i < r) sort(i , r , CARDS);
    if(j > l) sort(l , j , CARDS);
}



void input(int *num_of_accounts , struct THE_ATM CARDS[]) {
	
    FILE *fp = fopen("MAATM.txt", "r");
    
	int i = 1;
	while (fscanf(fp, "%s%s%f", CARDS[i].AccNumber, CARDS[i].Pins , &CARDS[i].Sodu) == 3) {
    	i++;
	}
	*num_of_accounts = i - 1;
	
    fclose(fp);

	sort(1 , i - 1, CARDS);
}


void inputPassword(char admin[MAX_ACCNUMBER_DIGITS] , char passWord[MAX_ACCNUMBER_DIGITS]) {
	FILE *fp = fopen("ADMIN.txt", "r");
	fscanf(fp , "%s" , admin);
	fscanf(fp , "%s" , passWord);
	fclose(fp);
}

int findAcc(struct THE_ATM CARDS[] , char AccNumber[] , int num_of_accounts) {
	int l = 1 , r = num_of_accounts;
	int i;

	while(l <= r) {
		int i = (l + r) / 2;
		
		if(strcmp(CARDS[i].AccNumber , AccNumber) == 0) return i;

		if(strcmp(CARDS[i].AccNumber , AccNumber) < 0) l = i + 1;

		else r = i - 1;
	}

	return -1;
}

void checkAcc(struct THE_ATM CARDS[] , char AccNumber[] , char Pins[]  , int SoLanGD[] , int i) {
	check = -1;
	
	if(i == -1) {
		check = 0;
		return;
	}

	if(strcmp(CARDS[i].AccNumber , AccNumber) != 0) {
		check = 0;
		return;
	}

	if(strcmp(CARDS[i].Pins , Pins) != 0) {
		check = 2;
		return;
	}

	if(SoLanGD[i] >= TRANSACTION_LIMIT) {
		check = 4;
		return;
	}
}

void print(struct BIEN_LAI BL[] , int *total_transactions) {
	
	FILE *fp;
   	fp = fopen("Bienlai.txt", "w");
   	
	int i;
	
	for(i = 0 ; i < *total_transactions ; ++i) {
		fprintf(fp , "# %d:\n" , i + 1);
		fprintf(fp , "MA CARD    :%s\n" , BL[i].MaThe);
		fprintf(fp , "Ma GD      :%d\n" , BL[i].MaLoaiGD);
		fprintf(fp , "Ten Loai GD:%s\n" , BL[i].TenLoaiGD);
		fprintf(fp , "So Tien GD :%0.0f\n" , BL[i].SoTienGD);
		fprintf(fp , "Le Phi     :%0.0f\n" , BL[i].LePhi);
		fprintf(fp , "So Du TK   :%0.0f\n" , BL[i].SoDuConLai);
		fprintf(fp , "***********************************************\n");
	}
	
	fclose(fp);
}


void transferMoney(struct BIEN_LAI BL[] , struct THE_ATM CARDS[]  , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int i) {
	struct BIEN_LAI RL;
	strcpy(RL.MaThe , AccNumber);
	RL.MaLoaiGD = 1;
	strcpy(RL.TenLoaiGD , "GUI TIEN");
	RL.SoTienGD = SoTienGD;
	RL.LePhi = TRANSACTION;
	RL.SoDuConLai = 0;

	CARDS[i].Sodu = CARDS[i].Sodu + SoTienGD;
	RL.SoDuConLai = RL.SoDuConLai + CARDS[i].Sodu;
	
	SoLanGD[i]++;
	
	BL[*total_transactions] = RL;
	*total_transactions+=1;
}


void withdrawMoney(struct BIEN_LAI BL[] , struct THE_ATM CARDS[]  , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions, int i) {
	struct BIEN_LAI RL;
	strcpy(RL.MaThe , AccNumber);
	RL.MaLoaiGD = 2;
	strcpy(RL.TenLoaiGD , "RUT TIEN");
	RL.SoTienGD = SoTienGD;
	RL.LePhi = FEE_AMOUNT;
	RL.SoDuConLai = 0;
	
	CARDS[i].Sodu = CARDS[i].Sodu - SoTienGD - RL.LePhi;
	RL.SoDuConLai = RL.SoDuConLai + CARDS[i].Sodu;
	
	SoLanGD[i]++;
	
	BL[*total_transactions] = RL;
	*total_transactions+=1;
}



void checkBalance(struct BIEN_LAI BL[] , struct THE_ATM CARDS[]  , char AccNumber[] , char Pins[] , int SoLanGD[] , int *total_transactions , int i) {
	struct BIEN_LAI RL; 
	strcpy(RL.MaThe , AccNumber);
	RL.MaLoaiGD = 3;
	strcpy(RL.TenLoaiGD , "XEM TAI KHOAN");
	RL.SoTienGD = 0;
	RL.LePhi = FEE_AMOUNT;
	RL.SoDuConLai = 0;
	
	CARDS[i].Sodu = CARDS[i].Sodu - RL.LePhi;
	RL.SoDuConLai = RL.SoDuConLai + CARDS[i].Sodu;
	
	printf("***********************************************\n\n");
	printf("MA CARD             : %s\n\n", AccNumber);
	printf("So tien Du kha dung : %0.0f\n\n", RL.SoDuConLai);
	
	SoLanGD[i]++;
	
	BL[*total_transactions] = RL;
	*total_transactions+=1;
}


void transferMoneyActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int num_of_accounts , int i)  {
	if(strcmp(CARDS[i].AccNumber , AccNumber) == 0 && strcmp(CARDS[i].Pins , Pins) == 0) {
		printf("\n\nSo tien GD: ");
		scanf("%f" , &SoTienGD);
		printf("\n\n");
			
		while(SoTienGD <= 0) {
			printf("So tien khong hop le!\n");
			printf("Vui long nhap lai so tien GD: ", CARDS[i].Sodu);
			scanf("%f" , &SoTienGD);
		}
			
		transferMoney( BL ,  CARDS , AccNumber , Pins , SoLanGD ,  SoTienGD , total_transactions , i);
			
		check = 1;
		return;
	}
}


void withdrawMoneyActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , float SoTienGD , int *total_transactions , int num_of_accounts , int i) {
	if(strcmp(CARDS[i].AccNumber , AccNumber) == 0 && strcmp(CARDS[i].Pins , Pins) == 0) {
		printf("\n\nSo tien GD: ");
		scanf("%f" , &SoTienGD);
		printf("\n\n");
			
		while(SoTienGD <= 0) {
			printf("So tien khong hop le!\n");
			printf("Vui long nhap lai so tien GD: ", CARDS[i].Sodu);
			scanf("%f" , &SoTienGD);
		}
			
		if(SoTienGD > CARDS[i].Sodu - FEE_AMOUNT) {
			check = 3;
			return;
		}

		withdrawMoney( BL ,  CARDS , AccNumber , Pins , SoLanGD ,  SoTienGD , total_transactions , i);
			
		check = 1;
		return;
	}
}


void checkBalanceActivity(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , char AccNumber[] , char Pins[] , int SoLanGD[] , int *total_transactions , int num_of_accounts , int i) {
	if(strcmp(CARDS[i].AccNumber , AccNumber) == 0 && strcmp(CARDS[i].Pins , Pins) == 0) {
		checkBalance( BL ,  CARDS , AccNumber , Pins , SoLanGD  , total_transactions , i);

		check = 1;
		return;
	}
}

void menu(int *selected_option) {
	int TempChoice;

	system("cls");
	printf("Cac loai Dich Vu : \n\n");
	printf("1. Nop Tien \n\n");
	printf("2. Rut Tien \n\n");
	printf("3. Xem TK \n\n");
	printf("Enter your choice: ");	scanf("%d" , &TempChoice);

	while(TempChoice < 1 || TempChoice > 3) {
		printf("Enter your choice again: ");
		scanf("%d", &TempChoice);
	}

	*selected_option = TempChoice;
	system("cls");
}

void atmScreen(struct BIEN_LAI BL[] , struct THE_ATM CARDS[] , int SoLanGD[] , float SoTienGD , int total_transactions , int num_of_accounts , char admin[MAX_ACCNUMBER_DIGITS] , char passWord[MAX_ACCNUMBER_DIGITS]) {
	char AccNumber[15];
	char Pins[7]; 
	
	int selected_option;
	int is_active = 1;
	char TE[15];
	
	while(is_active == 1) {
		selected_option = 0;

		printf("\n***********Welcome to ATM Service**************\n\n");
		printf("Ma CARD          : ");	scanf("%s" , &AccNumber); printf("\n");
		printf("PINS             : ");	scanf("%s" , &Pins);	  printf("\n");

		int i = findAcc(CARDS , AccNumber , num_of_accounts);
		checkAcc(CARDS , AccNumber , Pins  , SoLanGD , i);
	 	
		if(check == 0) printf("**********Khong tim thay Ma Card!!***********\n\n");
		else if(check == 2) {
			printf("*****************Ma Pin Sai!!****************\n\n");
			int count = 0;
			while(count <= 3 && check == 2) {
				printf("Vui Long Nhap Lai Ma Pin       :  "); scanf("%s" , &Pins);	  printf("\n");
				checkAcc(CARDS , AccNumber , Pins  , SoLanGD , i);
				if(check == 2) printf("*****************Ma Pin Sai!!****************\n\n");
				else {
					menu(&selected_option);
					break;
				}
				count++;
			}
			if(check == 2) printf("*****************Ban Da Vuot Qua So Lan Thu Ma Pin Cho Phep!!****************\n\n");
		}
		else if(check == 4) printf("Ban da GD qua gioi han cho phep trong 1 ngay. Vui long tro lai sau !!!\n\n");
		else menu(&selected_option);

		if(selected_option == 1) transferMoneyActivity(BL , CARDS , AccNumber , Pins , SoLanGD , SoTienGD , &total_transactions , num_of_accounts , i);
		else if(selected_option == 2) withdrawMoneyActivity(BL , CARDS , AccNumber , Pins , SoLanGD , SoTienGD , &total_transactions , num_of_accounts , i);
		else if(selected_option == 3) checkBalanceActivity(BL , CARDS , AccNumber , Pins , SoLanGD , &total_transactions , num_of_accounts , i);
		
		if(check == 1) printf("***********GIAO DICH THANH CONG!!!*************\n\n");
		else if(check == 3) printf("So du tai khoan hien tai khong du de thuc hien Giao Dich. Vui long nap them de su dung DICH VU :))\n\n");

		printf("Nhap ki tu bat ki de tiep tuc: ");
		scanf("%s" , &TE);
		system("cls");

		if(strcmp(TE , admin) == 0) {
			printf("Nhap MA BAO MAT ATM: ");
			scanf("%s" , &TE);
			printf("\n");
			if(strcmp(TE , passWord) == 0) is_active = 0;
		}
	}

    print(BL , &total_transactions);

}