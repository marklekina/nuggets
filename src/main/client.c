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

#define MAX_STATUS_LINE_LENGTH 50 // maximum length of the status line displayed at the top
#define CTRL(c) ((c)-0100)        // control character

int main(int argc, char *argv[])
{
    // validate command-line parameters
    char *program = argv[0];
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
    if (!message_setAddr(hostname, port, &server_address))
    {
        printf("%s: error initializing server address\n", program);
        return 3;
    }

    // initialize display
    display_t *display = init_display(server_address);
    if (!display)
    {
        printf("%s: error initializing the client's display\n", program);
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
    bool exit_status = message_loop(display, 0, NULL, handleInput, handleMessage);

    // shut down the messaging module
    message_done();

    // free display memory
    mem_free(display);

    // return successfully
    return exit_status ? 0 : 1;
}

bool handleInput(void *arg)
{
    // recast void * arg to display struct
    display_t *display = (display_t *)arg;

    // get server address
    addr_t to = display->server_address;

    // read one character at a time
    char keystroke = getch();

    // terminate game and message_loop if Ctrl + C is entered (or we reach EOF)
    if (keystroke == CTRL('c') || keystroke == EOF)
    {
        return handle_quit("Goodbye!");
    }

    // otherwise send keystroke to server
    else
    {
        send_key(to, keystroke);
        return false;
    }
}

void send_play(addr_t to, char *name)
{
    // define string to carry information to server
    char message[message_MaxBytes];

    // format message
    sprintf(message, "PLAY %s", name);

    // send message
    message_send(to, message);
}

void send_spectate(addr_t to)
{
    // send message
    message_send(to, "SPECTATE");
}

void send_key(addr_t to, char keystroke)
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
    // recast void * arg to display struct
    display_t *display = (display_t *)arg;

    // only accept messages from the server
    if (!message_eqAddr(from, display->server_address))
    {
        return false;
    }

    // return value for helper handler functions
    bool handler_return;

    // check type of message and handle appropriately
    // for each message type:
    //    parse relevant info
    //    pass info to specific handler function

    if (strncmp(message, "OK ", strlen("OK ")) == 0)
    {
        char letter = message[strlen("OK ")];
        handler_return = handle_ok(display, letter);
    }
    else if (strncmp(message, "GRID ", strlen("GRID ")) == 0)
    {
        int nrows, ncols;
        sscanf(message, "GRID %d %d", &nrows, &ncols);
        handler_return = handle_grid(nrows, ncols);
    }
    else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0)
    {
        int n, p, r;
        sscanf(message, "GOLD %d %d %d", &n, &p, &r);
        handler_return = handle_gold(display, n, p, r);
    }
    else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0)
    {
        char *mapstring = (char *)message + strlen("DISPLAY");
        handler_return = handle_display(display, mapstring);
    }
    else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0)
    {
        const char *explanation = message + strlen("QUIT ");
        handler_return = handle_quit(explanation);
    }
    else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0)
    {
        char *explanation = (char *)message + strlen("ERROR ");
        handler_return = handle_error(display, explanation);
    }
    else
    {
        // ignore malformatted messages
        handler_return = false;
    }

    // return false to continue message loop, true to terminate
    return handler_return;
}

bool handle_ok(display_t *display, char letter)
{
    // assign player letter
    display->player_letter = letter;

    // return false to keep message_loop running
    return false;
}

bool handle_grid(int nrows, int ncols)
{
    // initialize ncurses display
    initscr(); // CURSES

    // accept input one keystroke at a time
    cbreak(); // CURSES

    // do not display keystrokes on the screen
    noecho(); // CURSES

    // initialize a color pair for the display
    start_color();                          // CURSES
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // CURSES
    attron(COLOR_PAIR(1));                  // CURSES

    // ensure window size is large enough to fit the display
    int x, y;
    do
    {
        // prompt user to expand the window and wait for one second
        getmaxyx(stdscr, y, x);
        printw("Please ensure the display window is atleast %d by %d.", nrows, ncols); // CURSES
        sleep(1);

        // clear output from the window before starting the game
        move(0, 0); // CURSES
        clear(); // CURSES
    } while (y < (nrows + 1) || x < (ncols + 1));

    // continue message loop
    return false;
}

bool handle_gold(display_t *display, int n, int p, int r)
{
    // update gold stats
    display->n = n;
    display->p = p;
    display->r = r;

    // update the display
    update_display(display);

    // return false to keep message_loop running
    return false;
}

bool handle_display(display_t *display, char *mapstring)
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
    printf("%s", explanation);

    // terminate message_loop
    return true;
}

bool handle_error(display_t *display, char *explanation)
{
    // display error next to the status line
    mvprintw(0, MAX_STATUS_LINE_LENGTH, explanation);

    // return false to keep message_loop running
    return false;
}

void update_display(display_t *display)
{
    // ensure at least the first mapstring has been received from the server
    if (!display->mapstring)
    {
        return;
    }

    // clear previous display
    erase();

    // fetch player information
    int n = display->n;
    int p = display->p;
    int r = display->r;
    char letter = display->player_letter;

    // display the status line
    // spectator
    if (letter == '_' && p == 0)
    {
        mvprintw(0, 0, "Spectator: %d nuggets unclaimed. ", r);
    }
    // player
    else
    {
        mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed). ", letter, p, r);
    }

    // announce that gold has been collected (if any has)
    if (n > 0)
    {
        // display error next to the status line
        mvprintw(0, MAX_STATUS_LINE_LENGTH, "GOLD received: %d", n);
    }

    // finally display the mapstring
    mvprintw(getcury(stdscr) + 1, 0, display->mapstring);  // CURSES

    // render the full display
    refresh();              // CURSES
}

display_t *init_display(addr_t address)
{
    // ensure we have a valid server address
    if (!message_isAddr(address))
    {
        return NULL;
    }

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
    display->server_address = address;

    // return struct
    return display;
}
