//
//  nxie.cpp
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
#include "protocol_structures.h"

static const char USAGE[] =
    R"(nXie.

    Usage:
        nxie test <port>
        nxie type (read | set <type>) <port>
        nxie mode (read | set <mode>) <port>
        nxie (-h | --help)
        nxie --version

    Options:
        -h --help   Show this screen.
        --version   Show version.
)";

int main(int argc, const char * argv[])
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                    { argv + 1, argv + argc },
                                                    true,
                                                    "nXie 0.1");

    bool arg_read = args.find("read")->second.asBool();
    bool arg_set = args.find("set")->second.asBool();

    bool arg_test = args.find("test")->second.asBool();
    bool arg_type = args.find("type")->second.asBool();
    bool arg_mode = args.find("mode")->second.asBool();

    if(arg_test || arg_type || arg_mode)
    {
        std::cout << "Connecting... ";
        protocol::Protocol prot(args.find("<port>")->second.asString());

        if(arg_type)
        {
            protocol::msgNixTyp_t nixTyp;
            if(arg_read)
            {
                if(prot.sendMsgPoll(&nixTyp))
                {
                    std::cout << "Read Nixie type successful!" << std::endl;
                    std::cout << (int)nixTyp.type << std::endl;
                }
                else
                {
                    std::cout << "Read Nixie type failed..." << std::endl;
                }
            }

            if(arg_set)
            {
                nixTyp.type = (uint8_t)std::stoul(args.find("<type>")->second.asString());
                if(prot.sendMsg(nixTyp))
                {
                    std::cout << "Set Nixie type successful!" << std::endl;
                }
                else
                {
                    std::cout << "Set Nixie type failed..." << std::endl;
                }
            }
        }

        if(arg_mode)
        {
            protocol::msgNixMod_t nixMod;
            if(arg_read)
            {
                if(prot.sendMsgPoll(&nixMod))
                {
                    std::cout << "Read Nixie mode successful!" << std::endl;
                    std::cout << (int)nixMod.mode << std::endl;
                }
                else
                {
                    std::cout << "Read Nixie mode failed..." << std::endl;
                }
            }

            if(arg_set)
            {
                nixMod.mode = (uint8_t)std::stoul(args.find("<mode>")->second.asString());
                if(prot.sendMsg(nixMod))
                {
                    std::cout << "Set Nixie mode successful!" << std::endl;
                }
                else
                {
                    std::cout << "Set Nixie mode failed..." << std::endl;
                }
            }
        }

        if(arg_test)
        {
            protocol::msgNixTst_t nixTst;
            nixTst.test = 42;
            if(prot.sendMsg(nixTst))
            {
                std::cout << "Nixie test successful!" << std::endl;
            }
            else
            {
                std::cout << "Nixie test failed..." << std::endl;
            }
        }
    }

    return 0;
}
