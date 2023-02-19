/*
 * client.c - server program for the Nuggets game
 *
 * see client.h for more information.
 *
 * Mark Lekina Rorat, February 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "message.h"
#include "client.h"

// size of client's display
static int NROWS;
static int NCOLS;

int main(int argc, char const *argv[])
{
    return 0;
}

void send_play(const addr_t to, const char *name)
{
    // define string to carry information to server
    char message[message_MaxBytes];

    // format message
    sprintf(message, "PLAY %s", name);

    // send message
    message_send(to, message);
}

void send_spectate(const addr_t to)
{
    // define string to carry information to server
    char *message = "SPECTATE";

    // send message
    message_send(to, message);
}

void send_key(const addr_t to, const char keystroke)
{
    // define string to carry information to server
    char message[message_MaxBytes];

    // format message
    sprintf(message, "KEY %c", keystroke);

    // send message
    message_send(to, message);
}

bool handleMessage(void *arg, const addr_t from, const char *message)
{
    // return value for helper handler functions
    bool handler_return;

    // check type of message and handle appropriately
    if (strncmp(message, "OK ", strlen("OK ")) == 0)
    {
        // send to PLAY handler
        player_letter = message + strlen("OK ");
    }
    else
    {
        // send error message to correspondent
        send_error(from, "malformatted message");
        handler_return = false;
    }

    // return false to continue message loop, false to terminate
    return handler_return;
}

bool handle_grid(const char *grid_message)
{
}

bool handle_gold(const char *gold_message)
{
}

bool handle_display(const char *mapstring)
{
}

bool handle_quit(const char *explanation)
{
}

bool handle_error(const char *explanation)
{
}
