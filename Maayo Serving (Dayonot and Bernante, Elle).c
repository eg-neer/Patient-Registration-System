#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

struct User{
	char fullName[50];
	char email[50];
	char password[50];
	char username[50];
	char phone[50];
};

struct Appointment
{
	char doctorName[50];
	char date[20];
	char time[10];
	struct Appointment* next;
};
struct Appointment* patientAppointment = NULL;

void takeinput(char ch[50]); //Handles user input and removes trailing newline characters
void takepassword(char pwd[50]); //Reads password input while masking characters with *
void generateUsername(char email[50], char username[50]); //Extracts the username from the email before the @ symbol
void displayRegisterMenu(); //Displays the registration and login menu
void registerPatient(); //Registers a patient and placing it in the correct file
void registerDoctor(); //Registers a doctor and placing it in the correct file
void logIn(); //Handles user login by validating credentials from the files
void patientDashboard(char username[50]); //Displays patient options (view profile, schedule/view appointments)
void viewprofile(char username[50]); //Displays patient profile details(history, existing conditions)
void createAppointment(char username[50]); //Allows a patient to schedule an appointment
void viewAppointment(char username[50]); //Checks Status of the Appointment
void doctorDashBoard(char username[50]); //Displays doctor options
void viewDoctorprofile(char username[50]); //Displays doctor profile

int main()
{
	int choice;
	
	do{
		displayRegisterMenu();
		printf("\nSelect Option: ");
		scanf("%d", &choice);
		while(getchar() != '\n');
		
		switch (choice){
			case 1:
				registerPatient();
				break;
			case 2:
				registerDoctor();
				break;
			case 3:
				logIn();
				break;
			case 4: 
				printf("\nThank you! MaAyo Serving at your Service!");
			default:
				printf("\nInvalid choice! Please try again.\n");
		}
	} while (choice != 4);
	
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

void generateUsername(char email[50], char username[50]){
	int i;
	for (i=0; i<strlen(email); i++){
		if(email[i] == '@'){
			break;
		}
		else username[i] = email[i];
	}
	username[i] = '\0';
}

void displayRegisterMenu()
{
	system("cls");
	printf("\t|------------------WELCOME TO MaAyo Serving-------------------|\n");
	printf("\t|-------------------------------------------------------------|\n");
    printf("\t|                 [1] REGISTER AS PATIENT                     |\n");
    printf("\t|                 [2] REGISTER AS DOCTOR                      |\n");
    printf("\t|                 [3] Log in                                  |\n");
    printf("\t|-------------------------------------------------------------|\n");
}

void patientDashboard(char username[50]) {
    int choice;
    
    do {
    	system("cls");
        printf("\n---------------- Patient Dashboard ----------------\n");
        printf("1. View Profile\n");
        printf("2. Schedule an Appointment\n");
        printf("3. View Appointments\n");
        printf("4. Log Out\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        while(getchar() != '\n');
		//system("cls");
		
        switch (choice) {
            case 1:
                viewprofile(username);
                break;
            case 2:
                createAppointment(username);
				break;
            case 3:
                viewAppointment(username);
                break;
            case 4:
                printf("\nLogging out...\n");
                return;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void doctorDashBoard(char username[50]) { // Ensure name matches function call
    int choice;
    
    do {
    	system("cls");
        printf("\n---------------- Doctor Dashboard ----------------\n");
        printf("1. View Profile\n");
        printf("2. View Appointments\n");
        printf("3. Manage Appointments (Accept/Decline)\n");
        printf("4. Log Out\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        while(getchar() != '\n'); // Clear buffer

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
                printf("\nLogging out...\n");
                return;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 4);
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

    fp = fopen("Patients.txt", "r");
    if (fp == NULL) {
        printf("\nError: Could not open Patients.txt\n");
    } else {
        while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.fullName, user.email, user.phone, user.username, user.password) != EOF) {
            printf("\n[DEBUG] Read from Patients.txt: [%s] [%s] [%s] [%s] [%s]", 
                   user.fullName, user.email, user.phone, user.username, user.password);
            
            if (strcmp(user.username, username) == 0 && strcmp(user.password, pword) == 0) {
                usrFound = 1;
                fclose(fp);
                system("cls");
                printf("\nWelcome, %s! You have successfully logged in as a Patient.\n", user.fullName);
                patientDashboard(user.username);
                return;
            }
        }
        fclose(fp);
    }
    
    fp = fopen("Doctors.txt", "r");
    if (!usrFound && fp == NULL) {
        printf("\nError: Could not open Doctors.txt\n");
    } else if (!usrFound) {
        while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                      user.fullName, user.email, user.phone, user.username, user.password) != EOF) {
            printf("\n[DEBUG] Read from Doctors.txt: [%s] [%s] [%s] [%s] [%s]", 
                   user.fullName, user.email, user.phone, user.username, user.password);
            
            if (strcmp(user.username, username) == 0 && strcmp(user.password, pword) == 0) {
                usrFound = 1;
                fclose(fp);
                system("cls");
                printf("\nWelcome, Dr. %s! You have successfully logged in as a Doctor.\n", user.fullName);
                doctorDashBoard(user.username);
                return;
            }
        }
        fclose(fp);
    }
    if (!usrFound) {
        printf("\nUser not found! Would you like to register? (yes/no): ");
        char choice[10];
        takeinput(choice);
        if (strcmp(choice, "yes") == 0 || strcmp(choice, "YES") == 0) {
            displayRegisterMenu();
        } else {
            printf("\nReturning to the main menu...\n");
        }
    }
}


void registerUser(const char *filename) {
    FILE *fp;
    struct User user;
    char password2[50];

    printf("Enter your full name: ");
    takeinput(user.fullName);
    printf("Enter your email: ");
    takeinput(user.email);
    printf("Enter your contact no: ");
    takeinput(user.phone);
    printf("Enter your password: ");
    takepassword(user.password);
    printf("\nConfirm your password: ");
    takepassword(password2);

    if (strcmp(user.password, password2) == 0) {
        generateUsername(user.email, user.username);
        fp = fopen(filename, "a");
        if (fp == NULL) {
            printf("\nError opening file!\n");
            return;
        }
        fprintf(fp, "%s,%s,%s,%s,%s\n", user.fullName, user.email, user.phone, user.username, user.password);
        fclose(fp);
        printf("\nUser registration successful! Your username is %s\n", user.username);
    } else {
        printf("\nPasswords do not match!\n");
    }
}

void registerPatient() {
    registerUser("Patients.txt");
}

void registerDoctor() {
    registerUser("Doctors.txt");
}

void viewprofile(char username[50])
{
	struct User user;
	int click;
	FILE *fp;
	fp = fopen("Patients.txt", "r");
	
	int found = 0;
	while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  user.fullName, user.email, user.phone, user.username, user.password) != EOF) {
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
    printf("\t|-----------------------Patient Profile-----------------------|\n");
    printf("\t|-------------------------------------------------------------|\n");
    printf("\t                                                               \n");
    printf("\t                        Fullname: [%s]\n", user.fullName);
    printf("\t                           Email: [%s]\n", user.email);
    printf("\t                           Phone: [%s]\n", user.phone);
    printf("\t                        Username: [%s]\n", user.username);
    printf("\t                        Password: [%s]\n", user.password);
    printf("\n");
    printf("\t|-------------------------------------------------------------|\n");
    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if(click == 1)
    {
    	patientDashboard(username);
	}
}

void viewDoctorprofile(char username[50])
{
	struct User user;
	int click;
	FILE *fp;
	fp = fopen("Doctors.txt", "r");

	int found = 0;
	while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  user.fullName, user.email, user.phone, user.username, user.password) != EOF) {
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
    printf("\t|-----------------------Doctor Profile-----------------------|\n");
    printf("\t|-------------------------------------------------------------|\n");
    printf("\t                                                               \n");
    printf("\t                        Fullname: [%s]\n", user.fullName);
    printf("\t                           Email: [%s]\n", user.email);
    printf("\t                           Phone: [%s]\n", user.phone);
    printf("\t                        Username: [%s]\n", user.username);
    printf("\t                        Password: [%s]\n", user.password);
    printf("\n");
    printf("\t|-------------------------------------------------------------|\n");

    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    while(getchar() != '\n'); // Clear input buffer

    if(click == 1)
    {
    	doctorDashBoard(username);  // Ensure this matches exactly!
	}

    // Pause before exiting function to avoid instant screen clear
    printf("\nPress Enter to continue...");
    while(getchar() != '\n'); // Wait for Enter key
}


void createAppointment(char username[50])
{
	int click;
    if (patientAppointment != NULL) {
        printf("\nYou already have an appointment scheduled with Dr. %s on %s at %s.\n", 
               patientAppointment->doctorName, patientAppointment->date, patientAppointment->time);
        return;
    }

    FILE *fp = fopen("Doctors.txt", "r");
    if (fp == NULL) {
        printf("\nError: Unable to open Doctors.txt!\n");
        return;
    }

    // Display registered doctors
    printf("\nAvailable Doctors:\n");
    printf("------------------------------------------\n");

    struct User doctor;
    int doctorCount = 0;
    char doctorNames[100][50];  // Store doctor names for validation

    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.fullName, doctor.email, doctor.phone, doctor.username, doctor.password) != EOF) {
        doctorCount++;
        strcpy(doctorNames[doctorCount - 1], doctor.fullName);
        printf("%d. Dr. %s\n", doctorCount, doctor.fullName);
    }
    fclose(fp);

    if (doctorCount == 0) {
        printf("\nNo registered doctors found. Please try again later.\n");
        return;
    }

    printf("------------------------------------------\n");

    // Memory allocation for new appointment
    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    if (newAppointment == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }

    int choice;
    do {
        printf("\nSelect a doctor (1-%d): ", doctorCount);
        scanf("%d", &choice);
        while(getchar() != '\n'); // Clear input buffer

        if (choice < 1 || choice > doctorCount) {
            printf("\nInvalid choice! Please enter a valid number.\n");
        }
    } while (choice < 1 || choice > doctorCount);

    strcpy(newAppointment->doctorName, doctorNames[choice - 1]);

    printf("Enter Appointment Date (DD/MM/YYYY): ");
    takeinput(newAppointment->date);
    printf("Enter Appointment Time (HH:MM AM/PM): ");
    takeinput(newAppointment->time);
    
    newAppointment->next = NULL;
    patientAppointment = newAppointment;

    char doctorFileName[60];
    sprintf(doctorFileName, "Dr_%s.txt", newAppointment->doctorName);

    FILE *doctorFile = fopen(doctorFileName, "a");
    if (doctorFile == NULL) {
        printf("\nError: Unable to save appointment details!\n");
        return;
    }
    
    fprintf(doctorFile, "Patient: %s\nDate: %s\nTime: %s\n----------------------\n",
            username, newAppointment->date, newAppointment->time);
    fclose(doctorFile);

    printf("\nAppointment successfully scheduled with Dr. %s on %s at %s.\n",
           newAppointment->doctorName, newAppointment->date, newAppointment->time);
    
	printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if(click == 1)
    {
    	patientDashboard(username);
	}
}

void viewAppointment(char username[50]) {
    char doctorFileName[60];
    int click;

    // Search for the appointment in all doctor files
    FILE *fp = fopen("Doctors.txt", "r");
    if (fp == NULL) {
        printf("\nError: Unable to open Doctors.txt!\n");
        return;
    }

    struct User doctor;
    int found = 0;

    while (fscanf(fp, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                  doctor.fullName, doctor.email, doctor.phone, 
			   doctor.username, doctor.password) != EOF) {
        
  	   	sprintf(doctorFileName, "Dr_%s.txt", doctor.fullName);

	     FILE *doctorFile = fopen(doctorFileName, "r");
	     if (doctorFile == NULL) {
	        continue; // Skip if file does not exist
	     }

	char line[100];
	char storedUsername[50];
	char storedDate[20];
     char storedTime[10];
     char status[20] = "In Process"; // Default status
		
	system("cls");
        while (fgets(line, sizeof(line), doctorFile)) {
            if (sscanf(line, "Patient: %49[^\n]", storedUsername) == 1 && strcmp(storedUsername, username) == 0) {
                fgets(line, sizeof(line), doctorFile); // Read Date
                sscanf(line, "Date: %19[^\n]", storedDate);
                fgets(line, sizeof(line), doctorFile); // Read Time
                sscanf(line, "Time: %9[^\n]", storedTime);

                // Read for status (Accepted/Declined)
                while (fgets(line, sizeof(line), doctorFile)) {
                    if (strstr(line, "Accepted")) {
                        strcpy(status, "Accepted");
                    } else if (strstr(line, "Declined")) {
                        strcpy(status, "Declined");
                    }
                }

                printf("\n---------------- Your Appointment ----------------\n");
                printf("Doctor: %s\nDate: %s\nTime: %s\nStatus: %s\n", doctor.fullName, storedDate, storedTime, status);
                printf("--------------------------------------------------\n");
                found = 1;
                break;
            }
        }
        fclose(doctorFile);
        if (found) break; 
    }
    fclose(fp);

    if (!found) {
        printf("\nNo appointment scheduled.\n");
    }
    
    printf("Click 1 to return to Dashboard: ");
    scanf("%d", &click);
    if(click == 1){
    		patientDashboard(username);
    }
}
