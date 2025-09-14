/********* a1_functions.c ********

    Student Name 	= Khalid Kayyem
    Student Number	= 101277502
*/
#include <stdio.h>

#include "a1_functions.h"

/*
   Get unsigned short integer user input.
   The value entered by the user must be positive.
   If the user enters a negative value, the function asks user to re-enter a positive value.
*/
unsigned short int get_input_usi(void)
{
    // Variable to store the user input
    // Used int in order to reject negative and non numeric inputs
    // (unsigned short int does not recognise negative values since it is unsigned
    // and if used will allow them as inputs since it converts negative numbers to very high positive numbers
    // also non numeric characters are converted to zero)
    // i cast the input back to unsigned short int before returning
    int input;
    do
    {
        // read the user input and store it
        scanf("%d", &input);

        // clear input buffer (prevent infinite loop)
        while (getchar() != '\n')
            ;

        // invalid response reprompt the user
        if (input <= 0)
        {
            printf("Invalid Input. Enter a positive value: ");
        }
    } while (input <= 0);

    // return the user input but cast as an unsigned short int
    return (unsigned short int)input;
}

/*
   Get float user input. The value must be positive.
   The value entered by the user must be positive.
   If the user enters a negative value, the function asks user to re-enter a positive value.
*/
float get_input_f(void)
{
    // Variable to store the user input
    float input;

    do
    {
        // read the user input and store it
        scanf("%f", &input);

        // clear input buffer (prevent infinite loop)
        while (getchar() != '\n')
            ;

        // invalid response reprompt the user
        if (input <= 0.0f)
        {
            printf("Invalid Input. Enter a positive value: ");
        }
    } while (input <= 0.0f);

    // return the user input
    return input;
}

/*
   Initialize all fields in an activity as per the instructions (3)
*/
void init_activity(activity_t *activity_to_int)
{
    // Activity name input
    printf("Enter the activity name: ");
    scanf("%99s", activity_to_int->name);

    // Planned duration input
    printf("Enter the planned duration (hours): ");
    activity_to_int->planned_duration = get_input_usi();

    // Planned cost calculation
    activity_to_int->planned_cost = activity_to_int->planned_duration * AVERAGE_HOURLY_SALARY;

    // Initialize actual cost and duration to zero
    activity_to_int->actual_cost = 0.0f;
    activity_to_int->actual_duration = 0;

    // Initialize the activity as not completed
    activity_to_int->completed = false;
}

/*
   Initialize all fields in the milestone as per the instructions (3)
   The implementation of your function MUST call the function init_activity.
*/
void init_milestone(milestone_t *milestone_to_int, unsigned short int num_activities)
{
    // Track all previously entered milestone IDs for uniqueness checking
    static unsigned short int milestone_ids[NUM_MILESTONES] = {0};                     // Initialize to zero
    static unsigned short int all_activity_ids[NUM_MILESTONES * NUM_ACTIVITIES] = {0}; // Track all activity IDs across all milestones
    static int milestone_count = 0;                                                    // Number of milestones initialized so far
    static int activity_count = 0;                                                     // Total number of activities initialized so far

    // Milestone ID input
    unsigned short int milestone_id;
    _Bool unique_milestone_id = false;

    while (!unique_milestone_id)
    {
        unique_milestone_id = true; // Assume the ID is unique
        printf("Enter the milestone ID (positive value): ");
        milestone_id = get_input_usi();

        // Check if this milestone ID has already been used
        for (int i = 0; i < milestone_count; i++)
        {
            if (milestone_ids[i] == milestone_id)
            {
                printf("Milestone ID %hu has already been used. Please enter a unique ID.\n", milestone_id);
                unique_milestone_id = false; // Mark as false if duplicate found
                break;
            }
        }
    }

    // Store the unique milestone ID in the list and assign it to the milestone
    milestone_ids[milestone_count] = milestone_id;
    milestone_to_int->id = milestone_id;
    milestone_count++; // Increment milestone count after successfully assigning an ID

    // Milestone name input
    printf("Enter the milestone name: ");
    scanf("%99s", milestone_to_int->name);

    // Initialize each activity
    for (int i = 0; i < num_activities; i++)
    {
        _Bool unique_activity_id = false;
        unsigned short int activity_id;

        // Ensure each activity ID is unique across all milestones
        while (!unique_activity_id)
        {
            unique_activity_id = true; // Assume ID is unique
            printf("Enter the activity ID for activity %d (positive value): ", i + 1);
            activity_id = get_input_usi();

            // Check if this activity ID has already been used across all milestones
            for (int j = 0; j < activity_count; j++)
            {
                if (all_activity_ids[j] == activity_id)
                {
                    printf("Activity ID %hu has already been used. Please enter a unique ID.\n", activity_id);
                    unique_activity_id = false;
                    break;
                }
            }
        }

        // Store the unique activity ID and assign it to the activity
        all_activity_ids[activity_count] = activity_id;
        activity_count++;                                    // Increment the global activity count
        init_activity(&milestone_to_int->activity_list[i]);  // Initialize the activity
        milestone_to_int->activity_list[i].id = activity_id; // Assign the unique activity ID
    }

    // Initialize milestone completion to false
    milestone_to_int->completed = false;

    // Initialize milestone actual cost and duration to zero
    milestone_to_int->actual_cost = 0.0f;
    milestone_to_int->actual_duration = 0;
}

/*
   Initialize all fields in the project as per the instructions (4)
   number_activities contains the number of activities per milestone
*/
project_t init_project(char name[], milestone_t *milestone_list, int number_milestones, const int *number_activities)
{
    // Create a new project instance
    project_t new_project;

    // Copy the project name into the new project
    for (int i = 0; i < sizeof(new_project.name) - 1; i++)
    {
        new_project.name[i] = name[i];
    }

    // Initialize the actual cost and duration to zero
    new_project.actual_cost = 0.0f;
    new_project.actual_duration = 0;

    // Initialize the project completion to false
    new_project.completed = false;

    // Initialize planned cost and duration by summing the values from all activities
    new_project.planned_cost = 0.0f;
    new_project.planned_duration = 0;

    // Loop through all milestones
    for (int i = 0; i < number_milestones; i++)
    {
        // Loop through all activities in each milestone
        for (int j = 0; j < number_activities[i]; j++)
        {
            // Sum the planned costs from all activities
            new_project.planned_cost += milestone_list[i].activity_list[j].planned_cost;

            // Sum the planned durations from all activities (in hours)
            new_project.planned_duration += milestone_list[i].activity_list[j].planned_duration;
        }
    }

    // Convert total planned duration from hours to days (8 hours per day)
    new_project.planned_duration = (new_project.planned_duration + (HOURS_PER_DAY - 1)) / HOURS_PER_DAY; // Round up to nearest day

    // Return the initialized project
    return new_project;
}

/*
   Print the main menu as per the instructions (5)
*/
void print_main_menu(void)
{
    printf("Please select one of the following options:\n");
    printf("1. Update activity\n");
    printf("2. Print stats\n");
    printf("3. Exit\n");
}

/*
   Print out milestone stats as per the instructions (6)
   number_activities contains the number of activities per milestone
*/
void print_milestone_stats(const milestone_t *list_milestones, int num_milestones, const int *number_activities)
{
    // Loop through each milestone
    for (int i = 0; i < num_milestones; i++)
    {
        const milestone_t *milestone = &list_milestones[i]; // Current milestone

        // Print milestone name
        printf("Milestone %s\n", milestone->name);

        // If the milestone is completed
        if (milestone->completed)
        {
            printf("This milestone is completed.\n");

            // Calculate cost difference (actual vs planned)
            float cost_difference = milestone->actual_cost - 0.0f;
            for (int j = 0; j < number_activities[i]; j++)
            {
                cost_difference += milestone->activity_list[j].actual_cost - milestone->activity_list[j].planned_cost;
            }

            if (cost_difference < 0)
            {
                printf("Finished under budget by $%.2f\n", -cost_difference);
            }
            else if (cost_difference > 0)
            {
                printf("Finished over budget by $%.2f\n", cost_difference);
            }
            else
            {
                printf("Finished exactly on budget.\n");
            }

            // Calculate duration difference (actual vs planned)
            int duration_difference = milestone->actual_duration;
            for (int j = 0; j < number_activities[i]; j++)
            {
                duration_difference += milestone->activity_list[j].actual_duration - milestone->activity_list[j].planned_duration;
            }

            // Convert hours to days (round up)
            int duration_difference_days = (duration_difference + (HOURS_PER_DAY - 1)) / HOURS_PER_DAY;

            if (duration_difference_days < 0)
            {
                printf("Finished early by %d days\n", -duration_difference_days);
            }
            else if (duration_difference_days > 0)
            {
                printf("Finished late by %d days\n", duration_difference_days);
            }
            else
            {
                printf("Finished exactly on time.\n");
            }
        }
        // If the milestone is not completed
        else
        {
            printf("This milestone is not completed.\n");

            // Count the number of completed activities
            int completed_activities = 0;
            for (int j = 0; j < number_activities[i]; j++)
            {
                if (milestone->activity_list[j].completed)
                {
                    completed_activities++;
                }
            }

            // Print number of completed activities
            printf("Completed activities: %d out of %d\n", completed_activities, number_activities[i]);

            // Print planned duration (in days) and planned cost
            int planned_duration_days = (milestone->actual_duration + (HOURS_PER_DAY - 1)) / HOURS_PER_DAY; // Convert hours to days
            printf("Planned duration: %d days\n", planned_duration_days);
            printf("Planned cost: $%.2f\n", milestone->actual_cost);
        }

        printf("-----------------------------\n"); // Separator between milestones
    }
}

/*
   Print out project stats as per the instructions (6)
   number_activities contains the number of activities per milestone
   The implementation MUST call print_stats_milestone
*/
void print_project_stats(project_t details, const milestone_t *list_milestones, int num_milestones, const int *number_activities)
{
    // Print the project name
    printf("Project: %s\n", details.name);

    // Check if the project is completed
    if (details.completed)
    {
        printf("The project is completed.\n");

        // Calculate the cost difference (actual vs planned)
        float cost_difference = details.actual_cost - details.planned_cost;
        if (cost_difference < 0)
        {
            printf("The project finished under budget by $%.2f\n", -cost_difference);
        }
        else if (cost_difference > 0)
        {
            printf("The project finished over budget by $%.2f\n", cost_difference);
        }
        else
        {
            printf("The project finished exactly on budget.\n");
        }

        // Calculate the duration difference (actual vs planned, in days)
        int duration_difference_days = details.actual_duration - details.planned_duration;
        if (duration_difference_days < 0)
        {
            printf("The project finished early by %d days\n", -duration_difference_days);
        }
        else if (duration_difference_days > 0)
        {
            printf("The project finished late by %d days\n", duration_difference_days);
        }
        else
        {
            printf("The project finished exactly on time.\n");
        }
    }
    // If the project is not completed
    else
    {
        printf("The project is not completed.\n");

        // Call the print_milestone_stats function to print detailed milestone stats
        print_milestone_stats(list_milestones, num_milestones, number_activities);
    }

    printf("-----------------------------\n"); // Separator for the project stats
}

/*
   Update activity per the instructions (6)
*/
void update_activity(activity_t *activity_to_update)
{
    // Update actual duration
    printf("Enter the actual duration for activity %s (in hours): ", activity_to_update->name);
    activity_to_update->actual_duration = get_input_usi();

    // Update completion status
    char completed;
    do
    {
        printf("Is the activity completed? (Y/N): ");
        scanf(" %c", &completed); // Note the space before %c to handle newline
        while (getchar() != '\n')
            ; // Clear input buffer to avoid issues
    } while (completed != 'Y' && completed != 'N');

    // Set completion status based on user input
    activity_to_update->completed = (completed == 'Y');

    // Calculate the actual cost
    // Actual Cost = Actual Duration (hours) * Average Hourly Salary (30$/hour)
    activity_to_update->actual_cost = activity_to_update->actual_duration * AVERAGE_HOURLY_SALARY;

    // Print confirmation message
    printf("Activity %s updated.\n", activity_to_update->name);
    printf("Actual duration: %hu hours\n", activity_to_update->actual_duration);
    printf("Actual cost: $%.2f\n", activity_to_update->actual_cost);
    printf("Completion status: %s\n", activity_to_update->completed ? "Completed" : "Not completed");
    printf("-----------------------------\n");
}
/*
   Update milestone per the instructions (6)
*/
void update_milestone(milestone_t *milestone_to_update, int activities_in_milestone)
{
    // Update each activity in the milestone
    for (int i = 0; i < activities_in_milestone; i++)
    {
        activity_t *current_activity = &milestone_to_update->activity_list[i];

        // Only update the activity if it is not completed yet
        if (!current_activity->completed)
        {
            printf("Updating activity %s (ID: %hu)\n", current_activity->name, current_activity->id);
            update_activity(current_activity);
            break; // Once the user updates an activity, exit this loop
        }
    }

    // Recalculate the actual cost and duration for the milestone
    milestone_to_update->actual_cost = 0.0f;
    milestone_to_update->actual_duration = 0;
    milestone_to_update->completed = true; // Assume milestone is completed until proven otherwise

    for (int i = 0; i < activities_in_milestone; i++)
    {
        activity_t *current_activity = &milestone_to_update->activity_list[i];

        // Add the actual cost and duration from the activity to the milestone
        milestone_to_update->actual_cost += current_activity->actual_cost;
        milestone_to_update->actual_duration += current_activity->actual_duration;

        // If any activity is not completed, the milestone is not completed
        if (!current_activity->completed)
        {
            milestone_to_update->completed = false;
        }
    }

    // Print the updated milestone information
    printf("Milestone %s updated.\n", milestone_to_update->name);
    printf("Actual cost: $%.2f\n", milestone_to_update->actual_cost);
    printf("Actual duration: %d hours\n", milestone_to_update->actual_duration);
    printf("Completion status: %s\n", milestone_to_update->completed ? "Completed" : "In progress");
    printf("-----------------------------\n");
}


/*
   Update project per the instructions (6)
   number_activities contains the number of activities per milestone
*/
void update_project(const milestone_t *milestone_array, int num_milestones, const int *number_activities, project_t *my_project)
{
    // Reset the actual cost and duration
    my_project->actual_cost = 0.0f;
    my_project->actual_duration = 0;
    my_project->completed = true; // Assume the project is completed until proven otherwise

    // Iterate through all milestones to update the project's actual cost and duration
    for (int i = 0; i < num_milestones; i++)
    {
        const milestone_t *current_milestone = &milestone_array[i];

        // Sum the actual cost and duration from all milestones
        my_project->actual_cost += current_milestone->actual_cost;
        my_project->actual_duration += current_milestone->actual_duration;

        // If any milestone is not completed, the project is not completed
        if (!current_milestone->completed)
        {
            my_project->completed = false;
        }
    }

    // Convert actual duration from hours to days (8 hours/day, round up)
    my_project->actual_duration = (my_project->actual_duration + (HOURS_PER_DAY - 1)) / HOURS_PER_DAY;

    // Print updated project information
    printf("Project %s updated.\n", my_project->name);
    printf("Actual cost: $%.2f\n", my_project->actual_cost);
    printf("Actual duration: %hu days\n", my_project->actual_duration);
    printf("Completion status: %s\n", my_project->completed ? "Completed" : "In progress");
    printf("-----------------------------\n");
}
