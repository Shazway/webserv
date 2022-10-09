#include "Colors.hpp"
#include "HttpRequest.hpp"
#include "Parsing.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include "BookMark.hpp"
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 4096

//pour start

//extraire les donnees pertinentes de Server pour les mettre dans sockaddr
void	init_addr(struct sockaddr_in& addr, Server server)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server.getPort());
	inet_aton(server.getIp().c_str(), &(addr.sin_addr));
}

void	initFdSet(fd_set &fdSet, std::vector<Socket> *sockets, std::list<Socket> &fds)
{
	FD_ZERO(&fdSet);
	if (sockets)
		for (std::vector<Socket>::iterator it = sockets->begin(); it != sockets->end(); it++)
		{
			FD_SET((*it).getFd(), &fdSet);
		}
	for (std::list<Socket>::iterator it = fds.begin(); it != fds.end(); it++)
		FD_SET((*it).getFd(), &fdSet);
}

void	answer_get(HttpRequest request, std::map<int, std::string>& answers, std::map<int, BookMark>& bookmarks, int i)
{
	char								buf[BUFFER_SIZE];
	std::map<int, BookMark>::iterator	it;
	BookMark							tmp;
	std::ifstream						file;

	it = bookmarks.find(i);
	if (it == bookmarks.end())
	{
		tmp.setFd(open(request.getPath().c_str(), O_RDONLY));
		if (tmp.getFd() == -1)
			return ; //throw un truc
	}
	else
		tmp.setFd(bookmarks[i].getFd());
	tmp.setRet(read(tmp.getFd(), buf, BUFFER_SIZE));
	if (tmp.getRet() == BUFFER_SIZE)
	{
		if (it != bookmarks.end())
			bookmarks.erase(it);
		bookmarks[i] = tmp;
	}
	answers[i] = "HTTP 1.1";
	answers[i] += "200 OK\n";
	answers[i] += "Content-Length: ";
	answers[i] += tmp.getRet();
	answers[i] += "\n\n";
	answers[i] += buf;
	if (tmp.getRet() < BUFFER_SIZE)
		close(tmp.getFd());
}

void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers, std::map<int, BookMark>& bookmarks)
{
	(void)answers;
	(void)bookmarks;
	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((*it).second.getMethod() == "GET")
			std::cout << "GET" << std::endl;
			//answer_get((*it), answers, bookmarks, (*it).first);
//		if ((*it).second.getMethod() == "POST")
			//answer_post((*it), answers);
//		if ((*it).second.getMethod() == "DELETE")
			//answer_delete((*it), answers);
	}
	requests.clear();
}

int	running(std::vector<Server> &servers)
{
	int					num = 0; //plus grand fd en utilisation + 1;
	std::vector<Socket>	sockets;
	std::list<Socket>		fds;
	Socket				socketMaker;
	fd_set				fdread;
	fd_set				fdwrite;
	std::map<int, HttpRequest>	requests;
	std::map<int, std::string>	answers;
	std::map<int, BookMark>		bookmarks;
	std::string					buffer;

	int opt; //ne sert a rien, mais calme le proto de setsockopt
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		init_addr(socketMaker.getAddr(), (*it));
		socketMaker.setFd(socket(PF_INET, SOCK_STREAM, 0));
		if (socketMaker.getFd() >= num)
			num = socketMaker.getFd() + 1;
		socketMaker.setServ((*it));
		bind(socketMaker.getFd(), (const struct sockaddr*)(&(socketMaker.getAddr())), sizeof(socketMaker.getAddr()));
		listen(socketMaker.getFd(), 32);
		setsockopt(socketMaker.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		sockets.push_back(socketMaker);
	}
	while (true)
	{
		initFdSet(fdread, &sockets, fds);
		initFdSet(fdwrite, NULL, fds);
		if (select(num, &fdread, &fdwrite, 0, 0) < 1)
			continue ;
		//for des sockets
		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
		{
			if (FD_ISSET((*it).getFd(), fdread))
			{
				socketMaker.setFd(accept((*it).getFd(), (*it).getAddr(), sizeof(struct sockaddr_in)));
				if (socketMaker.getFd() >= num)
					num = socketMaker.getFd() + 1;
				socketMaker.setServ((*it).getServ());
				fcntl(socketMaker.getFd(), O_NONBLOCK);
				//faire un truc avec socketMaker._addr
				fds.push_back(socketMaker);
				if (socketMaker.getFd() >= num)
					num = socketMaker.getFd() + 1;
			}
		}
		//for des fds
		for (std::list<Socket>::iterator it = fds.begin(); it != fds.end(); it++)
		{
			HttpRequest tmp_request((*it).getServ());
			if (FD_ISSET((*it).getS(), fdread))
			{
				if (recv((*it).getS(), buffer, 2048, 0) <= 0)
				{
					close ((*it).getS());
					fds.erase((*it).getS());
				}
				else if (!buffer.empty())
				{
					do
					{
						try
							ParsingRequest(tmp_request, buffer);
						catch(const std::exception& e)
							std::cerr << RED << e.code() << " " << e.what() << END << std::endl;
					}
					while (tmp_request.getPartiallyCompleted());
					requests[(*it).getS()] = tmp_request;
				}
			}
			if (FD_ISSET((*it).getS(), fdwrite))
				if (!answers.empty())
					send((*it).getS(), answers[(*it).getS()], answers[(*it).getS()].size(), 0);
		}
		answers_gen(requests, answers, bookmarks);
		//ici, générer des reponses
	}
	return 0;
}

int	main(int ac, char** av)
{
	std::vector<Server> servers;
	

	if (ac == 1)
		servers = parse_config("config/default.conf");
	else if (ac == 2)		
		servers = parse_config(av[1]);
	else
	{
		std::cout << RED << "ERROR" << END << std::endl;
		std::cout << "Too many arguments" << std::endl;
		return (1);
	}
	return (running(servers));
}