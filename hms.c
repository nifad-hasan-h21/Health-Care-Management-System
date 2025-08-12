/* Health Care Management System */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_DOCTORS 100
#define MAX_APPOINTMENTS 100
#define MAX_LEN 100

/* ---------- Structures ---------- */
struct User {
    char username[MAX_LEN];
    char password[MAX_LEN];
    int role; // 1.Admin 2.Doctor 3.Patient
    char id[MAX_LEN];
};

struct Doctor {
    char name[MAX_LEN];
    char specialization[MAX_LEN];
    char timeSchedule[MAX_LEN];
    int charge;
    char id[MAX_LEN];
};

struct Appointment {
    char patientName[MAX_LEN];
    char patientUsername[MAX_LEN];
    char disease[MAX_LEN];
    char doctorName[MAX_LEN];
    char time[MAX_LEN];
    int charge;
    char date[MAX_LEN];
};

struct HealthRecord {
    char username[MAX_LEN];
    char bloodGroup[5];
    float height;
    float weight;
    char allergies[MAX_LEN];
};

struct PrescriptionItem {
    char medicine[MAX_LEN];
    char dosage[MAX_LEN];


};

struct Bill {
    char patientUsername[MAX_LEN];
    char doctorName[MAX_LEN];
    char date[MAX_LEN];
    int amount;
    int isPaid; // 0 = unpaid, 1 = paid
    char billDate[MAX_LEN];
    int billNumber;  // Add bill number field
    char paymentMethod[20]; // Add payment method
    float discount; // Add discount field
    float finalAmount; // Add final amount after discount
};
#pragma pack(push, 1)
struct Prescription {
    char patientUsername[MAX_LEN];
    char patientName[MAX_LEN];
    char patientAge[10];
    char patientSex[10];
    char doctorName[MAX_LEN];
    char date[MAX_LEN];
    struct PrescriptionItem items[20];
    int itemCount;
    char doctorSignature[MAX_LEN];
};
#pragma pack(pop)

/* ---------- Globals ---------- */
struct User users[MAX_USERS];
struct Doctor doctors[MAX_DOCTORS];
struct Appointment appointments[MAX_APPOINTMENTS];
char currentUsername[MAX_LEN] = "";
int currentRole = 0;
int userCount = 0, doctorCount = 0, appointmentCount = 0;
struct Bill bills[MAX_APPOINTMENTS];
int billCount = 0;

/* ---------- Function Prototypes ---------- */
void inputPassword(char password[]);
int isPasswordStrong(char *pass);
void saveUsersToFile();
void registerUser();
int loginUser(int *role);
void afterLoginMenu(int role);
void adminMenu();
void doctorMenu();
void patientMenu();
void viewDoctors();
void addDoctor();
void billingMenu();
void appointmentMenu();
void searchDoctorBySpecialization();
void viewDoctorAppointments();
void initializeDoctors();
void loadUsersFromFile();
void writePrescription();
void debugCheckPrescriptionFile();
void viewPrescriptions();
void displayPrescription();
void savePrescriptionToFile();
void addHealthRecord(char username[]);
int userExists(const char *username);
void listDoctorsWithUsernames();
void generateMonthlyReport();
void generateAnnualReport();
void saveAppointmentsToFile();
void saveDoctorsToFile();
void loadDoctorsFromFile();
void loadAppointmentsFromFile();
void cancelAppointment();
void rescheduleAppointment();
void saveBillsToFile();
void loadBillsFromFile();
void viewBills();
void payBill();
float calculateTotalUnpaidBills();
void adminBillingReport();
void loadDoctorsFromFile();
void showWelcomeMessage();

/* ========== MAIN FUNCTION ========== */
int main() {

    srand(time(0));
    loadDoctorsFromFile();
    loadUsersFromFile();
    loadAppointmentsFromFile();
    loadBillsFromFile();
    showWelcomeMessage();

    int choice, loggedIn;
    while(1) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                MAIN MENU                     *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    1. Register                               *\n");
        printf("*    2. Login                                  *\n");
        printf("*    3. Exit                                   *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1:
                registerUser();
                printf("\n");
                printf("*==============================================*\n");
                printf("*           REGISTRATION SUCCESSFUL!           *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Thank you for registering with us!        *\n");
                printf("*    You can now login with your credentials.  *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
                break;

            case 2:
                loggedIn = loginUser(&currentRole);
                if(loggedIn) {
                    printf("\n");
                    printf("===============================================\n");
                    printf("|                                             |\n");
                    printf("|           LOGIN SUCCESSFUL!                 |\n");
                    printf("|           Welcome %-20s      |\n", currentUsername);
                    printf("|                                             |\n");
                    printf("===============================================\n");
                    printf("\n");
                    afterLoginMenu(currentRole);
                }
                else {
                    printf("\n");
                    printf("*==============================================*\n");
                    printf("*        INVALID CREDENTIALS!                  *\n");
                    printf("*==============================================*\n");
                    printf("*                                              *\n");
                    printf("*    Please check your username and password.  *\n");
                    printf("*    Try again or register if you're new.      *\n");
                    printf("*                                              *\n");
                    printf("*==============================================*\n");
                }
                break;

            case 3:
                saveUsersToFile();
                saveDoctorsToFile();
                saveAppointmentsToFile();
                saveBillsToFile();
                printf("\n");
                printf("*==============================================*\n");
                printf("*              SYSTEM SHUTDOWN                 *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    All data has been saved successfully.     *\n");
                printf("*    Thank you for using our system!           *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
                exit(0);

            default:
                printf("\n");
                printf("*==============================================*\n");
                printf("*           INVALID SELECTION!                 *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Please enter a number between 1 and 3.    *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
        }
    }
    return 0;
}

/* ========== AUTHENTICATION FUNCTIONS ========== */
void inputPassword(char password[]) {
    int i = 0;
    char ch;
    while(1) {
        ch = getch();
        if(ch == 13) { password[i] = '\0'; break; }
        else if(ch == 8 && i>0) { i--; printf("\b \b"); }
        else { password[i++] = ch; printf("*"); }
    }
    printf("\n");
}

int isPasswordStrong(char *pass) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0;
    for(int i = 0; pass[i]; i++) {
        if(isupper(pass[i])) hasUpper = 1;
        if(islower(pass[i])) hasLower = 1;
        if(isdigit(pass[i])) hasDigit = 1;
    }
    return (hasUpper && hasLower && hasDigit && strlen(pass) >= 8);
}

/* ========== FILE HANDLING ========== */
void saveUsersToFile() {
    FILE *file = fopen("users.dat", "wb");
    if(file) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(struct User), userCount, file);
        fclose(file);
    }
}

void loadUsersFromFile() {
    FILE *file = fopen("users.dat", "rb");
    if(file) {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(struct User), userCount, file);
        fclose(file);
    }
}

void saveAppointmentsToFile() {
    FILE *file = fopen("appointments.dat", "wb");
    if(file) {
        fwrite(&appointmentCount, sizeof(int), 1, file);
        fwrite(appointments, sizeof(struct Appointment), appointmentCount, file);
        fclose(file);
    }
}

void loadAppointmentsFromFile() {
    FILE *file = fopen("appointments.dat", "rb");
    if(file) {
        fread(&appointmentCount, sizeof(int), 1, file);
        fread(appointments, sizeof(struct Appointment), appointmentCount, file);
        fclose(file);
    }
}

void saveBillsToFile() {
    FILE *file = fopen("bills.dat", "wb");
    if(file) {
        fwrite(&billCount, sizeof(int), 1, file);
        fwrite(bills, sizeof(struct Bill), billCount, file);
        fclose(file);
    }
}

void loadBillsFromFile() {
    FILE *file = fopen("bills.dat", "rb");
    if(file) {
        fread(&billCount, sizeof(int), 1, file);
        fread(bills, sizeof(struct Bill), billCount, file);
        fclose(file);
    }
}

void saveDoctorsToFile() {
    FILE *file = fopen("doctors.dat", "wb");
    if(file) {
        fwrite(&doctorCount, sizeof(int), 1, file);
        fwrite(doctors, sizeof(struct Doctor), doctorCount, file);
        fclose(file);
    }
}

void loadDoctorsFromFile() {
    FILE *file = fopen("doctors.dat", "rb");
    if(file) {
        fread(&doctorCount, sizeof(int), 1, file);
        fread(doctors, sizeof(struct Doctor), doctorCount, file);
        fclose(file);
    } else {
        initializeDoctors();
    }
}

/* ========== REGISTRATION ========== */
void registerUser() {
    if(userCount >= MAX_USERS) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*               REGISTRATION ERROR             *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    User limit reached! Cannot register more. *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        return;
    }

    printf("\n");
    printf("*==============================================*\n");
    printf("*                REGISTRATION                  *\n");
    printf("*==============================================*\n");
    printf("*                                              *\n");
    printf("*    1. Admin                                  *\n");
    printf("*    2. Doctor                                 *\n");
    printf("*    3. Patient                                *\n");
    printf("*                                              *\n");
    printf("*==============================================*\n");
    printf("Enter role (1-3): ");
    scanf("%d", &users[userCount].role);
    while(getchar() != '\n');

    char tempUsername[MAX_LEN];
    int usernameExists;
    do {
        usernameExists = 0;
        printf("\nUsername: ");
        scanf("%99s", tempUsername);
        while(getchar() != '\n');

        for(int i = 0; i < userCount; i++) {
            if(strcmp(users[i].username, tempUsername) == 0) {
                printf("\n");
                printf("*==============================================*\n");
                printf("*               REGISTRATION ERROR             *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Username already exists!                  *\n");
                printf("*    Please choose another username.           *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
                usernameExists = 1;
                break;
            }
        }
    } while(usernameExists);

    strcpy(users[userCount].username, tempUsername);

    printf("Password: ");
    do {
        inputPassword(users[userCount].password);
        if(!isPasswordStrong(users[userCount].password)) {
            printf("\n");
            printf("*==============================================*\n");
            printf("*             PASSWORD REQUIREMENTS            *\n");
            printf("*==============================================*\n");
            printf("*                                              *\n");
            printf("*    Password must contain:                    *\n");
            printf("*    - At least 8 characters                   *\n");
            printf("*    - Uppercase and lowercase letters         *\n");
            printf("*    - At least one number                     *\n");
            printf("*                                              *\n");
            printf("*==============================================*\n");
            printf("Please try again: ");
        }
    } while(!isPasswordStrong(users[userCount].password));

    sprintf(users[userCount].id, "U%03d", userCount+1);

    if(users[userCount].role == 2) {  // Doctor registration
        if(doctorCount >= MAX_DOCTORS) {
            printf("\n");
            printf("*==============================================*\n");
            printf("*               REGISTRATION ERROR             *\n");
            printf("*==============================================*\n");
            printf("*                                              *\n");
            printf("*    Cannot register more doctors!             *\n");
            printf("*    Maximum limit reached.                    *\n");
            printf("*                                              *\n");
            printf("*==============================================*\n");
            return;
        }

        printf("\n");
        printf("*==============================================*\n");
        printf("*            DOCTOR REGISTRATION               *\n");
        printf("*==============================================*\n");

        printf("Full Name: ");
        fgets(doctors[doctorCount].name, MAX_LEN, stdin);
        doctors[doctorCount].name[strcspn(doctors[doctorCount].name, "\n")] = 0;

        // Set doctor ID to match user ID
        sprintf(doctors[doctorCount].id, "D%03d", doctorCount+1);

        strcpy(users[userCount].id, doctors[doctorCount].id);

        printf("Specialization: ");
        fgets(doctors[doctorCount].specialization, MAX_LEN, stdin);
        doctors[doctorCount].specialization[strcspn(doctors[doctorCount].specialization, "\n")] = 0;

        printf("Schedule (e.g. 9AM-5PM): ");
        fgets(doctors[doctorCount].timeSchedule, MAX_LEN, stdin);
        doctors[doctorCount].timeSchedule[strcspn(doctors[doctorCount].timeSchedule, "\n")] = 0;

        printf("Consultation Fee: ");
        scanf("%d", &doctors[doctorCount].charge);
        while(getchar() != '\n');

        doctorCount++;
        saveDoctorsToFile();
    }

    if(users[userCount].role == 3) {  // Patient registration
        printf("\n");
        printf("*==============================================*\n");
        printf("*           PATIENT REGISTRATION               *\n");
        printf("*==============================================*\n");
        addHealthRecord(users[userCount].username);
    }

    userCount++;
    saveUsersToFile();
}

int loginUser(int *role) {
    char uname[MAX_LEN], pass[MAX_LEN];
    printf("\n");
    printf("*==============================================*\n");
    printf("*                   LOGIN                      *\n");
    printf("*==============================================*\n");
    printf("Enter username: ");
    scanf("%s", uname);
    while(getchar() != '\n');

    printf("Enter password: ");
    inputPassword(pass);

    for(int i = 0; i < userCount; i++) {
        if(strcmp(uname, users[i].username) == 0 &&
           strcmp(pass, users[i].password) == 0) {
            *role = users[i].role;
            strcpy(currentUsername, users[i].username);
            return 1;
        }
    }
    return 0;
}

/* ========== HEALTH RECORDS ========== */
void addHealthRecord(char username[]) {
    struct HealthRecord r;
    strcpy(r.username, username);

    printf("\n");
    printf("*==============================================*\n");
    printf("*            HEALTH INFORMATION                *\n");
    printf("*==============================================*\n");
    printf("Blood Group: "); scanf("%4s", r.bloodGroup);
    printf("Height (cm): "); scanf("%f", &r.height);
    printf("Weight (kg): "); scanf("%f", &r.weight);
    printf("Allergies: "); scanf("%s", r.allergies);

    FILE *file = fopen("health_records.dat", "ab");
    if(file) {
        fwrite(&r, sizeof(struct HealthRecord), 1, file);
        fclose(file);
    }
}

/* ========== MENU SYSTEM ========== */
void afterLoginMenu(int role) {
    switch(role) {
        case 1: adminMenu(); break;
        case 2: doctorMenu(); break;
        case 3: patientMenu(); break;
        default:
            printf("\n");
            printf("*==============================================*\n");
            printf("*                 ACCESS ERROR                 *\n");
            printf("*==============================================*\n");
            printf("*                                              *\n");
            printf("*          Invalid user role detected!         *\n");
            printf("*                                              *\n");
            printf("*==============================================*\n");
    }
}

void adminMenu() {
    int choice;
    while(1) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 ADMIN MENU                   *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    1. View Doctors                           *\n");
        printf("*    2. View Appointments                      *\n");
        printf("*    3. List Doctors with Usernames            *\n");
        printf("*    4. Generate Monthly Report                *\n");
        printf("*    5. Generate Annual Report                 *\n");
        printf("*    6. Billing Report                         *\n");
        printf("*    7. Logout                                 *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: viewDoctors(); break;
            case 2: appointmentMenu(); break;
            case 3: listDoctorsWithUsernames(); break;
            case 4: generateMonthlyReport(); break;
            case 5: generateAnnualReport(); break;
            case 6: adminBillingReport(); break;
            case 7:
                printf("\n");
                printf("*==============================================*\n");
                printf("*             LOGGED OUT SUCCESSFULLY          *\n");
                printf("*==============================================*\n");
                return;
            default:
                printf("\n");
                printf("*==============================================*\n");
                printf("*           INVALID SELECTION!                 *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Please enter a number between 1 and 7.    *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
        }
    }
}

void doctorMenu() {
    int choice;
    while(1) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                DOCTOR MENU                   *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    1. View My Appointments                   *\n");
        printf("*    2. Write Prescription                     *\n");
        printf("*    3. Search Patient Prescriptions           *\n");
        printf("*    4. Logout                                 *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: viewDoctorAppointments(); break;
            case 2: writePrescription(); break;
            case 3: viewPrescriptions(); break;
            case 4:
                printf("\n");
                printf("*==============================================*\n");
                printf("*             LOGGED OUT SUCCESSFULLY          *\n");
                printf("*==============================================*\n");
                return;
            default:
                printf("\n");
                printf("*==============================================*\n");
                printf("*           INVALID SELECTION!                 *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Please enter a number between 1 and 4.    *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
        }
    }
}

void patientMenu() {
    int choice;
    while(1) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                PATIENT MENU                  *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    1. View Doctors                           *\n");
        printf("*    2. Book Appointment                       *\n");
        printf("*    3. View My Appointments                   *\n");
        printf("*    4. Cancel Appointment                     *\n");
        printf("*    5. Reschedule Appointment                 *\n");
        printf("*    6. View Bills                             *\n");
        printf("*    7. Pay Bill                               *\n");
        printf("*    8. Search Doctors                         *\n");
        printf("*    9. Logout                                 *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        printf("Enter your choice (1-9): ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: viewDoctors(); break;
            case 2: billingMenu(); break;
            case 3: appointmentMenu(); break;
            case 4: cancelAppointment(); break;
            case 5: rescheduleAppointment(); break;
            case 6: viewBills(); break;
            case 7: payBill(); break;
            case 8: searchDoctorBySpecialization(); break;
            case 9:
                printf("\n");
                printf("*==============================================*\n");
                printf("*             LOGGED OUT SUCCESSFULLY          *\n");
                printf("*==============================================*\n");
                return;
            default:
                printf("\n");
                printf("*==============================================*\n");
                printf("*           INVALID SELECTION!                 *\n");
                printf("*==============================================*\n");
                printf("*                                              *\n");
                printf("*    Please enter a number between (1 and 9.)  *\n");
                printf("*                                              *\n");
                printf("*==============================================*\n");
        }
    }
}

/* ========== DOCTOR FUNCTIONS ========== */
void viewDoctors() {
    printf("\n");
    printf("*=============================================================*\n");
    printf("*                      AVAILABLE DOCTORS                      *\n");
    printf("*=============================================================*\n");
    printf("* %-4s | %-20s | %-20s | %-15s | %-6s *\n", "ID", "Name", "Specialization", "Schedule", "Fee");
    printf("*-------------------------------------------------------------*\n");

    for(int i = 0; i < doctorCount; i++) {
        printf("* %-4s | %-20s | %-20s | %-15s | %-6d *\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].timeSchedule,
               doctors[i].charge);
    }
    printf("*=============================================================*\n");
}

void initializeDoctors(void) {
    strcpy(doctors[0].name,  "Dr. Azim");
    strcpy(doctors[0].specialization, "Cardiologist");
    strcpy(doctors[0].timeSchedule,   "10am-1pm");
    doctors[0].charge = 800;
    strcpy(doctors[0].id, "D001");

    strcpy(doctors[1].name,  "Dr. Soha");
    strcpy(doctors[1].specialization, "Dermatologist");
    strcpy(doctors[1].timeSchedule,   "2pm-5pm");
    doctors[1].charge = 600;
    strcpy(doctors[1].id, "D002");

    doctorCount = 2;
}

void viewDoctorAppointments() {
    printf("\n");
    printf("*==============================================*\n");
    printf("*        PATIENT'S BOOKED APPOINTMENTS         *\n");
    printf("*==============================================*\n");

    // Find the current doctor's name
    char doctorName[MAX_LEN] = "";

    // First try to find the doctor by username in users array
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, currentUsername) == 0 && users[i].role == 2) {
            // This is a doctor user, find their name in doctors array
            for (int j = 0; j < doctorCount; j++) {
                if (strcmp(doctors[j].id, users[i].id) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }
            break;
        }
    }

    // If still not found, try to match username directly with doctor name
    if (strlen(doctorName) == 0) {
        for (int i = 0; i < doctorCount; i++) {
            if (strcmp(doctors[i].name, currentUsername) == 0) {
                strcpy(doctorName, doctors[i].name);
                break;
            }
        }
    }

    if (strlen(doctorName) == 0) {
        printf("* No doctor profile found for current user!    *\n");
        printf("*==============================================*\n");
        return;
    }

    // Display all appointments for this doctor
    int found = 0;
    for (int i = 0; i < appointmentCount; i++) {
        // Compare against both doctor ID and doctor name
        int isMatch = 0;

        // Check if appointment's doctor ID matches any doctor's ID
        for (int j = 0; j < doctorCount; j++) {
            if (strcmp(appointments[i].doctorName, doctors[j].id) == 0 &&
                strcmp(doctors[j].name, doctorName) == 0) {
                isMatch = 1;
                break;
            }
        }

        // Or check if appointment's doctor name matches directly
        if (!isMatch && strcmp(appointments[i].doctorName, doctorName) == 0) {
            isMatch = 1;
        }

        if (isMatch) {
            printf("* Appointment ID: %-29d*\n", i+1);
            printf("* Patient: %-34s  *\n", appointments[i].patientName);
            printf("* Username: %-33s  *\n", appointments[i].patientUsername);
            printf("* Date: %-10s at %-8s                 *\n",
                  appointments[i].date, appointments[i].time);
            printf("* Reason: %-32s     *\n", appointments[i].disease);
            printf("* Fee: $%-36d   *\n", appointments[i].charge);
            printf("*----------------------------------------------*\n");
            found = 1;
        }
    }

    if (!found) {
        printf("* No appointments booked with you yet.         *\n");
        printf("*----------------------------------------------*\n");
    }

    printf("*==============================================*\n");
}
void writePrescription() {
    struct Prescription p;
    p.itemCount = 0;

    // Initialize with default name
    strcpy(p.doctorName, "Unknown Doctor");

    // Find current doctor's name by matching username in both users and doctors arrays
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, currentUsername) == 0 && users[i].role == 2) {
            // This is a doctor user, find their details
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, users[i].id) == 0) {
                    strcpy(p.doctorName, doctors[j].name);
                    break;
                }
            }
            break;
        }
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(p.date, MAX_LEN, "%Y-%m-%d", t);

    printf("\n");
    printf("*==============================================*\n");
    printf("*                NEW PRESCRIPTION              *\n");
    printf("*==============================================*\n");

    do {
        printf("Patient Username: ");
        scanf("%s", p.patientUsername);
        while(getchar() != '\n');

        if(!userExists(p.patientUsername)) {
            printf("\n");
            printf("*==============================================*\n");
            printf("*                 ERROR                         *\n");
            printf("*==============================================*\n");
            printf("*                                              *\n");
            printf("*    No patient found with username '%s'!      *\n", p.patientUsername);
            printf("*                                              *\n");
            printf("*==============================================*\n");
        }
    } while(!userExists(p.patientUsername));

    printf("Patient Full Name: ");
    fgets(p.patientName, MAX_LEN, stdin);
    p.patientName[strcspn(p.patientName, "\n")] = 0;

    printf("Patient Age: ");
    fgets(p.patientAge, 10, stdin);
    p.patientAge[strcspn(p.patientAge, "\n")] = 0;

    printf("Patient Sex (M/F/O): ");
    fgets(p.patientSex, 10, stdin);
    p.patientSex[strcspn(p.patientSex, "\n")] = 0;

    int choice;
    do {
        if(p.itemCount >= 20) {
            printf("\n");
            printf("*==============================================*\n");
            printf("*                 LIMIT REACHED                *\n");
            printf("*==============================================*\n");
            printf("*                                              *\n");
            printf("*    Maximum 20 medicines reached.             *\n");
            printf("*                                              *\n");
            printf("*==============================================*\n");
            break;
        }

        printf("\nMedicine %d:\n", p.itemCount+1);
        printf("Name: ");
        fgets(p.items[p.itemCount].medicine, MAX_LEN, stdin);
        p.items[p.itemCount].medicine[strcspn(p.items[p.itemCount].medicine, "\n")] = 0;

        printf("Dosage: ");
        fgets(p.items[p.itemCount].dosage, MAX_LEN, stdin);
        p.items[p.itemCount].dosage[strcspn(p.items[p.itemCount].dosage, "\n")] = 0;

        p.itemCount++;

        printf("\nAdd another medicine? (1=Yes, 0=No): ");
        scanf("%d", &choice);
        while(getchar() != '\n');
    } while(choice == 1);

    // Doctor's signature
    printf("\nDoctor Signature: ");
    fgets(p.doctorSignature, MAX_LEN, stdin);
    p.doctorSignature[strcspn(p.doctorSignature, "\n")] = 0;

    savePrescriptionToFile(p);
    displayPrescription(p);
}

void displayPrescription(struct Prescription p) {
    printf("\n");
    printf("*================================================================*\n");
    printf("*                         PRESCRIPTION                           *\n");
    printf("*================================================================*\n");
    printf("* %-20s: %-40s *\n", "Date", p.date);
    printf("* %-20s: Dr. %-36s *\n", "Doctor", p.doctorName);
    printf("* %-20s: %-40s *\n", "Patient", p.patientName);

    char ageSex[20];
    if (strlen(p.patientAge) > 0 && strlen(p.patientSex) > 0) {
        snprintf(ageSex, sizeof(ageSex), "%s/%s", p.patientAge, p.patientSex);
    } else {
        strcpy(ageSex, "N/A");
    }
    printf("* %-20s: %-40s *\n", "Age/Sex", ageSex);

    printf("*----------------------------------------------------------------*\n");
    printf("* %-20s | %-40s*\n", "MEDICINE", "DOSAGE");
    printf("*----------------------+-----------------------------------------*\n");

    for(int i = 0; i < p.itemCount; i++) {
        printf("* %-20s | %-40s*\n",
               p.items[i].medicine, p.items[i].dosage);
    }

    printf("*================================================================*\n");
    printf("* Doctor's Signature: %-40s   *\n", p.doctorSignature);
    printf("*                        (Dr. %-30s)    *\n", p.doctorName);
    printf("*================================================================*\n\n");

}

void savePrescriptionToFile(struct Prescription p) {
    FILE *file = fopen("prescriptions.dat", "ab");
    if(!file) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 FILE ERROR                   *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    Cannot open prescriptions file!           *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        return;
    }

    fwrite(p.patientUsername, sizeof(char), MAX_LEN, file);
    fwrite(p.patientName, sizeof(char), MAX_LEN, file);
    fwrite(p.patientAge, sizeof(char), 10, file);
    fwrite(p.patientSex, sizeof(char), 10, file);
    fwrite(p.doctorName, sizeof(char), MAX_LEN, file);
    fwrite(p.doctorSignature, sizeof(char), MAX_LEN, file);
    fwrite(p.date, sizeof(char), MAX_LEN, file);
    fwrite(&p.itemCount, sizeof(int), 1, file);

    for(int i = 0; i < p.itemCount; i++) {
        fwrite(p.items[i].medicine, sizeof(char), MAX_LEN, file);
        fwrite(p.items[i].dosage, sizeof(char), MAX_LEN, file);
    }

    fclose(file);
}

void viewPrescriptions() {
    char searchUsername[MAX_LEN];

    printf("\n");
    printf("*==============================================*\n");
    printf("*            SEARCH PRESCRIPTIONS              *\n");
    printf("*==============================================*\n");
    printf("Enter patient username to search: ");
    scanf("%99s", searchUsername);
    while(getchar() != '\n');

    FILE *file = fopen("prescriptions.dat", "rb");
    if(!file) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 INFORMATION                  *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    No prescriptions found in the system.     *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        return;
    }

    printf("\n");
    printf("*==============================================*\n");
    printf("*        PRESCRIPTIONS FOR %-20s*\n", searchUsername);
    printf("*==============================================*\n");

    struct Prescription p;
    int found = 0;

    while(1) {
        if(fread(p.patientUsername, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.patientName, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.patientAge, sizeof(char), 10, file) != 10) break;
        if(fread(p.patientSex, sizeof(char), 10, file) != 10) break;
        if(fread(p.doctorName, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.doctorSignature, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.date, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(&p.itemCount, sizeof(int), 1, file) != 1) break;

        for(int i = 0; i < p.itemCount; i++) {
            if(fread(p.items[i].medicine, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
            if(fread(p.items[i].dosage, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        }

        if(strcmp(p.patientUsername, searchUsername) == 0) {
            printf("*                                              *\n");
            printf("* Date: %-40s \n", p.date);
            printf("* Doctor: %-38s\n", p.doctorName);
            printf("* Patient: %-20s Age/Sex: %s/%s   *\n",
                  p.patientName, p.patientAge, p.patientSex);
            printf("*-----------------------------------------------*\n");
            printf("* %-20s | %-20s             *\n", "MEDICINE", "DOSAGE");
            printf("*----------------------+-----------------------*\n");

            for(int i = 0; i < p.itemCount; i++) {
                printf("* %-20s | %-20s             \n",
                      p.items[i].medicine, p.items[i].dosage);
            }

            printf("*----------------------------------------------*\n");
            printf("* Doctor's Signature: %-25s*\n", p.doctorSignature);
            printf("*                                              *\n");
            printf("*==============================================*\n");
            printf("\n");
            found = 1;
        }
    }

    if(!found) {
        printf("*                                              *\n");
        printf("* No prescriptions found for username: %-8s *\n", searchUsername);
        printf("*                                              *\n");
        printf("*==============================================*\n");
    }

    fclose(file);
}

/* ========== APPOINTMENT FUNCTIONS ========== */
void billingMenu() {
    if (doctorCount == 0) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 INFORMATION                  *\n");
        printf("*==============================================*\n");
        printf("*    No doctors available at the moment!       *\n");
        printf("*==============================================*\n");
        return;
    }

    // Display available doctors
    printf("\n");
    printf("*===============================================================*\n");
    printf("*                        AVAILABLE DOCTORS                      *\n");
    printf("*===============================================================*\n");
    printf("* %-3s | %-20s | %-20s | %-15s | %-6s *\n",
           "No", "Name", "Specialization", "Schedule", "Fee");
    printf("*---------------------------------------------------------------*\n");

    for (int i = 0; i < doctorCount; i++) {
        printf("* %-3d | %-20s | %-20s | %-15s | %-6d *\n",
               i + 1,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].timeSchedule,
               doctors[i].charge);
    }
    printf("*===============================================================*\n");

    // Doctor selection
    int docChoice;
    printf("\nSelect Doctor (1-%d): ", doctorCount);
    scanf("%d", &docChoice);
    while (getchar() != '\n');

    if (docChoice < 1 || docChoice > doctorCount) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                    ERROR                     *\n");
        printf("*==============================================*\n");
        printf("*    Invalid doctor selection!                 *\n");
        printf("*==============================================*\n");
        return;
    }

    struct Appointment a;

    // Patient details
    printf("\nEnter Patient Name: ");
    fgets(a.patientName, MAX_LEN, stdin);
    a.patientName[strcspn(a.patientName, "\n")] = '\0';
    strcpy(a.patientUsername, currentUsername);

    printf("Enter Medical Issue: ");
    fgets(a.disease, MAX_LEN, stdin);
    a.disease[strcspn(a.disease, "\n")] = '\0';

    // Date input
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char defaultDate[MAX_LEN];
    strftime(defaultDate, MAX_LEN, "%Y-%m-%d", t);

    printf("Appointment Date (YYYY-MM-DD) [%s]: ", defaultDate);
    fgets(a.date, MAX_LEN, stdin);
    a.date[strcspn(a.date, "\n")] = '\0';
    if (strlen(a.date) == 0) {
        strcpy(a.date, defaultDate);
    }

    // Store doctor details in appointment
    strcpy(a.doctorName, doctors[docChoice - 1].id); // store doctor ID
    strcpy(a.time, doctors[docChoice - 1].timeSchedule);
    a.charge = doctors[docChoice - 1].charge;

    // Calculate discount if applicable
    float discount = 0.0;
    float finalAmount = a.charge;

    if (a.charge >= 5000) {
        discount = a.charge * 0.10;
        finalAmount = a.charge - discount;
    }

    // Save appointment
    if (appointmentCount < MAX_APPOINTMENTS) {
        appointments[appointmentCount++] = a;
        saveAppointmentsToFile();

        // Create and save bill
        if (billCount < MAX_APPOINTMENTS) {
            struct Bill newBill;
            strcpy(newBill.patientUsername, currentUsername);
            strcpy(newBill.doctorName, doctors[docChoice - 1].name);
            strcpy(newBill.date, a.date);
            newBill.amount = a.charge;
            newBill.isPaid = 0;
            newBill.discount = discount;
            newBill.finalAmount = finalAmount;
            newBill.billNumber = 0; // Will be set when paid
            strcpy(newBill.paymentMethod, "Not Paid");

            // Bill creation date
            strftime(newBill.billDate, MAX_LEN, "%Y-%m-%d", t);
            bills[billCount++] = newBill;
            saveBillsToFile();

            // Detailed bill display
            printf("\n");
            printf("*==============================================*\n");
            printf("*                 BILL DETAILS                  *\n");
            printf("*==============================================*\n");
            printf("* Patient: %-34s *\n", a.patientName);
            printf("* Doctor : Dr. %-30s *\n", doctors[docChoice - 1].name);
            printf("* Date   : %-34s *\n", a.date);
            printf("* Time   : %-34s *\n", a.time);
            printf("*----------------------------------------------*\n");
            printf("* Consultation Fee: %-26d *\n", a.charge);
            if (discount > 0) {
                printf("* Discount (10%%): %-29.2f *\n", discount);
            }
            printf("* Total Amount: %-30.2f *\n", finalAmount);
            printf("*----------------------------------------------*\n");
            printf("* Status : %-34s *\n", "UNPAID");
            printf("* Bill Dt: %-34s *\n", newBill.billDate);
            printf("*==============================================*\n");
            printf("* Note: 10%% discount applied for bills >= 5000 *\n");
            printf("*==============================================*\n");
        }
    }

    // Appointment confirmation
    printf("\n");
    printf("*==============================================*\n");
    printf("*            APPOINTMENT CONFIRMED             *\n");
    printf("*==============================================*\n");
    printf("* Doctor : Dr. %-30s *\n", doctors[docChoice - 1].name);
    printf("* Patient: %-34s *\n", a.patientName);
    printf("* Date   : %-10s at %-8s         *\n", a.date, a.time);
    printf("* Reason : %-34s *\n", a.disease);
    printf("* Charge : %-34d *\n", a.charge);
    if (discount > 0) {
        printf("* Discount: %-33.2f *\n", discount);
    }
    printf("* Total  : %-34.2f *\n", finalAmount);
    printf("*==============================================*\n");
    printf("* Please visit the Pay Bill option to complete *\n");
    printf("* your payment with your preferred method.      *\n");
    printf("*==============================================*\n");
}


void appointmentMenu() {
    printf("\n");
    printf("*=============================================================*\n");
    printf("*                      APPOINTMENTS                           *\n");
    printf("*=============================================================*\n");

    if (appointmentCount == 0) {
        printf("*                                                             *\n");
        printf("*               No appointments found.                        *\n");
        printf("*                                                             *\n");
        printf("*=============================================================*\n");
        return;
    }

    printf("* #  Doctor                Patient              Date       Time   *\n");
    printf("*=== ==================== ==================== ========== ========*\n");

    int displayCount = 0;
    for (int i = 0; i < appointmentCount; i++) {
        if (currentRole == 1 || strcmp(appointments[i].patientUsername, currentUsername) == 0) {
            char doctorName[MAX_LEN] = "Unknown";
            for (int j = 0; j < doctorCount; j++) {
                if (strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }

            // Display basic appointment info in the main row
            printf("*%-3d%-21s%-21s%-11s%-8s *\n",
                   ++displayCount,
                   doctorName,
                   appointments[i].patientName,
                   appointments[i].date,
                   appointments[i].time);

            // Display details in subsequent rows
            printf("*   Reason: %-54s*\n", appointments[i].disease);
            printf("*   Charge: $%-53d*\n", appointments[i].charge);
            printf("*-----------------------------------------------------------------*\n");
        }
    }

    printf("*=================================================================*\n");
}

void cancelAppointment() {
    printf("\n");
    printf("*==============================================*\n");
    printf("*            YOUR APPOINTMENTS                 *\n");
    printf("*==============================================*\n");

    int count = 0;
    int indices[MAX_APPOINTMENTS];

    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].patientUsername, currentUsername) == 0) {
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }

            printf("* %-2d. Dr. %-20s on %-10s at %-8s *\n",
                  count+1,
                  doctorName,
                  appointments[i].date,
                  appointments[i].time);
            indices[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("*                                              *\n");
        printf("*           No appointments found.             *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        return;
    }

    printf("\nSelect appointment to cancel (1-%d, 0 to cancel): ", count);
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice < 1 || choice > count) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*           CANCELLATION ABORTED               *\n");
        printf("*==============================================*\n");
        return;
    }

    int indexToRemove = indices[choice-1];
    char doctorName[MAX_LEN] = "Unknown";
    for(int j = 0; j < doctorCount; j++) {
        if(strcmp(doctors[j].id, appointments[indexToRemove].doctorName) == 0) {
            strcpy(doctorName, doctors[j].name);
            break;
        }
    }

    for(int i = indexToRemove; i < appointmentCount-1; i++) {
        appointments[i] = appointments[i+1];
    }
    appointmentCount--;
    saveAppointmentsToFile();

    printf("\n");
    printf("*==============================================*\n");
    printf("*          APPOINTMENT CANCELLED               *\n");
    printf("*==============================================*\n");
    printf("*                                              *\n");
    printf("*    Appointment with Dr. %-20s cancelled.*\n", doctorName);
    printf("*                                              *\n");
    printf("*==============================================*\n");
}

void rescheduleAppointment() {
    printf("\n");
    printf("*==============================================*\n");
    printf("*            YOUR APPOINTMENTS                 *\n");
    printf("*==============================================*\n");

    int count = 0;
    int indices[MAX_APPOINTMENTS];

    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].patientUsername, currentUsername) == 0) {
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }

            printf("* %-2d. Dr. %-20s on %-10s at %-8s *\n",
                  count+1,
                  doctorName,
                  appointments[i].date,
                  appointments[i].time);
            indices[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("*                                              *\n");
        printf("*           No appointments found.             *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
        return;
    }

    printf("\nSelect appointment to reschedule (1-%d, 0 to cancel): ", count);
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice < 1 || choice > count) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*         RESCHEDULING ABORTED                 *\n");
        printf("*==============================================*\n");
        return;
    }

    struct Appointment *apt = &appointments[indices[choice-1]];
    char doctorName[MAX_LEN] = "Unknown";
    for(int j = 0; j < doctorCount; j++) {
        if(strcmp(doctors[j].id, apt->doctorName) == 0) {
            strcpy(doctorName, doctors[j].name);
            break;
        }
    }

    printf("\nCurrent Appointment with Dr. %s\n", doctorName);
    printf("Current date: %s\n", apt->date);
    printf("Current time: %s\n", apt->time);

    printf("Enter new date (YYYY-MM-DD): ");
    char newDate[MAX_LEN];
    fgets(newDate, MAX_LEN, stdin);
    newDate[strcspn(newDate, "\n")] = '\0';


    strcpy(apt->date, newDate);

    saveAppointmentsToFile();

    printf("\n");
    printf("*==============================================*\n");
    printf("*         APPOINTMENT RESCHEDULED              *\n");
    printf("*==============================================*\n");
    printf("*                                              *\n");
    printf("*    Appointment rescheduled.                  *\n", doctorName);
    printf("*    New Date: %-30s  *\n", apt->date);
    printf("*                                              *\n");
    printf("*==============================================*\n");
}

/* ========== BILLING MANAGEMENT ========== */
void viewBills() {
    float totalUnpaid = 0.0;
    float totalPaid = 0.0;
    int unpaidCount = 0;
    int paidCount = 0;

    // Calculate totals first
    for(int i = 0; i < billCount; i++) {
        if(strcmp(bills[i].patientUsername, currentUsername) == 0 || currentRole == 1) {
            if(bills[i].isPaid) {
                totalPaid += bills[i].finalAmount;
                paidCount++;
            } else {
                totalUnpaid += bills[i].finalAmount;
                unpaidCount++;
            }
        }
    }

    printf("\n");
    printf("*===============================================================================================*\n");
    printf("*                                      BILL SUMMARY                                             *\n");
    printf("*===============================================================================================*\n");
    printf("* %-5s | %-20s | %-10s | %-8s | %-9s | %-10s | %-8s *\n",
           "Bill#", "Doctor", "Date", "Amount", "Discount", "Final Amt", "Status");
    printf("*-------+----------------------+------------+----------+-----------+------------+----------*\n");

    int found = 0;
    for(int i = 0; i < billCount; i++) {
        if(strcmp(bills[i].patientUsername, currentUsername) == 0 || currentRole == 1) {
            printf("* %-5d | %-20s | %-10s | %-8d | %-9.2f | %-10.2f | %-8s *\n",
                  bills[i].billNumber,
                  bills[i].doctorName,
                  bills[i].date,
                  bills[i].amount,
                  bills[i].discount,
                  bills[i].finalAmount,
                  bills[i].isPaid ? "Paid" : "Unpaid");
            found = 1;
        }
    }

    if(!found) {
        printf("* No bills found for current user.                                                   *\n");
    }
    printf("*===============================================================================================*\n");

    // Display summary statistics
    if(found) {
        printf("* SUMMARY:                                                                                   *\n");
        printf("* - Total Paid:    %-8.2f (%d bills)                                                      *\n",
               totalPaid, paidCount);
        printf("* - Total Unpaid:  %-8.2f (%d bills)                                                      *\n",
               totalUnpaid, unpaidCount);
        printf("* - Grand Total:   %-8.2f (%d bills)                                                      *\n",
               totalPaid + totalUnpaid, paidCount + unpaidCount);
        printf("*===============================================================================================*\n");

        if(currentRole == 3 && unpaidCount > 0) {  // Only show for patients with unpaid bills
            printf("* NOTE: You have %d unpaid bill(s) totaling %.2f. Use 'Pay Bill' option to make payment. *\n",
                   unpaidCount, totalUnpaid);
            printf("*===============================================================================================*\n");
        }
    }
}

void payBill() {
    float totalUnpaid = calculateTotalUnpaidBills();

    printf("\n");
    printf("*==============================================*\n");
    printf("*                  PAYMENT MENU                *\n");
    printf("*==============================================*\n");

    if (totalUnpaid > 0.01) {
        printf("* 1. Pay Total Bills (%-10.2f)          *\n", totalUnpaid);
    } else {
        printf("* No unpaid bills available.                  *\n");
    }

    printf("* 0. Cancel                                    *\n");
    printf("*==============================================*\n");
    printf("Enter your choice: ");

    int menuChoice;
    scanf("%d", &menuChoice);
    while (getchar() != '\n');

    if (menuChoice == 0) {
        printf("\nPayment cancelled.\n");
        return;
    }

    if (menuChoice == 1 && totalUnpaid > 0.01) {
        printf("\nYou have %.2f in unpaid bills.\n", totalUnpaid);
        printf("Confirm payment of total amount? (Y/N): ");
        char confirm;
        scanf(" %c", &confirm);
        while (getchar() != '\n');

        if (toupper(confirm) != 'Y') {
            printf("\nPayment cancelled.\n");
            return;
        }

        // Payment method selection
        printf("\nSelect Payment Method:\n");
        printf("1. Credit/Debit Card\n");
        printf("2. Cash\n");
        printf("3. bKash\n");
        printf("4. Nagad\n");
        printf("5. Rocket\n");
        printf("Enter choice (1-5): ");
        int paymentChoice;
        scanf("%d", &paymentChoice);
        while (getchar() != '\n');

        const char* paymentMethods[] = { "Card", "Cash", "bKash", "Nagad", "Rocket" };

        if (paymentChoice < 1 || paymentChoice > 5) {
            printf("\nInvalid payment method selected.\n");
            return;
        }

        // Process payment for all unpaid bills
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char paymentDate[MAX_LEN];
        strftime(paymentDate, MAX_LEN, "%Y-%m-%d", t);

        int billsPaid = 0;
        for (int i = 0; i < billCount; i++) {
            if (strcmp(bills[i].patientUsername, currentUsername) == 0 && bills[i].isPaid == 0) {
                // Apply discount if applicable
                if (bills[i].amount >= 5000) {
                    bills[i].discount = bills[i].amount * 0.10;
                    bills[i].finalAmount = bills[i].amount - bills[i].discount;
                } else {
                    bills[i].discount = 0.0;
                    bills[i].finalAmount = bills[i].amount;
                }

                bills[i].isPaid = 1;
                strcpy(bills[i].paymentMethod, paymentMethods[paymentChoice - 1]);
                strcpy(bills[i].billDate, paymentDate);
                bills[i].billNumber = (rand() % 100) + 1;
                billsPaid++;
            }
        }

        saveBillsToFile();
        printf("\n");
        printf("*==============================================*\n");
        printf("*           TOTAL PAYMENT PROCESSED            *\n");
        printf("*==============================================*\n");
        printf("* Paid %.2f for %-3d bill(s)                 *\n", totalUnpaid, billsPaid);
        printf("* Payment Method: %-25s *\n", paymentMethods[paymentChoice - 1]);
        printf("* Payment Date: %-26s *\n", paymentDate);
        printf("*==============================================*\n");
        return;
    }

    printf("\nInvalid choice.\n");
}


float calculateTotalUnpaidBills() {
    float total = 0.0;
    for (int i = 0; i < billCount; i++) {
        if (strcmp(bills[i].patientUsername, currentUsername) == 0 && bills[i].isPaid == 0) {
            // Calculate fresh every time
            float amt = bills[i].amount;
            if (amt >= 5000) {
                amt -= amt * 0.10; // 10% discount
            }
            total += amt;
        }
    }
    return total;
}

void adminBillingReport() {
    printf("\n");
    printf("*===================================================================================================*\n");
    printf("*                                      BILLING REPORT                                              *\n");
    printf("*===================================================================================================*\n");
    printf("* %-20s | %-20s | Amount  | Discount | Final   | Method   | Status   | Date       *\n",
           "Patient", "Doctor");
    printf("*----------------------+----------------------+---------+----------+---------+----------+----------+------------*\n");

    float totalRevenue = 0;
    float paidRevenue = 0;
    float totalDiscount = 0;

    for(int i = 0; i < billCount; i++) {
        char patientName[MAX_LEN] = "Unknown";
        for(int j = 0; j < appointmentCount; j++) {
            if(strcmp(appointments[j].patientUsername, bills[i].patientUsername) == 0) {
                strcpy(patientName, appointments[j].patientName);
                break;
            }
        }

        printf("* %-20s | %-20s | %-7d | %-8.2f | %-7.2f | %-8s | %-8s | %-10s *\n",
              patientName,
              bills[i].doctorName,
              bills[i].amount,
              bills[i].discount,
              bills[i].finalAmount,
              bills[i].isPaid ? bills[i].paymentMethod : "N/A",
              bills[i].isPaid ? "Paid" : "Unpaid",
              bills[i].date);

        totalRevenue += bills[i].amount;
        if(bills[i].isPaid) {
            paidRevenue += bills[i].finalAmount;
        }
        totalDiscount += bills[i].discount;
    }

    printf("*===================================================================================================*\n");
    printf("\n* Total Revenue: %-60.2f *\n", totalRevenue);
    printf("* Total Discounts: %-58.2f *\n", totalDiscount);
    printf("* Paid Revenue: %-61.2f *\n", paidRevenue);
    printf("* Outstanding: %-62.2f *\n", totalRevenue - paidRevenue - totalDiscount);
    printf("*=============================================================================*\n");
}
void searchDoctorBySpecialization() {
    char searchTerm[MAX_LEN];
    printf("\n");
    printf("*==============================================*\n");
    printf("*           SEARCH DOCTORS BY SPECIALTY        *\n");
    printf("*==============================================*\n");
    printf("Enter specialization to search: ");
    fgets(searchTerm, MAX_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    printf("\n");
    printf("*=============================================================*\n");
    printf("*                      SEARCH RESULTS                         *\n");
    printf("*=============================================================*\n");
    printf("* %-20s | %-20s | %-20s | %-6s *\n", "Name", "Specialization", "Schedule", "Fee");
    printf("*----------------------+----------------------+----------------------+--------*\n");

    int found = 0;
    for(int i = 0; i < doctorCount; i++) {
        if(strstr(doctors[i].specialization, searchTerm) ||
           strcasecmp(doctors[i].specialization, searchTerm) == 0) {
            printf("* %-20s | %-20s | %-20s | %-6d *\n",
                  doctors[i].name,
                  doctors[i].specialization,
                  doctors[i].timeSchedule,
                  doctors[i].charge);
            found = 1;
        }
    }

    if(!found) {
        printf("* %-66s *\n", "No doctors found with that specialization");
    }
    printf("*=============================================================*\n");
}

int userExists(const char *username) {
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return 1;
        }
    }
    return 0;
}

void debugCheckPrescriptionFile() {
    FILE *test = fopen("prescriptions.dat", "rb");
    if(test) {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 DEBUG INFO                   *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    Prescription file exists (%ld bytes)      *\n", ftell(test));
        printf("*                                              *\n");
        printf("*==============================================*\n");
        fclose(test);
    } else {
        printf("\n");
        printf("*==============================================*\n");
        printf("*                 DEBUG INFO                   *\n");
        printf("*==============================================*\n");
        printf("*                                              *\n");
        printf("*    No prescription file found                *\n");
        printf("*                                              *\n");
        printf("*==============================================*\n");
    }
}

void listDoctorsWithUsernames() {
    printf("\n");
    printf("*=======================================================================*\n");
    printf("*                          DOCTORS LIST                                 *\n");
    printf("*=======================================================================*\n");
    printf("* %-20s | %-20s | %-20s | %-20s | %-6s *\n",
           "Username", "Name", "Specialization", "Schedule", "Fee");
    printf("*----------------------+----------------------+----------------------+----------------------+--------*\n");

    for(int i = 0; i < doctorCount; i++) {
        printf("* %-20s | %-20s | %-20s | %-20s | %-6d *\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].timeSchedule,
               doctors[i].charge);
    }
    printf("*=======================================================================*\n");
}

void generateMonthlyReport() {
    int month, year;
    printf("\n");
    printf("*==============================================*\n");
    printf("*            MONTHLY APPOINTMENT REPORT        *\n");
    printf("*==============================================*\n");
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter year: ");
    scanf("%d", &year);
    while(getchar() != '\n');

    printf("\n");
    printf("*=============================================================*\n");
    printf("*    Appointments for %02d/%d                                 *\n", month, year);
    printf("*=============================================================*\n");
    printf("* %-20s | %-20s | %-10s | %-8s *\n", "Doctor", "Patient", "Date", "Time");
    printf("*----------------------+----------------------+------------+----------*\n");

    int found = 0;
    for(int i = 0; i < appointmentCount; i++) {
        int apptYear, apptMonth, apptDay;
        if(sscanf(appointments[i].date, "%d-%d-%d", &apptYear, &apptMonth, &apptDay) != 3) {
            continue;
        }

        if(apptYear == year && apptMonth == month) {
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }

            printf("* %-20s | %-20s | %-10s | %-8s *\n",
                   doctorName,
                   appointments[i].patientName,
                   appointments[i].date,
                   appointments[i].time);
            found = 1;
        }
    }

    if(!found) {
        printf("* No appointments found for this month                *\n");
    }
    printf("*=============================================================*\n");
}

void generateAnnualReport() {
    int year;
    printf("\n");
    printf("*==============================================*\n");
    printf("*            ANNUAL APPOINTMENT REPORT         *\n");
    printf("*==============================================*\n");
    printf("Enter year: ");
    scanf("%d", &year);
    while(getchar() != '\n');

    printf("\n");
    printf("*=========================================================================*\n");
    printf("*    Appointments for %d                                                  *\n", year);
    printf("*=========================================================================*\n");
    printf("* %-6s | %-20s | %-20s | %-10s *\n", "Month", "Doctor", "Patient", "Date");
    printf("*--------+----------------------+----------------------+------------*\n");

    int monthlyCount[12] = {0};
    int doctorAppointments[MAX_DOCTORS] = {0};
    char doctorNames[MAX_DOCTORS][MAX_LEN] = {0};

    for(int i = 0; i < doctorCount; i++) {
        strcpy(doctorNames[i], doctors[i].name);
    }

    int found = 0;
    for(int i = 0; i < appointmentCount; i++) {
        int apptYear, apptMonth, apptDay;
        if(sscanf(appointments[i].date, "%d-%d-%d", &apptYear, &apptMonth, &apptDay) != 3) {
            continue;
        }

        if(apptYear == year) {
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    doctorAppointments[j]++;
                    break;
                }
            }

            const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

            printf("* %-6s | %-20s | %-20s | %-10s *\n",
                   months[apptMonth-1],
                   doctorName,
                   appointments[i].patientName,
                   appointments[i].date);
            monthlyCount[apptMonth-1]++;
            found = 1;
        }
    }

    if(!found) {
        printf("* No appointments found for this year                          *\n");
    }
    printf("*=========================================================================*\n");

    // Display summary statistics
    printf("\n");
    printf("*==============================================*\n");
    printf("*               ANNUAL SUMMARY                 *\n");
    printf("*==============================================*\n");
    printf("* %-10s | %-15s *\n", "Month", "Appointments");
    printf("*------------+-----------------*\n");

    const char *months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};

    for(int i = 0; i < 12; i++) {
        printf("* %-10s | %-15d *\n", months[i], monthlyCount[i]);
    }
    printf("*==============================================*\n");
}
void showWelcomeMessage() {
    printf("\n");
    printf("                                  **=================================================**\n");
    printf("                                  ||                                                 ||\n");
    printf("                                  ||    H   H  EEEEE  AAAAA  L      TTTTT  H   H     ||\n");
    printf("                                  ||    H   H  E      A   A  L        T    H   H     ||\n");
    printf("                                  ||    HHHHH  EEEE   AAAAA  L        T    HHHHH     ||\n");
    printf("                                  ||    H   H  E      A   A  L        T    H   H     ||\n");
    printf("                                  ||    H   H  EEEEE  A   A  LLLLL    T    H   H     ||\n");
    printf("                                  ||                                                 ||\n");
    printf("                                  ||       HEALTH CARE MANAGEMENT SYSTEM             ||\n");
    printf("                                  ||                                                 ||\n");
    printf("                                  **=================================================**\n");
    printf("\n");
}
