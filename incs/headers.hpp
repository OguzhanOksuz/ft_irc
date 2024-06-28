#ifndef HEADERS_HPP
#define HEADERS_HPP

#define MAX 42
#define BUFFER_SIZE 128

#define	ERR_NEEDMOREPARAMS 461
#define	ERR_ALREADYREGISTERED 462
#define	ERR_PASSWDMISMATCH 464

//NICK
#define	ERR_NONICKNAMEGIVEN 431
#define	ERR_NICKNAMEINUSE 433
#define	ERR_NICKCOLLISION 436
#define	ERR_UNAVAILRESOURCE 437
#define	ERR_RESTRICTED 475

//JOIN
#define	ERR_BANNEDFROMCHAN 474
#define	ERR_BADCHANNELKEY
#define	ERR_TOOMANYCHANNELS 405

//PART
#define	ERR_NOSUCHCHANNEL 403
#define	ERR_NOTONCHANNEL 442

//TOPIC
#define	ERR_CHANOPRIVSNEEDE 482

//KCIK
#define	ERR_USERNOTINCHANNEL 441

//PRIVMSG
#define	ERR_NORECIPIENT 411
#define	ERR_NOTEXTTOSEND 412
#define	ERR_CANNOTSENDTOCHAN 404
#define	ERR_NOTOPLEVEL 413
#define	ERR_WILDTOPLEVEL 414
#define	ERR_TOOMANYTARGETS 407
#define	ERR_NOSUCHNICK 401

#define	ERR_UNKNOWNCOMMAND 421
#define	ERR_NOTREGISTERED 451
#define	ERR_NOPRIVILEGES 481

#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <sstream>
#include <csignal>
#include <string>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#endif
