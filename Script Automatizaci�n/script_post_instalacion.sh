#!/bin/bash
############################################################################
# Nombre      : Script de Post-Instalaxion de nodos MOSIX
# Autor       : Juan Lautaro Silva Ortíz, Rodrigo Alexis Venegas Muñoz
# fecha       : 03-02-2012
# Descripción : Aplica cambios y configuración a los nodos y server
# Require     : archivo LEEME
# Ejecutar    : ./script_post_instalacion
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


if [ $# -lt 1 ]; then
  echo \ "sintaxis: ./asdf.sh numero_nodo "
  echo \ \ "donde numero_nodo es un numero natural ej: 1, 2, 3 ..."
  exit 1
fi

echo "
"
echo \ \ "El Script procedera a realizar los cambios necesarios para integrar un nodo al Cluster"
echo \ \ "Espere unos segundos mientras el Script sigue la ejecucion..."
echo "
"
sleep 10
clear

echo \ \ "Se va a configurar el nodo"$1". Espere un momento porfavor..."
sleep 5 

#edita sudoers
echo "
"
echo \ \ ">> Agregando el nodo a los usuarios ""sudo"""
sleep 3
echo '

nodo'$1'	ALL=(ALL) ALL
blender		ALL=(ALL) ALL 

' >> /etc/sudoers


#edita interfaz de red
echo \ \ ">> Configurando la Interfaz de Red"
sleep 3
cp interfaces /etc/network
echo '

#GeT
iface eth0 inet static
address 192.168.1.10'$1'
netmask 255.255.255.0
gateway 192.168.1.1 

' >> /etc/network/interfaces

#añade archivos MOSIX para el cluster
echo \ \ ">> Copiando archivos MOSIX al nodo"$1""
sleep 3
cp userview.map /etc/mosix
cp mosix.map /etc/mosix

echo \ \ ">> Creando configuracion MOSIX para el nodo"$1""
sleep 3
echo '192.168.1.10'$1'
' >> /etc/mosix/mosip

#instala directorios NAS en el nodo
echo \ \ ">> Instalando NAS Network Attached Storage"
mkdir /media/NAS
sleep 3
mkdir /media/NAS/Respaldo
mkdir /media/NAS/Documentos
mkdir /media/NAS/Videos
mkdir /media/NAS/Imagenes
mkdir /media/NAS/Musica
mkdir /media/NAS/Modelos
mkdir /media/cddvd

#edita fstab para NAS y cd dvd
echo \ \ ">> Editando fstab"
sleep 3
sudo sed -i '/'cdrom0'/d' /etc/fstab
echo '
#cd dvd
/dev/sdc0       /media/cddvd    udf,iso9660 user,noauto 0 0
#NAS
192.168.1.106:/nfs/Backups /media/NAS/Respaldo nfs rw 0 0
192.168.1.106:/nfs/Documents /media/NAS/Documentos nfs rw 0 0
192.168.1.106:/nfs/Movies /media/NAS/Videos nfs rw 0 0
192.168.1.106:/nfs/Pictures /media/NAS/Imagenes nfs rw 0 0
192.168.1.106:/nfs/Music /media/NAS/Musica nfs rw 0 0
192.168.1.106:/nfs/Modelos /media/NAS/Modelos nfs rw 0 0

' >> /etc/fstab

#añade usuario blender para el cluster
mount -a
echo \ \ ">> Añadiendo usuario blender"
sleep 3
adduser blender --/home /media/NAS/Documentos/blender

#añade nuevos preositorios a sources.list
echo \ \ ">> Añadiendo los nuevos Repositorios"
sleep 3
echo '

#Chromium Browser
deb http://ppa.launchpad.net/chromium-daily/ppa/ubuntu lucid main 
deb-src http://ppa.launchpad.net/chromium-daily/ppa/ubuntu lucid main

#Blender
deb http://ppa.launchpad.net/cheleb/blender-svn/ubuntu maverick main 
deb-src http://ppa.launchpad.net/cheleb/blender-svn/ubuntu maverick main

#BACKPORTS
deb http://backports.debian.org/debian-backports squeeze-backports main contrib non-free

#Non-Free
deb http://ftp.cl.debian.org/debian/ squeeze non-free contrib

' >> /etc/apt/sources.list

#instala keys para los nuevos repositorios
echo \ \ ">> Instalando Keys de Repositorios"
sleep 3
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 108A879C
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4E5E17B5

#actualiza repositorios
echo \ \ ">> Actualizando Repositorios"
sleep 3
sudo aptitude update

#instala paquetes necesarios para trabajar en nodos
echo \ \ ">> Instalando Paquetes"
sleep 3
sudo aptitude -t squeeze-backports install nautilus-dropbox
sudo aptitude install htop vim screen gparted blender chromium-browser netspeed guake gnome-do sun-java6-jre sun-java6-plugin fuseiso9660 flashplugin-nonfree filezilla mencoder ffmpeg ganglia-monitor


echo "
"
echo \ \ ">>>> Se ha terminado de configurar el nodo"$1" <<<<"

sleep 3
