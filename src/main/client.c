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
#include <pthread.h>
#include <ctype.h>
#include "message.h"
#include "mem.h"
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

    // set seed for random behavior
    srand(getpid());

    // check whether a player is a bot
    bool bot_mode = true;
    if (!player_name || strncmp(player_name, "bot", strlen("bot")))
    {
        bot_mode = false;
    }

    // start bot thread if player is a bot
    pthread_t bot_thread;
    if (bot_mode)
    {
        pthread_create(&bot_thread, NULL, run_bot, display);
    }

    // loop and wait for input or messages; exit when message_loop returns true
    bool exit_status = message_loop(display, 0, NULL, handleInput, handleMessage);

    // terminate bot thread (if running)
    if (bot_mode)
    {
        pthread_cancel(bot_thread);
    }

    // free display memory
    mem_free(display);

    // shut down the messaging module
    message_done();

    // return successfully
    return exit_status ? 0 : 1;
}

bool handleInput(void *arg)
{
    // recast void * arg to display struct
    display_t *display = (display_t *)arg;

    // get server address
    addr_t to = display->server_address;

    // read keystroke from stdin
    char keystroke = getch();

    // terminate game and message_loop if Ctrl + X is entered (or we reach EOF)
    if (keystroke == CTRL('X') || keystroke == EOF)
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
        char *mapstring = (char *)message + strlen("DISPLAY\n");
        handler_return = handle_display(display, mapstring);
    }
    else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0)
    {
        const char *explanation = message + strlen("QUIT ");
        handler_return = handle_quit(explanation);
    }
    else
    {
        // ignore malformatted and error messages
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
    initscr();

    // accept input one keystroke at a time
    cbreak();

    // do not display keystrokes on the screen
    noecho();

    // initialize color pairs for the display
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    // define minimum screen size
    int min_y = nrows + 1;
    int min_x = ncols + 1;

    // check current window size
    int x, y;
    getmaxyx(stdscr, y, x);

    // ensure window size is large enough to fit the display
    while (y < min_y || x < min_x)
    {
        // prompt user to expand the window
        mvprintw(0, 0, "Ensure the display window is atleast %d by %d, then press any key to continue.\n", min_y, min_x);
        refresh();

        // wait for user input and update screen size
        getch();
        getmaxyx(stdscr, y, x);

        // clear output from the window before starting the game
        clear();
    }

    // continue message loop
    return false;
}

bool handle_gold(display_t *display, int n, int p, int r)
{
    // update gold stats
    display->n = n;
    display->p = p;
    display->r = r;

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
    endwin();

    // print explanation
    printf("%s\n", explanation);

    // terminate message_loop
    return true;
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
    int p = display->p;
    int r = display->r;
    char letter = display->player_letter;

    // display the status line
    // spectator
    if (letter == '_' && p == 0)
    {
        printw("Spectator: %d nuggets unclaimed. ", r);
    }
    // player
    else
    {
        printw("Player %c has %d nuggets (%d nuggets unclaimed). ", letter, p, r);
    }

    // display the mapstring
    mvprintw(getcury(stdscr) + 1, 0, display->mapstring);

    // render the full display
    refresh();
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
    display->server_address = address;

    // return struct
    return display;
}

char get_random_keystroke()
{
    // define an array of valid move keystrokes
    char move_keystrokes[] = {'y', 'k', 'u', 'h', 'l', 'b', 'j', 'n'};

    // expand this list to include sprint keystrokes
    int num_move_keystrokes = sizeof(move_keystrokes) / sizeof(move_keystrokes[0]);
    char all_keystrokes[2 * num_move_keystrokes];
    for (int i = 0; i < num_move_keystrokes; i++)
    {
        all_keystrokes[i] = move_keystrokes[i];
        all_keystrokes[i + num_move_keystrokes] = toupper(move_keystrokes[i]);
    }

    // randomly generate keystroke from valid list
    int idx = rand() % (2 * num_move_keystrokes);
    return all_keystrokes[idx];
}

void *run_bot(void *arg)
{
    display_t *display = (display_t *)arg;
    while (true)
    {
        // generate random keystroke
        char keystroke = get_random_keystroke();

        // send keystroke to server
        addr_t to = display->server_address;
        send_key(to, keystroke);

        // wait before sending next keystroke
        usleep(100000); 
    }
    return NULL;
}
