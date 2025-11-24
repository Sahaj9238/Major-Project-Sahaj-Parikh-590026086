#include <stdio.h>

struct Train
{
    int train_no;
    char train_name[50];
    char source[30];
    char destination[30];
    int seats;
};

struct Passenger
{
    int pnr;
    char name[50];
    int age;
    char gender;
    int train_no;
};

struct Train t[3] = {
    {101, "Rajdhani Express", "Delhi", "Mumbai", 100},
    {102, "Shatabdi Express", "Bangalore", "Chennai", 80},
    {103, "Duronto Express", "Kolkata", "Delhi", 90}};

int generatepnr();
void viewtrains();
void bookticket();
void viewbookings();
void cancelticket();

int main()
{
    int ch;

    do
    {
        printf("\n=== RAILWAY RESERVATION SYSTEM ===\n");
        printf("1. View Available Trains\n");
        printf("2. Book Ticket\n");
        printf("3. View Bookings By PNR\n");
        printf("4. Cancel Ticket\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            viewtrains();
            break;
        case 2:
            bookticket();
            break;
        case 3:
            viewbookings();
            break;
        case 4:
            cancelticket();
            break;
        case 5:
            printf("Thank you for using the system!\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    } while (ch != 5);

    return 0;
}


void viewtrains()
{
    printf("\nTrain No\tTrain Name\t\tFrom\t\tTo\t\tSeats\n");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 3; i++)
    {
        printf("%d\t%s\t\t%s\t\t%s\t\t%d\n",
               t[i].train_no,
               t[i].train_name,
               t[i].source,
               t[i].destination,
               t[i].seats);
    }
}


int generatepnr()
{
    static int pnr = 1000;
    pnr++;
    return pnr;
}


void bookticket()
{
    struct Passenger p;
    FILE *fp;
    int train_no, index = -1, tickets, pnr;

    viewtrains();

    printf("\nEnter Train Number: ");
    scanf("%d", &train_no);

    // Find train
    for (int i = 0; i < 3; i++)
    {
        if (t[i].train_no == train_no){
            index = i;
        }
    }

    if (index == -1)
    {
        printf("Invalid Train Number!\n");
        return;
    }

    printf("Enter number of tickets: ");
    scanf("%d", &tickets);

    
    if (tickets > t[index].seats)
    {
        printf("Only %d seats available!\n", t[index].seats);
        return;
    }

    pnr = generatepnr();

    fp = fopen("bookings.dat", "ab");

    for (int i = 1; i <= tickets; i++)
    {
        printf("\nEnter passenger %d details:\n", i);
        printf("Name(firstname only): ");
        scanf("%s", p.name);
        printf("Age: ");
        scanf("%d", &p.age);
        printf("Gender (M/F): ");
        scanf(" %c", &p.gender);

        p.pnr = pnr;
        p.train_no = train_no;

        fwrite(&p, sizeof(p), 1, fp);
        t[index].seats--;
    }

    fclose(fp);

    printf("\nBooking Successful! Your PNR is %d\n", pnr);
}


void viewbookings()
{
    int search_pnr;
    struct Passenger p;
    FILE *fp;

    printf("Enter PNR to view: ");
    scanf("%d", &search_pnr);

    fp = fopen("bookings.dat", "rb");

    if (fp == NULL)
    {
        printf("No bookings found!\n");
        return;
    }

    int found = 0;

    while (fread(&p, sizeof(p), 1, fp))
    {
        if (p.pnr == search_pnr)
        {
            found = 1;
            printf("\nPNR: %d\nName: %s\nAge: %d\nGender: %c\nTrain No: %d\n",
                   p.pnr, p.name, p.age, p.gender, p.train_no);
        }
    }

    fclose(fp);

    if (found==0){
        printf("PNR not found!\n");
    }
}

// ---------------- CANCEL TICKET ----------------

void cancelticket()
{
    int search_pnr, train_no, index = -1, count = 0;
    struct Passenger p;
    FILE *fp, *temp;

    printf("Enter PNR to cancel: ");
    scanf("%d", &search_pnr);

    fp = fopen("bookings.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL)
    {
        printf("No bookings found!\n");
        return;
    }

    while (fread(&p, sizeof(p), 1, fp))
    {
        if (p.pnr == search_pnr)
        {
            train_no = p.train_no;
            count++;
        }
        else
        {
            fwrite(&p, sizeof(p), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("bookings.dat");
    rename("temp.dat", "bookings.dat");

    if (count == 0)
    {
        printf("PNR not found!\n");
        return;
    }
    
    for (int i = 0; i < 3; i++)
    {
        if (t[i].train_no == train_no)
        {
            index = i;
            break;
        }
    }

    t[index].seats += count;

    printf("Successfully cancelled %d ticket(s) for PNR %d\n", count, search_pnr);
}
