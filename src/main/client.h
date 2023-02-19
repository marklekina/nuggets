/*
 * client.h - header file for the nuggets game client
 *
 * Mark Lekina Rorat, January 2023
 *
 */

#ifndef __CLIENT_H
#define __CLIENT_H

/**************** functions ****************/

// handlers
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handle_ok(const char *ok_message);
bool handle_grid(const char* grid_message);
bool handle_gold(const char* gold_message);
bool handle_display(const char* mapstring);
bool handle_quit(const char* explanation);
bool handle_error(const char* explanation);

// senders
void send_play(const addr_t to, const char* name);
void send_key(const addr_t to, const char keystroke);
void send_spectate(const addr_t to);

// other functions

#endif //__CLIENT_H