
<a name="readme-top"></a>


<!-- ABOUT THE PROJECT -->
## À Propos

Ce répertoire propose un outil logiciel dans le cadre du projet de Navigation sous-voile autonome.

L'objectif est de permettre l'aquisition de données intéressantes à partir d'un capteur Mti-680G de la gamme Xsens de Movella : 

* [https://www.movella.com/products/xsens](https://www.movella.com/products/xsens).

Il repose sur l'utilisation de XDA (Xsens Device API) qui est libre de droit et dont la documentation est disponible suite à l'installation de [la suite logicielle du capteur](https://www.movella.com/support/software-documentation)

L'outil a été développé pour l'utilisation du modèle Mti-680G mais peut être modifié facilement pour tout autre modèle de la gamme ou pour d'autres applications.

## Bibliothèques

* Xsens Device API : Bibliothèque C++ pour utilisation des capteurs de la gamme Xsens
* [libxlsxwriter.h](https://github.com/jmcnamara/libxlsxwriter) : Bibliothèque C pour le traitement de fichiers Excel

## INSTALLATION
### Compilation

L'outil est pour l'instant disponible sous systèmes Windows 64 bits et Linux ARM

#### Options de compilation

* Pour Windows 64 bits : compilation via CMake et compilateur MSVC 2019 minimum (sources XDA --> dossier "include")
* Pour Linux ARM :       compilation via MakeFile et compilateur GCC 5.4.0 minimum (sources XDA --> dossier "xspublic")

Ces deux options requièrent C++ version 11 au minimum.

#### Version Linux

Pour utiliser l'outil sur OS Linux, suivre les consignes de compilation correspondantes.
ATTENTION : L'outil est conçu pour générer un document au format Excel xlsx en sortie. Il faudra donc modifier le source "MtiDataValues.cpp" pour l'exporter vers un format utilisable par Linux (i.e. csv).

### Installation Rapide

Pour une utilisation rapide sans compilation, un exécutable est présent dans le répertoire "Setup".
Il permet d'installer directement le logiciel sous windows via un installeur.

