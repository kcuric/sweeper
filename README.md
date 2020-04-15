# Sweeper (envelope-filter/autowah)

## 📕 Description
🇬🇧 ENG:
"Sweeper" is an autowah/envelope filter VST plugin implemented in C++ using the famous JUCE framework. I've started this project so I could experiment with the IIR Filter using the new DSP Module. In the future I would like to add sync tempo to host functionality - that would be a neat experiment and a lovely addition.

🇭🇷 CRO:
"Sweeper" (još nema ime :( ) je autowah/envelope filter VST plugin implementiran u C++ koristeći se slavnim JUCE razvojnim okvirom. Projekt sam pokrenuo kako bih mogao eksperimentirati s IIR Filterom koristeći novi DSP modul. U budućnosti namjeravam dodati još i funkcionalnost usklađivanja tempa filtera s programom domaćinom.

## 🖥️ User Interface
### Design
![Capture](https://user-images.githubusercontent.com/29998991/79339962-f6400980-7f29-11ea-97a9-bc1230c9ad73.PNG)  
*(1. VST Design)*

## How to run this? 🏁
1. Download Procjucer (JUCE) -> https://juce.com/discover/projucer
2. Download Steinberg VST 3 Audio Plug-Ins SDK -> https://www.steinberg.net/vst3sdk
3. Open "Sweeper.jucer" with Projucer.
4. Set the Procjucer VST3 SDK path to match the downloaded Steinberg VST3_SDK folder path and set the path to the JUCE modules.
5. Open the source code with Visual Studio or some other IDE (depends on what you are using), this can be done through Projucer.
6. Include the downloaded Steinberg VST directories to your include directories (so the compiler knows they exist).
7. Build the project.
8. Copy the Sweeper.vst3 to your VST3 directory.
9. Enjoy the sweeping/filtering/autowah madness! :)

## Contributors 👥
Code: Krešimir Ćurić (kcuric)  
Background image: Karlo Bonetić (kbonetic)
