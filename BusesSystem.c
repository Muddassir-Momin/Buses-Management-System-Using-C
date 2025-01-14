#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 30

typedef struct {
    char name[50];
    char rollNo[15];
    char contactNumber[10];
    char email[50];
    char password[50];
} User;

typedef struct {
    char routeName[50];
    char stops[4][50];
    int prices[4];
    int seatStatus[MAX_SEATS]; // 0 = Available, 1 = Booked
} Route;

typedef struct {
    char rollNo[15];
    char route[50];
    char stop[50];
    int seatNumber;
    int price;
} Booking;

// Global variables
Route routes[3]; // 3 routes
FILE *userFile, *bookingFile;

// Function declarations
void initializeRoutes();
void registerUser();
int loginUser(char *loggedInRollNo);
void displayRoutes();
void displaySeatAvailability(int routeIndex);
void bookTicket(char *loggedInRollNo);
void showBookingHistory(char *loggedInRollNo);

// Main function
int main() {
    char loggedInRollNo[15] = "";
    int choice;

    initializeRoutes();

    while (1) {
        if (strlen(loggedInRollNo) == 0) { // User not logged in
            printf("\n1. Register\n2. Login\n3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    if (loginUser(loggedInRollNo)) {
                        printf("\n===Login successful! Welcome.===\n");
                    }
                    break;
                case 3:
                    printf("\n===Exiting program. Goodbye!===\n");
                    exit(0);
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } else { // User logged in
            printf("\n1. View Routes\n2. Check Seat Availability\n3. Book Ticket\n4. View Booking History\n5. Logout\n");
            printf("\nEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\n======================================\n");
                    displayRoutes();
                    printf("======================================\n");
                    break;
                case 2:
                    printf("\n1-Ichalkaranji to Jaysingpur \n2-Sangli to Jaysingpur \n3-Miraj to Jaysingpur \nEnter Route Number:  ");
                    int routeIndex;
                    scanf("%d", &routeIndex);
                    if (routeIndex >= 1 && routeIndex <= 3) {
                        displaySeatAvailability(routeIndex - 1);
                    } else {
                        printf("\n==Invalid Route Number!==\n");
                    }
                    break;
                case 3:
                    bookTicket(loggedInRollNo);
                    break;
                case 4:
                    showBookingHistory(loggedInRollNo);
                    break;
                case 5:
                    printf("\nLogged out successfully.\n");
                    strcpy(loggedInRollNo, ""); // Reset login
                    break;
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }
    return 0;
}

// Initialize routes with stops and prices
void initializeRoutes() {
    strcpy(routes[0].routeName, "Ichalkaranji to Jaysingpur");
    strcpy(routes[0].stops[0], "Ichalkaranji");
    strcpy(routes[0].stops[1], "Kabnur");
    strcpy(routes[0].stops[2], "Korochi");
    strcpy(routes[0].stops[3], "Jaysingpur");
    routes[0].prices[0] = 50;
    routes[0].prices[1] = 40;
    routes[0].prices[2] = 30;
    routes[0].prices[3] = 20;

    strcpy(routes[1].routeName, "Sangli to Jaysingpur");
    strcpy(routes[1].stops[0], "Sangli");
    strcpy(routes[1].stops[1], "Vishrambag");
    strcpy(routes[1].stops[2], "Madhavnagar");
    strcpy(routes[1].stops[3], "Jaysingpur");
    routes[1].prices[0] = 30;
    routes[1].prices[1] = 25;
    routes[1].prices[2] = 20;
    routes[1].prices[3] = 10;

    strcpy(routes[2].routeName, "Miraj to Jaysingpur");
    strcpy(routes[2].stops[0], "Miraj");
    strcpy(routes[2].stops[1], "Wanlesswadi");
    strcpy(routes[2].stops[2], "Bedag");
    strcpy(routes[2].stops[3], "Jaysingpur");
    routes[2].prices[0] = 35;
    routes[2].prices[1] = 30;
    routes[2].prices[2] = 25;
    routes[2].prices[3] = 15;

    // Initialize seat availability
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < MAX_SEATS; j++) {
            routes[i].seatStatus[j] = 0; // All seats available
        }
    }
}

// Register a new user
void registerUser() {
    User user;
    userFile = fopen("users.dat", "ab");

    printf("Enter Name: ");
    scanf("%s", user.name);
    printf("Enter Student Id: ");
    scanf("%d", user.rollNo);
    printf("Enter Contact Number: ");
    scanf("%s", user.contactNumber);
    printf("Enter Email: ");
    scanf("%s", user.email);
    printf("Enter Password: ");
    scanf("%s", user.password);

    fwrite(&user, sizeof(User), 1, userFile);
    fclose(userFile);

    printf("\n====Registration successful! Please login to continue.====\n");
}

// Login user
int loginUser(char *loggedInRollNo) {
    User user;
    char rollNo[15], password[50];
    int found = 0;

    printf("Enter Student Id: ");
    scanf("%d", rollNo);
    printf("Enter Password: ");
    scanf("%s", password);

    userFile = fopen("users.dat", "rb");
    while (fread(&user, sizeof(User), 1, userFile)) {
        if (strcmp(user.rollNo, rollNo) == 0 && strcmp(user.password, password) == 0) {
            strcpy(loggedInRollNo, rollNo);
            found = 1;
            break;
        }
    }
    fclose(userFile);

    if (!found) {
        printf("Invalid credentials! Please try again.\n");
    }
    return found;
}

// Display available routes
void displayRoutes() {
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, routes[i].routeName);
    }
}

// Display seat availability for a route
void displaySeatAvailability(int routeIndex) {
    printf("\n\nSeat Availability for %s:\n", routes[routeIndex].routeName);
    for (int i = 0; i < MAX_SEATS; i++) {
        printf("Seat %d: %s\n", i + 1, routes[routeIndex].seatStatus[i] == 0 ? "Available" : "Booked");
    }
}

// Book a ticket
void bookTicket(char *loggedInRollNo) {
    Booking booking;
    int routeIndex, seatNumber, stopIndex;

    printf("\n1-Ichalkaranji to Jaysingpur \n2-Sangli to Jaysingpur \n3-Miraj to Jaysingpur \nEnter Route Number: ");
    scanf("%d", &routeIndex);
    routeIndex--;

    displaySeatAvailability(routeIndex);

    printf("Enter Seat Number: ");
    scanf("%d", &seatNumber);
    if (routes[routeIndex].seatStatus[seatNumber - 1] == 1) {
        printf("Seat already booked! Please choose a different seat.\n");
        return;
    }

    printf("Available Stops:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (₹%d)\n", i + 1, routes[routeIndex].stops[i], routes[routeIndex].prices[i]);
    }
    printf("Choose Stop: ");
    scanf("%d", &stopIndex);
    stopIndex--;

    // Book the seat
    routes[routeIndex].seatStatus[seatNumber - 1] = 1;

    // Save booking details
    strcpy(booking.rollNo, loggedInRollNo);
    strcpy(booking.route, routes[routeIndex].routeName);
    strcpy(booking.stop, routes[routeIndex].stops[stopIndex]);
    booking.seatNumber = seatNumber;
    booking.price = routes[routeIndex].prices[stopIndex];

    bookingFile = fopen("bookings.dat", "ab");
    fwrite(&booking, sizeof(Booking), 1, bookingFile);
    fclose(bookingFile);

    printf("\n====Booking successful! Seat %d booked for %s (%d Rupees).====\n", seatNumber, routes[routeIndex].stops[stopIndex], booking.price);
}

// Show booking history
void showBookingHistory(char *loggedInRollNo) {
    Booking booking;
    int found = 0;

    bookingFile = fopen("bookings.dat", "rb");
    if (bookingFile == NULL) {
        printf("No booking history found.\n");
        return;
    }

    printf("Booking History for Roll No: %s\n", loggedInRollNo);
    printf("-------------------------------------------------\n");
    printf("| Route Name            | Stop         | Seat | Price |\n");
    printf("-------------------------------------------------\n");

    while (fread(&booking, sizeof(Booking), 1, bookingFile)) {
        if (strcmp(booking.rollNo, loggedInRollNo) == 0) {
            printf("| %-20s | %-12s | %4d | ₹%5d |\n",
                   booking.route, booking.stop, booking.seatNumber, booking.price);
            found = 1;
        }
    }
    fclose(bookingFile);

    if (!found) {
        printf("No bookings found for your account.\n");
    } else {
        printf("-------------------------------------------------\n");
    }
}