// APS105 Lab 9
// This is a program written to maintain a personal music library,
// using a linked list to hold the songs in the library.
// Author: RZII

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A node in the linked list

// Each string in the node is declared as a character pointer variable,
// so they need to be dynamically allocated using the malloc() function,
// and deallocated using the free() function after use.
typedef struct node {
  char *artist;
  char *songName;
  char *genre;
  struct node *next;
} Node;

const int MAX_LENGTH = 1024;

typedef struct linkedList {
    Node *head;
} LinkedList;

void inputStringFromUser(char *prompt, char *s, int maxStrLength) {
  int i = 0;
  char c;

  printf("%s --> ", prompt);
  while (i < maxStrLength && (c = getchar()) != '\n')
    s[i++] = c;
  s[i] = '\0';
}

// Function to call when the user is trying to insert a song name
// that is already in the personal music library.
void songNameDuplicate(char *songName) {
  printf("\nA song with the name '%s' is already in the music library.\n"
         "No new song entered.\n",
         songName);
}

// Function to call when a song name was found in the personal music library.
void songNameFound(char *songName) {
  printf("\nThe song name '%s' was found in the music library.\n", songName);
}

// Function to call when a song name was not found in the personal music
// library.
void songNameNotFound(char *songName) {
  printf("\nThe song name '%s' was not found in the music library.\n",
         songName);
}

// Function to call when a song name that is to be deleted
// was found in the personal music library.
void songNameDeleted(char *songName) {
  printf("\nDeleting a song with name '%s' from the music library.\n",
         songName);
}

// Function to call when printing an empty music library.
void printMusicLibraryEmpty(void) { 
  printf("\nThe music library is empty.\n"); 
}

// Function to call to print a title when the entire music library is printed.
void printMusicLibraryTitle(void) { 
  printf("\nMy Personal Music Library: \n"); 
}

Node *createNode(char *inputName, char *inputArtist, char *inputGenre) {
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode != NULL) {
        newNode->songName = malloc (strlen (inputName) + 1);
        newNode->artist = malloc (strlen (inputArtist) + 1);
        newNode->genre = malloc (strlen (inputGenre) + 1);

        strcpy (newNode->songName, inputName);
        strcpy (newNode->artist, inputArtist);
        strcpy (newNode->genre, inputGenre);
        newNode->next = NULL;
    }

    return newNode;
}

void initList(LinkedList *list) {
    list->head = NULL;
}

bool isEmpty(LinkedList *list) {
    if (list->head == NULL) {
        return true;
    }
    return false;
}



bool insertAtFront(LinkedList *list, char *songName, char *artist, char *genre) {
    Node *temp = createNode(songName, artist, genre);
    if (temp == NULL) {
        return false;
    }
    temp->next = list->head;
    list->head = temp;
    return true;
}


bool insertIntoOrderedList(LinkedList *orderedList, char *inputName, char *inputArtist, char *inputGenre) {
    if (isEmpty (orderedList)) {
        return insertAtFront(orderedList, inputName, inputArtist, inputGenre);
    }

    if (strcmp (inputName, orderedList->head->songName) < 0) {
        return insertAtFront(orderedList, inputName, inputArtist, inputGenre);
    }
    
    ///////////////////////////////////////////////////////
    // to check if there is duplicate within the library //

    Node *traCur = orderedList->head;

    if (traCur->next == NULL) { //if there is only one node
        if (strcmp (traCur->songName, inputName)) {
            ;
        } else {
            songNameDuplicate (inputName);
            return false;
        }
    }

    while (traCur->next != NULL) {
        if (strcmp (traCur->songName, inputName)) {
            traCur = traCur->next;
        } else {
            songNameDuplicate (inputName);
            return false;
        }
    }

    // to check if there is duplicate within the library //
    ///////////////////////////////////////////////////////
    Node *current = orderedList->head;

    while (current->next != NULL && strcmp (inputName, current->next->songName) > 0 ) {
        current = current->next;
    }

    Node *newNode = createNode (inputName, inputArtist, inputGenre);

    if (newNode == NULL) {
        return false;
    }

    newNode->next = current->next;
    current->next = newNode;

    return true;
}

void printMusicLibrarySongs(LinkedList *list) {
    if (isEmpty (list)) {
        printMusicLibraryEmpty ();
        return;
    }
    printMusicLibraryTitle ();
    Node *current = list->head;

    while (current != NULL) {
        printf("\n%s\n%s\n%s\n", current->songName, current->artist, current->genre);
        current = current->next;
    }
}

void search_song (LinkedList *list, char *inputName) {
    if (isEmpty (list)) {
        songNameNotFound (inputName);
        return;
    }

    Node *current = list->head;

    while (current != NULL) {
        if (strcmp (current->songName, inputName)) {
            current = current->next;
        } else {
            songNameFound (inputName);
            printf ("\n%s\n%s\n%s\n", current->songName, current->artist, current->genre);
            return;
        }
    }

    songNameNotFound (inputName);
    return;
}

void deleteFront (LinkedList *list) {
    if (isEmpty (list)) {
        return;
    }

    Node *newHead = list->head->next;
    songNameDeleted (list->head->songName);
    free (list->head->songName);
    free (list->head->artist);
    free (list->head->genre);
    free (list->head);
    list->head = newHead;
}


void delete_song (LinkedList *list, char *inputName) {
    if (isEmpty (list)) {
        songNameNotFound (inputName);
        return;
    }

    if (strcmp (list->head->songName, inputName) == 0) { 
        deleteFront (list);
        return;
    }

    Node *current = list->head;

    while (current->next != NULL && strcmp (current->next->songName, inputName) != 0 ) {
        current = current->next;
    }

    if (current->next != NULL) {
        Node *temp = current->next;
        current->next = temp->next;
        songNameDeleted (inputName);
        free (temp->songName);
        free (temp->artist);
        free (temp->genre);
        free (temp);
        return;
    }

    songNameNotFound (inputName);
    return;
}



void quit_program (LinkedList *list) {
    while (!isEmpty (list)) {
        deleteFront (list);
    }

    list->head = NULL;
}

int main(void) {
    // Declare the head of the linked list.
    LinkedList library;
    initList (&library);



    printf("Personal Music Library.\n\n");
    printf("%s", "Commands are I (insert), D (delete), S (search by song name),\n"
                 "P (print), Q (quit).\n");

    char response;
    char input [1025];
    do {
        inputStringFromUser("\nCommand", input, MAX_LENGTH);

        // Response is the first character entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I') {

            char *promptName = "Song name";
            char *promptArtist = "Artist";
            char *promptGenre = "Genre";

            char *inputName = (char *) malloc (sizeof (char) * MAX_LENGTH);
            char *inputArtist = (char *) malloc (sizeof (char) * MAX_LENGTH);
            char *inputGenre = (char *) malloc (sizeof (char) * MAX_LENGTH);

            inputStringFromUser (promptName, inputName, MAX_LENGTH);
            inputStringFromUser (promptArtist, inputArtist, MAX_LENGTH);
            inputStringFromUser (promptGenre, inputGenre, MAX_LENGTH);

            insertIntoOrderedList (&library, inputName, inputArtist, inputGenre);
          
          	free (inputName);
            free (inputArtist);
            free (inputGenre);
        } else if (response == 'D') {

            char *inputName = (char *) malloc (sizeof (char) * MAX_LENGTH);
            char *prompt = "\nEnter the name of the song to be deleted";
            inputStringFromUser (prompt, inputName, MAX_LENGTH);
            delete_song (&library, inputName);
            free (inputName);


        } else if (response == 'S') {

            char *inputName = (char *) malloc (sizeof (char) * MAX_LENGTH);
            char *prompt = "\nEnter the name of the song to search for";
            inputStringFromUser (prompt, inputName, MAX_LENGTH);
            search_song (&library, inputName);
            free (inputName);


        } else if (response == 'P') {
            if (isEmpty (&library) ) {
                printMusicLibraryEmpty ();
            } else {
                printMusicLibrarySongs (&library);
            }


        } else if (response == 'Q') {
            ; // do nothing, we'll catch this below
        } else {
            // do this if no command matched ...
            printf("\nInvalid command.\n");
        }
    } while (response != 'Q');

    //delete all the songs
    quit_program (&library);


    printMusicLibrarySongs (&library);

    return 0;
}

