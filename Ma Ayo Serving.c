#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<direct.h>
#include<sys/stat.h>
#include<dirent.h>
#include<time.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

struct User{
	char FirstName[50];
	char LastName[50];
	char email[50];
	char password[50];
	char username[50];
	char phone[50];
	char address[100];
	char gender[10];
	char dateOfBirth[11];
	char specialization[50];
	char assignedDoctor[50];
};

struct Appointment{
	char doctorName[50];
	char doctorUsername[50];
	char date[20];
	char time[10];
	struct Appointment* next;
};
struct Appointment* patientAppointment = NULL;

struct AcceptedAppointment{
	char patientUsername[50];
    char doctorUsername[50];
    char date[20];
    char time[20];
    struct AcceptedAppointment *next;	
};

//Elle naka fix nako sa warning
struct AppointmentEntry{
	char patientUsername[50];
	char patientFullName[50];
	char date[20];
	char time[20];
	struct AppointmentEntry *next; 
};

void color(int colorCode);
void takeinput(char ch[50]);
void takepassword(char pwd[50]);
int isUsernameTaken(const char *filename, const char *username);
void trimNewline(char *str);
void displayRegisterMenu(int highlight);
void registerDoctor();
void registerPatient();
void chooseSpecialization(char specialization[50]);
void logIn();
void doctorDashBoard(char username[50]);
void viewDoctorprofile(char username[50]);
void manageAppointments(char doctorUsername[50]);
int compareAppointments(const void *a, const void *b);
void getPatientFullName(const char username[50], char fullName[100]);
void viewAppointments(char doctorUsername[50]);
void getTodayDate(char *buffer);
void viewPatientRecords(char doctorUsername[50]);
void displayPatientMedicalRecord(const char doctorUsername[50], const char patientUsername[50]);
void patientDashboard(char username[50]);
void viewPatientprofile(char username[50]);
void createAppointment(char username[50]);
void viewAppointment(char username[50]);
int folderExists(const char *path);
void selectDoctor(char doctorUsername[50], char doctorName[50]);
void createMedicalRecord(const char patientUsername[50], const char doctorUsername[50]);

int main(){
	int choice = 1;
	int key;
	
	do{
		displayRegisterMenu(choice);
		
		key = getch();
		
		if (key == 0 || key == 224){
			key = getch();
			switch (key){
				case 72:
					choice = (choice == 1) ? 4 : choice - 1;
					break;
				case 80:
					choice = (choice == 4) ? 1 : choice + 1;
					break;
			}
		}
		else if (key == 13){
			switch (choice){
			case 1:
				system("cls");
				printf("\t\t\t\tPatient Registration Form\n\n");
				registerPatient();
				break;
			case 2:
				system("cls");
				printf("\t\t\t\tDoctor Registration Form\n\n");
				registerDoctor();
				break;
			case 3:
				logIn();
				printf("Loggin in...\n");
				break;
			case 4: 
				printf("\nThank you! MaAyo Serving at your Service!");
				return 0;
			}
		}	
	} while (1);
	
	return 0;
}
void color(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
void takeinput(char ch[50]){
	fgets(ch, 50, stdin);
	ch[strcspn(ch, "\n")] = 0;
}
void takepassword(char pwd[50]){
	int i = 0;
	char ch;
	while(1){
		ch =  getch();
		if(ch== ENTER || ch == TAB)
		{
			pwd[i] = '\0';
			break;
		}
		else if(ch == BCKSPC){
			if(i>0){
				i--;
				printf("\b \b");
			}
		}
		else{
			pwd[i++] = ch;
			printf("*");
		}
	}
}
int isUsernameTaken(const char *filename, const char *username) {
    FILE *fp = fopen(filename, "r");
    char line[300], *token;
	int i;
    if (fp == NULL) return 0;

    while (fgets(line, sizeof(line), fp)) {
        token = strtok(line, ",");
        for (i = 0; token && i < 4; i++) {
            if (i == 3 && strcmp(token, username) == 0) {
                fclose(fp);
                return 1;
            }
            token = strtok(NULL, ",");
        }
    }
    
    fclose(fp);
    return 0;
}
void trimNewline(char *str){
	size_t len = strlen(str);
    while(len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }	
}
void chooseSpecialization(char specialization[50]){
	int highlight = 0;
    int key;
    int i;
    color(11);
    const char *specializations[] = {
        "Family Medicine",
        "Internal Medicine",
        "Pediatrics",
        "Dermatology",
        "Cardiology",
        "Hematology"
    };
    int numOptions = 6;
	
	color(12);
    printf("\nChoose your specialization:\n");

    while (1) {
    	color(11);
        for (i = 0; i < numOptions; i++) {
            if (i == highlight)
                printf("-> %s\n", specializations[i]);
            else
                printf("   %s\n", specializations[i]);
        }

        key = getch();
        
        // Move cursor up to redraw the list
        for (i = 0; i < numOptions; i++) {
            printf("\033[F"); // ANSI escape code to move cursor up
        }

        if (key == 0 || key == 224) {
            key = getch();
            switch (key) {
                case 72: // UP arrow
                    highlight = (highlight == 0) ? numOptions - 1 : highlight - 1;
                    break;
                case 80: // DOWN arrow
                    highlight = (highlight == numOptions - 1) ? 0 : highlight + 1;
                    break;
            }
        } else if (key == ENTER) {
            strcpy(specialization, specializations[highlight]);
            //printf("\nSelected specialization: %s\n", specialization);
            for ( i= 0; i<numOptions; i++)
            {
            	printf("\033[F");
        		printf("\033[2K\r");  
			}
			printf("\nSelected specialization: %s\n", specialization);
            break;
        }
    }
}
void registerDoctor() {
	FILE *fp;
    struct User user;
    char password2[50];
    
    color(12);
    printf("Enter your first name: ");
    color(11);
    takeinput(user.FirstName);
    color(12);
    printf("Enter your last name: ");
    color(11);
    takeinput(user.LastName);
    color(12);
    printf("Enter your email: ");
    color(11);
    takeinput(user.email);
    color(12);
    printf("Enter your contact no: ");
    color(11);
    takeinput(user.phone);
    color(12);
    printf("Enter your address: ");
    color(11);
    takeinput(user.address);
    color(12);
    printf("Enter your gender: ");
    color(11);
    takeinput(user.gender);
    color(12);
    printf("Enter your birth date (YYYY-MM-DD): ");
    color(11);
    takeinput(user.dateOfBirth);
    
    do{
    	color(12);
    	printf("Enter your username: ");
    	color(11);
    	takeinput(user.username);
    	if (isUsernameTaken("DoctorsFile.txt", user.username)){
    		printf("Username already taken. Input anotha one.\n");
		} else{
			break;
		}
	}while(1);
	color(12);
	printf("Enter your password: ");
	color(11);
    takepassword(user.password);
    color(12);
    printf("\nConfirm your password: ");
    color(11);
    takepassword(password2);

    if (strcmp(user.password, password2) != 0) {
        printf("\nPasswords do not match!\n");
        return;
    }
    
    chooseSpecialization(user.specialization);
    
    fp = fopen("DoctorsFile.txt", "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            user.FirstName, user.LastName, user.email,
            user.phone, user.username, user.password,
            user.address, user.gender, user.dateOfBirth,
			user.specialization);

    fclose(fp);
    printf("\nDoctor registration successful!\n");
    
    char doctorFolder[100];
    sprintf(doctorFolder, "Appointments\\%s", user.username);
    mkdir("Appointments");
    mkdir(doctorFolder);
    
    //printf("\nDoctor folder created at: %s\n", doctorFolder);
}
void displayRegisterMenu(int highlight){
	system("cls");
	color(12);
    printf("\t\t\t\t|-------------------------------------------------------------|\n");
	printf("\t\t\t\t|                      WELCOME TO MaAyo Serving               |\n");
	printf("\t\t\t\t|-------------------------------------------------------------|\n");
	color(11);
    printf("\t\t\t\t|-------------------------------------------------------------|\n");
    printf("\t\t\t\t|   %s [1] REGISTER AS PATIENT                                |\n", (highlight == 1 ? "->" : "  "), (highlight == 1 ? "" : "  "));
    printf("\t\t\t\t|   %s [2] REGISTER AS DOCTOR                                 |\n", (highlight == 2 ? "->" : "  "), (highlight == 2 ? "" : "  "));
    printf("\t\t\t\t|   %s [3] Log in                                             |\n", (highlight == 3 ? "->" : "  "), (highlight == 3 ? "" : "  "));
    printf("\t\t\t\t|   %s [4] Exit                                               |\n", (highlight == 4 ? "->" : "  "), (highlight == 4 ? "" : "  "));
    printf("\t\t\t\t|-------------------------------------------------------------|\n");
}
void logIn() {
    struct User user;
    char username[50], pword[50];
    int usrFound = 0;
    int isDoctor = 0;
    FILE *fp;

    system("cls");
    color(11);
    printf("\t\t\t\t|-------------------------------------------------------------|\n");
    color(12);
	printf("\t\t\t\t|                           MaAyo Serving                     |\n");
	color(11);
	printf("\t\t\t\t|-------------------------------------------------------------|\n");
    color(12);
	printf("\t\t\t\t                          Log In                               \n");
    printf("\t\t\t\t                        Username: ");
    color(11);
	takeinput(username);
	color(12);
    printf("\t\t\t\t                        Password: ");
    color(11);
    takepassword(pword);
    printf("\n");
    printf("\t\t\t\t|-------------------------------------------------------------|\n");

    fp = fopen("PatientFile.txt", "r");
    if (fp == NULL) {
        printf("\nError: Could not open Patients.txt\n");
    } else {
        while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.FirstName, user.LastName,user.email, user.phone, user.username, user.password, user.address, user.gender, user.dateOfBirth) != EOF) {
            printf("\nIncorrect username or password!");
            
            if (strcmp(user.username, username) == 0 && strcmp(user.password, pword) == 0) {
                usrFound = 1;
                fclose(fp);
                system("cls");
                printf("\nWelcome, %s! You have successfully logged in as a Patient.\n", user.FirstName);
                patientDashboard(user.username);
                return;
            }
        }
        fclose(fp);
    }
    
    fp = fopen("DoctorsFile.txt", "r");
    if (!usrFound && fp == NULL) {
        printf("\nError: Could not open Doctors.txt\n");
    } else if (!usrFound) {
        while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.FirstName, user.LastName,user.email, user.phone, user.username, user.password, user.address, user.gender, user.dateOfBirth, user.specialization) != EOF) {
            printf("\nIncorrect username or password!");
            
            if (strcmp(user.username, username) == 0 && strcmp(user.password, pword) == 0) {
                usrFound = 1;
                fclose(fp);
                system("cls");
                printf("\nWelcome, Dr. %s! You have successfully logged in as a Doctor.\n", user.FirstName);
                doctorDashBoard(user.username);
                return;
            }
        }
        fclose(fp);
    }
    if (!usrFound) {
        printf("\nUser not found! Would you like to register? (yes/no): ");
        char choice[10];
        int ambot;
        takeinput(choice);
        if (strcmp(choice, "yes") == 0 || strcmp(choice, "YES") == 0) {
            displayRegisterMenu(ambot);
        } else {
            printf("\nReturning to the main menu...\n");
        }
    }
}
void doctorDashBoard(char username[50]) {
    trimNewline(username);
    int choice = 1;
    int key;

    do {
        system("cls");
        color(12);
    	printf("\t\t\t\t|-------------------------------------------------------------|\n");
    	color(11);
		printf("\t\t\t\t|                         Doctor Dashboard                    |\n");
		color(12);
		printf("\t\t\t\t|-------------------------------------------------------------|\n");
        color(11);
        printf("\t\t\t\t\t\t%s\n", (choice == 1) ? "-> 1. View Profile" : "   1. View Profile");
        printf("\t\t\t\t\t\t%s\n", (choice == 2) ? "-> 2. View Appointments" : "   2. View Appointments");
        printf("\t\t\t\t\t\t%s\n", (choice == 3) ? "-> 3. Manage Appointments (Accept/Decline)" : "   3. Manage Appointments (Accept/Decline)");
        printf("\t\t\t\t\t\t%s\n", (choice == 4) ? "-> 4. View Patient Records" : "   4. View Patient Records");
        printf("\t\t\t\t\t\t%s\n", (choice == 5) ? "-> 5. Log Out" : "   5. Log Out");
        
        key = getch();
        if (key == 0 || key == 224) {
            key = getch();
            switch (key) {
                case 72: // UP arrow
                    choice = (choice == 1) ? 5 : choice - 1;
                    break;
                case 80: // DOWN arrow
                    choice = (choice == 5) ? 1 : choice + 1;
                    break;
            }
        } else if (key == 13) { // ENTER key
            system("cls");
            switch (choice) {
                case 1:
                    viewDoctorprofile(username); 
                    break;
                case 2:
                    viewAppointments(username);
                    break;
                case 3:
                    manageAppointments(username);
                    break;
                case 4:
                    viewPatientRecords(username);
                    break;
                case 5:
                    printf("\nLogging out...\n");
                    main();
                    return;
            }
            printf("\nPress any key to return to the dashboard...");
            getch();
        }
    } while (1);
}
void viewDoctorprofile(char username[50]){
	struct User user;
	int click;
	FILE *fp;
	fp = fopen("DoctorsFile.txt", "r");
	
	if (!fp) {
        printf("Error opening Doctors.txt\n");
        return;
    }

    trimNewline(username); // Ensure input is clean
    printf("Comparing file username: [%s] with input: [%s]\n", user.username, username);
	int found = 0;
	while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.FirstName, user.LastName,user.email, user.phone, user.username, 
					  user.password, user.address, user.gender, user.dateOfBirth, user.specialization) != EOF) {
        trimNewline(user.username);
		if (strcmp(user.username, username) == 0) {
            found = 1;
            break; 
        }
    }
    fclose(fp);

    if(!found)
    {
    	printf("\nError: User profile not found!\n");
    	return;
	}

	system("cls");
	color(12);
	printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");
	color(11);
	printf("\t\t\t\t|                                          Doctor Profile                                        |\n");
	color(12);
	printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");
    printf("\t                                                                                               \n");
    // First row
	color(12); // label color
	printf("\t\t\t\tFirst Name    : ");
	color(11); // output color
	printf("%s  ", user.FirstName);
	
	color(12);
	printf("\tGender       : ");
	color(11);
	printf("%s  ", user.gender);
	
	color(12);
	printf("\tEmail: ");
	color(11);
	printf("%s\n", user.email);
	
	// Second row
	color(12);
	printf("\t\t\t\tLast Name     : ");
	color(11);
	printf("%s  ", user.LastName);
	
	color(12);
	printf("\tDate of Birth: ");
	color(11);
	printf("%s  ", user.dateOfBirth);
	
	color(12);
	printf("\tAddress: ");
	color(11);
	printf("%s\n", user.address);
	
	// Third row
	color(12);
	printf("\t\t\t\tSpecialization: ");
	color(11);
	printf("%s\n", user.specialization);
	
	// Fourth row
	color(12);
	printf("\t\t\t\tUserName      : ");
	color(11);
	printf("%s\n", user.username);

    printf("\n");
    printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");

    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    while(getchar() != '\n'); // Clear input buffer

    if(click == 1)
    {
    	doctorDashBoard(username);  // Ensure this matches exactly!
	}
}
void registerPatient(){
	FILE *fp;
    struct User user;
    char username[50], password2[50];
    char doctorUsername[50], doctorName[50];
    
    color(12);
    printf("Enter your first name: ");
    color(11);
    takeinput(user.FirstName);
    color(12);
    printf("Enter your last name: ");
    color(11);
    takeinput(user.LastName);
    color(12);
    printf("Enter your email: ");
    color(11);
    takeinput(user.email);
    color(12);
    printf("Enter your contact no: ");
    color(11);
    takeinput(user.phone);
    color(12);
    printf("Enter your address: ");
    color(11);
    takeinput(user.address);
    color(12);
    printf("Enter your gender: ");
    color(11);
    takeinput(user.gender);
    color(12);
    printf("Enter your birth date (YYYY-MM-DD): ");
    color(11);
    takeinput(user.dateOfBirth);
    
    do{
    	color(12);
    	printf("Enter your username: ");
    	color(11);
    	takeinput(user.username);
    	if (isUsernameTaken("PatientFile.txt", user.username)){
    		printf("Username already taken. Input another one.\n");
		} else{
			break;
		}
	}while(1);
	
	color(12);
	printf("Enter your password: ");
	color(11);
    takepassword(user.password);
    color(12);
    printf("\nConfirm your password: ");
    color(11);
    takepassword(password2);

    if (strcmp(user.password, password2) != 0) {
        printf("\nPasswords do not match!\n");
        return;
    }
    
    printf("\nSelect Doctor:\n");
    selectDoctor(doctorUsername, doctorName);
    strcpy(user.assignedDoctor, doctorUsername);

    fp = fopen("PatientFile.txt", "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            user.FirstName, user.LastName, user.email,
            user.phone, user.username, user.password,
            user.address, user.gender, user.dateOfBirth,
		  user.assignedDoctor);

    fclose(fp);
    
    //system("cls");
    
    createMedicalRecord(user.username, doctorUsername);
    
    mkdir("Appointments");
    char doctorFolder[100], patientFolder[100];
    sprintf(doctorFolder, "Appointments\\%s", doctorUsername);
    mkdir(doctorFolder);
    sprintf(patientFolder, "%s\\%s", doctorFolder, user.username);
    mkdir(patientFolder);
    
    printf("\nPatient registration successful!\n");
}
void selectDoctor(char doctorUsername[50], char doctorName[50]) {
    FILE *fp = fopen("DoctorsFile.txt", "r");
    if (fp == NULL) {
        printf("\nError: No doctors available!\n");
        strcpy(doctorUsername, "");
        strcpy(doctorName, "");
        return;
    }
    
    struct User doctor;
    int doctorCount = 0;
    char doctorNames[100][100];
    char doctorUsernames[100][50];
    
    printf("\nAvailable Doctors:\n");
    printf("-----------------\n");
    
    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.FirstName, doctor.LastName, doctor.email, doctor.phone, 
                  doctor.username, doctor.password, doctor.address, doctor.gender, 
                  doctor.dateOfBirth, doctor.specialization) != EOF) {
        doctorCount++;
        printf("%d. Dr. %s %s (%s)\n", doctorCount, doctor.FirstName, doctor.LastName, doctor.specialization);
        sprintf(doctorNames[doctorCount-1], "Dr. %s %s", doctor.FirstName, doctor.LastName);
        strcpy(doctorUsernames[doctorCount-1], doctor.username);
    }
    fclose(fp);
    
    if (doctorCount == 0) {
        printf("\nNo doctors available!\n");
        strcpy(doctorUsername, "");
        strcpy(doctorName, "");
        return;
    }
    
    int choice;
    do {
        printf("\nSelect your doctor (1-%d): ", doctorCount);
        scanf("%d", &choice);
        while(getchar() != '\n'); // Clear input buffer
        
        if (choice < 1 || choice > doctorCount) {
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice < 1 || choice > doctorCount);
    
    strcpy(doctorName, doctorNames[choice-1]);
    strcpy(doctorUsername, doctorUsernames[choice-1]);
}
void createMedicalRecord(const char patientUsername[50], const char doctorUsername[50]) {
    char firstName[50], lastName[50], email[100], phone[20], username[50], password[50], address[100], gender[20], dob[20], assignedDoctor[50];
    int found = 0;
    
    FILE *fpPatient = fopen("PatientFile.txt", "r");
    if (!fpPatient) {
        printf("\nError opening PatientFile.txt!\n");
        return;
    }
    
    char line[512];
    while (fgets(line, sizeof(line), fpPatient)) {
        sscanf(line, "%49[^,],%49[^,],%99[^,],%19[^,],%49[^,],%49[^,],%99[^,],%19[^,],%19[^,],%49[^\n]",
               firstName, lastName, email, phone, username, password, address, gender, dob, assignedDoctor);
        if (strcmp(username, patientUsername) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fpPatient);
    
    if (!found) {
        printf("\nPatient details not found!\n");
        return;
    }
    
    char docFirstName[50], docLastName[50], docEmail[100], docPhone[20], docUsername[50], docPassword[50], docAddress[100], docGender[20], docDob[20], docSpecialization[100];
    int doctorFound = 0;
    
    FILE *fpDoctor = fopen("DoctorsFile.txt", "r");
    if (!fpDoctor) {
        printf("\nError opening DoctorFile.txt!\n");
        return;
    }
    
    char docLine[512];
    while (fgets(docLine, sizeof(docLine), fpDoctor)) {
        sscanf(docLine, "%49[^,],%49[^,],%99[^,],%19[^,],%49[^,],%49[^,],%99[^,],%19[^,],%19[^,],%49[^\n]",
               docFirstName, docLastName, docEmail, docPhone, docUsername, docPassword, docAddress, docGender, docDob, docSpecialization);
        if (strcmp(docUsername, doctorUsername) == 0) {
            doctorFound = 1;
            break;
        }
    }
    fclose(fpDoctor);
    
    if (!doctorFound) {
        printf("\nDoctor details not found!\n");
        return;
    }
    
    char today[20];
    getTodayDate(today);
    
    char recordPath[200];
    sprintf(recordPath, "MedicalRecords\\%s.txt", patientUsername);
    FILE *fpRecord = fopen(recordPath, "w");
    if (!fpRecord) {
        printf("\nError opening MedicalRecord.txt for writing!\n");
        return;
    }
    
    fprintf(fpRecord, "Patient Name: %s %s\n", firstName, lastName);
    fprintf(fpRecord, "Date of Last Update: %s\n", today);
    fprintf(fpRecord, "Gender: %s\n", gender);
    fprintf(fpRecord, "Date of Birth: %s\n", dob);
    fprintf(fpRecord, "Primary Doctor: Dr. %s %s (%s)\n\n", docFirstName, docLastName, docSpecialization);

    char input[256];
    
    fprintf(fpRecord, "Childhood Illnesses:\n");
    printf("\nMedical History Form\n");
    printf("--------------------\n");
    printf("Enter Childhood Illnesses (type 'done' to finish):\n");
    int count = 1;
    while (1) {
        printf("%d. ", count);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "done") == 0) break;
        fprintf(fpRecord, "%d. %s\n", count++, input);
    }
    
    fprintf(fpRecord, "\nList of Medical Problems:\n");
    printf("\nEnter List of Medical Problems (type 'done' to finish):\n");
    count = 1;
    while (1) {
        printf("%d. ", count);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "done") == 0) break;
        fprintf(fpRecord, "%d. %s\n", count++, input);
    }
    
    fprintf(fpRecord, "\nSurgeries:\n");
    printf("\nEnter Surgeries (format: Year, Reason, Hospital; type 'done' to finish):\n");
    while (1) {
        printf("Year, Reason, Hospital: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "done") == 0) break;
        fprintf(fpRecord, "%s\n", input);
    }
    
    fprintf(fpRecord, "\nAllergies to Medications:\n");
    printf("\nEnter Allergies to Medications (format: Drug, Reaction; type 'done' to finish):\n");
    while (1) {
        printf("Drug, Reaction: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "done") == 0) break;
        fprintf(fpRecord, "%s\n", input);
    }
    
    fprintf(fpRecord, "\nCurrent Medications:\n");
    printf("\nEnter Current Medications (type 'done' to finish):\n");
    count = 1;
    while (1) {
        printf("%d. ", count);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "done") == 0) break;
        fprintf(fpRecord, "%d. %s\n", count++, input);
    }
    
    fclose(fpRecord);
    printf("\nMedical record created successfully!\n");
}
void patientDashboard(char username[50]) {
    trimNewline(username);
    int choice = 1;
    int key;

    do {
        system("cls");
        color(12);
    	printf("\t\t\t\t|-------------------------------------------------------------|\n");
    	color(11);
		printf("\t\t\t\t|                         Patient Dashboard                   |\n");
		color(12);
		printf("\t\t\t\t|-------------------------------------------------------------|\n");
        color(11);
        printf("\t\t\t\t\t\t%s\n", (choice == 1) ? "-> 1. View Profile" : "   1. View Profile");
        printf("\t\t\t\t\t\t%s\n", (choice == 2) ? "-> 2. Schedule an Appointment" : "   2. Schedule an Appointment");
        printf("\t\t\t\t\t\t%s\n", (choice == 3) ? "-> 3. View Appointments" : "   3. View Appointments");
        printf("\t\t\t\t\t\t%s\n", (choice == 4) ? "-> 4. Log Out" : "   4. Log Out");
        
        key = getch();
        if (key == 0 || key == 224) {
            key = getch();
            switch (key) {
                case 72: // UP arrow
                    choice = (choice == 1) ? 5 : choice - 1;
                    break;
                case 80: // DOWN arrow
                    choice = (choice == 5) ? 1 : choice + 1;
                    break;
            }
        } else if (key == 13) { // ENTER key
            system("cls");
            switch (choice) {
                case 1:
                    viewPatientprofile(username); 
                    break;
                case 2:
                	createAppointment(username);
                    break;
                case 3:
                    viewAppointment(username);
                    break;
                case 4:
                    printf("\nLogging out...\n");
                    main();
                    return;
            }
            printf("\nPress any key to return to the mainboard...");
            getch();
            
        }
    } while (1);
}
void viewPatientprofile(char username[50]){
	struct User user;
	int click;
	FILE *fp;
	fp = fopen("PatientFile.txt", "r");
	
	if (!fp) {
        printf("Error opening PatientFile.txt\n");
        return;
    }

    trimNewline(username); // Ensure input is clean
    printf("Runtime Error Testing");
    printf("Comparing file username: [%s] with input: [%s]\n", user.username, username);
	int found = 0;
	while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.FirstName, user.LastName,user.email, user.phone, user.username, 
					  user.password, user.address, user.gender, user.dateOfBirth) != EOF) {
        trimNewline(user.username);
		if (strcmp(user.username, username) == 0) {
            found = 1;
            break; 
        }
    }
    fclose(fp);

    if(!found)
    {
    	printf("\nError: User profile not found!\n");
    	return;
	}

	system("cls");
   color(12);
	printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");
	color(11);
	printf("\t\t\t\t|                                          Patient Profile                                       |\n");
	color(12);
	printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");
	    printf("\t                                                                                               \n");
    color(12);
	printf("\t\t\t\tFirst Name    : ");
	color(11);
	printf("%s  ", user.FirstName);
	
	color(12);
	printf("\tGender       : ");
	color(11);
	printf("%s  ", user.gender);
	
	color(12);
	printf("\tEmail: ");
	color(11);
	printf("%s\n", user.email);
	
	// Second row
	color(12);
	printf("\t\t\t\tLast Name     : ");
	color(11);
	printf("%s  ", user.LastName);
	
	color(12);
	printf("\tDate of Birth: ");
	color(11);
	printf("%s  ", user.dateOfBirth);
	
	color(12);
	printf("\tAddress: ");
	color(11);
	printf("%s\n", user.address);
	
	// Third row
	color(12);
	printf("\t\t\t\tUserName      : ");
	color(11);
	printf("%s\n", user.username);
	    printf("\n");
    printf("\t\t\t\t|------------------------------------------------------------------------------------------------|\n");

    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    while(getchar() != '\n'); // Clear input buffer

    if(click == 1)
    {
    	patientDashboard(username);  // Ensure this matches exactly!
	}
}
void createAppointment(char username[50]){
	int click;
    if (patientAppointment != NULL) {
        printf("\nYou already have an appointment scheduled with Dr. %s on %s at %s.\n", 
               patientAppointment->doctorName, patientAppointment->date, patientAppointment->time);
        return;
    }

    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    if (newAppointment == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }
    
      FILE *fp = fopen("PatientFile.txt", "r");
    if (!fp) {
        printf("\nError opening PatientFile.txt\n");
        free(newAppointment);
        return;
    }

    struct User temp;
    int found = 0;
    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  temp.FirstName, temp.LastName, temp.email, temp.phone, temp.username, temp.password,
                  temp.address, temp.gender, temp.dateOfBirth, temp.assignedDoctor) != EOF) {
        if (strcmp(temp.username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    
     if (!found) {
        printf("\nPatient not found.\n");
        free(newAppointment);
        return;
    }
    
     strcpy(newAppointment->doctorUsername, temp.assignedDoctor);
     
      fp = fopen("DoctorsFile.txt", "r");
    if (!fp) {
        printf("\nError opening DoctorsFile.txt\n");
        free(newAppointment);
        return;
    }

    struct User doctor;
    found = 0;
    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.FirstName, doctor.LastName, doctor.email, doctor.phone, doctor.username, doctor.password,
                  doctor.address, doctor.gender, doctor.dateOfBirth, doctor.specialization) != EOF) {
        if (strcmp(doctor.username, newAppointment->doctorUsername) == 0) {
            sprintf(newAppointment->doctorName, "%s %s", doctor.FirstName, doctor.LastName);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("\nAssigned doctor not found in DoctorsFile.txt\n");
        free(newAppointment);
        return;
    }


    printf("Enter Appointment Date (MM-DD-YYYY): ");
    takeinput(newAppointment->date);
    printf("Enter Appointment Time (HHMM AM/PM): ");
    takeinput(newAppointment->time);
    
    newAppointment->next = NULL;
    patientAppointment = newAppointment;
	
	//Folder
    mkdir("Appointments");
    char doctorFolder[100], patientFolder[150];
    sprintf(doctorFolder, "Appointments\\%s", newAppointment->doctorUsername);
    mkdir(doctorFolder);

    sprintf(patientFolder, "%s\\%s", doctorFolder, username);
    mkdir(patientFolder);
    
    //Doctor.txt
    char pathApptTxt[200];
    sprintf(pathApptTxt, "%s\\%s_%s.txt", patientFolder, newAppointment->date, newAppointment->time);
    FILE *fpAppt = fopen(pathApptTxt, "w");
    if (fpAppt != NULL) {
    	   fprintf(fpAppt, "Pending\n");
        fprintf(fpAppt, "Username: %s\nAssigned Doctor: Dr. %s\n\n", username, newAppointment->doctorName);
        fprintf(fpAppt, "Appointment Date: %s\nAppointment Time: %s\n\n", newAppointment->date, newAppointment->time);
        fprintf(fpAppt, "Feedback:\n");
        fclose(fpAppt);
    }else {
        printf("\nError creating appointment file!\n");
        free(newAppointment);
        return;
    }
    
    printf("\nAppointment successfully scheduled with Dr. %s on %s at %s.\n",
           newAppointment->doctorName, newAppointment->date, newAppointment->time);
    
	printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if(click == 1)
    {
    	patientDashboard(username);
	}
}
int folderExists(const char *path) {
    struct stat info;
    if (stat(path, &info) != 0) {
        return 0; // does not exist
    } else if (info.st_mode & S_IFDIR) {
        return 1; // exists and is directory
    } else {
        return 0; // exists but not directory
    }
}
void viewAppointment(char username[50]) {
    int click;
    char line [200];

    FILE *fp = fopen("DoctorsFile.txt", "r");
    if (fp == NULL) {
        printf("\nError: Unable to open DoctorsFile.txt!\n");
        return;
    }

    struct User doctor;
    int found = 0;

    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.FirstName, doctor.LastName, doctor.email, doctor.phone, doctor.username, doctor.password) != EOF) {
        
        char patientFolder[200];
        sprintf(patientFolder, "MedicalRecords\\%s\\%s", doctor.username, username);

        if (folderExists(patientFolder)) {
            char doctorPath[300], schedulePath[300], statusPath[300];
            char doctorName[100], date[20], time[20], status[20];

            // Read Doctor.txt
            sprintf(doctorPath, "%s\\Doctor.txt", patientFolder);
            FILE *fpDoctor = fopen(doctorPath, "r");
            if (fpDoctor != NULL) {
                while (fgets(line, sizeof(line), fpDoctor)) {
			        if (sscanf(line, "Assigned Doctor: %[^\n]", doctorName) == 1) {
			            break;
			        }
			    }
			    fclose(fpDoctor);
            } else {
                strcpy(doctorName, "Unknown");
            }

            // Read Schedule.txt
            sprintf(schedulePath, "%s\\Schedule.txt", patientFolder);
            FILE *fpSchedule = fopen(schedulePath, "r");
            if (fpSchedule != NULL) {
                while (fgets(line, sizeof(line), fpSchedule)) {
			        if (sscanf(line, "Appointment Date: %[^\n]", date) == 1) {
			            continue;
			        }
			        if (sscanf(line, "Appointment Time: %[^\n]", time) == 1) {
			            continue;
			        }
			    }
			    fclose(fpSchedule);
            } else {
                strcpy(date, "Unknown");
                strcpy(time, "Unknown");
            }

            // Read Status.txt
            sprintf(statusPath, "%s\\Status.txt", patientFolder);
            FILE *fpStatus = fopen(statusPath, "r");
            if (fpStatus != NULL) {
                if (fgets(status, sizeof(status), fpStatus)) {
			        strtok(status, "\n");
			    }
			    fclose(fpStatus);
            } else {
                strcpy(status, "Trial");
            }

            printf("\n---------------- Your Appointment ----------------\n");
            printf("Doctor: %s\nDate: %s\nTime: %s\nStatus: %s\n", doctorName, date, time, status);
            printf("--------------------------------------------------\n");
            found = 1;
            break;  // stop after finding first match
        }
    }
    fclose(fp);

    if (!found) {
        printf("\nNo appointment scheduled.\n");
    }

    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if(click == 1) {
        patientDashboard(username);
    }
}
void manageAppointments(char doctorUsername[50]) {
    int click;
    char doctorFolder[100];
    sprintf(doctorFolder, "Appointments\\%s", doctorUsername);

    DIR *dir = opendir(doctorFolder);
    if (dir == NULL) {
        printf("\nDoctor folder not found: %s\n", doctorFolder);
        return;
    }

    struct dirent *entry;
    char patients[100][50];
    int patientCount = 0;
    
    struct AcceptedAppointment *acceptedHead = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char fullPath[200];
            sprintf(fullPath, "%s\\%s", doctorFolder, entry->d_name);

            DWORD attr = GetFileAttributes(fullPath);
            if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
                strcpy(patients[patientCount], entry->d_name);
                patientCount++;
            }
        }
    }
    closedir(dir);

    if (patientCount == 0) {
        printf("\nNo scheduled appointments to manage.\n");
        return;
    }

    int selected = 0;
    int choice;
    int i;

    do {
        system("cls");
        printf("\n--------- Manage Appointments ---------\n");
        for (i = 0; i < patientCount; i++) {
            if (i == selected) {
                printf("-> %s\n", patients[i]);
            } else {
                printf("   %s\n", patients[i]);
            }
        }
        printf("\nUse Up/Down to select, Enter to manage, 0 to exit.\n");

        choice = getch();  // capture arrow keys or Enter

        if (choice == 72) { // Up arrow
            if (selected > 0) selected--;
        } else if (choice == 80) { // Down arrow
            if (selected < patientCount - 1) selected++;
        } else if (choice == 13) { // Enter
            // Manage selected appointment
            char statusPath[200];
            sprintf(statusPath, "%s\\%s\\Status.txt", doctorFolder, patients[selected]);

            printf("\nManaging appointment for: %s\n", patients[selected]);
            printf("Set status:\n");
            printf("1. Accept\n");
            printf("2. Decline\n");
            printf("Choice: ");

            int statusChoice;
            scanf("%d", &statusChoice);
            while(getchar() != '\n');  // clear input buffer

            FILE *fpStatus = fopen(statusPath, "w");
            if (fpStatus != NULL) {
                if (statusChoice == 1) {
                    fprintf(fpStatus, "Accepted\n");
                    printf("\nAppointment marked as ACCEPTED.\n");

                    // Read date and time from Schedule.txt
                    char schedulePath[200];
                    sprintf(schedulePath, "%s\\%s\\Schedule.txt", doctorFolder, patients[selected]);
                    FILE *fpSchedule = fopen(schedulePath, "r");
                    if (fpSchedule != NULL) {
                        char dateLine[100], timeLine[100];
                        char date[20], time[10];
                        if (fgets(dateLine, sizeof(dateLine), fpSchedule) &&
                            fgets(timeLine, sizeof(timeLine), fpSchedule)) {
                            sscanf(dateLine, "Appointment Date: %[^\n]", date);
                            sscanf(timeLine, "Appointment Time: %[^\n]", time);

                            // Create new accepted appointment node
                            struct AcceptedAppointment *newNode = (struct AcceptedAppointment *)malloc(sizeof(struct AcceptedAppointment));
                            if (newNode != NULL) {
                                strcpy(newNode->patientUsername, patients[selected]);
                                strcpy(newNode->doctorUsername, doctorUsername);
                                strcpy(newNode->date, date);
                                strcpy(newNode->time, time);
                                newNode->next = acceptedHead;
                                acceptedHead = newNode;
                            }
                        }
                        fclose(fpSchedule);
                    } else {
                        printf("Warning: Could not open Schedule.txt to read date/time.\n");
                    }
                } else if (statusChoice == 2) {
                    fprintf(fpStatus, "Declined\n");
                    printf("\nAppointment marked as DECLINED.\n");
                } else {
                    printf("\nInvalid choice. No changes made.\n");
                }
                fclose(fpStatus);
            } else {
                printf("\nError: Could not open status file!\n");
            }

            printf("\nPress any key to continue...\n");
            getch();
        }

    } while (choice != '0');

    printf("\nReturning to doctor dashboard...\n");
    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if (click == 1) {
        doctorDashBoard(doctorUsername);
    }
}
int compareAppointments(const void *a, const void *b) {
    struct AppointmentEntry *appA = (struct AppointmentEntry *)a;
    struct AppointmentEntry *appB = (struct AppointmentEntry *)b;

    char dayA[3], monthA[3], yearA[5];
    char dayB[3], monthB[3], yearB[5];

    sscanf(appA->date, "%2[^/]/%2[^/]/%4s", dayA, monthA, yearA);
    sscanf(appB->date, "%2[^/]/%2[^/]/%4s", dayB, monthB, yearB);

    int cmpYear = strcmp(yearA, yearB);
    if (cmpYear != 0) return cmpYear;

    int cmpMonth = strcmp(monthA, monthB);
    if (cmpMonth != 0) return cmpMonth;

    int cmpDay = strcmp(dayA, dayB);
    if (cmpDay != 0) return cmpDay;

    return strcmp(appA->time, appB->time);
}
void getPatientFullName(const char username[50], char fullName[100]) {
    FILE *fp = fopen("PatientFile.txt", "r");
    if (fp == NULL) {
        strcpy(fullName, "Unknown");
        return;
    }

    char line[300];
    char firstName[50], lastName[50], email[50], phone[50], user[50], pass[50];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]",
               firstName, lastName, email, phone, user, pass);

        if (strcmp(user, username) == 0) {
            sprintf(fullName, "%s %s", firstName, lastName);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    strcpy(fullName, "Unknown");
}
void viewAppointments(char doctorUsername[50]) {
    char doctorFolder[100];
    int i;
    sprintf(doctorFolder, "Appointments\\%s", doctorUsername);

    DIR *dir = opendir(doctorFolder);
    if (dir == NULL) {
        printf("\nError: Unable to open folder %s!\n", doctorFolder);
        return;
    }

    struct dirent *entry;
    struct AppointmentEntry appointments[100];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char patientFolder[150];
        sprintf(patientFolder, "%s\\%s", doctorFolder, entry->d_name);

        char statusPath[200];
        sprintf(statusPath, "%s\\Status.txt", patientFolder);
        FILE *fpStatus = fopen(statusPath, "r");
        if (fpStatus == NULL) continue;

        char status[20];
        fgets(status, sizeof(status), fpStatus);
        strtok(status, "\n");
        fclose(fpStatus);

        if (strcmp(status, "Accepted") != 0) continue;

        char schedulePath[200];
        sprintf(schedulePath, "%s\\Schedule.txt", patientFolder);
        FILE *fpSchedule = fopen(schedulePath, "r");
        if (fpSchedule == NULL) continue;

        char dateLine[50], timeLine[50], date[20], time[20];
        fgets(dateLine, sizeof(dateLine), fpSchedule);
        sscanf(dateLine, "Appointment Date: %19[^\n]", date);
        fgets(timeLine, sizeof(timeLine), fpSchedule);
        sscanf(timeLine, "Appointment Time: %19[^\n]", time);
        fclose(fpSchedule);

        strcpy(appointments[count].patientUsername, entry->d_name);
        getPatientFullName(entry->d_name, appointments[count].patientFullName);
        strcpy(appointments[count].date, date);
        strcpy(appointments[count].time, time);
        count++;
    }
    closedir(dir);

    if (count == 0) {
        printf("\nNo accepted appointments found.\n");
        return;
    }

    qsort(appointments, count, sizeof(struct AppointmentEntry), compareAppointments);

    printf("\n--------- Accepted Appointments (Sorted) ---------\n");
    for (i = 0; i < count; i++) {
        printf("%d. Patient: %s \n   Date: %s\n   Time: %s\n",
               i + 1, appointments[i].patientFullName,
               appointments[i].date, appointments[i].time);
    }
    printf("--------------------------------------------------\n");
}
void getTodayDate(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 20, "%d/%m/%Y", t);
}
void displayPatientMedicalRecord(const char doctorUsername[50], const char patientUsername[50]) {
	FILE *fpPatient = fopen("PatientFile.txt", "r");
    if (!fpPatient) {
        printf("\nError opening PatientFile.txt!\n");
        return;
    }
    char doctorFolder[100];
    sprintf(doctorFolder, "MedicalRecords\\%s\\%s\\MedicalRecord.txt", doctorUsername, patientUsername);

    FILE *fp = fopen(doctorFolder, "r");
    if (!fp) {
        printf("\nNo medical record found for this patient.\n");
        return;
    }

    char line[256];
    printf("\n===========================================\n");
    printf("         PATIENT MEDICAL RECORD\n");
    printf("===========================================\n");

    int inChildhoodIllness = 0, inMedicalProblems = 0, inSurgeries = 0, inAllergies = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Strip newline
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "Patient Name:", 13) == 0 || 
            strncmp(line, "Date of Last Update:", 20) == 0 ||
            strncmp(line, "Gender:", 7) == 0 ||
            strncmp(line, "Date of Birth:", 14) == 0 ||
            strncmp(line, "Previous or Referring Doctor:", 29) == 0) {
            printf("%s\n", line);
        }
        else if (strstr(line, "Childhood Illnesses")) {
            printf("\nPersonal Health History\n");
            printf("Childhood Illnesses:\n");
            inChildhoodIllness = 1; inMedicalProblems = inSurgeries = inAllergies = 0;
        }
        else if (strstr(line, "List of Medical Problems")) {
            printf("\nList of Medical Problems:\n");
            inMedicalProblems = 1; inChildhoodIllness = inSurgeries = inAllergies = 0;
        }
        else if (strstr(line, "Surgeries")) {
            printf("\nSurgeries:\n");
            printf("%-20s %-25s %-20s\n", "Year", "Reason", "Hospital");
            inSurgeries = 1; inChildhoodIllness = inMedicalProblems = inAllergies = 0;
        }
        else if (strstr(line, "Allergies to Medications")) {
            printf("\nAllergies to Medications:\n");
            printf("%-25s %-30s\n", "Name of the Drug", "Reaction You Had");
            inAllergies = 1; inChildhoodIllness = inMedicalProblems = inSurgeries = 0;
        }
        else if (strncmp(line, "Diagnosis:", 9) == 0) {
            printf("\n%s\n", line);
        }
        else if (strncmp(line, "Prescription:", 12) == 0) {
            printf("%s\n", line);
        }
        else {
            if (inChildhoodIllness || inMedicalProblems) {
                printf("%s\n", line);
            }
            else if (inSurgeries) {
                printf("%s\n", line);
            }
            else if (inAllergies) {
                printf("%s\n", line);
            }
        }
    }

    fclose(fp);
    printf("===========================================\n");
}
void viewPatientRecords(char doctorUsername[50]) {
    char today[20];
    getTodayDate(today);
    int i;

    char doctorFolder[100];
    sprintf(doctorFolder, "MedicalRecords\\%s", doctorUsername);

    DIR *dir = opendir(doctorFolder);
    if (!dir) {
        printf("\nNo appointments found.\n");
        return;
    }

    struct dirent *entry;
    struct AppointmentEntry *head = NULL, *tail = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char schedulePath[200];
        sprintf(schedulePath, "%s\\%s\\Schedule.txt", doctorFolder, entry->d_name);

        FILE *fp = fopen(schedulePath, "r");
        if (!fp) continue;

        char dateLine[100], timeLine[100], date[20], time[20];
        fgets(dateLine, sizeof(dateLine), fp);
        fgets(timeLine, sizeof(timeLine), fp);
        fclose(fp);

        sscanf(dateLine, "Appointment Date: %[^\n]", date);
        sscanf(timeLine, "Appointment Time: %[^\n]", time);

        if (strcmp(date, today) == 0) {
            struct AppointmentEntry *newNode = (struct AppointmentEntry*)malloc(sizeof(struct AppointmentEntry));
            strcpy(newNode->patientUsername, entry->d_name);
            strcpy(newNode->date, date);
            strcpy(newNode->time, time);
            newNode->next = NULL;

            if (!head) head = tail = newNode;
            else { tail->next = newNode; tail = newNode; }
        }
    }
    closedir(dir);

    if (!head) {
        printf("\nNo patients scheduled for today (%s).\n", today);
        return;
    }

    int index = 1;
    struct AppointmentEntry *curr = head;
    printf("\nPatients Scheduled for Today (%s):\n", today);
    while (curr) {
        char fullName[100];
        getPatientFullName(curr->patientUsername, fullName);
        printf("%d. %s\n", index++, fullName);
        curr = curr->next;
    }

    int choice;
    printf("\nSelect patient number to update record: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    curr = head;
    for (i = 1; i < choice && curr; i++) curr = curr->next;
    if (!curr) {
        printf("Invalid choice.\n");
        return;
    }
	
	printf("\n1. View Medical Record\n2. Update Medical Record\nChoose option: ");
    int action;
    scanf("%d", &action);
    while(getchar() != '\n');
    
    if (action == 1) {
        displayPatientMedicalRecord(doctorUsername, curr->patientUsername);
    }
    else if (action == 2){
    	char firstName[50], lastName[50], email[100], phone[20], username[50], password[50], address[100], gender[20], dob[20];
		int found = 0;
		
		char docFirstName[50], docLastName[50], docEmail[100], docPhone[20], docUsername[50], docPassword[50], docSpecialization[100];
		int doctorFound = 0;
        FILE *fpPatient = fopen("PatientFile.txt", "r");
		if (!fpPatient) {
		    printf("\nError opening PatientFile.txt!\n");
		    return;
		}
		
		FILE *fpDoctor = fopen("DoctorsFile.txt", "r");
		if (!fpDoctor) {
		    printf("\nError opening DoctorFile.txt!\n");
		    return;
		}
		
		char line[512];
		while (fgets(line, sizeof(line), fpPatient)) {
		    sscanf(line, "%49[^,],%49[^,],%99[^,],%19[^,],%49[^,],%49[^,],%99[^,],%19[^,],%19[^\n]",
		           firstName, lastName, email, phone, username, password, address, gender, dob);
		    if (strcmp(username, curr->patientUsername) == 0) {
		        found = 1;
		        break;
		    }
		}
		char docLine[512];
		while (fgets(docLine, sizeof(docLine), fpDoctor)) {
		    sscanf(docLine, "%49[^,],%49[^,],%99[^,],%19[^,],%49[^,],%49[^,],%99[^\n]",
		           docFirstName, docLastName, docEmail, docPhone, docUsername, docPassword, docSpecialization);
		    if (strcmp(docUsername, doctorUsername) == 0) {
		        doctorFound = 1;
		        break;
		    }
		}
		fclose(fpPatient);
		fclose(fpDoctor);
		
		if (!found) {
		    printf("\nPatient details not found!\n");
		    return;
		}
		
		if (!doctorFound) {
		    printf("\nDoctor details not found!\n");
		    return;
		}

		char recordPath[200];
		sprintf(recordPath, "MedicalRecords\\%s\\%s\\MedicalRecord.txt", doctorUsername, curr->patientUsername);
		FILE *fpRecord = fopen(recordPath, "w");
		if (!fpRecord) {
		    printf("\nError opening MedicalRecord.txt for writing!\n");
		    return;
		}
		
		fprintf(fpRecord, "Patient Name: %s %s\n", firstName, lastName);
		fprintf(fpRecord, "Date of Last Update: %s\n", today);
		fprintf(fpRecord, "Gender: %s\n", gender);
		fprintf(fpRecord, "Date of Birth: %s\n", dob);
		fprintf(fpRecord, "Previous or Referring Doctor: Dr. %s %s \n\n", docFirstName, docLastName);
	
	    char input[256];
		
		fprintf(fpRecord, "Childhood Illnesses:\n");
	    printf("\nEnter Childhood Illnesses (type 'done' to finish):\n");
	    int count = 1;
	    while (1) {
	        printf("%d. ", count);
	        fgets(input, sizeof(input), stdin);
	        input[strcspn(input, "\n")] = 0;
	        if (strcmp(input, "done") == 0) break;
	        fprintf(fpRecord, "%d. %s\n", count++, input);
	    }
	
	    fprintf(fpRecord, "List of Medical Problems:\n");
		printf("\nEnter List of Medical Problems (type 'done' to finish):\n");
	    count = 1;
	    while (1) {
	        printf("%d. ", count);
	        fgets(input, sizeof(input), stdin);
	        input[strcspn(input, "\n")] = 0;
	        if (strcmp(input, "done") == 0) break;
	        fprintf(fpRecord, "%d. %s\n", count++, input);
	    }
		
		fprintf(fpRecord, "Surgeries:\n");
	    printf("\nEnter Surgeries (format: Year, Reason, Hospital; type 'done' to finish):\n");
	    while (1) {
	        printf("Year, Reason, Hospital: ");
	        fgets(input, sizeof(input), stdin);
	        input[strcspn(input, "\n")] = 0;
	        if (strcmp(input, "done") == 0) break;
	        fprintf(fpRecord, "%s\n", input);
	    }
		
		fprintf(fpRecord, "Allergies to Medications:\n");
	    printf("\nEnter Allergies to Medications (format: Drug, Reaction; type 'done' to finish):\n");
	    while (1) {
	        printf("Drug, Reaction: ");
	        fgets(input, sizeof(input), stdin);
	        input[strcspn(input, "\n")] = 0;
	        if (strcmp(input, "done") == 0) break;
	        fprintf(fpRecord, "%s\n", input);
	    }
	
	    printf("\nEnter Final Diagnosis: ");
	    fgets(input, sizeof(input), stdin);
	    input[strcspn(input, "\n")] = 0;
	    fprintf(fpRecord, "Diagnosis: %s\n", input);
	
	    printf("\nEnter Prescription: ");
	    fgets(input, sizeof(input), stdin);
	    input[strcspn(input, "\n")] = 0;
	    fprintf(fpRecord, "Prescription: %s\n", input);
	
	    fclose(fpRecord);
	    printf("\nMedical record updated successfully!\n");
	}
	else{
		printf("Invalid action.\n");
	}
    while (head) {
        struct AppointmentEntry *temp = head;
        head = head->next;
        free(temp);
    }
}
