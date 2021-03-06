Cargol
======

A short and sequential molecular dynamics code

.. figure:: logo.png
    :width: 400px
    :align: center

This molecular simulation code comes without any guaranties. The main
goal of this code is to quickly take a tour of a molecular dynamics program and
see the diffrent parts it contains. For example it could be used for educational purpose 
either in chemical physics or in order to develop programming skills.

This code is under GPL Licence .

contenu
-------

Source and header files are respectivelly in directories ``src`` and
``include``. The ``doc`` directroy contains a partial documentation. The
``inputs`` directory contains examples of input files.

Finally, the ``cargol_struct`` directroy contains a more object oriented
version of the code. In this version, data are store in new defined C-types.
Nevertheless, this version was not used a lot and may contains critical bugs.
Thus, if you try to use it, take care about the results and compare with the
standard version.

installation
------------

L'archive contient les sources dans le dossier src/. Les fichiers
d'entête sont dans le dossier include/. A la fin de la
compilation les fichiers objets seront dans le dossier obj et 
l'exécutable sera à la racine du code. Voici la procédure :

  1.  unzip cargol.zip
  2.  cd cargol/
  3.  make install (ou make)
   
Dans le dossier cargol_struct vous trouverez d'autres sources de
cargol qui ont été écrites dans un esprit différents. Dans src, le 
code utilise des variables globales, déclarées dans globales.h 
et accessibles à toutes les fonctions car le fichier extern.h est
inclu dans tous les fichiers. Dans cargol_struct, les variables
globales sont regroupées dans des structures (structures.h) et les
structures sont passées aux fonctions en paramètre. Les deux codes
devraient donner les mêmes résultats, seulent l'écriture est 
différente.

utilisation
-----------

Le dossier ``inputs/`` contient des exemples de fichiers input. 
Le fichier input_all contient tous les mots clefs. La
documentation est encore très sommaire.

Pour utiliser le programme :

cargol -help 
    affiche une aide sommaire

cargol input
    exécute le calcul demandé dans le fichier input. Le nom 
    du fichier d'input n'a pas d'importance.

Bonne simulation !

Germain


