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
#include <string>

#include "docopt.h"
#include "protocol.h"

static const char USAGE[] =
    R"(nChron.

    Usage:
        nchron (sync | read) all <port>
        nchron (sync | read | set <timestamp>) rtc  <port>
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
                                                    "nChron 0.2");

    bool arg_all = args.find("all")->second.asBool();
    bool arg_rtc = args.find("rtc")->second.asBool();
    bool arg_tz = args.find("tz")->second.asBool();
    bool arg_sync = args.find("sync")->second.asBool();
    bool arg_read = args.find("read")->second.asBool();
    bool arg_set = args.find("set")->second.asBool();


    if(arg_rtc || arg_tz || arg_all)
    {
        std::cout << "Connecting... ";
        protocol::Protocol prot(args.find("<port>")->second.asString());

        std::time_t t = time(0);
        struct tm *now = gmtime(&t);

        std::cout << "Global time: ";
        std::cout << now->tm_year + 1900 
            << "-" << std::setfill('0') << std::setw(2) << now->tm_mon + 1 
            << "-" << std::setfill('0') << std::setw(2) << now->tm_mday 
            << " " << std::setfill('0') << std::setw(2) << now->tm_hour 
            << ":" << std::setfill('0') << std::setw(2) << now->tm_min 
            << ":" << std::setfill('0') << std::setw(2) << now->tm_sec 
            << " " << std::setfill('0') << std::setw(2) << now->tm_zone
            << std::endl;

        struct tm *local = localtime(&t);

        std::cout << "Local time: ";
        std::cout << local->tm_year + 1900 
            << "-" << std::setfill('0') << std::setw(2) << local->tm_mon + 1 
            << "-" << std::setfill('0') << std::setw(2) << local->tm_mday 
            << " " << std::setfill('0') << std::setw(2) << local->tm_hour 
            << ":" << std::setfill('0') << std::setw(2) << local->tm_min 
            << ":" << std::setfill('0') << std::setw(2) << local->tm_sec 
            << " " << std::setfill('0') << std::setw(2) << local->tm_zone
            << std::endl;

        if(arg_rtc || arg_all)
        {
            if(arg_sync || arg_all)
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

            if(arg_read || arg_all)
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
        
        if(arg_tz || arg_all)
        {
            std::map<std::string, protocol::msgTimStd_t> const stdMap
            {
                // US Pacific Time (Los Angeles)
                { "PST", {-480, 2, 6, 1, 11} },
                // US Mountain Time (Salt Lake City)
                { "MST", {-420, 2, 6, 1, 11} },
                // US Central Time (Chicago)
                { "CST", {-360, 2, 6, 1, 11} },
                // US Eastern Time (New York)
                { "EST", {-300, 2, 6, 1, 11} },
                // Greenwitch Mean Time (London)
                { "GMT", {0, 2, 6, 0, 10} },
                // Central European Time (Berlin, Paris)
                { "CET", {60, 3, 6, 0, 10} },
                // Australia Eastern Time (Sydney, Melbourne)
                { "AEST", {600, 3, 6, 1, 10} }
            };

            std::map<std::string, protocol::msgTimDst_t> const dstMap
            {
                // US Pacific Time (Los Angeles)
                { "PDT", {-420, 2, 6, 2, 3} },
                // US Mountain Time (Salt Lake City)
                { "MDT", {-360, 2, 6, 2, 3} },
                // US Central Time (Chicago)
                { "CDT", {-300, 2, 6, 2, 3} },
                // US Eastern Time (New York)
                { "EDT", {-240, 2, 6, 2, 3} },
                // Greenwitch Mean Time (London)
                { "BST", {60, 1, 6, 0, 3} },
                // Central European Time (Berlin, Paris)
                { "CEST", {120, 2, 6, 0, 3} },
                // Australia Eastern Time (Sydney, Melbourne)
                { "AEDT", {660, 2, 6, 1, 4} }
            };

            std::string tz;

            if(arg_sync || arg_set || arg_all)
            {
                if(arg_sync && arg_all)
                {
                    tz = local->tm_zone;
                }

                if(arg_set)
                {
                    tz = args.find("<timezone>")->second.asString();
                }

                if( (stdMap.find(tz) != stdMap.end()) && (dstMap.find(tz) != dstMap.end()) )
                {
                    std::cout << tz << " found in database" << std::endl;

                    protocol::msgTimStd_t timStd;
                    protocol::msgTimDst_t timDst;
                    
                    if(tz == "PST" || tz == "PDT")
                    {
                        timStd = stdMap.at("PST");
                        timDst = dstMap.at("PDT");
                    }
                    else if(tz == "MST" || tz == "MDT")
                    {
                        timStd = stdMap.at("MST");
                        timDst = dstMap.at("MDT");
                    }
                    else if(tz == "CST" || tz == "CDT")
                    {
                        timStd = stdMap.at("CST");
                        timDst = dstMap.at("CDT");
                    }
                    else if(tz == "EST" || tz == "EDT")
                    {
                        timStd = stdMap.at("EST");
                        timDst = dstMap.at("EDT");
                    }
                    else if(tz == "GMT" || tz == "BST")
                    {
                        timStd = stdMap.at("GMT");
                        timDst = dstMap.at("BST");
                    }
                    else if(tz == "CET" || tz == "CEST")
                    {
                        timStd = stdMap.at("CET");
                        timDst = dstMap.at("CEST");
                    }
                    else if(tz == "AEST" || tz == "AEDT")
                    {
                        timStd = stdMap.at("AEST");
                        timDst = dstMap.at("AEDT");
                    }

                    if(prot.sendMsg(timStd) && prot.sendMsg(timDst))
                    {
                        std::cout << "Sync TZ to " << tz << " successful!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Sync TZ failed..." << std::endl;
                    }    
                }
                else
                {
                    std::cout << tz << " not in database" << std::endl;
                    std::cout << "Sync TZ failed..." << std::endl;
                }
            }

            if(arg_read || arg_all)
            {
                protocol::msgTimStd_t timStd;
                protocol::msgTimDst_t timDst;

                if(prot.sendMsgPoll(&timStd))
                {
                    std::cout << "Read STD successful!" << std::endl;
                    std::cout << "Month: " << timStd.month << ", Week: " << timStd.week << ", Dow: " << timStd.dow << ", Hour: " << timStd.hour << std::endl;
                    std::cout << "Offset: " << timStd.offset << std::endl;
                }
                else
                {
                    std::cout << "Read STD failed..." << std::endl;
                }
                if(prot.sendMsgPoll(&timDst))
                {
                    std::cout << "Read DST successful!" << std::endl;
                    std::cout << "Month: " << timDst.month << ", Week: " << timDst.week << ", Dow: " << timDst.dow << ", Hour: " << timDst.hour << std::endl;
                    std::cout << "Offset: " << timDst.offset << std::endl;
                }
                else
                {
                    std::cout << "Read DST failed..." << std::endl;
                }
            }       
        }
    }

    return 0;
}
