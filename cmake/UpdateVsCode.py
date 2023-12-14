#!/usr/bin/env python
import json
import os

if __name__ == "__main__":
    with open(os.path.join(".vscode", "c_cpp_properties.json"), "r") as f:
        props = json.load(f)
    configs = {}
    for config in props["configurations"]:
        configs[config["name"]] = config

    for dirname, _, filenames in os.walk("firmware"):
        if "project_description.json" in filenames:
            parts = dirname.split(os.sep)
            if parts[-1] != "bootloader":
                project_name = f"{parts[1]}:{parts[-1]}"
                with open(os.path.join(dirname, "project_description.json"), "r") as f:
                    desc = json.load(f)
                configs[project_name] = {
                    "name": project_name,
                    "includePath": sorted([
                        "${workspaceFolder}/" + os.path.relpath(os.path.join(comp["dir"], dir), os.curdir).replace(os.sep, "/")
                        for comp in desc["build_component_info"].values()
                        for dir in comp["include_dirs"]
                    ] + [f"${{workspaceFolder}}/{dirname.replace(os.sep, '/')}/config"]),
                    "cStandard": "c17",
                    "cppStandard": "c++17",
                    "intelliSenseMode": "gcc-x86",
                    "compilerPath": "${workspaceFolder}/" + os.path.relpath(desc["c_compiler"], os.curdir).replace(os.sep, "/")
                }

    props = {
        "configurations": list(configs.values()),
        "version": 4
    }
    with open(os.path.join(".vscode", "c_cpp_properties.json"), "w") as f:
        json.dump(props, f, indent=4, sort_keys=True)
        f.write("\n")
