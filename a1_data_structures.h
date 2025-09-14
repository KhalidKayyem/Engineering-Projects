/********* a1_data_structures.h ********

    Student Name 	= Khalid Kayyem
    Student Number	= 101277502
*/
#include <stdbool.h>

#define NUM_ACTIVITIES 3
#define NUM_MILESTONES 5
#define AVERAGE_HOURLY_SALARY 30.0f
#define HOURS_PER_DAY 8

/********** DON'T MODIFY FROM HERE **********/

typedef struct activity
{
    unsigned short int id;
    char name[100];
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
} activity_t;

typedef struct milestone
{
    unsigned short int id;
    char name[100];
    activity_t activity_list[NUM_ACTIVITIES];
    _Bool completed;
    float actual_cost;
    short int actual_duration;
} milestone_t;

typedef struct project
{
    char name[100];
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
} project_t;

/********** DON'T MODIFY UNTIL HERE **********/