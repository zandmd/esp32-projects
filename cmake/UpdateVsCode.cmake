add_custom_target(
    "vscode" ALL
    COMMAND "${esp_idf_python}" "${esp_idf_path}/../cmake/UpdateVsCode.py"
    WORKING_DIRECTORY "${esp_idf_path}/.."
)
