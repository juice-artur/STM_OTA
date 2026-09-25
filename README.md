# STM OTA

## ClangFormat

The repository uses the shared [`.clang-format`](.clang-format) configuration. VS Code is configured to use it through the Microsoft C/C++ extension for both STM32 projects.

Format project-owned C/C++ files from PowerShell:

```powershell
.\scripts\format.ps1
```

Check formatting without changing files:

```powershell
.\scripts\format.ps1 -Check
```

The script excludes generated/vendor paths (`Drivers`, `build`, and `cmake`). It uses `clang-format` from `PATH`, the binary bundled with `ms-vscode.cpptools`, or the executable specified by the `CLANG_FORMAT` environment variable.
