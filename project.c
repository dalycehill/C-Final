#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 8

int ch, entries = 0;

struct book {
    int index;
    char libraryName[225];
    char author[225];
    char title[225];
    char publisher[225];
    char genre[225];
    char borrower[225];
    char dueDate[100];
    float replacementCost;
    struct book *next;
};

//functions
struct book *LoadSavedData();

struct book *AddNewEntry(struct book *start);
struct book *DeleteEntry(struct book *start);
struct book *SortAlphabetically(struct book *start);

void BrowseEntries(struct book *start); 
void FillStructData(struct book *current);
void ModifyEntry(struct book *start);
void SaveData(struct book *start);
void SearchTitle(struct book *start);
void SearchAuthor(struct book *start);

void DisplayOne(struct book *move);
void DisplayAll(struct book *move);

int main() {
    int optionLoad = 0, option = 0;
    char proceed = 'n';
    struct book *start = 0;
    struct book *move;

    // ask for loaded data or new
    puts("Welcome\nWould you like to (1)load saved data or (2)start anew?");
    scanf("%d", &optionLoad);
    if (optionLoad == 1) {
        start = LoadSavedData(start);
    } else if (optionLoad == 2) {
        puts("\nStarting anew...");
    } else {
        puts("\nThat isn't an option. Starting anew...");
    }

    while (proceed == 'n') {
        // ask for what user wants to do
        puts("\nWhat would you like to do?\n1-Add new entry\n2-Browse entries\n3-Modify entry\n4-Delete entry\n5-Save data\n6-Sort entries by author\n7-Search by book title\n8-Search by author\n9-Exit");
        scanf("%d", &option);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        switch (option) {
            case 1: start = AddNewEntry(start); break;
            case 2: BrowseEntries(start); break;
            case 3: ModifyEntry(start); break;
            case 4: start = DeleteEntry(start); break;
            case 5: SaveData(start); break;
            case 6: start = SortAlphabetically(start); break;
            case 7: SearchTitle(start); break;
            case 8: SearchAuthor(start); break;
            case 9: puts("\nGoodbye"); proceed = 'y'; break;
            default: puts("\nThat isn't an option.");
        }
        // my mother helped me find this one (by mistake)
        option = 0;
    }
    return 0;
}

struct book *LoadSavedData(struct book *start) {
    struct book *current, *last;
    struct book read;
    int count = 0;
    char filename[100] = "LibraryBooksLoaned.bin";
    FILE *fp;

    fp = fopen(filename, "rb");
    if (fp) {
        printf("\nRead file '%s'.\n", filename);
    } else {
        puts("\nCannot open file. Exiting...");
        exit(1);
    }

    // read number of entries
    fread(&entries, sizeof(int), 1, fp);

    // malloc a place to start reading in     
    last = (struct book *)malloc(sizeof(struct book));
    while (count < entries) {

        // set up next node
        fread(&read, sizeof(struct book), 1, fp);
        read.next = 0;
    
        // malloc last->next & point it to where it was read in
        last->next = (struct book *)malloc(sizeof(struct book));
        *(last->next) = read;

        // set start
        if (count == 0) {
            start = last->next;
        }
        count++;

        // go to next
        last = last->next;
    }

    fclose(fp);
    puts("Saved data loaded.");
    return start;
}

struct book *AddNewEntry(struct book *start) {
    struct book *move, *current;
    int count = 0;
        
    // limit amount of entries
    if (entries == MAX) {
        puts("\nMax entries has been reached.");
    } else {
        move = start;
        if (start == 0) {
            // first node
            current = (struct book *)malloc(sizeof(struct book));
            if (current == NULL) {
                puts("MEMORY ALLOCATION ERROR. Exiting...");
                exit(1);
            }

            start = current;
            count++;
            entries++;
            FillStructData(current);
            current->next = 0;
        } else {
            // get to the last entry
            while (move->next != 0) {
                move = move->next;
            }

            // set up next node; link to last node
            current = (struct book *)malloc(sizeof(struct book));
            if (current == NULL) {
                puts("MEMORY ALLOCATION ERROR. Exiting...");
                exit(1);
            }

            count++;
            entries++;
            FillStructData(current);
            current->next = 0;
            move->next = current;
        } 
        puts("\nEntry added.");
    }
    return start;
}

struct book *DeleteEntry(struct book *start) {
    struct book *move, *next, *last;
    int count = 0, count2 = 0, enteredIndex = 0;    
    char proceed = 'n';

    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else {
        move = start;
        DisplayAll(move);

        // ask for things
        puts("\nEnter index of book you wish to delete:");
        scanf("%d", &enteredIndex);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        puts("\nAre you sure? y/n");
        scanf("%c", &proceed);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        move = start;
        if (proceed == 'y') {

            // make sure next of previous is pointed to next of next
            while (move->next != 0) {
                next = move->next;

                if (move->index == enteredIndex) {
                    // delete it
                    free(move);
                    entries--;
                    puts("\nEntry deleted.");
                    break;
                } 
                count++;
                move = move->next;
            }

            if (count == 0) {
                // for first entry
                if (entries-1==0) {
                    entries--;
                    start = 0;
                    puts("\nEntry deleted.");
                    puts("\nThere are no more entries left.");
                } else {
                    start = next;
                }
            } else if (move->next == 0) {

                if (move->index == enteredIndex) {
                    // delete last entry
                    free(move);
                    entries--;
                    // set new last entry's next=0
                    move = start;
                    for (int i=0;i<count-1;i++) {
                        move = move->next;
                    }
                    move->next = 0;
                    puts("\nEntry deleted.");
                } else {
                    // no match
                    puts("\nSorry, that book does not exist.");
                }
                
            } else {
                // make sure the ones around the one deleted are connected
                move = start;
                while (move->next != 0) {
                    count2++;
                    if (count == count2) {
                        move->next = next;
                    }
                    move = move->next;
                }
            }

        } 
    }
   
    return start;
}

struct book *SortAlphabetically(struct book *start) {
    struct book *inside, *outside, *temp, *inLast, *outLast;
    bool startChanged = false;

    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else { 
        inside = start;
        outside = start;

        for (int i=0;i<entries;i++) {
            // get outside into position
            for (int y=0;y<i;y++) {
                outside = outside->next;
            }

            for (int j=i+1;j<entries;j++) {
                // get inside into position
                for (int z=0;z<j;z++) {
                    inside = inside->next;
                }
                
                // if the outside author is less than the inside author...
                if (strcmp(outside->author, inside->author) > 0) {
                    //if start is the problem...
                    if (i==0) {
                        
                        if (entries==2) {
                            // end with the outside
                            inside->next = start;
                            // start with the inside
                            start = outside->next;
                            outside->next = 0;
                            break; 
                        } else {
                            // switch inside->next with outside->next
                            temp = inside->next;
                            inside->next = outside->next;
                            outside->next = temp;

                            temp = start;

                            // if comparing with the second entry...
                            if (j==i+1) {
                                // switch start with inside
                                start = inside->next;
                                inside->next = temp;
                            } else {
                                // switch start with the previous inside
                                start = inLast->next;
                                inLast->next = temp;
                            }
                            startChanged = true;
                        } 

                    //if start isn't the problem...
                    } else { 

                        // switch inside->next with outside->next
                        temp = inside->next;
                        inside->next = outside->next;
                        outside->next = temp;

                        temp = outLast->next;

                        if (entries==3) {
                            // switch previous outside with inside->next
                            outLast->next = inside->next;
                            inside->next = temp;
                        } else {
                            // if comparing with the second entry...
                            if (j==i+1) {
                                // there is no previous inside
                                inLast = inside;
                            } 

                            // switch previous outside with previous inside
                            outLast->next = inLast->next;
                            inLast->next = temp;

                            // switch inside with outside
                            temp = inside;
                            inside = outside;
                            outside = temp;
                        }

                    }
                }

                // if the start was changed...
                if (startChanged == true) {
                    // the previous inside is now the outside & the outside is the start
                    inLast = outside;
                    outside = start;
                    startChanged = false;
                } else {
                    // the previous inside is the current inside
                    inLast = inside;
                } 
                inside = start;

            }   
            // get the previous outside
            outLast = outside;
            outside = start;
        }

        puts("\nEntries sorted.");
        DisplayAll(start);
    }
   return start;
}

void FillStructData(struct book *current) { 
    
    puts("Enter book index: ");
    scanf("%d", &current->index);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter library name: ");
    scanf("%[^\t\n]", current->libraryName);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter author's name: ");
    scanf("%[^\t\n]", current->author);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter book title: ");
    scanf("%[^\t\n]", current->title);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter publisher's name: ");
    scanf("%[^\t\n]", current->publisher);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter genre: ");
    scanf("%[^\t\n]", current->genre);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter borrower: ");
    scanf("%[^\t\n]", current->borrower);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter due date: ");
    scanf("%[^\t\n]", current->dueDate);
    while ((ch=getchar()) != '\n' && ch!= EOF);

    puts("Enter replacement cost: ");
    scanf("%f", &current->replacementCost);
    while ((ch=getchar()) != '\n' && ch!= EOF);

}

void BrowseEntries(struct book *start) {
    struct book *move;
    int count = 0;    
    char proceed = 'y';
    
    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else {
        move = start;
        while (proceed == 'y') {
            DisplayOne(move);
            move = move->next;
            count++;

            // if at the end...
            if (count == entries) {
                puts("\nThe end. Start over? y/n");
                scanf("%c", &proceed);
                while ((ch=getchar()) != '\n' && ch!= EOF);

                // start over
                if (proceed == 'y') {
                    move = start;
                    count = 0;
                }

            } else {
                puts("\nContinue? y/n");
                scanf("%c", &proceed);
                while ((ch=getchar()) != '\n' && ch!= EOF);
            }
        } 
    }

}

void ModifyEntry(struct book *start) {
    struct book *move;
    int count = 0, enteredIndex = 0, enteredChange = 0;    
    char proceed = 'y';
    bool entryModified = false;
    
    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else {
        move = start;
        DisplayAll(move);

        // ask for things
        puts("\nEnter index of book you wish to change:");
        scanf("%d", &enteredIndex);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        puts("\nWhat would you like to change?\n1-Index\n2-Library Name\n3-Author\n4-Book Title\n5-Publisher\n6-Genre\n7-Borrower\n8-Due Date\n9-Replacement Cost");
        scanf("%d", &enteredChange);
        while ((ch=getchar()) != '\n' && ch!= EOF);
        
        // change the entry requested
        move = start;
        while (move->next != 0) {
            if (move->index == enteredIndex) {
                // this switch has so many lines of code i wondered if it'd be better to put in a function since it's there twice, but i'm tired so no
                switch (enteredChange) {
                    case 1: 
                        puts("\nEnter new index: ");
                        scanf("%d", &move->index);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 2:
                        puts("\nEnter new library name: ");
                        scanf("%[^\t\n]", move->libraryName);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 3:
                        puts("\nEnter new author: ");
                        scanf("%[^\t\n]", move->author);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 4:
                        puts("\nEnter new title: ");
                        scanf("%[^\t\n]", move->title);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 5: 
                        puts("\nEnter new publisher: ");
                        scanf("%[^\t\n]", move->publisher);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 6:
                        puts("\nEnter new genre: ");
                        scanf("%[^\t\n]", move->genre);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 7:
                        puts("\nEnter new borrower: ");
                        scanf("%[^\t\n]", move->borrower);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 8:
                        puts("\nEnter new due date: ");
                        scanf("%[^\t\n]", move->dueDate);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    case 9:
                        puts("\nEnter new replacement cost: ");
                        scanf("%f", &move->replacementCost);
                        while ((ch=getchar()) != '\n' && ch!= EOF); 
                        entryModified = true;
                        break;
                    default: puts("\nNot an option. Try again."); break;
                }
                if (entryModified == true) {
                    puts("\nEntry modified:");
                    DisplayOne(move);
                }
            }
            move = move->next;
        }

        // check the last entry
        if (move->index == enteredIndex) {
            switch (enteredChange) {
                case 1: 
                    puts("\nEnter new index: ");
                    scanf("%d", &move->index);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 2:
                    puts("\nEnter new library name: ");
                    scanf("%[^\t\n]", move->libraryName);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 3:
                    puts("\nEnter new author: ");
                    scanf("%[^\t\n]", move->author);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 4:
                    puts("\nEnter new title: ");
                    scanf("%[^\t\n]", move->title);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 5: 
                    puts("\nEnter new publisher: ");
                    scanf("%[^\t\n]", move->publisher);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 6:
                    puts("\nEnter new genre: ");
                    scanf("%[^\t\n]", move->genre);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 7:
                    puts("\nEnter new borrower: ");
                    scanf("%[^\t\n]", move->borrower);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 8:
                    puts("\nEnter new due date: ");
                    scanf("%[^\t\n]", move->dueDate);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                case 9:
                    puts("\nEnter new replacement cost: ");
                    scanf("%f", &move->replacementCost);
                    while ((ch=getchar()) != '\n' && ch!= EOF); 
                    entryModified = true;
                    break;
                default: puts("\nNot an option. Try again."); break;
            }
            if (entryModified == true) {
                puts("\nEntry modified:");
                DisplayOne(move);
            }
        }
    }
    
}

void SaveData(struct book *start) {
    struct book *move;
    struct book write;
    char filename[100] = "LibraryBooksLoaned.bin";
    FILE *fp;

    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else { 
        move = start;
        // open file
        fp = fopen(filename, "wb");
        if (!fp) {
            puts("\nCannot open file. Exiting...");
            exit(1);
        }

        // write number of entries
        fwrite(&entries, sizeof(int), 1, fp);

        while (move->next != 0) {
            write = *move;
            // insert data
            fwrite(&write, sizeof(struct book), 1, fp);
            move = move->next;
        }
        write = *move;
        fwrite(&write, sizeof(struct book), 1, fp);

        fclose(fp);
        printf("\nData saved in %s.\n", filename);
    }
    
}

void SearchTitle(struct book *start) {
    struct book *move;
    char enteredTitle[225];
    bool match = false;

    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else { 
        move = start;
        // ask for title of book
        puts("\nEnter title of book to search:");
        scanf("%[^\t\n]", enteredTitle);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        while (move->next != 0) {
            // search for book
            if (strcmp(move->title, enteredTitle) == 0) {
                match = true;
                break;
            }
            move = move->next;        
        }

        if (strcmp(move->title, enteredTitle) == 0) {
            match = true;
        }

        if (match == true) {
            DisplayOne(move);
        } else {
            puts("\nNo match found.");
        }
    }
}

void SearchAuthor(struct book *start) {
    struct book *move;
    char enteredAuthor[225];
    bool match = false;

    if (entries == 0) {
        puts("\nPlease add entries before trying to use this functionality.");
    } else { 
        move = start;
        // ask for author of book
        puts("\nEnter author of book to search:");
        scanf("%[^\t\n]", enteredAuthor);
        while ((ch=getchar()) != '\n' && ch!= EOF);

        while (move->next != 0) {
            // search for book
            if (strcmp(move->author, enteredAuthor) == 0) {
                match = true;
                break;
            }
            move = move->next;        
        }

        if (strcmp(move->author, enteredAuthor) == 0) {
            match = true;
        }

        if (match == true) {
            DisplayOne(move);
        } else {
            puts("\nNo match found.");
        }
    }
}

void DisplayOne(struct book *move) {
    // print one entry
    printf("\nBook Index: %d\n", move->index);
    printf("Library: %s\n", move->libraryName);
    printf("Author: %s\n", move->author);
    printf("Title: %s\n", move->title);
    printf("Publisher: %s\n", move->publisher);
    printf("Genre: %s\n", move->genre);
    printf("Borrower: %s\n", move->borrower);
    printf("Due Date: %s\n", move->dueDate);
    printf("Replacement Cost: $%.2f\n", move->replacementCost);
}

void DisplayAll(struct book *move) {
    // print all entries
    puts("\nBooks:");
    while (move->next != 0) {
        DisplayOne(move);
        move = move->next;
    }
    DisplayOne(move);
}