param(
    [Parameter(Mandatory = $true)]
    [string]$InputFile,

    [Parameter(Mandatory = $true)]
    [string]$OutputFile
)

$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.Drawing

$content = Get-Content $InputFile -Raw
$lines = $content -split "`r?`n"

if ($lines.Count -eq 0) {
    $lines = @("")
}

$font = New-Object System.Drawing.Font("Consolas", 15, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel)
$padding = 24
$lineSpacing = 8
$background = [System.Drawing.Color]::FromArgb(15, 23, 42)
$panel = [System.Drawing.Color]::FromArgb(30, 41, 59)
$textColor = [System.Drawing.Color]::FromArgb(226, 232, 240)
$accent = [System.Drawing.Color]::FromArgb(56, 189, 248)

$probeBitmap = New-Object System.Drawing.Bitmap(1, 1)
$graphics = [System.Drawing.Graphics]::FromImage($probeBitmap)
$graphics.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit

$maxWidth = 0
foreach ($line in $lines) {
    $size = $graphics.MeasureString($line, $font)
    if ($size.Width -gt $maxWidth) {
        $maxWidth = [math]::Ceiling($size.Width)
    }
}

$lineHeight = [math]::Ceiling($graphics.MeasureString("Ag", $font).Height) + $lineSpacing
$titleBarHeight = 34
$width = [math]::Max(700, $maxWidth + ($padding * 2))
$height = ($lines.Count * $lineHeight) + ($padding * 2) + $titleBarHeight

$graphics.Dispose()
$probeBitmap.Dispose()

$bitmap = New-Object System.Drawing.Bitmap($width, $height)
$canvas = [System.Drawing.Graphics]::FromImage($bitmap)
$canvas.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
$canvas.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
$canvas.Clear($background)

$panelBrush = New-Object System.Drawing.SolidBrush($panel)
$canvas.FillRectangle($panelBrush, 0, 0, $width, $height)

$accentBrush = New-Object System.Drawing.SolidBrush($accent)
$canvas.FillRectangle($accentBrush, 0, 0, $width, 4)

$dotRed = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(248, 113, 113))
$dotYellow = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(250, 204, 21))
$dotGreen = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(74, 222, 128))
$canvas.FillEllipse($dotRed, 16, 11, 12, 12)
$canvas.FillEllipse($dotYellow, 34, 11, 12, 12)
$canvas.FillEllipse($dotGreen, 52, 11, 12, 12)

$textBrush = New-Object System.Drawing.SolidBrush($textColor)
$y = $padding + $titleBarHeight - 10
foreach ($line in $lines) {
    $canvas.DrawString($line, $font, $textBrush, 24, $y)
    $y += $lineHeight
}

$outputDir = Split-Path -Parent $OutputFile
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Force $outputDir | Out-Null
}

$bitmap.Save($OutputFile, [System.Drawing.Imaging.ImageFormat]::Png)

$textBrush.Dispose()
$accentBrush.Dispose()
$panelBrush.Dispose()
$dotRed.Dispose()
$dotYellow.Dispose()
$dotGreen.Dispose()
$canvas.Dispose()
$bitmap.Dispose()
$font.Dispose()
