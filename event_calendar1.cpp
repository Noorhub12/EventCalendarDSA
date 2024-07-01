#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for an event
struct event {
    char name[100];
    char date[20];
    char time[20];
    struct event *prev;                      //pointers for doubly linked list operation
    struct event *next;
};

// Global variable for the head of the sorted double-linked list
struct event *head = NULL;

// Function to create a new event node
struct event *createEventNode(char name[], char date[], char time[]) {
    struct event *newEvent = (struct event *)malloc(sizeof(struct event));
    if (newEvent != NULL) {
        strcpy(newEvent->name, name);
        strcpy(newEvent->date, date);
        strcpy(newEvent->time, time);
        newEvent->prev = NULL;
        newEvent->next = NULL;
    }
    return newEvent;
}

// Function to insert an event into the sorted list           Tansveres the link compares date and time and insert adjusting prev and next
void insertEvent(char name[], char date[], char time[]) {
    struct event *newEvent = createEventNode(name, date, time);
    if (newEvent == NULL) {
        printf("Memory allocation failed. Event not added.\n");
        return;
    }

    if (head == NULL) {
        // List is empty, insert at the beginning
        head = newEvent;
    } else {
        struct event *current = head;
        while (current != NULL) {
            // Compare dates (and times if necessary) to maintain sorted order
            if (strcmp(current->date, date) > 0 || (strcmp(current->date, date) == 0 && strcmp(current->time, time) > 0)) {
                // Found the position to insert before current
                if (current->prev == NULL) {
                    // Insert at the beginning
                    newEvent->next = current;
                    current->prev = newEvent;
                    head = newEvent;
                } else {
                    // Insert in between
                    newEvent->prev = current->prev;
                    newEvent->next = current;
                    current->prev->next = newEvent;
                    current->prev = newEvent;
                }
                break;
            }
            // Move to the next event
            if (current->next == NULL) {
                // Insert at the end
                current->next = newEvent;
                newEvent->prev = current;
                break;
            }
            current = current->next;
        }
    }
    printf("Event '%s' on %s at %s added successfully.\n", newEvent->name, newEvent->date, newEvent->time);
}

// Function to delete an event from the calendar
void deleteEvent(char date[], char time[]) {
    if (head == NULL) {
        printf("Calendar is empty. No events to delete.\n");
        return;
    }

    struct event *current = head;
    while (current != NULL) {
        if (strcmp(current->date, date) == 0 && strcmp(current->time, time) == 0) {
            // Found the event to delete
            if (current->prev == NULL) {
                // Deleting the first node
                head = current->next;
                if (head != NULL) {
                    head->prev = NULL;
                }
            } else {
                // Deleting a node in between or at the end
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
            }
            free(current);
            printf("Event at %s on %s deleted successfully.\n", time, date);
            return;
        }
        current = current->next;
    }

    printf("Event at %s on %s not found. Unable to delete.\n", time, date);
}

// Function to update details of an event
void updateEvent(char date[], char time[]) {
    if (head == NULL) {
        printf("Calendar is empty. No events to update.\n");
        return;
    }

    struct event *current = head;
    while (current != NULL) {
        if (strcmp(current->date, date) == 0 && strcmp(current->time, time) == 0) {
            // Found the event to update
            char newName[100], newDate[20], newTime[20];
            printf("Enter updated event name: ");
            scanf(" %[^\n]s", newName);
            printf("Enter updated date (DD/MM/YYYY): ");
            scanf("%s", newDate);
            printf("Enter updated time (HH:MM): ");
            scanf("%s", newTime);

            // Update event details
            strcpy(current->name, newName);
            strcpy(current->date, newDate);
            strcpy(current->time, newTime);

            // Reinsert the updated event to maintain sorted order
            struct event *temp = createEventNode(newName, newDate, newTime);
            if (temp == NULL) {
                printf("Memory allocation failed. Event not updated.\n");
                return;
            }
            deleteEvent(date, time);
            insertEvent(temp->name, temp->date, temp->time);

            printf("Event updated successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("Event at %s on %s not found. Unable to update.\n", time, date);
}

// Function to display all events in chronological order
void displayEvents() {
    if (head == NULL) {
        printf("Calendar is empty. No events to display.\n");
        return;
    }

    printf("Events in the calendar:\n");
    struct event *current = head;
    while (current != NULL) {
        printf("Event: %s\n", current->name);
        printf("Date: %s\n", current->date);
        printf("Time: %s\n", current->time);
        printf("-------------------------\n");
        current = current->next;
    }
}

// Function to count total events in the calendar
int countEvents() {
    int count = 0;
    struct event *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Function to clear all events from the calendar
void clearCalendar() {
    struct event *current = head;
    while (current != NULL) {
        struct event *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    printf("All events cleared from the calendar.\n");
}

// Function to free all allocated memory
void freeMemory() {
    clearCalendar();
}

// Function to save events to a file
void saveEventsToFile(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    struct event *current = head;
    while (current != NULL) {
        fprintf(fp, "%s;%s;%s\n", current->name, current->date, current->time);
        current = current->next;
    }

    fclose(fp);
    printf("Events saved to file %s successfully.\n", filename);
}

// Function to load events from a file
void loadEventsFromFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s for reading.\n", filename);
        return;
    }

    clearCalendar();  // Clear existing events before loading new ones

    char name[100], date[20], time[20];
    while (fscanf(fp, "%[^;];%[^;];%[^\n]\n", name, date, time) == 3) {
        insertEvent(name, date, time);
    }

    fclose(fp);
    printf("Events loaded from file %s successfully.\n", filename);
}

int main() {
    int choice;
    char name[100], date[20], time[20], filename[100];

    printf("Welcome to the Event Calendar\n");

    do {
        printf("\nMenu:\n");
        printf("1. Add Event\n");
        printf("2. Delete Event\n");
        printf("3. Update Event\n");
        printf("4. View Events\n");
        printf("5. Count Total Events\n");
        printf("6. Clear All Events\n");
        printf("7. Save Events to File\n");
        printf("8. Load Events from File\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter event name: ");
                scanf(" %[^\n]s", name);
                printf("Enter date (DD/MM/YYYY): ");
                scanf("%s", date);
                printf("Enter time (HH:MM): ");
                scanf("%s", time);
                insertEvent(name, date, time);
                break;
            case 2:
                printf("Enter date of the event to delete (DD/MM/YYYY): ");
                scanf("%s", date);
                printf("Enter time of the event to delete (HH:MM): ");
                scanf("%s", time);
                deleteEvent(date, time);
                break;
            case 3:
                printf("Enter date of the event to update (DD/MM/YYYY): ");
                scanf("%s", date);
                printf("Enter time of the event to update (HH:MM): ");
                scanf("%s", time);
                updateEvent(date, time);
                break;
            case 4:
                displayEvents();
                break;
            case 5:
                printf("Total number of events: %d\n", countEvents());
                break;
            case 6:
                clearCalendar();
                break;
            case 7:
                printf("Enter file name to save events: ");
                scanf("%s", filename);
                saveEventsToFile(filename);
                break;
            case 8:
                printf("Enter file name to load events: ");
                scanf("%s", filename);
                loadEventsFromFile(filename);
                break;
            case 9:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 9);

    // Free allocated memory before exiting
    freeMemory();

    return 0;
}

