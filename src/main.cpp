//
//  main.cpp
//  nchron
//
//  Created by Janis on 26.03.15.
//  Copyright (c) 2015 Janis. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <ctime>

#include "docopt.h"
#include "protocol.h"

static const char USAGE[] =
    R"(nChron.

    Usage:
        nchron (sync | read) all <port>
        nchron (sync | read | set <timestamp>) rtc <port>
        nchron (sync | read | set <timezone>) tz  <port>
        nchron (-h | --help)
        nchron --version

    Options:
        -h --help   Show this screen.
        --version   Show version.
)";


int main(int argc, const char * argv[])
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                    { argv + 1, argv + argc },
                                                    true,
                                                    "nChron 0.1");

    bool arg_all = args.find("all")->second.asBool();
    bool arg_rtc = args.find("rtc")->second.asBool();
    bool arg_tz = args.find("tz")->second.asBool();
    bool arg_sync = args.find("sync")->second.asBool();
    bool arg_read = args.find("read")->second.asBool();
    bool arg_set = args.find("set")->second.asBool();


    if(arg_rtc || arg_all)
    {
        std::cout << "Connecting... ";
        protocol::Protocol prot(args.find("<port>")->second.asString());

        std::time_t t = time(0);
        struct tm *now = gmtime(&t);

        std::cout << "Host time: ";
        std::cout << now->tm_year + 1900 
        	<< "-" << std::setfill('0') << std::setw(2) << now->tm_mon + 1 
        	<< "-" << std::setfill('0') << std::setw(2) << now->tm_mday 
        	<< " " << std::setfill('0') << std::setw(2) << now->tm_hour 
        	<< ":" << std::setfill('0') << std::setw(2) << now->tm_min 
        	<< ":" << std::setfill('0') << std::setw(2) << now->tm_sec 
        	<< " " << std::setfill('0') << std::setw(2) << now->tm_zone
        	<< std::endl;

        if(arg_sync)
        {
            protocol::msgTimUtc_t timUtc;
            timUtc.year = now->tm_year + 1900;
            timUtc.month = now->tm_mon + 1;
            timUtc.day = now->tm_mday;
            timUtc.hour = now->tm_hour;
            timUtc.min = now->tm_min;
            timUtc.sec = now->tm_sec;
            timUtc.valid = true;

            if(prot.sendMsg(timUtc))
            {
                std::cout << "Sync RTC successful!" << std::endl;
            }
            else
            {
                std::cout << "Sync RTC failed..." << std::endl;
            }        
        }

        if(arg_read)
        {
            protocol::msgTimUtc_t timUtc;
            if(prot.sendMsgPoll(&timUtc))
            {
                std::cout << "Read RTC successful!" << std::endl;
                std::cout << "nChron time: ";
                std::cout << std::dec << (int)timUtc.year 
                	<< "-" << std::setfill('0') << std::setw(2) << (int)timUtc.month 
                	<< "-" << std::setfill('0') << std::setw(2) << (int)timUtc.day 
                	<< " " << std::setfill('0') << std::setw(2) << (int)timUtc.hour 
                	<< ":" << std::setfill('0') << std::setw(2) << (int)timUtc.min 
                	<< ":" << std::setfill('0') << std::setw(2) << (int)timUtc.sec 
                	<< std::endl;
            }
            else
            {
                std::cout << "Read RTC failed..." << std::endl;
            }
        }

        if(arg_set)
        {
            uint32_t tsul = std::stoul(args.find("<timestamp>")->second.asString());
            std::time_t ts = tsul;
            struct tm *now = gmtime(&t);

            protocol::msgTimUtc_t timUtc;
            timUtc.year = now->tm_year + 1900;
            timUtc.month = now->tm_mon + 1;
            timUtc.day = now->tm_mday;
            timUtc.hour = now->tm_hour;
            timUtc.min = now->tm_min;
            timUtc.sec = now->tm_sec;
            timUtc.valid = true;
            if(prot.sendMsg(timUtc))
            {
                std::cout << "Set RTC to timestamp: " << tsul << " successful!" << std::endl;
            }
            else
            {
                std::cout << "Set RTC failed..." << std::endl;
            }     
        }
    }

    return 0;
}
