[CmdletBinding()]
param(
    [switch]$Check
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Resolve-ClangFormat {
    if (-not [string]::IsNullOrWhiteSpace($env:CLANG_FORMAT)) {
        $configured = Get-Command $env:CLANG_FORMAT -CommandType Application -ErrorAction SilentlyContinue
        if ($null -ne $configured) {
            return $configured.Source
        }
    }

    $command = Get-Command clang-format -CommandType Application -ErrorAction SilentlyContinue
    if ($null -ne $command) {
        return $command.Source
    }

    $extensionsDirectory = Join-Path $env:USERPROFILE ".vscode/extensions"
    if (Test-Path $extensionsDirectory) {
        $bundledFormatter = Get-ChildItem $extensionsDirectory -Filter "clang-format.exe" -File -Recurse -ErrorAction SilentlyContinue |
            Where-Object { $_.FullName -match "[\\/]ms-vscode\.cpptools-[^\\/]+[\\/]LLVM[\\/]bin[\\/]clang-format\.exe$" } |
            Sort-Object LastWriteTime -Descending |
            Select-Object -First 1

        if ($null -ne $bundledFormatter) {
            return $bundledFormatter.FullName
        }
    }

    throw "clang-format was not found. Install LLVM or set the CLANG_FORMAT environment variable."
}

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$formatter = Resolve-ClangFormat

Push-Location $repositoryRoot
try {
    $sourceFiles = @(
        & git ls-files --cached --others --exclude-standard -- `
            "*.c" "*.h" "*.cc" "*.cpp" "*.cxx" "*.hpp" "*.hh" "*.hxx"
    )

    if ($LASTEXITCODE -ne 0) {
        throw "Unable to enumerate source files with Git."
    }

    $sourceFiles = @(
        $sourceFiles | Where-Object {
            $_ -and
            (Test-Path -LiteralPath $_ -PathType Leaf) -and
            $_ -notmatch "(^|/)(Drivers|build|cmake|ThirdParty)(/|$)"
        }
    )

    if ($sourceFiles.Count -eq 0) {
        Write-Host "No project source files found."
        return
    }

    $formatArguments = @("--style=file")
    if ($Check) {
        $formatArguments += @("--dry-run", "--Werror")
    }
    else {
        $formatArguments += "-i"
    }

    foreach ($sourceFile in $sourceFiles) {
        & $formatter @formatArguments $sourceFile
        if ($LASTEXITCODE -ne 0) {
            throw "clang-format failed for '$sourceFile'."
        }
    }

    $action = if ($Check) { "Checked" } else { "Formatted" }
    Write-Host "$action $($sourceFiles.Count) project source file(s) with $formatter."
}
finally {
    Pop-Location
}
