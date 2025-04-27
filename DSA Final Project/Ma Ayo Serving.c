#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<direct.h>

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
};

struct Appointment
{
	char doctorName[50];
	char doctorUsername[50];
	char date[20];
	char time[10];
	struct Appointment* next;
};
struct Appointment* patientAppointment = NULL;

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
void patientDashboard(char username[50]);
void viewPatientprofile(char username[50]);
void createAppointment(char username[50]);

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
				printf("Patient Registration Form\n");
				registerPatient();
				break;
			case 2:
				system("cls");
				printf("Doctor Registration Form\n\n");
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
			printf("\nPress any key to continue...");
			getch();
		}	
	} while (1);
	
	return 0;
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
    const char *specializations[] = {
        "Family Medicine",
        "Internal Medicine",
        "Pediatrics",
        "Dermatology",
        "Cardiology",
        "Hematology"
    };
    int numOptions = 6;

    printf("\nChoose your specialization (use ? ? and Enter):\n");

    while (1) {
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
            break;
        }
    }
}
void registerDoctor() {
	FILE *fp;
    struct User user;
    char password2[50];
    
    printf("Enter your first name: ");
    takeinput(user.FirstName);
    printf("Enter your last name:");
    takeinput(user.LastName);
    printf("Enter your email: ");
    takeinput(user.email);
    printf("Enter your contact no: ");
    takeinput(user.phone);
    printf("Enter your address: ");
    takeinput(user.address);
    printf("Enter your gender: ");
    takeinput(user.gender);
    printf("Enter your birth date (YYYY-MM-DD): ");
    takeinput(user.dateOfBirth);
    
    do{
    	printf("Enter your username: ");
    	takeinput(user.username);
    	if (isUsernameTaken("DoctorsFile.txt", user.username)){
    		printf("Username already taken. Input anotha one.\n");
		} else{
			break;
		}
	}while(1);
	printf("Enter your password: ");
    takepassword(user.password);
    printf("\nConfirm your password: ");
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
    sprintf(doctorFolder, "MedicalRecords\\%s", user.username);
    mkdir("MedicalRecords");
    mkdir(doctorFolder);
    
    printf("\nDoctor folder created at: %s\n", doctorFolder);
}
void displayRegisterMenu(int highlight){
	system("cls");
    printf("\t|------------------WELCOME TO MaAyo Serving-------------------|\n");
    printf("\t|-------------------------------------------------------------|\n");

    printf("\t|   %s [1] REGISTER AS PATIENT                              %s|\n", (highlight == 1 ? "->" : "  "), (highlight == 1 ? "<-" : "  "));
    printf("\t|   %s [2] REGISTER AS DOCTOR                               %s|\n", (highlight == 2 ? "->" : "  "), (highlight == 2 ? "<-" : "  "));
    printf("\t|   %s [3] Log in                                           %s|\n", (highlight == 3 ? "->" : "  "), (highlight == 3 ? "<-" : "  "));
    printf("\t|   %s [4] Exit                                             %s|\n", (highlight == 4 ? "->" : "  "), (highlight == 4 ? "<-" : "  "));
    printf("\t|-------------------------------------------------------------|\n");
}
void logIn() {
    struct User user;
    char username[50], pword[50];
    int usrFound = 0;
    int isDoctor = 0;
    FILE *fp;

    system("cls");
    printf("\t|------------------WELCOME TO MaAyo Serving-------------------|\n");
    printf("\t|-------------------------------------------------------------|\n");
    printf("\t                          Log In                               \n");
    printf("\t                        Username: ");
    takeinput(username);
    printf("\t                        Password: ");
    takepassword(pword);
    printf("\n");
    printf("\t|-------------------------------------------------------------|\n");

    fp = fopen("PatientFile.txt", "r");
    if (fp == NULL) {
        printf("\nError: Could not open Patients.txt\n");
    } else {
        while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.FirstName, user.LastName,user.email, user.phone, user.username, user.password, user.address, user.gender, user.dateOfBirth) != EOF) {
            printf("\n[DEBUG] Read from Doctors.txt: [%s] [%s] [%s] [%s] [%s]", 
                   user.FirstName, user.email, user.phone, user.username, user.password);
            
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
            printf("\n[DEBUG] Read from Doctors.txt: [%s] [%s] [%s] [%s] [%s]", 
                   user.FirstName, user.email, user.phone, user.username, user.password);
            
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
        printf("\n---------------- Doctor Dashboard ----------------\n");
        printf("%s\n", (choice == 1) ? "-> 1. View Profile" : "   1. View Profile");
        printf("%s\n", (choice == 2) ? "-> 2. View Appointments" : "   2. View Appointments");
        printf("%s\n", (choice == 3) ? "-> 3. Manage Appointments (Accept/Decline)" : "   3. Manage Appointments (Accept/Decline)");
        printf("%s\n", (choice == 4) ? "-> 4. View Patient Records" : "   4. View Patient Records");
        printf("%s\n", (choice == 5) ? "-> 5. Log Out" : "   5. Log Out");
        
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
                    printf("\nFetching appointment data...\n");
                    break;
                case 3:
                    printf("\nAppointment management feature coming soon!\n");
                    break;
                case 4:
                    printf("\nView Patient Records feature coming soon!\n");
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
    printf("\t|--------------------------------------------Doctor Profile--------------------------------------------|\n");
    printf("\t|------------------------------------------------------------------------------------------------------|\n");
    printf("\t                                                                                               \n");
    printf("                        First Name    : %s              Gender       : %s      Email: %s\n", user.FirstName, user.gender, user.email);
    printf("                        Last Name     : %s              Date of Birth: %s      Address: %s\n", user.LastName, user.dateOfBirth, user.address);
    printf("                        Specialization: %s\n", user.specialization);
    printf("                        UserName      : %s\n", user.username);
    printf("\n");
    printf("\t|-----------------------------------------------------------------------------------------------------|\n");

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
    char password2[50];
    
    printf("Enter your first name: ");
    takeinput(user.FirstName);
    printf("Enter your last name:");
    takeinput(user.LastName);
    printf("Enter your email: ");
    takeinput(user.email);
    printf("Enter your contact no: ");
    takeinput(user.phone);
    printf("Enter your address: ");
    takeinput(user.address);
    printf("Enter your gender: ");
    takeinput(user.gender);
    printf("Enter your birth date (YYYY-MM-DD): ");
    takeinput(user.dateOfBirth);
    
    do{
    	printf("Enter your username: ");
    	takeinput(user.username);
    	if (isUsernameTaken("PatientFile.txt", user.username)){
    		printf("Username already taken. Input anotha one.\n");
		} else{
			break;
		}
	}while(1);
	printf("Enter your password: ");
    takepassword(user.password);
    printf("\nConfirm your password: ");
    takepassword(password2);

    if (strcmp(user.password, password2) != 0) {
        printf("\nPasswords do not match!\n");
        return;
    }
    
    fp = fopen("PatientFile.txt", "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            user.FirstName, user.LastName, user.email,
            user.phone, user.username, user.password,
            user.address, user.gender, user.dateOfBirth);

    fclose(fp);
    printf("\nPatient registration successful!\n");
}
void patientDashboard(char username[50]) {
    trimNewline(username);
    int choice = 1;
    int key;

    do {
        system("cls");
        printf("\n---------------- Patient Dashboard ----------------\n");
        printf("%s\n", (choice == 1) ? "-> 1. View Profile" : "   1. View Profile");
        printf("%s\n", (choice == 2) ? "-> 2. Schedule an Appointment" : "   2. Schedule an Appointment");
        printf("%s\n", (choice == 3) ? "-> 3. View Appointments" : "   3. View Appointments");
        printf("%s\n", (choice == 4) ? "-> 4. Log Out" : "   4. Log Out");
        
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
                    printf("\nFetching appointment data...\n");
                    break;
                case 3:
                    printf("\nAppointment management feature coming soon!\n");
                    break;
                case 4:
                    printf("\nLogging out...\n");
                    main();
                    return;
            }
            printf("\nPress any key to return to the dashboard...");
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
    printf("\t|--------------------------------------------Patient Profile--------------------------------------------|\n");
    printf("\t|------------------------------------------------------------------------------------------------------|\n");
    printf("\t                                                                                               \n");
    printf("                        First Name    : %s              Gender       : %s      Email: %s\n", user.FirstName, user.gender, user.email);
    printf("                        Last Name     : %s              Date of Birth: %s      Address: %s\n", user.LastName, user.dateOfBirth, user.address);
    printf("                        UserName      : %s\n", user.username);
    printf("\n");
    printf("\t|-----------------------------------------------------------------------------------------------------|\n");

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

    FILE *fp = fopen("DoctorsFile.txt", "r");
    if (fp == NULL) {
        printf("\nError: Unable to open DoctorsFile.txt!\n");
        return;
    }

    printf("\nAvailable Doctors:\n");
    printf("------------------------------------------\n");

    struct User doctor;
    int doctorCount = 0;
    char doctorNames[100][50];
	char doctorUsernames[100][50];  

    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.FirstName,doctor.LastName, doctor.email, doctor.phone, doctor.username, doctor.password) != EOF) {
        doctorCount++;
        strcpy(doctorNames[doctorCount - 1], doctor.FirstName);
        strcpy(doctorUsernames[doctorCount - 1], doctor.username);
        printf("%d. Dr. %s\n", doctorCount, doctor.FirstName);
    }
    fclose(fp);

    if (doctorCount == 0) {
        printf("\nNo registered doctors found. Please try again later.\n");
        return;
    }

    printf("------------------------------------------\n");

    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    if (newAppointment == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }

    int choice;
    do {
        printf("\nSelect a doctor (1-%d): ", doctorCount);
        scanf("%d", &choice);
        while(getchar() != '\n'); 

        if (choice < 1 || choice > doctorCount) {
            printf("\nInvalid choice! Please enter a valid number.\n");
        }
    } while (choice < 1 || choice > doctorCount);

    strcpy(newAppointment->doctorName, doctorNames[choice - 1]);
    strcpy(newAppointment->doctorUsername, doctorUsernames[choice - 1]);

    printf("Enter Appointment Date (DD/MM/YYYY): ");
    takeinput(newAppointment->date);
    printf("Enter Appointment Time (HH:MM AM/PM): ");
    takeinput(newAppointment->time);
    
    newAppointment->next = NULL;
    patientAppointment = newAppointment;
	
	//Folder
    mkdir("MedicalRecords");
    char doctorFolder[100], patientFolder[150];
    sprintf(doctorFolder, "MedicalRecords\\%s", newAppointment->doctorUsername);
    mkdir(doctorFolder);

    sprintf(patientFolder, "%s\\%s", doctorFolder, username);
    mkdir(patientFolder);
    
    //Doctor.txt
    char pathDoctorTxt[200];
    sprintf(pathDoctorTxt, "%s\\Doctor.txt", patientFolder);
    FILE *fpDoctor = fopen(pathDoctorTxt, "w");
    if (fpDoctor != NULL) {
        fprintf(fpDoctor, "Assigned Doctor: Dr. %s\nUsername: %s\n", newAppointment->doctorName, newAppointment->doctorUsername);
        fclose(fpDoctor);
    }
    
	//Schedule.txt
	char pathScheduleTxt[200];
    sprintf(pathScheduleTxt, "%s\\Schedule.txt", patientFolder);
    FILE *fpSchedule = fopen(pathScheduleTxt, "w");
    if (fpSchedule != NULL) {
        fprintf(fpSchedule, "Appointment Date: %s\nAppointment Time: %s\n", newAppointment->date, newAppointment->time);
        fclose(fpSchedule);
    }
    
    //Feedback.txt
    char pathFeedbackTxt[200];
    sprintf(pathFeedbackTxt, "%s\\Feedback.txt", patientFolder);
    FILE *fpFeedback = fopen(pathFeedbackTxt, "w");
    if (fpFeedback != NULL) {
        fprintf(fpFeedback, "Feedback:\n");
        fclose(fpFeedback);
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

