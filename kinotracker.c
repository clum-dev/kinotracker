#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file.h"
#include "kinotracker.h"

#define SPACE_LEN 40

/**
 *  TODO:
 *      -   add 'less' prompt for printing, which prints like 10 at a time
 *      -   add index swapping capability
 *          -   e.g. move watched stuff to the end
 *      -   add windows make functionality
*/


// Debug
void unimp(char* msg) {
    fprintf(stderr, "[UNIMP]\t%s\n", msg);
}


// Gets the enum form from a given genre string (default undefined if no match)
Genre get_genre(char* text) {

    if (!strcmp("scifi", text)) {
        return G_SCIFI;
    } else if (!strcmp("drama", text)) {
        return G_DRAMA;
    } else if (!strcmp("mystery", text)) {
        return G_MYSTERY;
    } else if (!strcmp("horror", text)) {
        return G_HORROR;
    } else if (!strcmp("action", text)) {
        return G_ACTION;
    } else if (!strcmp("comedy", text)) {
        return G_COMEDY;
    } else if (!strcmp("anime", text)) {
        return G_ANIME;
    } else if (!strcmp("war", text)) {
        return G_WAR;

    } else {
        return G_UNDEF;
    }
}

// Gets the string form of a given genre enum 
char* get_genre_str(Genre genre) {
    char* NAMES[9] = {
        "undefined",
        "scifi",
        "drama",
        "mystery",
        "horror",
        "action",
        "comedy",
        "anime",
        "war"
    };

    return NAMES[genre];
}

// Converts y/n prompt to bool
bool str_to_bool(char* text) {
    if (!strcmp("y", text) || !strcmp("Y", text)) {
        return true;
    } else if (!strcmp("n", text) || !strcmp("N", text)) {
        return false;
    } else {
        return false;
    }
}

// Gets the number of spaces needed to fill a given string length
// (used for good looking tabs)
String* get_space(String* input, int spaceLen) {
    
    int diff = spaceLen - input->len;

    String* out = str_init(NULL);
    for (int i = 0; i < diff; i++) {
        str_concat_char(out, ' ');
    }

    return out;
}

// Checks if a string is an int
bool is_number(String* input) {
    
    size_t count = 0;
    for (size_t i = 0; i < input->len; i++) {
        char c = input->text[i];
        if (isdigit(c)) {
            count++;
        }
    }

    return count == input->len;
}


// Initialises an item struct
Item* item_init(StringList* entry, String* raw, int index) {

    if (entry->len != 5) {
        return NULL;
    }

    Item* item = (Item*)malloc(sizeof(Item));
    item->index = index;

    item->name = str_init(entry->strings[I_NAME]->text);
    item->releaseYear = atoi(entry->strings[I_YEAR]->text);
    item->genre = get_genre(entry->strings[I_GENRE]->text);
    item->watched = str_to_bool(entry->strings[I_WATCHED]->text);
    item->favourite = str_to_bool(entry->strings[I_FAV]->text);

    item->vals = strlist_clone(entry);
    item->raw = str_init(raw->text);

    return item;
}

// Frees an item struct
void item_free(Item* item) {

    if (item != NULL) {
        str_free(item->name);
        str_free(item->raw);
        
        strlist_free(item->vals);

        free(item);
        item = NULL;
    }
}

// Prints an item struct
void item_print(Item* item) {
    
    if (item == NULL) {
        return;
    }

    String* space = get_space(item->name, SPACE_LEN);

    printf("[%d]\t%s%s(%d)\t%s", 
        (int)item->index, item->name->text, space->text, (int)item->releaseYear, get_genre_str(item->genre)
    );
    str_free(space);

    String* temp = str_init(get_genre_str(item->genre));
    String* space2 = get_space(temp, 12);
    printf("%s", space2->text);
    str_free(temp);
    str_free(space2);

    item->watched ? printf("[w]") : printf("[ ]");
    printf("\t");
    item->favourite ? printf("[*]") : printf("[ ]");
    printf("\n");

}


// Initialises an itemlist struct
ItemList* itemlist_init() {

    ItemList* list = (ItemList*)malloc(sizeof(ItemList));
    list->size = 0;
    list->items = NULL;

    return list;
}

// Frees an itemlist struct
void itemlist_free(ItemList* list) {

    if (list != NULL) {

        if (list->items != NULL) {
            for (size_t i = 0; i < list->size; i++) {
                item_free(list->items[i]);
            }

            free(list->items);
            list->items = NULL;
        }
        
        free(list);
        list = NULL;
    }
}

// Rebuilds the indexing of items in an itemlist
// Useful when indexing is interrupted by removes
void itemlist_re_index(ItemList* list) {
    
    // printf("list size: %d\n", (int)list->size);
    
    // Re-index items
    for (size_t i = 0; i < list->size; i++) {
        list->items[i]->index = i;
    }
}

// Prints an itemlist struct
void itemlist_print(ItemList* list, size_t numItems) {

    itemlist_re_index(list);

    if (numItems > list->size) {
        numItems = list->size;
    }

    printf("ItemList: (len = %d)\nPrinting first %d items:\n\n", (int)list->size, (int)numItems);
    for (size_t i = 0; i < numItems; i++) {
        if (list->items[i] != NULL) {
            item_print(list->items[i]);
        }
    }

}

// Prints the raw text for an itemlist
// (identical to stored text file)
void itemlist_print_raw(ItemList* list) {    
    for (size_t i = 0; i < list->size; i++) {
        printf("%s\n", list->items[i]->raw->text);
    }
}

// Adds an item to a given itemlist
void itemlist_add(ItemList* list, Item* item) {
    
    // printf("TESTING\tADDING:\n");
    // item_print(item);

    if (list->size == 0) {
        list->items = (Item**)malloc(sizeof(Item*) * ++list->size);
    } else {
        list->items = (Item**)realloc(list->items, sizeof(Item*) * ++list->size);
    }
    
    list->items[list->size - 1] = item;
}

// Adds item to the start of a given itemlist
void itemlist_add_start(ItemList* list, Item* item) {
    
    if (list->size == 0) {
        list->items = (Item**)malloc(sizeof(Item*) * ++list->size);
    } else {
        list->items = (Item**)realloc(list->items, sizeof(Item*) * ++list->size);
    }

    list->items[list->size - 1] = NULL;

    for (size_t i = list->size - 1; i > 0; i--) {
        list->items[i] = list->items[i - 1];
    }

    list->items[0] = item;
    
}

// Removes an item from a given itemlist
void itemlist_remove(ItemList* list, int index) {

    // Bounds check
    if (index < 0 || index >= list->size) {
        return;
    }

    // Free item at index
    item_free(list->items[index]);
    
    // Overwrite for next item
    for (size_t i = index; i < list->size - 1; ++i) {
        list->items[i] = list->items[i + 1];
    }

    // Reduce size by one (removed one item)
    list->size--;

    itemlist_re_index(list);

}

// Attempts to find an item in an itemlist
// Can provide either the item name, or its index
// (Returns -1 if the item cannot be found)
int itemlist_find(ItemList* list, String* name) {

    int index = -1;
   
    if (is_number(name)) {
        // printf("%s is a number\n", name->text);
        index = atoi(name->text);

    } else {
        // printf("%s is NOT a number\n", name->text);
        
        for (size_t i = 0; i < list->size; i++) {
            if (str_equals(list->items[i]->name, name, true)) {
                index = (int)i;
                break;
            }
        }
    }
    
    return index;
}

// Loads items from a stored file into an itemlist struct
ItemList* load_items(char* path) {
    
    // Load file
    FILE* fp = open_file(path, "r");
    StringList* lines = get_file_lines(fp);
    fclose(fp);

    // Add stored items from text file
    ItemList* items = itemlist_init();
    for (size_t i = 0; i < lines->len; i++) {
        String* raw = str_init(lines->strings[i]->text);
        StringList* temp = str_split(lines->strings[i], ",");
        itemlist_add(items, item_init(temp, raw, (int)i));
        str_free(raw);
        strlist_free(temp);
    }

    strlist_free(lines);

    return items;
}

// Prompts a user with a message and records their input as a string
String* prompt(char* msg) {
    printf("%s\n> ", msg);

    String* temp = dynamic_read(stdin, true);
    fflush(stdin);

    return temp;
}

// Prompts a user with a messange, records their input, 
// and checks against the given accept string
// Returns true if it matches
bool accept_prompt(char* msg, char* accept) {

    bool out = false;
    String* temp = prompt(msg);

    if (!strcmp(temp->text, accept)) {
        out = true;
    }
    str_free(temp);

    return out;
}

// Appends a string to the end of a file
void append_data_to_file(String* str, char* filepath) {
    // Append text to file
    FILE* fp = open_file(filepath, "a");
    fprintf(fp, "\n%s", str->text);
    fclose(fp);
}

// Overwrites a file with the given itemlist
void overwrite_file_data(ItemList* items, char* filepath) {
    // Overwrite file
    FILE* fp = open_file(filepath, "w");
    for (size_t i = 0; i < items->size; ++i) {
        fprintf(fp, "%s\n", items->items[i]->raw->text);
    }
    fclose(fp);
}

// Checks if a stringlist has no commas
// Needed to sanitize user input, as items are stored with comma separators
bool has_no_commas(StringList* vals) {
    for (size_t i = 0; i < vals->len; i++) {
        if (strchr(vals->strings[i]->text, ',') != NULL) {
            return false;
        }
    }
    return true;
}

// Adds an item from a user
void add_item_from_user(ItemList* list) {
    
    printf("Adding new item:\n");
    StringList* vals = strlist_init();

    strlist_add(vals, prompt("Name:"));
    strlist_add(vals, prompt("Release year:"));
    strlist_add(vals, prompt("Genre:"));
    strlist_add(vals, prompt("Watched? (y/n):"));
    strlist_add(vals, prompt("Favourite? (y/n):"));
    printf("\n");

    if (has_no_commas(vals)) {
        String* raw = strlist_join(vals, ',');

        printf("Adding '%s'\n\n", raw->text);    

        Item* temp = item_init(vals, raw, list->size - 1);
        itemlist_add_start(list, temp);

        str_free(raw);

    } else {
        printf("Error: inputs cannot contain commas - please try again\n\n");
    }

    strlist_free(vals);

}

// Displays the welcome message
void welcome_msg() {
    
    printf("  _  ___          _______             _             \n\
 | |/ (_)        |__   __|           | |            \n\
 | ' / _ _ __   ___ | |_ __ __ _  ___| | _____ _ __ \n\
 |  < | | '_ \\ / _ \\| | '__/ _` |/ __| |/ / _ \\ '__|\n\
 | . \\| | | | | (_) | | | | (_| | (__|   <  __/ |   \n\
 |_|\\_\\_|_| |_|\\___/|_|_|  \\__,_|\\___|_|\\_\\___|_|");

}


// Menu add path
void menu_add(ItemList* items, char* path) {
    // Check for user prompt
    while (true) {
        if (accept_prompt("Add item? (y/n)", "y")) {
            add_item_from_user(items);
            // append_data_to_file(items->items[items->size - 1]->raw, path);
        }

        if (accept_prompt("Done? (y/n)", "y")) {
            break;
        }
    }
}

// Menu print path
void menu_print(ItemList* list) {
    
    size_t numItems = 5;
    bool done = false;
    itemlist_print(list, numItems);
    
    while (!done) {

        String* response = prompt("\nPrint options:\n(M)ore\n(A)ll\n(C)lear\n(D)one\n");

        if (str_equals_text(response, "M", false)) {
            numItems += 5;
            itemlist_print(list, numItems);
        } else if (str_equals_text(response, "A", false)) {
            itemlist_print(list, list->size);
        } else if (str_equals_text(response, "C", false)) {
            system("clear");
        } else if (str_equals_text(response, "D", false)) {
            done = true;
        } else {
            printf("Unhandled option: '%s'\n", response->text);
            done = true;
        }

        str_free(response);
    }
}

// Filters list by genre
void filter_genre(ItemList* list) {
    
    printf("\nGenres:\n");
    for (int i = 0; i < GENRE_MAX; i++) {
        printf("[%d]\t%s\n", i, get_genre_str(i));
    }

    String* temp = prompt("\nSelect genre (0-8)\n");
    
    Genre genre = G_UNDEF;
    if (is_number(temp)) {
        genre = atoi(temp->text);
        if (genre < G_UNDEF || genre > GENRE_MAX) {
            genre = G_UNDEF;
        }
    }

    printf("Filtering by genre = '%s'\n", get_genre_str(genre));
    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i]->genre == genre) {
            item_print(list->items[i]);
        }
    }
    
    str_free(temp);
}

// Filters list by year
void filter_year(ItemList* list) {
    String* lower = prompt("Lower year bound?");
    String* upper = prompt("Upper year bound?");

    if (!is_number(lower) || !is_number(upper)) {
        printf("Invalid year range\n");
    } else {
        int lowerYear = atoi(lower->text);
        int upperYear = atoi(upper->text);

        printf("Filtering by years: (%d - %d)\n", lowerYear, upperYear);

        for (size_t i = 0; i < list->size; ++i) {
            if (list->items[i]->releaseYear >= lowerYear && list->items[i]->releaseYear <= upperYear) {
                item_print(list->items[i]);
            }
        }
    }

    str_free(lower);
    str_free(upper);
}

// Filters list by watched
void filter_watched(ItemList* list) {
    
    bool showWatched = false;
    if (accept_prompt("Only watched? (y/n)", "y")) {
        showWatched = true;
    }

    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i]->watched == showWatched) {
            item_print(list->items[i]);
        }
    }
}

// Filters list by favourites
void filter_favourites(ItemList* list) {

    bool showFavs = false;
    if (accept_prompt("Only favourites? (y/n)", "y")) {
        showFavs = true;
    }

    for (size_t i = 0; i < list->size; ++i) {
        if (list->items[i]->favourite == showFavs) {
            item_print(list->items[i]);
        }
    }

}

//
void filter_name(ItemList* list) {
    
    String* name = prompt("Enter name to find:");
    int index = itemlist_find(list, name);

    if (index != -1) {
        item_print(list->items[index]);
    } else {
        printf("Item '%s' could not be found\n", name->text);
    }

    str_free(name);
}

// Menu filter path
void menu_filter(ItemList* list) {
    
    printf("\nFilter by:\n");
    String* response = prompt("(G)enre\n(Y)ear\n(W)atched\n(F)avourites\n(N)ame");

    if (!strcmp(response->text, "G") || !strcmp(response->text, "g")) {
        filter_genre(list);
    } else if (!strcmp(response->text, "Y") || !strcmp(response->text, "y")) {
        filter_year(list);
    } else if (!strcmp(response->text, "W") || !strcmp(response->text, "w")) {
        filter_watched(list);
    } else if (!strcmp(response->text, "F") || !strcmp(response->text, "f")) {
        filter_favourites(list);
    } else if (!strcmp(response->text, "N") || !strcmp(response->text, "n")) {
        filter_name(list);
    } else {
        printf("Unhandled option: '%s'\n", response->text);
    }

    str_free(response);
    
}

// Menu edit path
void menu_edit(ItemList* list) {
    String* temp = prompt("Enter name or index to edit:");
    int index = itemlist_find(list, temp);
    str_free(temp);

    if (index != -1 && index < list->size) {
        printf("Editing item '%s':\n", list->items[index]->raw->text);
        StringList* vals = strlist_init();

        strlist_add(vals, prompt("Name:"));
        strlist_add(vals, prompt("Release year:"));
        strlist_add(vals, prompt("Genre:"));
        strlist_add(vals, prompt("Watched? (y/n):"));
        strlist_add(vals, prompt("Favourite? (y/n):"));
        printf("\n");

        String* raw = strlist_join(vals, ',');

        printf("Changed item '%s' to '%s'\n", list->items[index]->raw->text, raw->text);

        item_free(list->items[index]);
        list->items[index] = item_init(vals, raw, index);

        str_free(raw);

    } else {
        printf("Invalid index: '%d'\n", index);
    }

}

// Menu mark path
void menu_mark(ItemList* list) {
    String* temp = prompt("Enter name or index to mark:");
    int index = itemlist_find(list, temp);
    str_free(temp);

    if (index != -1 && index < list->size) {
        printf("Marking item '%s':\n", list->items[index]->raw->text);
        String* watched = prompt("Watched? (y/n)");
        String* fav = prompt("Favourite? (y/n)");

        list->items[index]->watched = str_to_bool(watched->text);
        list->items[index]->favourite = str_to_bool(fav->text);

        str_free(watched);
        str_free(fav);
    
    } else {
        printf("Invalid index: '%d'\n", index);
    }

}

// Menu remove path
void menu_remove(ItemList* list, char* filename) {

    String* temp = prompt("Enter name or index to remove:");
    int index = itemlist_find(list, temp);

    // printf("TEST: index = %d\tlist size = %lu\n", index, list->size);
    
    if (index != -1 && index < list->size) {
        printf("Removing item %d: '%s'\n", index, list->items[index]->raw->text);
        itemlist_remove(list, index);
    } else {
        printf("Invalid index: '%d'\n", index);
    }

    str_free(temp);
}

// Menu quit path
void menu_quit(ItemList* list, char* path) {
    if (accept_prompt("Write item changes to file? (y/n)", "y")) {
        printf("Updating file '%s' with changes\n", path);
        overwrite_file_data(list, path);
    } else {
        printf("Ignoring changes to '%s'\n", path);
    }

    printf("\nQuitting...\n\n");
}


// Main interaction menu
void menu(ItemList* list, char* path) {
    
    welcome_msg();

    bool exit = false;

    while (true) {

        printf("\n-----------------------------------------------------\n");
        printf("\nChoose an option:\n");
        String* response =  prompt("(A)dd items\n(P)rint items\n(E)dit items\n(M)ark items\n(F)ilter items\n(R)emove items\n(C)lear terminal\n(Q)uit\n");

        if (!strcmp(response->text, "A") || !strcmp(response->text, "a")) {
            menu_add(list, path);
        } else if (!strcmp(response->text, "P") || !strcmp(response->text, "p")) {
            menu_print(list);
        } else if (!strcmp(response->text, "F") || !strcmp(response->text, "f")) {
            menu_filter(list);
        } else if (!strcmp(response->text, "E") || !strcmp(response->text, "e")) {
            menu_edit(list);
        } else if (!strcmp(response->text, "M") || !strcmp(response->text, "m")) {
            menu_mark(list);
        } else if (!strcmp(response->text, "R") || !strcmp(response->text, "r")) {
            menu_remove(list, path);
        } else if (!strcmp(response->text, "C") || !strcmp(response->text, "c")) {
            system("clear");
        } else if (!strcmp(response->text, "Q") || !strcmp(response->text, "q")) {
            menu_quit(list, path);
            exit = true;
        } else {
            printf("Unhandled option: '%s'\n", response->text);
        }

        str_free(response);
        if (exit) {
            break;
        }
    }
}


// Main entrypoint
int main(int argc, char** argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: ./kinotracker <filepath>\n");
        return 1;
    }

    char* path = argv[1];
    ItemList* items = load_items(path);
           
    menu(items, path);

    itemlist_free(items);


    return 0;
}