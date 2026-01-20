#include "stdafx.h"
#include "MiaoSender.h"
#include <cpr/cpr.h>

#define SOCKS5_PROXY "socks5h://localhost:1080" 

int sendMiaoCode(std::string code) {

    std::stringstream geturl;
    geturl << "http://miaotixing.com/trigger?id=";
    geturl << code;

    std::string result;

    //cpr::Response r = cpr::Get(cpr::Url{ geturl.str().c_str() }, cpr::Proxies{ {"http", SOCKS5_PROXY} }, cpr::VerifySsl(false));
    cpr::Response r = cpr::Get(cpr::Url{ geturl.str().c_str() }, cpr::VerifySsl(false), cpr::Timeout{ 5000 });
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string

    return r.status_code;

