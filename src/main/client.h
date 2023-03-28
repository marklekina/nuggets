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
    char player_letter;     // player's letter (as displayed on the map)
    int n;                  // gold last collected by player
    int p;                  // gold in player's wallet
    int r;                  // gold remaining in the game
    char *mapstring;        // string representation of the game's map
    addr_t server_address;  // server address for correspondence
    bool bot_mode;          // indicates whether a player is a bot
} display_t;

/**************** functions ****************/

// handlers
bool handleInput(void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);
bool handle_ok(display_t *display, char letter);
bool handle_grid(int nrows, int ncols);
bool handle_gold(display_t *display, int n, int p, int r);
bool handle_display(display_t *display, char *mapstring);
bool handle_quit(const char *explanation);

// senders
void send_play(addr_t to, char *name);
void send_key(addr_t to, char keystroke);
void send_spectate(addr_t to);

// other functions
void update_display(display_t *display);
display_t *init_display(addr_t address, bool bot_mode);
char get_random_keystroke();

#endif //__CLIENT_H
