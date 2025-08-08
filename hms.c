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
void loadAppointmentsFromFile();
void cancelAppointment();
void rescheduleAppointment();
void saveBillsToFile();
void loadBillsFromFile();
void viewBills();
void payBill();
void adminBillingReport();
void showWelcomeMessage();

/* ========== MAIN FUNCTION ========== */
int main() {
    initializeDoctors();
    loadUsersFromFile();
    loadAppointmentsFromFile();
    loadBillsFromFile();
    showWelcomeMessage();

    int choice, loggedIn;
    while(1) {

printf("\n");
printf("*==============================================*\n");
printf("*                    Main Menu                 *\n");
printf("*==============================================*\n");
printf("\n");
        printf("1. Register\n2. Login\n3. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: registerUser(); break;
            case 2: loggedIn = loginUser(&currentRole);
                    if(loggedIn) {
        printf("\n");
        printf("===============================================\n");
        printf("|                                             |\n");
        printf("|           Login successful! Welcome         |\n");
        printf("|                %-20s         |\n", currentUsername);
        printf("|                                             |\n");
        printf("===============================================\n");
        printf("\n");

                        afterLoginMenu(currentRole);
                    }
                    else printf("Invalid credentials!\n");
                    break;
            case 3: saveUsersToFile();
                    saveAppointmentsToFile();
                    saveBillsToFile();
                    printf("\nData saved. Exiting system...\n");
                    exit(0);
            default: printf("Invalid choice!\n");
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
/* ========== CLOSING FILE HANDLING ========== */

void generateMonthlyReport() {
    int month, year;
    printf("\n=== MONTHLY APPOINTMENT REPORT ===\n");
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter year: ");
    scanf("%d", &year);
    while(getchar() != '\n');

    printf("\nAppointments for %d/%d:\n", month, year);
    printf("+----------------------+----------------------+----------------------+----------------------+\n");
    printf("| %-20s | %-20s | %-20s | %-20s |\n", "Doctor", "Patient", "Date", "Time");
    printf("+----------------------+----------------------+----------------------+----------------------+\n");

    int found = 0;
    for(int i = 0; i < appointmentCount; i++) {
        // Parse appointment date
        int apptYear, apptMonth, apptDay;
        if(sscanf(appointments[i].date, "%d-%d-%d", &apptYear, &apptMonth, &apptDay) != 3) {
            continue; // Skip if date format is invalid
        }

        if(apptYear == year && apptMonth == month) {
            // Find doctor's name
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }

            printf("| %-20s | %-20s | %-20s | %-20s |\n",
                   doctorName,
                   appointments[i].patientName,
                   appointments[i].date,
                   appointments[i].time);
            found = 1;
        }
    }

    if(!found) {
        printf("| %-76s |\n", "No appointments found for this month");
    }
    printf("+----------------------+----------------------+----------------------+----------------------+\n");
}

void generateAnnualReport() {
    int year;
    printf("\n=== ANNUAL APPOINTMENT REPORT ===\n");
    printf("Enter year: ");
    scanf("%d", &year);
    while(getchar() != '\n');

    // Array to count appointments per month
    int monthlyCount[12] = {0};
    int doctorAppointments[MAX_DOCTORS] = {0};
    char doctorNames[MAX_DOCTORS][MAX_LEN] = {0};

    // Initialize doctor names
    for(int i = 0; i < doctorCount; i++) {
        strcpy(doctorNames[i], doctors[i].name);
    }

    printf("\nAppointments for %d:\n", year);
    printf("+--------+----------------------+----------------------+----------------------+\n");
    printf("| %-6s | %-20s | %-20s | %-20s |\n", "Month", "Doctor", "Patient", "Date");
    printf("+--------+----------------------+----------------------+----------------------+\n");

    int found = 0;
    for(int i = 0; i < appointmentCount; i++) {
        // Parse appointment date
        int apptYear, apptMonth, apptDay;
        if(sscanf(appointments[i].date, "%d-%d-%d", &apptYear, &apptMonth, &apptDay) != 3) {
            continue; // Skip if date format is invalid
        }

        if(apptYear == year) {
            // Find doctor's name
            char doctorName[MAX_LEN] = "Unknown";
            for(int j = 0; j < doctorCount; j++) {
                if(strcmp(doctors[j].id, appointments[i].doctorName) == 0) {
                    strcpy(doctorName, doctors[j].name);
                    doctorAppointments[j]++;
                    break;
                }
            }

            // Get month name
            const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

            printf("| %-6s | %-20s | %-20s | %-20s |\n",
                   months[apptMonth-1],
                   doctorName,
                   appointments[i].patientName,
                   appointments[i].date);
            monthlyCount[apptMonth-1]++;
            found = 1;
        }
    }

    if(!found) {
        printf("| %-70s |\n", "No appointments found for this year");
    }
    printf("+--------+----------------------+----------------------+----------------------+\n");

    // Print summary statistics
    printf("\n=== SUMMARY STATISTICS ===\n");
    printf("+--------+----------------+\n");
    printf("| Month  | Appointments   |\n");
    printf("+--------+----------------+\n");

    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for(int i = 0; i < 12; i++) {
        printf("| %-6s | %-14d |\n", months[i], monthlyCount[i]);
    }
    printf("+--------+----------------+\n");

    // Doctor-wise statistics
    printf("\nDoctor-wise Appointments:\n");
    printf("+----------------------+----------------+\n");
    printf("| Doctor               | Appointments   |\n");
    printf("+----------------------+----------------+\n");
    for(int i = 0; i < doctorCount; i++) {
        printf("| %-20s | %-14d |\n", doctorNames[i], doctorAppointments[i]);
    }
    printf("+----------------------+----------------+\n");
}
/* ========== REGISTRATION ========== */
void registerUser() {
    if(userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }

    printf("\n=== Registration ===\n");
    printf("1. Admin\n2. Doctor\n3. Patient\n");
    printf("Enter role (1-3): ");
    scanf("%d", &users[userCount].role);
    while(getchar() != '\n');

    // Get username - must be unique
    char tempUsername[MAX_LEN];
    int usernameExists;
    do {
        usernameExists = 0;
        printf("Username: ");
        scanf("%99s", tempUsername);
        while(getchar() != '\n');

        for(int i = 0; i < userCount; i++) {
            if(strcmp(users[i].username, tempUsername) == 0) {
                printf("Username already exists! Please choose another.\n");
                usernameExists = 1;
                break;
            }
        }
    } while(usernameExists);

    strcpy(users[userCount].username, tempUsername);

    // Get password
    printf("Password: ");
    do {
        inputPassword(users[userCount].password);
        if(!isPasswordStrong(users[userCount].password)) {
            printf("Password must contain:\n- 8+ characters\n- Uppercase & lowercase\n- Number\n");
            printf("Please try again: ");
        }
    } while(!isPasswordStrong(users[userCount].password));

    // Generate user ID
    sprintf(users[userCount].id, "U%03d", userCount+1);

    // Handle role-specific registration
    if(users[userCount].role == 2) { // Doctor registration
        if(doctorCount >= MAX_DOCTORS) {
            printf("Cannot register more doctors! Limit reached.\n");
            return;
        }

        printf("\n=== DOCTOR REGISTRATION ===\n");

        // Doctor name
        printf("Full Name: ");
        fgets(doctors[doctorCount].name, MAX_LEN, stdin);
        doctors[doctorCount].name[strcspn(doctors[doctorCount].name, "\n")] = 0;

        // Store username as doctor ID
        strcpy(doctors[doctorCount].id, users[userCount].username);

        // Specialization
        printf("Specialization: ");
        fgets(doctors[doctorCount].specialization, MAX_LEN, stdin);
        doctors[doctorCount].specialization[strcspn(doctors[doctorCount].specialization, "\n")] = 0;

        // Schedule
        printf("Schedule (e.g. 9AM-5PM): ");
        fgets(doctors[doctorCount].timeSchedule, MAX_LEN, stdin);
        doctors[doctorCount].timeSchedule[strcspn(doctors[doctorCount].timeSchedule, "\n")] = 0;

        // Fee
        printf("Consultation Fee: ");
        scanf("%d", &doctors[doctorCount].charge);
        while(getchar() != '\n');

        doctorCount++;
        printf("Doctor registration successful!\n");
    }
    else if(users[userCount].role == 3) { // Patient registration
        printf("\n=== PATIENT REGISTRATION ===\n");
        addHealthRecord(users[userCount].username);
    }

    userCount++;
    saveUsersToFile();
    printf("\nRegistration successful! Your ID: %s\n", users[userCount-1].id);
}
int loginUser(int *role) {
    char uname[MAX_LEN], pass[MAX_LEN];
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

    printf("\n=== Health Details ===\n");
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
        default: printf("Invalid role!\n");
    }
}
void adminMenu() {
    int choice;
    while(1) {
        printf("\n");
printf("*==============================================*\n");
printf("*                   Admin Menu                 *\n");
printf("*==============================================*\n");
printf("\n");
        printf("1. View Doctors\n");
        printf("2. View Appointments\n");
        printf("3. List Doctors with Usernames\n");
        printf("4. Generate Monthly Report\n");
        printf("5. Generate Annual Report\n");
        printf("6. Billing Report\n");         // New option
        printf("7. Logout\n");
        printf("Choose: ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: viewDoctors(); break;
            case 2: appointmentMenu(); break;
            case 3: listDoctorsWithUsernames(); break;
            case 4: generateMonthlyReport(); break;
            case 5: generateAnnualReport(); break;
            case 6: adminBillingReport(); break;
            case 7: return;
            default: printf("Invalid choice!\n");
        }
    }
}

void doctorMenu() {
    int choice;
    while(1) {
        printf("\n");
printf("*==============================================*\n");
printf("*                   Doctor Menu                *\n");
printf("*==============================================*\n");
printf("\n");
        printf("1. View My Appointments\n");
        printf("2. Write Prescription\n");
        printf("3. Search Patient Prescriptions\n");  // Changed option text
        printf("4. Logout\n");
        printf("Choose: ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice) {
            case 1: viewDoctorAppointments(); break;
            case 2: writePrescription(); break;
            case 3: viewPrescriptions(); break;  // This now does the search
            case 4: return;
            default: printf("Invalid choice!\n");
        }
    }
}
void patientMenu() {
    int choice;
    while(1) {
        printf("\n");
printf("*==============================================*\n");
printf("*                 Patient Menu                 *\n");
printf("*==============================================*\n");
printf("\n");
        printf("1. View Doctors\n");
        printf("2. Book Appointment\n");
        printf("3. View My Appointments\n");
        printf("4. Cancel Appointment\n");      // New option
        printf("5. Reschedule Appointment\n");  // New option
        printf("6. View Bills\n");              // New option
        printf("7. Pay Bill\n");                // New option
        printf("8. Search Doctors\n");
        printf("9. Logout\n");
        printf("Choose: ");
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
            case 9: return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ========== DOCTOR FUNCTIONS ========== */
void viewDoctors() {
    printf("\n=== AVAILABLE DOCTORS ===\n");
    printf("%-5s %-20s %-20s %-15s %s\n",
           "ID", "Name", "Specialization", "Schedule", "Fee");

    for(int i = 0; i < doctorCount; i++) {
        printf("%-5s %-20s %-20s %-15s %d\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].timeSchedule,
               doctors[i].charge);
    }
}
void initializeDoctors(void)
{
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
    printf("\n=== YOUR APPOINTMENTS ===\n");
    int found = 0;

    for(int i = 0; i < appointmentCount; i++) {
        // Compare with current doctor's username
        if(strcmp(appointments[i].doctorName, currentUsername) == 0) {
            printf("Patient: %s\n", appointments[i].patientName);
            printf("Date: %s at %s\n", appointments[i].date, appointments[i].time);
            printf("Reason: %s\n\n", appointments[i].disease);
            found = 1;
        }
    }

    if(!found) {
        printf("No appointments found.\n");
        printf("Debug: Current doctor username: %s\n", currentUsername);
        printf("Debug: Total appointments: %d\n", appointmentCount);
    }
}
void writePrescription() {
    struct Prescription p;
    p.itemCount = 0;

    strcpy(p.doctorName, currentUsername);

    // Get current date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(p.date, MAX_LEN, "%Y-%m-%d", t);

    printf("\n=== NEW PRESCRIPTION ===\n");

    // Get patient details
    do {
        printf("Patient Username: ");
        scanf("%s", p.patientUsername);
        while(getchar() != '\n');  // Clear input buffer

        if(!userExists(p.patientUsername)) {
            printf("Error: No patient found with username '%s'!\n", p.patientUsername);
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

    // Get doctor name
    for(int i = 0; i < doctorCount; i++) {
        if(strstr(doctors[i].name, currentUsername)) {
            strcpy(p.doctorName, doctors[i].name);
            break;
        }
    }

    // Add medicines
    int choice;
    do {
        if(p.itemCount >= 20) {
            printf("Maximum 20 medicines reached.\n");
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

    // Save and display
    savePrescriptionToFile(p);
    displayPrescription(p);
}
/* ========== PRESCRIPTION DISPLAY ========== */
void displayPrescription(struct Prescription p) {
    printf("\n\n=== PRESCRIPTION ===\n");
    printf("+-------------------------------------------------------+\n");
    printf("| %-20s: %-30s |\n", "Date", p.date);
    printf("| %-20s: Dr. %-26s |\n", "Doctor", p.doctorName);
    printf("| %-20s: %-30s |\n", "Patient", p.patientName);

    // Fixed age/sex display
    char ageSex[20];
    if (strlen(p.patientAge) > 0 && strlen(p.patientSex) > 0) {
        snprintf(ageSex, sizeof(ageSex), "%s/%s", p.patientAge, p.patientSex);
    } else {
        strcpy(ageSex, "N/A");
    }
    printf("| %-20s: %-30s |\n", "Age/Sex", ageSex);

    printf("+-------------------------------------------------------+\n");
    printf("| %-20s | %-30s |\n", "MEDICINE", "DOSAGE");
    printf("+----------------------+---------------------------------+\n");

    for(int i = 0; i < p.itemCount; i++) {
        printf("| %-20s | %-30s |\n",
               p.items[i].medicine, p.items[i].dosage);
    }

    printf("+-------------------------------------------------------+\n");
    printf("| Doctor's Signature: ________________________           |\n");
    printf("|                        (Dr. %-25s) |\n", p.doctorName);
    printf("+-------------------------------------------------------+\n\n");
}
void savePrescriptionToFile(struct Prescription p) {
    FILE *file = fopen("prescriptions.dat", "ab");
    if(!file) {
        printf("Error: Cannot open prescriptions file\n");
        return;
    }

    // Write each field separately for reliability
    fwrite(p.patientUsername, sizeof(char), MAX_LEN, file);
    fwrite(p.patientName, sizeof(char), MAX_LEN, file);
    fwrite(p.patientAge, sizeof(char), 10, file);
    fwrite(p.patientSex, sizeof(char), 10, file);
    fwrite(p.doctorName, sizeof(char), MAX_LEN, file);
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

    printf("\nEnter patient username to search: ");
    scanf("%99s", searchUsername);
    while(getchar() != '\n');  // Clear input buffer

    FILE *file = fopen("prescriptions.dat", "rb");
    if(!file) {
        printf("\nNo prescriptions found in the system.\n");
        return;
    }

    printf("\n=== PRESCRIPTIONS FOR %s ===\n", searchUsername);
    struct Prescription p;
    int found = 0;

    while(1) {
        // Read each field separately
        if(fread(p.patientUsername, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.patientName, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.patientAge, sizeof(char), 10, file) != 10) break;
        if(fread(p.patientSex, sizeof(char), 10, file) != 10) break;
        if(fread(p.doctorName, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(p.date, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        if(fread(&p.itemCount, sizeof(int), 1, file) != 1) break;

        // Read medicine items
        for(int i = 0; i < p.itemCount; i++) {
            if(fread(p.items[i].medicine, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
            if(fread(p.items[i].dosage, sizeof(char), MAX_LEN, file) != MAX_LEN) break;
        }

        // Compare usernames
        if(strcmp(p.patientUsername, searchUsername) == 0) {
            displayPrescription(p);
            found = 1;
        }
    }

    if(!found) {
        printf("No prescriptions found for username: %s\n", searchUsername);
    }
    fclose(file);
}
/* ========== APPOINTMENT FUNCTIONS ========== */
void billingMenu() {
    if (doctorCount == 0) {
        printf("No doctors available!\n");
        return;
    }

    // Display available doctors
    printf("\n=== AVAILABLE DOCTORS ===\n");
    printf("%-3s %-20s %-20s %-15s %s\n",
           "No", "Name", "Specialization", "Schedule", "Fee");

    for (int i = 0; i < doctorCount; i++) {
        printf("%-3d %-20s %-20s %-15s %d\n",
               i+1,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].timeSchedule,
               doctors[i].charge);
    }

    // Get doctor selection
    int docChoice;
    printf("\nSelect Doctor (1-%d): ", doctorCount);
    scanf("%d", &docChoice);
    while (getchar() != '\n');  // Clear input buffer

    if (docChoice < 1 || docChoice > doctorCount) {
        printf("Invalid selection!\n");
        return;
    }

    // Create new appointment
    struct Appointment a;

    // Get patient details
    printf("\nPatient Name: ");
    fgets(a.patientName, MAX_LEN, stdin);
    a.patientName[strcspn(a.patientName, "\n")] = '\0';

    strcpy(a.patientUsername, currentUsername);

    printf("Medical Issue: ");
    fgets(a.disease, MAX_LEN, stdin);
    a.disease[strcspn(a.disease, "\n")] = '\0';

    // Get appointment date
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

    // Store doctor's username (from doctor record) in appointment
    strcpy(a.doctorName, doctors[docChoice-1].id);
    strcpy(a.time, doctors[docChoice-1].timeSchedule);
    a.charge = doctors[docChoice-1].charge;

    // Add to appointments array
    if (appointmentCount < MAX_APPOINTMENTS) {
        appointments[appointmentCount++] = a;
        saveAppointmentsToFile();

        // Generate bill
        if(billCount < MAX_APPOINTMENTS) {
            struct Bill newBill;
            strcpy(newBill.patientUsername, currentUsername);
            strcpy(newBill.doctorName, doctors[docChoice-1].name);
            strcpy(newBill.date, a.date);
            newBill.amount = a.charge;
            newBill.isPaid = 0; // Unpaid by default

            // Set bill date to current date
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            strftime(newBill.billDate, MAX_LEN, "%Y-%m-%d", t);

            bills[billCount++] = newBill;
            saveBillsToFile();

            // Display billing information
            printf("\n=== BILL GENERATED ===\n");
            printf("+-------------------------------+\n");
            printf("| %-15s: %-15s |\n", "Patient", a.patientName);
            printf("| %-15s: Dr. %-12s |\n", "Doctor", doctors[docChoice-1].name);
            printf("| %-15s: %-15s |\n", "Date", a.date);
            printf("| %-15s: %-15d |\n", "Amount", newBill.amount);
            printf("| %-15s: %-15s |\n", "Status", "UNPAID");
            printf("| %-15s: %-15s |\n", "Bill Date", newBill.billDate);
            printf("+-------------------------------+\n");
        }
    } else {
        printf("Cannot book more appointments! Limit reached.\n");
        return;
    }

    // Appointment confirmation
    printf("\n=== APPOINTMENT CONFIRMED ===\n");
    printf("Doctor: Dr. %s\n", doctors[docChoice-1].name);
    printf("Patient: %s\n", a.patientName);
    printf("Date: %s at %s\n", a.date, a.time);
    printf("Reason: %s\n", a.disease);
    printf("Charge: %d\n", a.charge);
    printf("Bill Number: %d\n", billCount);
    printf("\nPlease remember your bill number for payment.\n");
}
void appointmentMenu() {
    printf("\n=== APPOINTMENTS ===\n");
    int found = 0;
    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].patientUsername, currentUsername) == 0 || currentRole == 1) {
            printf("%d. %s on %s at %s\n", i+1,
                  appointments[i].doctorName,
                  appointments[i].date,
                  appointments[i].time);
            printf("   Reason: %s\n", appointments[i].disease);
            found = 1;
        }
    }
    if(!found) printf("No appointments found.\n");
}

void cancelAppointment() {
    printf("\n=== YOUR APPOINTMENTS ===\n");
    int count = 0;
    int indices[MAX_APPOINTMENTS];

    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].patientUsername, currentUsername) == 0) {
            printf("%d. Dr. %s on %s at %s\n",
                  count+1,
                  appointments[i].doctorName,
                  appointments[i].date,
                  appointments[i].time);
            indices[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("No appointments found.\n");
        return;
    }

    printf("\nSelect appointment to cancel (1-%d, 0 to cancel): ", count);
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice < 1 || choice > count) {
        printf("Cancellation aborted.\n");
        return;
    }

    int indexToRemove = indices[choice-1];
    for(int i = indexToRemove; i < appointmentCount-1; i++) {
        appointments[i] = appointments[i+1];
    }
    appointmentCount--;
    saveAppointmentsToFile();

    printf("Appointment cancelled successfully.\n");
}

void rescheduleAppointment() {
    printf("\n=== YOUR APPOINTMENTS ===\n");
    int count = 0;
    int indices[MAX_APPOINTMENTS];

    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].patientUsername, currentUsername) == 0) {
            printf("%d. Dr. %s on %s at %s\n",
                  count+1,
                  appointments[i].doctorName,
                  appointments[i].date,
                  appointments[i].time);
            indices[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("No appointments found.\n");
        return;
    }

    printf("\nSelect appointment to reschedule (1-%d, 0 to cancel): ", count);
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice < 1 || choice > count) {
        printf("Rescheduling aborted.\n");
        return;
    }

    struct Appointment *apt = &appointments[indices[choice-1]];

    printf("Current date: %s\n", apt->date);
    printf("Enter new date (YYYY-MM-DD): ");
    char newDate[MAX_LEN];
    fgets(newDate, MAX_LEN, stdin);
    newDate[strcspn(newDate, "\n")] = '\0';

    printf("Current time: %s\n", apt->time);
    printf("Enter new time: ");
    char newTime[MAX_LEN];
    fgets(newTime, MAX_LEN, stdin);
    newTime[strcspn(newTime, "\n")] = '\0';

    strcpy(apt->date, newDate);
    strcpy(apt->time, newTime);
    saveAppointmentsToFile();

    printf("Appointment rescheduled successfully.\n");
}

/* ========== BILLING MANAGEMENT ========== */
void viewBills() {
    printf("\n=== YOUR BILLS ===\n");
    int found = 0;

    for(int i = 0; i < billCount; i++) {
        if(strcmp(bills[i].patientUsername, currentUsername) == 0) {
            printf("Bill #%d\n", i+1);
            printf("Doctor: %s\n", bills[i].doctorName);
            printf("Appointment Date: %s\n", bills[i].date);
            printf("Amount: %d\n", bills[i].amount);
            printf("Status: %s\n", bills[i].isPaid ? "Paid" : "Unpaid");
            printf("Bill Date: %s\n\n", bills[i].billDate);
            found = 1;
        }
    }

    if(!found) {
        printf("No bills found.\n");
    }
}

void payBill() {
    viewBills();

    if(billCount == 0) return;

    printf("\nEnter bill number to pay (0 to cancel): ");
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');

    if(choice < 1 || choice > billCount) {
        printf("Payment cancelled.\n");
        return;
    }

    if(bills[choice-1].isPaid) {
        printf("This bill is already paid.\n");
        return;
    }

    bills[choice-1].isPaid = 1;
    saveBillsToFile();

    printf("Payment of %d processed successfully.\n", bills[choice-1].amount);
}

void adminBillingReport() {
    printf("\n=== BILLING REPORT ===\n");
    printf("+----------------------+----------------------+------------+--------+----------------+\n");
    printf("| Patient              | Doctor               | Date       | Amount | Status         |\n");
    printf("+----------------------+----------------------+------------+--------+----------------+\n");

    float totalRevenue = 0;
    float paidRevenue = 0;

    for(int i = 0; i < billCount; i++) {
        printf("| %-20s | %-20s | %-10s | %-6d | %-14s |\n",
              bills[i].patientUsername,
              bills[i].doctorName,
              bills[i].date,
              bills[i].amount,
              bills[i].isPaid ? "Paid" : "Unpaid");

        totalRevenue += bills[i].amount;
        if(bills[i].isPaid) {
            paidRevenue += bills[i].amount;
        }
    }

    printf("+----------------------+----------------------+------------+--------+----------------+\n");
    printf("\nTotal Revenue: %.2f\n", totalRevenue);
    printf("Paid Revenue: %.2f\n", paidRevenue);
    printf("Outstanding: %.2f\n", totalRevenue - paidRevenue);
}
void searchDoctorBySpecialization() {
    char spec[MAX_LEN];
    printf("Enter specialization: ");
    scanf("%s", spec);

    printf("\n=== SEARCH RESULTS ===\n");
    int found = 0;
    for(int i = 0; i < doctorCount; i++) {
        if(strstr(doctors[i].specialization, spec)) {
            printf("Dr. %s (%s) - %s - Fee: %d\n",
                  doctors[i].name,
                  doctors[i].specialization,
                  doctors[i].timeSchedule,
                  doctors[i].charge);
            found = 1;
        }
    }
    if(!found) printf("No doctors found.\n");
}
int userExists(const char *username) {
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return 1;  // User exists
        }
    }
    return 0;  // User not found
}
void debugCheckPrescriptionFile() {
    FILE *test = fopen("prescriptions.dat", "rb");
    if(test) {
        printf("\nDEBUG: Prescription file exists (%ld bytes)\n",
              ftell(test));
        fclose(test);
    } else {
        printf("\nDEBUG: No prescription file found\n");
    }
}
void listDoctorsWithUsernames() {
    printf("\n=== DOCTORS LIST ===\n");
    printf("%-20s %-20s %-15s\n", "Username", "Name", "Specialization");
    for(int i = 0; i < doctorCount; i++) {
        printf("%-20s %-20s %-15s\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization);
    }
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

