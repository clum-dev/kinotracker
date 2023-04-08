#ifndef __KINOTRACKER_H__
#define __KINOTRACKER_H__

#include "strings.h"

#define GENRE_MAX 6

//
typedef enum Genre {

    G_UNDEF,
    G_SCIFI,
    G_DRAMA,
    G_MYSTERY,
    G_HORROR,
    G_ACTION,
    G_COMEDY,
    
} Genre;

typedef enum ItemIndex {
    
    I_NAME,
    I_YEAR,
    I_GENRE,
    I_WATCHED,
    I_FAV,

} ItemIndex;

//
typedef struct Item {

    size_t index;

    String* name;
    int releaseYear;
    Genre genre;
    bool favourite;
    bool watched;

    // TODO ADD MOVIE/SHOW FLAG HERE

    StringList* vals;
    String* raw;
    
} Item;

//
typedef struct ItemList {

    Item** items;
    size_t size;
    
} ItemList;




#endif