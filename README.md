# Projekt_Cpp

>> Uruchamianie z pliku .exe

Należy pobrać wszystkie pliki z github i re rozpakować. Powinny znajdować się w tym samym folderze.
Aby włączyć symulację należy wejść w ProjektCpp -> x64 -> Debug 

Znajduje się tam plik ProjektCpp.exe
Po uruchomieniu go włączy się symulacja



>> Otwieranie projektu w Visual Studio i podłączanie bibliotek:

Uruchamianie w Visual Studio 2022.
Kliknij "Otwórz projekt lub rozwiązanie".
Wskaż plik .sln w folderze ProjektCpp i kliknij "Otwórz".
Podłączenie bibliotek SDL2 (jeśli są w tym samym folderze co projekt)

Wejdź w Project > Properties.
Przejdź do C/C++ > General > Additional Include Directories i dodaj:

SDL2-devel-2.30.8-VC\include

SDL2_ttf-2.24.0\include

Przejdź do Linker > General > Additional Library Directories i dodaj:

SDL2-devel-2.30.8-VC\lib\x64

SDL2_ttf-2.24.0\lib\x64

Przejdź do Linker > Input > Additional Dependencies i dopisz:


SDL2.lib

SDL2_ttf.lib

I teraz program powinien się kompilować.
