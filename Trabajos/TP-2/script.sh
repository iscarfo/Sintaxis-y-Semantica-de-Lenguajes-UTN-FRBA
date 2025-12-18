#!/bin/bash

#Comandos
#Punto 1:
#A-
sed 's/\./\.\n/g' breve_historia.txt > breve_historia_mod.txt
#B-
sed -i  '/^ $/d' breve_historia.txt
#C-
sed 's/\./\.\n/g' breve_historia.txt | sed -i  '/^ $/d' > breve_historia_2.txt
#D-
grep -i -o -n '[^.]*independencia[^.]*\.' breve_historia.txt
#E-
grep 'El\b[^.]*\.' breve_historia.txt 
#F-
grep -ic 'peronismo' breve_historia.txt
#G-
 grep -c '\.[^.]*Sarmiento[^.]*\.' breve_historia.txt | grep -c 'Rosas'
#H-
grep -E '(\.|^)[^.]*18[0-9][0-9][^.]*(\.|$)' breve_historia.txt
#I-
sed 's/^[a-zA-Z]*\b//g' breve_historia.txt
#J-
ls *.txt