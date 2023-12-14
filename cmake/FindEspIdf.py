#!/usr/bin/env python
import ctypes
import os
import serial.tools.list_ports
import sys
import subprocess

if __name__ == "__main__":
    args = [
        sys.executable,
        os.path.join(os.environ["IDF_PATH"], "tools", "idf.py"),
    ] + sys.argv[1:]

    os.environ["PATH"] = os.pathsep.join(
        [
            os.path.join(
                os.environ["IDF_TOOLS_PATH"], "tools", "cmake", "3.24.0", "bin"
            ),
            os.environ["PATH"],
        ]
    )
    proc = subprocess.run(
        [
            sys.executable,
            os.path.join(
                __file__, os.pardir, os.pardir, "esp-idf", "tools", "idf_tools.py"
            ),
            "export",
            "--format",
            "key-value",
        ],
        stdout=subprocess.PIPE,
        check=True,
        text=True,
    )
    for line in proc.stdout.splitlines():
        parts = line.split("=", 2)
        if parts[1].endswith("%PATH%"):
            parts[1] = parts[1][:-6] + os.environ["PATH"]
        os.environ[parts[0]] = parts[1]

    if "monitor" in args:
        ports: list[str] = []
        for port in serial.tools.list_ports.comports():
            if port.hwid.startswith("USB VID:PID=10C4:EA60"):
                ports.append(port.device)

        if len(ports) == 0:
            print(f"No connected devices found!", file=sys.stderr)
            sys.exit(1)

        if len(ports) > 1:
            reprint = True
            while True:
                if reprint:
                    reprint = False
                    print("Select port:")
                    for i, p in enumerate(ports):
                        print(f"  ({i}) {p}")
                if os.name == "nt" and len(ports) <= 10:
                    import msvcrt

                    data = msvcrt.getch()
                else:
                    data = sys.stdin.read(256).strip()
                    reprint = True
                try:
                    n = int(data)
                except ValueError:
                    pass
                else:
                    if n < len(ports):
                        ports = [ports[n]]
                        break
                if reprint:
                    print(f"Invalid selection '{data}'", file=sys.stderr)

        args.extend(["-p", ports[0]])

    if os.name == "nt":
        ctypes.windll.kernel32.SetConsoleMode(ctypes.windll.kernel32.GetStdHandle(-11), 7)

    sys.exit(subprocess.call(args))
