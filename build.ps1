param(
    [switch]$development=$false
)

if (Test-Path ./build -PathType Container) {
    rm -r -force ./build/;
}

mkdir ./build; cd ./build;

$exec = 'cmake -G "Visual Studio 15 2017 Win64"'

if ($development) {
    $exec = "$exec -DDEVELOPMENT=ON"
}

$exec = "$exec ..;"

Invoke-Expression $exec

explorer .; cd ..;