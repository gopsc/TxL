
int information_disk_get() {




    char that_file       [2048];
    char that_buffer     [16];
    char buffer_available[16];
    char buffer_used     [16];



    commandline_get("df", that_file);





    if ( strchr(that_file, '\n') != NULL ) {



        strcpy(
                that_file,
                strchr(that_file, '\n') + 1
              );

       *strchr( that_file, '\n' ) = '\0';



        sscanf(
                that_file, "%s %s %s %s",
                that_buffer,
                buffer_available,
                that_buffer,
                buffer_used
              );


        information_disk.available = atoi(buffer_available);
        information_disk.used      = atoi(buffer_used);}


    return 1;}
