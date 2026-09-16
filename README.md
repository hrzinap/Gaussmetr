# Gaussmetr
## Elixír do škol 2026
Archiv obsahuje potřebné soubory pro výrobu jednoduchého magnetometru (využívajícího Hallův senzor)
Jsou zde dostupné zdrojové kódy desky pro případné zpracování výrobních podkladů (formát KiCad 10)
Je použit klon desky Arduino NANO s procesorem Čínské výroby [LGT8f328p](https://wolles-elektronikkiste.de/lgt8f328p-lqfp32-boards).
K tomuto klonu je nutné stáhnout do vývojového prostředí Arduino IDE definici desky a základních knihoven.
[Více GitHub](https://github.com/dbuezas/lgt8fx)
Použití tohto procesoru umožní využít kombinace D/A a A/D převodníku pro kompenzaci ofsetu Hallova čidla. 
Navrženou desku plošných spojů lze použít i pro běžné Arduino NANO, ale je nutná úprava řídicího SW a bez dalších HW úprav se bude obtížně dosahovat symetrie měření. Nic méně sestava bude pro demonstrační účely použitelná.

<img width="2126" height="1697" alt="pohled_na_gaussmetr" src="https://github.com/user-attachments/assets/0a7238c6-1770-46cd-b306-a3a4476be0d2" />
