$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
Set-Location $projectRoot

$pathsToReset = @(
    "data\\books.txt",
    "data\\deleted_books.log",
    "exports\\books_export.csv"
)

foreach ($path in $pathsToReset) {
    if (Test-Path $path) {
        Remove-Item $path -Force
    }
}

New-Item -ItemType Directory -Force "docs\\runs" | Out-Null
New-Item -ItemType Directory -Force "docs\\screenshots" | Out-Null

@'
1
101
Clean Code
Robert C. Martin
2008
Software Engineering
yes
1
102
The Pragmatic Programmer
Andrew Hunt
1999
Programming
no
2
0
'@ | .\build\record_manager.exe | Set-Content "docs\\runs\\run_add_display.txt"

@'
3
101
4
102
The Pragmatic Programmer 20th Anniversary
Andrew Hunt and David Thomas
2019
Programming
yes
3
102
0
'@ | .\build\record_manager.exe | Set-Content "docs\\runs\\run_update_search.txt"

@'
6
2
7
8
5
101
2
0
'@ | .\build\record_manager.exe | Set-Content "docs\\runs\\run_sort_export_dashboard.txt"

.\scripts\render-terminal-screenshot.ps1 -InputFile "docs\\runs\\run_add_display.txt" -OutputFile "docs\\screenshots\\run_add_display.png"
.\scripts\render-terminal-screenshot.ps1 -InputFile "docs\\runs\\run_update_search.txt" -OutputFile "docs\\screenshots\\run_update_search.png"
.\scripts\render-terminal-screenshot.ps1 -InputFile "docs\\runs\\run_sort_export_dashboard.txt" -OutputFile "docs\\screenshots\\run_sort_export_dashboard.png"
