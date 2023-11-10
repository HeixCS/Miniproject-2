#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct usert
{
    char name[256];
    double longitude;
    double latitude;
    double altitude;
    double time;
}usertp;

typedef struct difference_t
{
    char name[256];
    double difference;
}difference_tp;
void print_user(usertp *current_user);
void print_difference(int index, difference_tp *difference);
void scan_user(usertp *current_user, double longitude, double latitude, double altitude, double time, char name[]);
void scan_fill(usertp *our_user, usertp *users, FILE *rf, int user_size);
double find_differences(struct usert our_user, struct usert user2);
void load_differences(usertp our_user, usertp *users, difference_tp *differences, int user_size);
void find_closest(difference_tp *differences, usertp *users, int user_size, usertp *closest);

int main(void)
{
    char filename[256];
    printf("Please enter the file name: ");
    scanf("%s", filename);

    // Opens File
    FILE *rf = fopen(filename, "r");

    // Checking if the file exists
    if (rf != NULL){
    // Reads the number of Users
    char buffer[256];
    fgets(buffer, 256, rf);
    int user_size = atoi(buffer);

    // Makes the array of differences
    difference_tp differences[user_size];
    difference_tp *difference_p = differences;

    // Makes array of users
    usertp users[user_size];
    usertp *p_users;
    p_users = users;

    // Makes our user
    usertp our_user;
    usertp *our_user_p;
    our_user_p = &our_user;

    // Makes the closest structure
    usertp closest;
    usertp *closest_p = &closest;

    // Fills the array of users with coordinates
    scan_fill(our_user_p, p_users, rf, user_size);

    // Fills array with differences
    load_differences(our_user, p_users, difference_p, user_size);

    // Generates the smallest difference user
    find_closest(difference_p, p_users, user_size, closest_p);

    // Closes the file
    fclose(rf);
    }
    else{
        printf("That file does not exist");
    }
    return 0;
}

void scan_user(usertp *current_user, double longitude, double latitude, double altitude, double time, char name[])
{
    current_user->longitude = longitude;
    current_user->latitude = latitude;
    current_user->altitude = altitude;
    current_user->time = time;
    strcpy(current_user->name, name);
}
void print_user(usertp *current_user)
{
    printf("User %s:\n", current_user->name);
    printf("Latitude %7.2lf\n", current_user->latitude);
    printf("Longitude %7.2lf\n", current_user->longitude);
    printf("altitude %7.2lf\n", current_user->altitude);
}
void print_difference(int index, difference_tp *difference)
{
    printf("User number:%d at distance %7.2lf named %s\n", index, difference->difference, difference->name);
}

void scan_fill(usertp *our_user, usertp *users, FILE *rf, int user_size)
{
    char buffer[256];
    // Takes the input from the user
    printf("What is your longitude: ");
    scanf("%lf", &our_user->longitude);

    printf("What is your latitude: ");
    scanf("%lf", &our_user->latitude);

    printf("What is your altitude: ");
    scanf("%lf", &our_user->altitude);

    printf("What is your name: ");
    scanf("%s", &our_user->name);

    printf("How much time has passed since the refrence time: ");
    scanf("%lf", &our_user->time);
    strcpy(our_user->name, "Me");

    // Displays the current user
    printf("\n");
    print_user(our_user);
    printf("\nList of Other Users:\n");

    // Reads the file for each user
    for (int i = 0; i < user_size; i++)
    {
        char read[5][256];
        // Reads 5 lines for each user
        for (int j = 0; j < 5; j++)
        {
            if (fgets(buffer, 256, rf) != NULL)
            {
                strcpy(read[j], buffer);
            }
        }
        // Checking for new line character
        if (read[4][strlen(read[4]) - 1] == '\n')
        {
            read[4][strlen(read[4]) - 1] = '\0';
        }
        // Creates the user in the array from the file
        scan_user(&users[i], atof(read[0]), atof(read[1]), atof(read[2]), atof(read[3]), read[4]);
        print_user(&users[i]);
    }
}

double find_differences(usertp our_user, usertp user2)
{
    // Generates the difference between two points on a 3d plane
    double difference = sqrt(pow(our_user.latitude - user2.latitude, 2) + pow(our_user.altitude - user2.altitude, 2) + pow(our_user.longitude - user2.longitude, 2));
    return difference;
}

void load_differences(usertp our_user, usertp *users, difference_tp *differences, int user_size)
{
    printf("\nActual distances from our users were:\n");
    for (int i = 0; i < user_size; i++)
    {
        differences[i].difference = find_differences(users[i], our_user);
        strcpy(differences[i].name, users[i].name);
        print_difference(i, &differences[i]);
    }
}

void find_closest(difference_tp *differences, usertp *users, int user_size, usertp *closest)
{
    double min = differences[0].difference;
    int index = 0;
    for (int i = 0; i < user_size; i++)
    {
        if (min > differences[i].difference)
        {
            min = differences[i].difference;
            index = i;
        }
    }
    closest->longitude = users[index].longitude;
    closest->latitude = users[index].latitude;
    closest->altitude = users[index].altitude;
    closest->time = users[index].time;
    strcpy(closest->name, users[index].name);
    // printing closest user
    printf("\nNearest User to Refrence User\n");
    print_user(closest);
}