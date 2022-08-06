#include <iostream>
#include <sched.h>
#include <csignal>
#include <sys/wait.h>
#include <sys/mount.h>

#include "creat_cgroup.h"

char * stack_memory();

template <typename... P>
int run(P... params){
    char *args[] = {(char *)params..., (char *) 0};
    return execvp(args[0],args);
}

template <typename Function>
void clone_process(Function&& function, int flags){
    auto pid = clone(function, stack_memory(), flags, 0);
    wait(nullptr);
}

void setup_env(){
    clearenv();
    setenv("TERM","xterm-256color", 0);
    setenv("PATH","/bin/:/sbin/:usr/bin:/usr/sbin", 0);
}
void setup_root(const char *folder){
    chroot(folder);
    chdir("/");
}

int jail(void* argv){

    printf("Hello world (child) %d!\n", getpid());
    limitProcessCreation();

    printf("limit done\n");
    setup_env();
    setup_root("./root");
    mount("proc", "/proc", "proc", 0, 0);

    auto runThis = [](void* argv) -> int {return run("bin/sh"); };

    clone_process(runThis, SIGCHLD);

    umount("/proc");

    return EXIT_SUCCESS;
}

char * stack_memory(){
    const int stackSize = 65536;
    auto *stack = new (std::nothrow) char[stackSize];

    if (stack == nullptr){
        printf("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    return stack + stackSize;
}

int main(int argc, char** argv){
    printf("Hello world (parent) %d!\n", getpid());

    clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);

    return EXIT_SUCCESS;
}

