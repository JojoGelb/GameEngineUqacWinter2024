$sources = @("include", "src")
$sources += Get-ChildItem -Path * -Exclude "*.zip" -File | ForEach-Object { $_.Name }
Compress-Archive -Path $sources -DestinationPath "remise.zip" -Force