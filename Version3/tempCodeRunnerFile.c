struct sigaction s;
    s.sa_handler = signal_handler;
    s.sa_flags=0;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &s, NULL); // change here to your signal
    sigaction(SIGQUIT, &s, NULL); // add how many you want
    sigaction(SIGTSTP, &s, NULL); // add how many you want
    printf("Haiku server, press Ctrl+Z to kill this process.\n");
    while (1){

    }