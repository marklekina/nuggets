/*
 * client.c - server program for the Nuggets game
 *
 * see client.h for more information.
 *
 * Mark Lekina Rorat, February 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "../support/message.h"
#include "../support/mem.h"
#include "client.h"

#define MAX_STATUS_LINE_LENGTH 50

int main(int argc, char const *argv[])
{
    // validate command-line parameters
    const char *program = argv[0];
    if (argc < 3 || argc > 4)
    {
        printf("usage: %s hostname port [player_name]\n", program);
        return 0;
    }

    // parse server hostname and port number
    char *hostname = argv[1];
    char *port = argv[2];

    // parse optional player name
    char *player_name = NULL;
    if (argc == 4)
    {
        player_name = argv[3];
    }

    // initialize the message module
    if (!(message_init(stderr)))
    {
        printf("%s: failed to initialize the message module\n", program);
        return 2;
    }

    // set address for the server
    addr_t server_address = message_noAddr();
    if (!message_setAddr(hostname, port, server_address))
    {
        printf("%s: error initializing server address\n", program);
        return 3;
    }

    // initialize display
    display_t *display = init_display();
    if (!display) {
        printf("%s: error allocating memory for the client's display\n", program);
        return 4;
    }

    // join game at the server's address
    if (player_name)
    {
        send_play(server_address, player_name);
    }
    else
    {
        send_spectate(server_address);
    }

    // loop and wait for input or messages
    // TODO: write the handleInput function
    bool exit_status = message_loop(display, 0, NULL, NULL, handleMessage);

    // shut down the messaging module
    message_done();

    // return successfully
    return exit_status ? 0 : 1;
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
    // recast void * arg to display object
    display_t *display = (display_t *)arg;

    // return value for helper handler functions
    bool handler_return;

    // check type of message and handle appropriately
    // for each message type:
    //    parse relevant info
    //    pass info to specific handler function

    if (strncmp(message, "OK ", strlen("OK ")) == 0)
    {
        const char letter = message + strlen("OK ");
        handler_return = handle_ok(display, letter);
    }
    else if (strncmp(message, "GRID ", strlen("GRID ")) == 0)
    {
        int nrows, ncols;
        sscanf(message, "GRID %d  %d", &nrows, &ncols);
        handler_return = handle_grid(nrows, ncols);
    }
    else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0)
    {
        int n, p, r;
        sscanf(message, "GOLD %d %d  %d", &n, &p & r);
        handler_return = handle_gold(display, n, p, r);
    }
    else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0)
    {
        const char *mapstring = message + strlen("DISPLAY");
        handler_return = handle_display(display, mapstring);
    }
    else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0)
    {
        const char *explanation = message + strlen("QUIT ");
        handler_return = handle_quit(explanation);
    }
    else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0)
    {
        const char *explanation = message + strlen("ERROR");
        handler_return = handle_error(explanation);
    }
    else
    {
        // ignore malformatted messages
        handler_return = false;
    }

    // after handling each command, reprint the screen if:
    //     a mapstring is present in the game and,
    //     the game is still running (if handler return is false)
    if (display->mapstring && !handler_return)
    {
        refresh();
    }

    // return false to continue message loop, false to terminate
    return handler_return;
}

bool handle_ok(const char letter)
{
    // assign player letter
    display->player_letter = letter;

    // return false to keep message_loop running
    return false;
}

bool handle_grid(const display_t *display, const int nrows, const int ncols)
{
    // initialize ncurses display
    initscr(); // CURSES

    // accept input one keystroke at a time
    cbreak();

    // do not display keystrokes on the screen
    noecho();

    // initialize a color pair for the display
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    // ensure window size is large enough to fit the display
    int x, y;
    do
    {
        // prompt user to expand the window and wait for one second
        getmaxyx(stdscr, y, x);
        printw("Please ensure the display window is atleast %d by %d.", nrows, ncols);
        sleep(1);

        // clear output from the window before starting the game
        move(0, 0);
        clrtoeol();
    } while (y < (nrows + 1) || x < (ncols + 1));
}

bool handle_gold(const display_t *display, const int n, const int p, const int r)
{
    // update gold stats
    display->n = n;
    display->p = p;
    display->r = r;

    // announce that gold has been collected (if any has)
    if (n > 0)
    {
        char gold_info[MAX_STATUS_LINE_LENGTH];
        sprintf(gold_info, "GOLD received: %d", n);
        display->supp_info = gold_info;
    }

    // update the display
    update_display(display);

    // return false to keep message_loop running
    return false;
}

bool handle_display(const display_t *display, const char *mapstring)
{
    // update the mapstring and update the display
    display->mapstring = mapstring;
    update_display(display);

    // return false to keep message_loop running
    return false;
}

bool handle_quit(const char *explanation)
{
    // terminate ncurses
    endwin(); // CURSES

    // print explanation
    printf("%s\n", explanation);

    // terminate message_loop
    return true;
}

bool handle_error(const display_t *display, const char *explanation)
{
    // update the error explanation and update the display
    display->supp_info = explanation;
    update_display(display);

    // return false to keep message_loop running
    return false;
}

void update_display(const display_t *display)
{
    // construct the display only if a mapstring has been received from the server
    char *mapstring = display->mapstring;
    if (!mapstring)
    {
        return;
    }

    // initialize status line
    char status_line[MAX_STATUS_LINE_LENGTH];

    // fetch player information
    int n = display->n;
    int p = display->p;
    int r = display->r;
    char letter = display->player_letter;

    // construct status line from gold info and player letter
    // spectator
    if (letter == '_' && p == 0)
    {
        sprintf(status_line, "Spectator: %d nuggets unclaimed. ", r);
    }
    // player
    else
    {
        sprintf(status_line, "Player %c has %d nuggets (%d nuggets unclaimed). ", letter, p, r);
    }

    // initialize display string
    char *full_display[message_MaxBytes];

    // concatenate the status line to the display string
    strncat(full_display, status_line, strlen(status_line));

    // if we have additional info, display it next to the status line
    char *info = display->supp_info;
    if (info)
    {
        strncat(full_display, info, strlen(info));
    }

    // concatenate the mapstring to complete the display
    strncat(full_display, mapstring, strlen(mapstring));

    // print out the full display
    mvprintw(0, 0, full_display); // CURSES
}

display_t init_display()
{
    // allocate memory for the display, return NULL if memory cannot be allocated
    display_t *display = mem_malloc(sizeof(display_t));
    if (!display)
    {
        return NULL;
    }

    // initialize default values for the display
    display->n = 0;
    display->p = 0;
    display->r = 0;
    display->player_letter = '_';
    display->mapstring = NULL;
    display->supp_info = NULL;

    // return struct
    return display;
}