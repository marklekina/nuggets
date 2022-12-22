/*
 * server.c - server program for the Nuggets game
 *
 * see server.h for more information.
 *
 * Mark Lekina Rorat, December 2022
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <assert.h>
 #include "file.h"

 int
 main(int argc, char const *argv[]) {
   // validate parameters
   if (argc < 2 || argc > 3) {
     printf("usage: %s mapfile [seed]\n", argv[0]);
     return(0);
   }

   // set seed for random behavior
   int seed = getpid();
   if (argc == 3) {
     seed = atoi(argv[2]);
   }
   srand(seed);

   // read map string into the game
   FILE* fp = fopen(argv[1], "r");
   assert(fp);
   fclose(fp);

   printf("Seed: %d\n", seed);
   printf("Mapfile: %s\n", argv[1]);

   // return successfully
   return 0;
 }
