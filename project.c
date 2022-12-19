

// project only contains a main file, no header file


/* including some headers which may be useful */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>



/* Creating a student structure */
struct student
{
    int student_number;
    char *lastname;
    char *firstname;
    int roundpoints[6]; /* will be an integer array to store points of every round */
    int totalpoints;
};




/* implementing a function for adding students to the data base (student array) */
struct student *add_student(char *buffer, struct student *students, int a)
{

    /* memory allocation is done for every part of the student structure */

    if (a == 0)
        students = malloc((sizeof(struct student))*(a+1)); /* each pointer in the struct is 8 bytes, so 5*8 = 40 bytes total */

    else
        students = realloc(students, ((sizeof(struct student))*(a+1))); /* re-allocate whenever adding more than 1 student to the structure */

    /* loop that separates the input buffer into pieces, e.g student number, lastname etc. */

    int ctr = 0;

    char *ptr = strtok(buffer, " ");

    while (ptr != NULL)
    {
        ptr = strtok(NULL, " ");
        ctr++;

        if (ctr == 1)
        {
            sscanf(ptr, "%d", (int*)&students[a].student_number); /* turns the student number from string to int */
        }

        else if (ctr == 2)
        {
            students[a].lastname = malloc(strlen(ptr)*sizeof(char*));
            strcpy(students[a].lastname, ptr); /* copies string from ptr to lastname */
        }

        else if (ctr == 3)
        {
            students[a].firstname = malloc(strlen(ptr)*sizeof(char*));
            strcpy(students[a].firstname, ptr); /* copies string from ptr to firstname */
        }
    }

    /* initializing round points and total points */

    for (int i = 0; i < 6; i++)
    {
        students[a].roundpoints[i] = 0;
    }

    students[a].totalpoints = 0; /* using 0th element to make adding points easier later on */

    return students;
}


/* function for updating points */
struct student *update_points(char *buffer, struct student *students, int a)
{
    int ctr = 0;
    int k = -1;
    int i;
    int round;
    int *rightnumber = NULL;

    /* loop to split the input string */

    char *ptr = strtok(buffer, " ");

    while (ptr != NULL)
    {
        ptr = strtok(NULL, " ");
        ctr++;

        if (ctr == 1) /* checking if the given student number exists in students array */
        {
            sscanf(ptr, "%d", (int*)&rightnumber);

            intptr_t temprightnum = (intptr_t) rightnumber; /* to match correct data types with struct student and format specifiers */

            for (i = 0; i < a; i++)
            {
                if (temprightnum == students[i].student_number)
                    k = i;
            }

            if (k == -1)
            {
                printf("Student %d not found!\n", (int)temprightnum); /* tells which student was not found */
                break;
            }
        }


        else if (ctr == 2)
        {

            sscanf(ptr, "%d", &round); /* indicate which round to add points to */

            if (round > 6)
            {
                printf("Not a valid round.\n");
                break;
            }

            else if (round < 1)
            {
                printf("Not a valid round.\n");
                break;
            }
        }

        else if (ctr == 3)
        {
            /* if updating already existing points on a round, remove them from total before updating,
               this prevents the previously stored points from remaining in the total points */
            if (students[k].roundpoints[(round-1)] != 0) {
                students[k].totalpoints -= students[k].roundpoints[(round-1)];
            }

            /* adds points to the specific round and total points */
            sscanf(ptr, "%d", &students[k].roundpoints[(round-1)]); /* round-1 to compensate for 0th element */
            students[k].totalpoints += students[k].roundpoints[(round-1)];
        }
    }

    return students;
}




/* print the attributes of all the students */
void *print_points(struct student *students, int a)
{
    /* printing attributes of every student (not sorted) */

    for (int x = 0; x < a; x++)
    {
        printf("Student number: %d\n", students[x].student_number);
        printf("Last name: %s\n", students[x].lastname);
        printf("First name: %s", students[x].firstname);
        printf("Exercise points:\n");
        printf("Round 1 - %d\n", students[x].roundpoints[0]); /* notice 0th element is 1st round */
        printf("Round 2 - %d\n", students[x].roundpoints[1]);
        printf("Round 3 - %d\n", students[x].roundpoints[2]);
        printf("Round 4 - %d\n", students[x].roundpoints[3]);
        printf("Round 5 - %d\n", students[x].roundpoints[4]);
        printf("Round 6 - %d\n", students[x].roundpoints[5]);
        printf("Total points - %d\n", students[x].totalpoints);
        printf("\n");
    }

    return 0;
}




/* release allocated memory */
void cleanup(struct student *students, int a)
{

    for (int i = 0; i < a; i++)
    {
        free(students[i].lastname);
        free(students[i].firstname);
    }

    free(students);


/*
    if (students == NULL)
        printf("Memory freeing succeeded!");

    else
        printf("Memory freeing failed!");
*/
}




int main(void)
{
	int continues = 1;

	struct student *students = NULL; /* data base where we can store students and their attributes */

	int a = 0; /* counter for add function, used to keep track of how many students exist in the student array */

    /* Reminder: Use dynamically allocated memory for the data base! */

    printf("This is the course exercise-program. Instructions listed below.\n");
    printf("'A studentnumber lastname firstname' adds a student to the student database.\n");
    printf("'U studentnumber round points' updates points for the round given. There are at most 6 rounds.\n");
    printf("'L' prints the attributes of each student in the data base.\n");
    printf("'Q' exits the program.\n");
    printf("\n");

	while(continues)
    {
        char buffer[80]; /* text buffer for storing text input */
        char *ret = fgets(buffer, 80, stdin); /* puts the input into the buffer */

        if (ret == NULL) /* if there is no input (nothing to read), the loop stops */
        {
            continues = 0;
            break;
        }

        switch(ret[0]) /* case specific switch which behaves according to the input */
        {
            case 'A': /* Adds student to the data base */
                students = add_student(buffer, students, a);
                a++;
                break; /* the breaks in the switch cases prevents it from jumping to the next case after current case is done */

            case 'U': /* Update student points */
                students = update_points(buffer,students,a);
                break;

            case 'L': /* Prints the stored points of a student, along with student number and names */
                print_points(students,a);
                break;

            case 'W': /* Save/write results to a file with given filename */
                //save_results(FILE filename); //skip this one
                break;

            case 'O': /* Load results from a file with given filename */
                //load_results(FILE filename); //skip this one
                break;

            case 'Q': /* Exits the program */
                printf("Exiting the program...");
                continues = 0;
                break;
        }
    }

    cleanup(students,a); /* release allocated memory */
}
