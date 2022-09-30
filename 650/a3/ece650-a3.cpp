#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int readin(){
        while (!std::cin.eof()) {
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                break;
            }
            std::cout << line << std::endl;
            std::cout.flush();
       }
       return 0;
}

int main (int argc, char **argv) {
    std::vector<pid_t> childprocids; //this will store all pids of kids
    
    //we need three pipes 
    int ass1_pipe[2];  //pipe file descriptors
    pipe(ass1_pipe);
    int ass2_pipe[2];
    pipe(ass2_pipe);
    int ass3_pipe[2];
    pipe(ass3_pipe);


    //fork the rgen child process
    pid_t rgenass = fork();
    if (rgenass == 0) { //if this is child process - map outputs to pipe of assignmet one
        dup2(ass1_pipe[1], STDOUT_FILENO);
        close(ass1_pipe[0]);
        close(ass1_pipe[1]);
        int rgen = execv("rgen", argv); //run (replace) rgen in current child
        return rgen;
    }
    else if (rgenass == -1) { //if pid =-1 - fork fail - throw error
        std::cerr << "Error: at rgen fork" << std::endl;
        return 1;
    }
    childprocids.push_back(rgenass); 


    pid_t assone = fork(); 
    if (assone == 0) {
        dup2(ass1_pipe[0], STDIN_FILENO); //assignment on input to assingment 1 pipe
        close(ass1_pipe[0]);
        close(ass1_pipe[1]);

        dup2(ass2_pipe[1], STDOUT_FILENO);
        close(ass2_pipe[0]);
        close(ass2_pipe[1]);
        char *argv1[3];
        argv1[0] = (char *) "python3";
        argv1[1] = (char *) "ece650-a1.py";
        argv1[2] = nullptr;
        return execvp("python3", argv1);
    }
    else if (assone == -1) {
        std::cerr << "Error: at assignment one fork" << std::endl;
        return 1;
    }
    childprocids.push_back(assone);



    pid_t asstwo = fork();
    if (asstwo == 0) {
        dup2(ass2_pipe[0], STDIN_FILENO);
        close(ass2_pipe[0]);
        close(ass2_pipe[1]);
        int a2 = execv("ece650-a2", argv);
        return a2;
    }
    else if (asstwo == -1) {
        std::cerr << "Error: at assignment two fork" << std::endl;
        return 1;
    }
    childprocids.push_back(asstwo);



    pid_t ssin = fork();  // forking a process to read in s statement from command line and input to assignment 2 pipe
    if (ssin == 0) {
        dup2(ass2_pipe[1], STDOUT_FILENO);
        close(ass2_pipe[0]);
        close(ass2_pipe[1]);
        return (readin());
    }
    else if (ssin == 0) {
        std::cerr << "Error: Fork Failure" << std::endl;
        return 1;
    }
    childprocids.push_back(ssin);


    wait(nullptr); //this will block parent till ANY child throws exit(1)

    //this will ensure child proccesses are terminated before main program end
    for (pid_t k : childprocids) { 
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }
    
    return 0;
}