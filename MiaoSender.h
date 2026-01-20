#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "KyStting.h"


class MiaoSender
{
public:
    MiaoSender() {
        MiaoCode_others = 来人喵码;
        MiaoCode_white = 白屋喵码;
        MiaoCode_boss = 精英Boss喵码;
        Miaocode_huangmen = 符文或黄门喵码;
        send_others = 0;
        send_boss = 0;
        send_white = 0;
        send_huangmen = 0;
        cooldown_others = 0;
        cooldown_boss = 0;
        cooldown_white = 0;
        cooldown_huangmen = 0;
    }

    ~MiaoSender() {

    }

    std::string MiaoCode_others;
    std::string MiaoCode_white;
    std::string MiaoCode_boss;
    std::string Miaocode_huangmen;
    int send_others;
    int send_boss;
    int send_white;
    int send_huangmen;
    unsigned long cooldown_others;
    unsigned long cooldown_boss;
    unsigned long cooldown_white;
    unsigned long cooldown_huangmen;

    void setOthers(int val) {
        send_others = val;
    }

    void setBoss(int val) {
        send_boss = val;
    }

    void setWhite(int val) {
        send_white = val;
    }

    void setHuangmen(int val) {
        send_huangmen = val;
    }
};

int sendMiaoCode(std::string code);
