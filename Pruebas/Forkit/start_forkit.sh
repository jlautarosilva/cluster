#!/bin/bash
############################################################################
# Nombre      : ForKit
# Autor       : Juan Lautaro Silva Ortíz, Rodrigo Alexis Venegas Muñoz
# fecha       : 03-02-2012
# Descripción : Esta prueba realiza un simple cálculo matemático de seno(x)+raiz(x)
#	        por un ciclo de 1000000 veces.El script es lanzado 5 veces a la vez para probar
#		poder de cálculo de cluster MOSIX.
#
# Require     : gcc
# Ejecutar    : gcc -o forkit forkit.c -lm
#
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA
#
############################################################################


# corriendo forkit 5 veces

for ((LOOP=1; LOOP<=5; LOOP++))
 do
 echo "iniciando forkit $LOOP. vez"
mosrun -e ./forkit
#./forkit

done
