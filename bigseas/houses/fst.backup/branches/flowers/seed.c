






















/*

 init_deamon ();

*/

/*

 that . hope . thread  =  malloc (  3  *  sizeof ( pthread_t )  );

*/

/*

pthread_create (  & thread [ 1 ], NULL,  gop_dog,  NULL  );

pthread_create (  & thread [ 2 ], NULL,  gop_connectted,  NULL  );

pthread_create (  & thread [ 3 ], NULL,  gop_connector,  NULL  );

*/

/*

 pthread_create (  & thread [ 4 ], NULL,  gop_corde,  NULL  );

*/

/*

char  note_command  [ 128 ]   =   "";

*/

/*

while   (   !  gop_about . flag . start   )

{

gop_wait   (  0.1  );

}

*/

/*

note_save   (   "command",   "command start",   "now"   );

*/

/*

while   (   gop_about . flag . main   )

{

*/

/*

if   (   gop_about . flag . deamon   )

{

gop_wait  (  0.1  );

}

*/

/*

else

{

*/

/*

fgets   (   note_command,   128,   stdin   );

*/

/*

if (

     strcmp   (   note_command,   ""   )    ==    0

   )

{

gop_wait  (  0.1  );

}

*/

/*

else

{

*/

/*

note_command    [   strlen  (  note_command  )  -  1   ]    =    '.';

*/

/*

output_print   (   "string",   "\n"   );

*/

/*

judge_message_command (
                        -1,
                        note_command,
                        note_command
                      );

*/

/*

strcpy   (   note_command,   ""   );

*/

/*

}

*/

/*

}

*/

/*

}

*/

/*

note_save ( "command", "command close", "now" );

*/

