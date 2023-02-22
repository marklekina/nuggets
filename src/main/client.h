/*
 * client.h - header file for the nuggets game client
 *
 * Mark Lekina Rorat, January 2023
 *
 */

#ifndef __CLIENT_H
#define __CLIENT_H

/**************** types ****************/
/*
 * a type representing all information we need to construct the player's display
 */
typedef struct display
{
    char player_letter; // player's letter (as displayed on the map)
    int n;              // gold last collected by player
    int p;              // gold in player's wallet
    int r;              // gold remaining in the game
    char *mapstring;    // string representation of the game's map
    char *supp_info;    // holds additional info to be displayed 
} display_t;

/**************** functions ****************/

// handlers
bool handleInput(void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);
bool handle_ok(const char letter);
bool handle_grid(const display_t *display, const int nrows, const int ncols);
bool handle_gold(const display_t *display, const int n, const int p, const int r);
bool handle_display(const display_t *display, const char *mapstring);
bool handle_quit(const char *explanation);
bool handle_error(const display_t *display, const char *explanation);

// senders
void send_play(const addr_t to, const char *name);
void send_key(const addr_t to, const char keystroke);
void send_spectate(const addr_t to);

// other functions
void update_display(const display_t *display);
display_t* init_display();

#endif //__CLIENT_H