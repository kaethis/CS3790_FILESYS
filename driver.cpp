#include "cfilesys"
#include "ncurses.cpp"


#define  ERR_FILE_NOT_FOUND        100
#define  ERR_FILE_ALREADY_EXISTS   101
#define  ERR_DIRECTORY_FULL        102
#define  ERR_BLOCKS_FULL           103
#define  ERR_UNKNOWN               104
#define  ERR_LOAD                  105

#define  KEY_RET   0x0A
#define  KEY_ESC   0x1B
#define  KEY_C     0x63
#define  KEY_D     0x64
#define  KEY_E     0x65
#define  KEY_I     0x69


FileSystem  *filesys = new FileSystem();


const File  *selected;

int  selected_i = 0;


std::string  input;


Window  *dir;


void drawAlert(int err){

	Window  *alert = new Window(30, 10, (SCREENY/2)-10, (SCREENX/2)-15);


	alert->drawHeader();

	alert->drawOK();


	int  key;

	while(key = getch())
		if(key == KEY_RET)
			break;


	alert->close();


	refresh();
}


bool drawDialog(int dial){

	Window  *dialog = new Window(30, 10, (SCREENY/2)-10, (SCREENX/2)-15);


	dialog->drawHeader();

	dialog->drawCancel();

	dialog->drawOK();


	int  key;

	while(key = getch())
		if((key == KEY_RET) || (key == KEY_ESC))
			break;


	dialog->close();


	refresh();


	if(key == KEY_RET)
		return true;
	else
		return false;
}


bool drawInput(){

	Window  *dialog = new Window(30, 10, (SCREENY/2)-10, (SCREENX/2)-15);


	dialog->drawHeader();

	dialog->drawCancel();

	dialog->drawOK();


	Window  *textbar = new Window(14, 3, (SCREENY/2)-6, (SCREENX/2)-7);


	input.clear();


	wmove(textbar->win, textbar->y_min, textbar->x_min);

	refresh();

	getyx(textbar->win, textbar->y_curr, textbar->x_curr);


	int  key;

	while(key = getch()){

		if((key == KEY_RET) || (key == KEY_ESC))
			break;

		else if(key != ERR){

			switch(key){

			    case KEY_BACKSPACE:	if(strlen(input.c_str()) != 0){

							input.pop_back();

							getyx(textbar->win,
							  textbar->y_curr,
							  textbar->x_curr);

							wmove(textbar->win,
							  textbar->y_curr,
							  (textbar->x_curr)-1);

							wprintw(textbar->win, " ");

							wmove(textbar->win,
					  		  textbar->y_curr,
							  (textbar->x_curr)-1);
						}

						break;

			    case KEY_UP:

			    case KEY_DOWN:

			    case KEY_LEFT:

			    case KEY_RIGHT: 	break;

			    default:		if(input.length() < textbar->x_max){

							std::string  ch;

							ch.push_back((char)key);

							input.push_back((char)key);

							
							wprintw(textbar->win,
							  ch.c_str());
						}
			}


			wrefresh(textbar->win);

			getyx(textbar->win, textbar->y_curr, textbar->x_curr);

			wmove(textbar->win, textbar->y_curr, textbar->x_curr);

			refresh();
		}
	}


	textbar->close();


	dialog->close();


	refresh();


	if(key == KEY_RET)
		return true;
	else
		return false;
}


void refreshDir(){

	dir->y_curr = dir->y_min;
	dir->x_curr = dir->x_min;

	for(int i = 0; i < DIRSIZE; i++){

		wmove(dir->win, dir->y_curr, dir->x_curr);


		if(i == selected_i)
			wattron(dir->win, COLOR_PAIR(2));
		else
			wattron(dir->win, COLOR_PAIR(1));

		if(filesys->getDir()[i].start != -1)
			wprintw(dir->win, " %12s %10d ",
			  filesys->getDir()[i].name, filesys->getDir()[i].size);
		else
			wprintw(dir->win, " %12s %10s ", " ", " ");


		dir->y_curr++;


		wrefresh(dir->win);
	}


	refresh();
}


void import(){

	std::string  source = input.substr(0, FILELENGTH);

	if((drawInput()) && (input.length() != 0)){

		std::string  dest = input.substr(0, FILELENGTH);


		int  import = filesys->import(source, dest);

		switch(import){

		    case  0:	break;

		    case -1:	drawAlert(ERR_FILE_NOT_FOUND);
				break;

		    case -2:	drawAlert(ERR_FILE_ALREADY_EXISTS);
				break;

		    case -3:	drawAlert(ERR_DIRECTORY_FULL);
				break;

		    case -4:	drawAlert(ERR_BLOCKS_FULL);
				break;

		    default:	drawAlert(ERR_UNKNOWN);
		}
	}
}


void copy(){

	std::string  dest = input.substr(0, FILELENGTH);


	int  copy = filesys->copy(selected->name, dest.c_str());

	switch(copy){

	    case  0:	break;

	    case -1:	drawAlert(ERR_FILE_NOT_FOUND);
			break;

	    case -2:	drawAlert(ERR_FILE_ALREADY_EXISTS);
			break;

	    case -3:	drawAlert(ERR_DIRECTORY_FULL);
			break;

	    case -4:	drawAlert(ERR_BLOCKS_FULL);
			break;

	    default:	drawAlert(ERR_UNKNOWN);
	}
}


void outport(){

	std::string  dest = input.substr(0, FILELENGTH);
	

	int  outport = filesys->outport(selected->name, dest);

	switch(outport){

	    case  0:	break;

	    case -1:	drawAlert(ERR_FILE_NOT_FOUND);
			break;

	    case -2:	drawAlert(ERR_FILE_ALREADY_EXISTS);
			break;

	    default:	drawAlert(ERR_UNKNOWN);
	}
}



void remove(){

	int  remove = filesys->remove(selected->name);

	switch(remove){

	    case  0:	break;

	    case -1:	drawAlert(ERR_FILE_NOT_FOUND);
			break;

	    default:	drawAlert(ERR_UNKNOWN);
	}
}


bool keyhandler(){

	int  key;

	while(key = getch()){

		selected = &(filesys->getDir()[selected_i]);


		switch(key){

		    case KEY_UP:	if(selected_i > 0)
						selected_i--;

					refreshDir();
					break;

		    case KEY_DOWN:	if(selected_i < (DIRSIZE-1))
						selected_i++;

					refreshDir();
					break;

		    case KEY_I:		if(drawInput())
						if(input.length() != 0)
							import();

					refreshDir();
					break;

		    case KEY_C:		if(selected->start != -1)
						if(drawInput())
							if(input.length() != 0)
								copy();

					refreshDir();
					break;

		    case KEY_E:		if(selected->start != -1)
						if(drawInput())
							if(input.length() != 0)
								outport();

					refreshDir();
					break;

		    case KEY_D:		if(selected->start != -1)
						if(drawDialog(0))
							remove();

					refreshDir();
					break;

		    case KEY_ESC:	return true;
		}
	}


	return false;
}


int main(){

	initNCURSES();


	dir = new Window(27, DIRSIZE+2, 5, 1);


	if(filesys->load() == -1)
		drawAlert(ERR_LOAD);


	refreshDir();


	keyhandler();


	filesys->save();

	delete filesys;


	dir->close();


	exitNCURSES();

	return 0;
}
