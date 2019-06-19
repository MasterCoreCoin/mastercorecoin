
Debian
====================
This directory contains files used to package mastercorecoind/mastercorecoin-qt
for Debian-based Linux systems. If you compile mastercorecoind/mastercorecoin-qt yourself, there are some useful files here.

## mastercorecoin: URI support ##


mastercorecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install mastercorecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your mastercorecoinqt binary to `/usr/bin`
and the `../../share/pixmaps/mastercorecoin128.png` to `/usr/share/pixmaps`

mastercorecoin-qt.protocol (KDE)

