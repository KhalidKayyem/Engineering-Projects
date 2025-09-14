/********* main.c ********

    Student Name 	= Khalid Kayyem
    Student Number	= 101277502
*/

// Complete the includes
#include <stdlib.h>
#include <stdio.h>

#include "a1_functions.h"

int main()
{

    /** 1-  Display a welcome message **/
    printf("Welcome to ABC Company project management application\n");

    /** 2- Get the project's name and number of milestones **/
    char project_name[100];
    printf("Enter the project name (no spaces): ");
    scanf("%99s", &project_name);

    unsigned short int num_milestones;
    do
    {
        printf("Enter the number of milestones (between 1 and 5): ");
        num_milestones = get_input_usi();
        if (num_milestones < 1 || num_milestones > NUM_MILESTONES)
        {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
        }
    } while (num_milestones < 1 || num_milestones > NUM_MILESTONES);

    /** 3- Initialize all milestones and activities.
     * You need an array of milestones.
     * The capacity of the array must be the max number of milestones the program
     * can handle as per the instructions **/

    milestone_t milestones[NUM_MILESTONES];           // Array for storing milestones
    int number_activities[NUM_MILESTONES];            // Array to store the number of activities for each milestone
    unsigned short int milestone_ids[NUM_MILESTONES]; // To store milestone IDs

    for (int i = 0; i < num_milestones; i++)
    {
        // Prompt the user for the number of activities for each milestone
        do
        {
            printf("Enter the number of activities for milestone %d (between 1 and 3): ", i + 1);
            number_activities[i] = get_input_usi();
            if (number_activities[i] > NUM_ACTIVITIES)
            {
                printf("Invalid input. Please enter a value between 1 and 3.\n");
            }
        } while (number_activities[i] > NUM_ACTIVITIES);

        printf("Initializing milestone %d\n", i + 1);
        init_milestone(&milestones[i], number_activities[i]);
        printf("-------------------\n");
    }

    /** 4- Initialize the project **/
    project_t my_project = init_project(project_name, milestones, num_milestones, number_activities);

    /** 5- Print the main menu **/
    print_main_menu();

    /** 6- Get user choice and handle it as per the instructions**/
    int choice;
    do
    {
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        // Clear input buffer
        while (getchar() != '\n')
            ;

        // Handle user choice
        switch (choice)
        {
       case 1:
    // Update activity
    printf("Which activity do you want to update?\n");

    // List all activities across all milestones
    int valid_choice = 0;
    int milestone_index = -1, activity_index = -1;

    // Display all activities, completed or not, across all milestones
    for (int i = 0; i < num_milestones; i++)
    {
        for (int j = 0; j < number_activities[i]; j++)
        {
            // Print activity details, adding "- Completed" if the activity is completed
            printf("Activity ID: %hu, Name: %s (Milestone: %s)",
                   milestones[i].activity_list[j].id,
                   milestones[i].activity_list[j].name,
                   milestones[i].name);

            if (milestones[i].activity_list[j].completed)
            {
                printf(" - Completed\n");
            }
            else
            {
                printf("\n");
            }
        }
    }

    unsigned short int activity_id;
    do
    {
        // Prompt the user to enter the activity ID to update
        printf("Enter the activity ID to update: ");
        activity_id = get_input_usi();

        // Find the selected activity across all milestones
        for (int i = 0; i < num_milestones; i++)
        {
            for (int j = 0; j < number_activities[i]; j++)
            {
                // If the activity is found, proceed with the update
                if (milestones[i].activity_list[j].id == activity_id)
                {
                    valid_choice = 1;
                    milestone_index = i;
                    activity_index = j;
                    break;
                }
            }
            if (valid_choice)
                break; // Break out of outer loop if activity is found
        }

        if (!valid_choice)
        {
            printf("Invalid activity ID. Enter a valid ID.\n");
        }
    } while (!valid_choice);

    // Call update_activity directly to update the selected activity
    update_activity(&milestones[milestone_index].activity_list[activity_index]);

    // After updating the activity, update the milestone and project
    update_milestone(&milestones[milestone_index], number_activities[milestone_index]);
    update_project(milestones, num_milestones, number_activities, &my_project);

    // Ensure we return to the main menu after updating
    printf("Returning to the main menu...\n");
    print_main_menu();
    break;

        case 2:
            // Print project stats
            print_project_stats(my_project, milestones, num_milestones, number_activities);
            print_main_menu();
            break;

        case 3:
            // Exit
            printf("Exiting the program. Goodbye!\n");
            break;

        default:
            printf("Error! Please enter a choice between 1 and 3.\n");
        }
    } while (choice != 3);

    return EXIT_SUCCESS;
}