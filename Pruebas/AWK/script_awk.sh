#!/bin/bash
############################################################################
#
# Nombre      : AWK
# Autor       : Juan Lautaro Silva Ortíz, Rodrigo Alexis Venegas Muñoz
# Fecha       : 03-02-2012
# Descripción : Esta prueba consiste en ejecutar un ciclo for anidado calculando (100000)²
#		de sólo contar dígitos. El script es lanzado 10 veces a la vez para probar
#		poder de cálculo de cluster MOSIX.
#
# Require     : awk,gawk
# Ejecutar    : ./start_awk.sh
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



date; awk 'BEGIN {for(i=0;i<100000;i++)for(j=0;j<100000;j++);}' ;date




