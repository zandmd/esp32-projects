#!/usr/bin/env python
import os
import sys

if __name__ == "__main__":
    if not os.path.exists("wifi.cpp"):
        print("Configuring Wi-Fi network...")
        print("Enter SSID:", flush=True)
        ssid = sys.stdin.readline().strip()
        print("Enter password:", flush=True)
        password = sys.stdin.readline().strip()
        with open("wifi.cpp", "x") as f:
            f.write(f"""
#include <zandmd/secrets/wifi.hpp>

using namespace zandmd::secrets;

const char *const wifi::ssid = "{ssid}";
const char *const wifi::password = "{password}";
"""[1:])
