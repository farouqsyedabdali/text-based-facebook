/********* definitions.c ********
    Student Name 	= Farouq Syed Abdali
    Student Number	= 101263256
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "functions.h"

// Your solution goes here

user_t *add_user(user_t *users, const char *username, int userid, const char *password)
{
    user_t *new_user = (user_t *)malloc(sizeof(user_t));
    if (!new_user)
        return users; // Handle allocation failure

    // Initialize the new user's details
    strcpy(new_user->username, username);
    new_user->userid = userid;
    strcpy(new_user->password, password);
    new_user->friends = NULL;
    new_user->posts = NULL;
    new_user->next = NULL;
    // Insertion into sorted list
    if (!users || userid < users->userid)
    {
        new_user->next = users;
        return new_user;
    }
    user_t *current = users;
    while (current->next && current->next->userid < userid)
    {
        current = current->next;
    }
    new_user->next = current->next;
    current->next = new_user;
    return users;
}
user_t *find_user(user_t *users, const char *username)
{
    while (users)
    {
        if (strcmp(users->username, username) == 0)
            return users;
        users = users->next;
    }
    return NULL; // User not found
}
friend_t *create_friend(const char *username, int id)
{
    friend_t *new_friend = (friend_t *)malloc(sizeof(friend_t));
    if (!new_friend)
        return NULL; // Failed allocation

    strcpy(new_friend->username, username);
    new_friend->id = id;
    new_friend->next = NULL;
    return new_friend;
}
void add_friend(user_t *user, const char *friend_name, int id)
{
    friend_t *new_friend = create_friend(friend_name, id);
    if (!user->friends || id < user->friends->id)
    {
        new_friend->next = user->friends;
        user->friends = new_friend;
        return;
    }
    friend_t *current = user->friends;
    while (current->next && current->next->id < id)
    {
        current = current->next;
    }
    new_friend->next = current->next;
    current->next = new_friend;
}
_Bool delete_friend(user_t *user, char *friend_name)
{
    if (!user->friends)
        return false;

    friend_t *current = user->friends, *prev = NULL;
    while (current && strcmp(current->username, friend_name) != 0)
    {
        prev = current;
        current = current->next;
    }
    if (!current)
        return false; // Friend not found

    if (!prev)
        user->friends = current->next; // First friend is the match
    else
        prev->next = current->next;

    free(current);
    return true;
}
post_t *create_post(const char *text)
{
    post_t *new_post = (post_t *)malloc(sizeof(post_t));
    if (!new_post)
        return NULL; // Failed allocation

    strncpy(new_post->content, text, sizeof(new_post->content) - 1);
    new_post->content[sizeof(new_post->content) - 1] = '\0'; // Ensure null termination
    new_post->next = NULL;
    return new_post;
}
void add_post(user_t *user, const char *text)
{
    if (!user)
        return; // User not found

    post_t *new_post = create_post(text);
    if (!new_post)
        return; // Failed to create post

    // LIFO addition
    new_post->next = user->posts;
    user->posts = new_post;
}
_Bool delete_post(user_t *user, int number)
{
    if (!user || !user->posts)
        return false; // No user or no posts to delete

    post_t *current = user->posts, *prev = NULL;
    int count = 1; // Assuming post numbering starts at 1
    // Find the post to delete
    while (current != NULL && count < number)
    {
        prev = current;
        current = current->next;
        count++;
    }
    // If the post number is invalid
    if (!current)
        return false;

    // If it's the first post
    if (prev == NULL)
    {
        user->posts = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    free(current);
    return true;
}
void display_user_posts(user_t *user)
{
    if (!user || !user->posts)
    {
        printf("No posts available for this user.\n");
        return;
    }
    printf("%s's posts:\n", user->username);
    post_t *current = user->posts;
    while (current)
    {
        printf("- %s\n", current->content);
        current = current->next;
    }
}
void display_user_friends(user_t *user)
{
    if (!user || !user->friends)
    {
        printf("No friends available for %s.\n", user->username);
        return;
    }
    printf("%s's friends:\n", user->username);
    friend_t *current = user->friends;
    while (current)
    {
        printf("- %s\n", current->username);
        current = current->next;
    }
}
void display_posts(user_t *users, int number)
{
    // Assuming 'number' is the count of posts to display each time
    // This function would require additional logic to handle user input (y/n)
    // and display the next 'number' of posts. Consider this a simplified version.
    if (!users || !users->posts)
    {
        printf("No posts to display.\n");
        return;
    }
    post_t *current = users->posts;
    int count = 0;
    while (current && count < number)
    {
        printf("%s\n", current->content);
        current = current->next;
        count++;
    }
    // Add logic here to ask the user if they want to see more posts, and if so,
    // continue displaying the next 'number' of posts.
}
void teardown(user_t *users)
{
    while (users)
    {
        user_t *temp = users;
        users = users->next;
        // Free posts
        while (temp->posts)
        {
            post_t *tempPost = temp->posts;
            temp->posts = temp->posts->next;
            free(tempPost);
        }
        // Free friends
        while (temp->friends)
        {
            friend_t *tempFriend = temp->friends;
            temp->friends = temp->friends->next;
            free(tempFriend);
        }
        free(temp);
    }
}
void print_menu()
{
    printf("MAIN MENU:\n");
    printf("1. Register a new User\n");
    printf("2. Manage a user's profile (change password)\n");
    printf("3. Manage a user's posts (add/remove)\n");
    printf("4. Manage a user's friends (display/add/remove)\n");
    printf("5. Display a user's posts\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}
void update_csv(user_t *users, user_t *target_user)
{
    FILE *file = fopen("user_details.csv", "w");
    if (!file)
    {
        perror("Failed to open the CSV file for writing");
        return;
    }
    // Write the CSV header
    fprintf(file, "Username,Password,Userid,Friends,,,Posts,,\n");
    // Iterate over all users
    for (user_t *cur = users; cur != NULL; cur = cur->next)
    {
        fprintf(file, "%s,%s,%d", cur->username, cur->password, cur->userid);
        // Write friends; ensure placeholders if no friends are present
        friend_t *current_friend = cur->friends;
        for (int i = 0; i < 3; ++i)
        { // Assuming there can be up to 3 friends
            if (current_friend)
            {
                fprintf(file, ",%s", current_friend->username);
                current_friend = current_friend->next;
            }
            else
            {
                fprintf(file, ","); // Placeholder for missing friends
            }
        }
        // Write posts
        post_t *current_post = cur->posts;
        while (current_post)
        {
            fprintf(file, ",%s", current_post->content);
            current_post = current_post->next;
        }
        fprintf(file, "\n"); // End of this user's line
    }
    fclose(file);
}
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters
        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        char *token3 = strtok(NULL, ",");
        users = add_user(users, token, atoi(token3), token2);
        char *username = token;
        int id = atoi(token3);
        token = strtok(NULL, ",");
        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token, rand());
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;
        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
