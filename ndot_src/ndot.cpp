//
//  main.cpp
//  ndot
//
//  Created by Janis on 25.11.15.
//  Copyright (c) 2015 Janis. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <string>

#include "docopt.h"
#include "protocol.h"

static const char USAGE[] =
    R"(nDot.

    Usage:
        ndot test <port>
        ndot type (read | set <type>) <port>
        ndot mode (read | set <mode>) <port>
        ndot (-h | --help)
        ndot --version

    Options:
        -h --help   Show this screen.
        --version   Show version.
)";


int main(int argc, const char * argv[])
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                    { argv + 1, argv + argc },
                                                    true,
                                                    "nDot 0.1");

    bool arg_all = args.find("all")->second.asBool();
    bool arg_read = args.find("read")->second.asBool();
    bool arg_set = args.find("set")->second.asBool();

    bool arg_test = args.find("test")->second.asBool();
    bool arg_type = args.find("type")->second.asBool();
    bool arg_mode = args.find("mode")->second.asBool();
    bool arg_flipdot = args.find("flipdot")->second.asBool();


    if(arg_test || arg_type || arg_mode)
    {
        std::cout << "Connecting... ";
        protocol::Protocol prot(args.find("<port>")->second.asString());

        if(arg_type)
        {
            protocol::msgFpdTyp_t fpdTyp;
            if(arg_read)
            {
                if(prot.sendMsgPoll(&fpdTyp))
                {
                    std::cout << "Read Flipdot type successful!" << std::endl;
                    std::cout << (int)fpdTyp.type << std::endl;
                }
                else
                {
                    std::cout << "Read Flipdot type failed..." << std::endl;
                }
            }

            if(arg_set)
            {
                fpdTyp.type = (uint8_t)args.find("<type>")->second.asLong();
                if(prot.sendMsg(fpdTyp))
                {
                    std::cout << "Set Flipdot type successful!" << std::endl;
                }
                else
                {
                    std::cout << "Set Flipdot type failed..." << std::endl;
                }
            }
        }

        if(arg_test)
        {
            protocol::msgFpdTst_t fpdTst;
            fpdTst.test = 42;
            if(prot.sendMsg(fpdTst))
            {
                std::cout << "Flipdot test successful!" << std::endl;
            }
            else
            {
                std::cout << "Flipdot test failed..." << std::endl;
            }

            protocol::msgFpdR12_t fpdR12;
            for (uint8_t i = 0; i < 84; ++i)
            {
                fpdR12.reg[i] = i;
            }
            if(prot.sendMsg(fpdR12))
            {
                std::cout << "Flipdot test raw successful " << std::endl;
            }
            else
            {
                std::cout << "Flipdot test raw failed..." << std::endl;
            }   
        }
    }

    return 0;
}
