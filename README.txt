-----------------------------------------------------------------
                                 ___ ___ _    ___ _____   _____ 
                                | __|_ _| |  | __/ __\ \ / / __|
                                | _| | || |__| _|\__ \\ V /\__ \
                                |_| |___|____|___|___/ |_| |___/
                          
-----------------------------------------------------------------
Author: 	Matt W. Martin, 4374851
		kaethis@tasmantis.net

Project:	CS3790, Bonus:
		FAT-Model Filesystem

Date Issued:	25-Nov-2015
Date Archived:	XX-Dec-2015

File:		README.txt


Comments:	This assignment consists of one program:

		- filesys:	A functioning filesystem implim-
				ented with a CLI GUI using the
				NCURSES lib.

Modelled after the FAT filesystem that MS-DOS used in the good
old days, this system supports up to 32 different files in its
directory so long as its size does not surpass the 1024 blocks
(each of which 512 BYTES) of allocated storage space or succeed
the number of entires in the file allocation table.

The contents of the filesystem are committed to a file called
"master.mfs".  The contents of this file are loaded and saved
every time the program is executed and terminated, respectively.
This file is only meant to be read by the filesystem; its
contents are encrypted using a rudimentary XOR scheme until it
is loaded by the filesystem.  It's not much for security, but it
does serve to try and prevent someone from simply bypassing the
program altogether.


Instructions:	This program supports several different functions
		for manipulating files inside the filesystem.
		Many of these functions relate to the file that's
		currently selected in the directory.  Said func-
		tions are invoked by input on the keyboard, which
		are as follows:

		[up-arrow]	Move selection up.

		[down-arrow]	Move selection down.

		[i]MPORT	Imports a file from the host
				filesystem specified in the
				dialog win, followed by the
				name it will be saved under in
				the filesystem directory.

		[e]XPORT	Exports the selected file from
				the filesystem to the host.  You
				must specify the name of the file
				to be saved on the host.

		[c]OPY		Copy the selected file to a new
				file specified in the dialog win.

		[d]ELETE	Deletes the currently-selected
				file in the directory.

		[esc]		Save changes and exit filesystem.

		------------------ COMING SOON! -----------------

		[r]ENAME	Prompts new name for currently
				selected file in directory.

		[v]IEW		View contents of currently
				selected file.



MAKE:		./filesys	Compiles the filesys program.
		./clean		Removes the program.

		./archive	Creates archive in parent-dir.


GIT Repository:	 https://github.com/kaethis/CS3790_FILESYS.git


Notes:		- This program requires the NCURSES library.

		- If the filesys detects that the "master.mfs"
		  file doesn't exist, it'll create a new one
		  upon saving and exiting (using the [esc] key).

		- Modal dialog windows currently provide little
	 	  to no feedback to the user whatsoever.  This
		  output simply hasn't been implemented yet.
		  That said, the context of the action the user
		  is performing should (hopefully) provide enough
		  information as to what the dialog window is
		  asking for (see Instructions for more detail).

		- There's a known issue with importing files that
		  surpass the available number of blocks.  For
		  the time being, please don't import files that
		  are obviously going to be too big (this
		  includes "master.mfs" itself!); expect some
		  undefined behaviour otherwise.  I'll fix this
		  later.
