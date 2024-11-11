
- README -

	Author: ttrossea

	Etape 0:
		Booter sur un linux live.
		Ouvrir un terminal.
		Aller dans le dossier contenant le code source.
		exemple : ($>cd ~/myDiskCopy/).

	Etape 1:
		Compilation du grabber.
		$>sudo apt-get install gcc
		$>gcc cpdsk.c -o cpdsk.bin

	Etape 2:
		Brancher disque (usb ou autre).
		$>dmesg | grep Attached
		... Pour trouver le bon DEVICE (ex: sg2)

	Etape 3:
		$>sudo ./cpdsk.bin /dev/{NOM DU DEVICE} mon_ImageDisque.iso
		exemple : ($>sudo ./cpdsk.bin /dev/sg2 /media/monDisqueDeSauvegarde/SAUVEGARDE.iso)

Liste de device qui fonctionne :
	hda (?)
	sda

- README -

