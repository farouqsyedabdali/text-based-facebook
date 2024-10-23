/********* main.c ********
    Student Name 	= Farouq Syed Abdali
    Student Number	= 101263256
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "functions.h"
#include "functions.c"

extern void print_menu();

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }

    user_t *users = read_CSV_and_create_users(csv_file, 50);
    fclose(csv_file);

    int choice;
    do
    {
        print_menu();
        scanf("%d", &choice);
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
        {
            char username[30], password[15];
            int userid;
            printf("Enter a username: ");
            scanf("%29s", username);
            while (getchar() != '\n')
                ;
            printf("Enter a userid: ");
            scanf("%d", &userid);
            while (getchar() != '\n')
                ;
            printf("Enter an up to 15 characters password: ");
            scanf("%14s", password);
            while (getchar() != '\n')
                ;

            FILE *file = fopen("user_details.csv", "a");
            if (file == NULL)
            {
                perror("Failed to open the CSV file");
                break;
            }
            fprintf(file, "%s,%s,%d\n", username, password, userid);
            fclose(file);

            users = add_user(users, username, userid, password);
            printf("**** User Added! ****\n");
            break;
        }
        case 2:
        {
            char username[30], newPassword[15];
            printf("Enter username to update their password: ");
            scanf("%29s", username);
            while (getchar() != '\n')
                ;

            printf("Enter a new password that is up to 15 characters: ");
            scanf("%14s", newPassword);
            while (getchar() != '\n')
                ;

            user_t *user = find_user(users, username);
            if (user)
            {
                strncpy(user->password, newPassword, sizeof(user->password) - 1);
                user->password[sizeof(user->password) - 1] = '\0';

                FILE *file = fopen("user_details.csv", "r");
                if (!file)
                {
                    perror("Error opening CSV file for reading");
                    break;
                }

                char line[1024];
                char **csvLines = NULL;
                int linesAllocated = 0;
                int numLines = 0;

                while (fgets(line, sizeof(line), file))
                {
                    if (numLines >= linesAllocated)
                    {
                        linesAllocated += 10;
                        csvLines = realloc(csvLines, linesAllocated * sizeof(char *));
                    }
                    csvLines[numLines] = strdup(line);
                    numLines++;
                }
                fclose(file);

                file = fopen("user_details.csv", "w");
                if (!file)
                {
                    perror("Error opening CSV file for writing");
                    break;
                }

                for (int i = 0; i < numLines; ++i)
                {
                    if (i != 0 && strstr(csvLines[i], username) == csvLines[i])
                    {
                        char *token = strtok(csvLines[i], ",");
                        fprintf(file, "%s", token);

                        token = strtok(NULL, ",");

                        fprintf(file, ",%s", newPassword);

                        while (token != NULL)
                        {
                            token = strtok(NULL, ",");
                            if (token != NULL)
                            {
                                fprintf(file, ",%s", token);
                            }
                        }

                        fprintf(file, "\n");
                    }
                    else
                    {
                        fputs(csvLines[i], file);
                    }
                    free(csvLines[i]);
                }
                fclose(file);
                free(csvLines);

                printf("**** Password changed! ****\n");
            }
            else
            {
                printf("User not found.\n");
            }
            break;
        }
        case 3:
        {
            char username[30];
            printf("Enter username to manage their posts: ");
            scanf("%29s", username);
            while (getchar() != '\n')
                ;

            user_t *user = find_user(users, username);
            if (user)
            {
                int postChoice;
                do
                {
                    display_user_posts(user);

                    printf("1. Add a new user post\n");
                    printf("2. Remove a user post\n");
                    printf("3. Return to main menu\n");
                    printf("Enter your choice: ");
                    scanf("%d", &postChoice);
                    while (getchar() != '\n')
                        ;

                    switch (postChoice)
                    {
                    case 1:
                    {
                        char postContent[250];
                        printf("Enter your post content: ");
                        scanf("%249[^\n]", postContent);
                        while (getchar() != '\n')
                            ;

                        add_post(user, postContent);
                        update_csv(users, user);
                        printf("Post added successfully.\n");
                        break;
                    }
                    case 2:
                    {
                        if (user->posts != NULL)
                        {
                            if (delete_post(user, 1))
                            {
                                printf("Latest post deleted successfully.\n");
                                update_csv(users, user);
                            }
                            else
                            {
                                printf("Failed to delete the latest post.\n");
                            }
                        }
                        else
                        {
                            printf("No posts to delete.\n");
                        }
                        break;
                    }
                    case 3:
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                        break;
                    }
                } while (postChoice != 3);
            }
            else
            {
                printf("User not found.\n");
            }
            break;
        }

        case 4:
        {
            char username[30];
            printf("Enter username to manage their friends: ");
            scanf("%29s", username);
            while (getchar() != '\n')
                ; // Clear the input buffer

            user_t *user = find_user(users, username); // Function to find the user
            if (!user)
            {
                printf("User not found.\n");
                break;
            }

            int friendChoice;
            do
            {

                printf("1. Display all friends\n");
                printf("2. Add a friend\n");
                printf("3. Remove a friend\n");
                printf("4. Return to main menu\n");
                printf("Enter your choice: ");
                scanf("%d", &friendChoice);
                while (getchar() != '\n')
                    ; // Clear the input buffer

                switch (friendChoice)
                {
                case 1:
                    display_user_friends(user); // Display the user's friends
                    break;

                case 2:
                { // Add a friend
                    char friendName[30];
                    int friendId;
                    printf("Enter a new friend's name: ");
                    scanf("%29s", friendName);
                    printf("Enter the new friend’s id: ");
                    scanf("%d", &friendId);
                    while (getchar() != '\n')
                        ;

                    add_friend(user, friendName, friendId); // Add the friend
                    update_csv(users, user);                // Update the CSV file
                    printf("Friend '%s' added to the list.\n", friendName);
                    break;
                }

                case 3:
                { // Remove a friend
                    printf("Enter the name of the friend to delete: ");
                    char friendName[30];
                    scanf("%29s", friendName);
                    while (getchar() != '\n')
                        ;

                    if (delete_friend(user, friendName))
                    {
                        update_csv(users, user); // Update the CSV file
                        printf("Friend '%s' removed from the list.\n", friendName);
                    }
                    else
                    {
                        printf("Friend '%s' not found.\n", friendName);
                    }
                    break;
                }

                case 4:
                    printf("Returning to the main menu.\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
                }
            } while (friendChoice != 4);
            break;
        }

        case 6:
            printf("Exiting and cleaning up...\n");
            teardown(users);
            break;
        }
    } while (choice != 6);

    return 0;
}
