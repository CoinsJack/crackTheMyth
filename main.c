#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// this is the myth map.
char myth[][31] = {

	"##############################", 
	"              #              #",
	"#  ####       #             ->",
	"#     #       # ######       #",
	"########      #              #",
	"#   ##    #   #  # ######## ##",
	"# # #       #    #           #",
	"# # ####### #    #  ######## #",
	"# #         #    #           #",
	"# # #  #    #    #    ###    #",        
	"##############################"
};

char book[12][30] = {0};

int myth_starty = 0;
int myth_startx = 0;

// this is a person struct.
typedef struct _person
{
        int x;
        int y;
	int step;

}Person;


// this function handle the person's moving.
void moveTo(WINDOW *w, Person *p, int y, int x);

// this function return a person pointer.
Person* con_person();

// this function destroy a person.
void destroy_person(Person *p);

// this a struct for a point in game.
typedef struct _loc
{
	int x, y;
}MLoc;

// this funcion creates a myth in the middle of the target window.
int create_myth(WINDOW *target);

//
void go_through(WINDOW *, Person *);



int main()
{
	// preparation for this program.
	initscr();
	clear();
	curs_set(0); // hide the cursor
	noecho(); // no echo.
	cbreak(); // close the line buffer.

	// make a border for the standred screen.
	wborder(stdscr,  '|', '|', '-', '-', '*', '*', '*', '*');
	refresh();


	// two var for the max size of the stdscr.
	int scry, scrx;
	getmaxyx(stdscr, scry, scrx);
	

	// use a LOC var for an area. y stands for height, x stands for width.
	MLoc title_part;
	title_part.y = 	(scry-15)/2 + 4;
	title_part.x =  (scrx-50)/2;	


	/* print some game info */
	char *title = " GAME: Crack the myth ";
	char *author = " AUTHOR: Jack ";
	char *version = " VERSION: 1.0.1 ";

	attron(A_REVERSE);
	mvprintw((title_part.y - 3)/2, (scrx-strlen(title))/2, "%s", title);
	attroff(A_REVERSE);	

	attron(A_UNDERLINE);
	mvprintw((title_part.y - 3)/2 + 1, (scrx-strlen(version))/2, "%s", version);
	mvprintw((title_part.y - 3)/2 + 2, (scrx-strlen(author))/2, "%s", author);
	attroff(A_UNDERLINE);

	refresh();

	// myth is a window for the myth map.
	WINDOW *myth = newwin(15, 50, (scry-15)/2 + 4, (scrx-50)/2); 

	// we called the function which we defined to show the myth map
	create_myth(myth);
	wborder(myth,  '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(myth);


	// put a person on myth.
	Person *player = con_person();
	moveTo(myth, player, 1, 0);

	go_through(myth, player);
		

	// get a character then exit.
	getchar();
	endwin();
    
    return 0;
}

int create_myth(WINDOW *target)
{
	int y, x;
	int starty, startx; 	getmaxyx(target, y, x);

	myth_starty = starty = (y-11)/2;
	myth_startx = startx = (x-31)/2;

	attron(A_BOLD);
    for (int i = 0; i < 11; ++i)
    {
		mvwprintw(target, starty+i, startx, "%s\n", myth[i]); 
    }
	attroff(A_BOLD);
	wrefresh(target);	
	return 0;
}


Person* con_person()
{
	Person *p = (Person*)malloc(sizeof(Person));
	if (p != NULL) p->x = p->y = p->step = 0;
	return p;
}

void destroy_person(Person *p)
{
	free(p);
}

void moveTo(WINDOW *w, Person *p, int y, int x)
{
	if (p->step != 0)
	{
		mvwaddch(w, myth_starty + p->y, myth_startx + p->x, ' ');
	}

	mvwaddch(w, myth_starty + y, myth_startx + x, 'J');
	wrefresh(w);
	p->y = y;
	p->x = x;
	++p->step;
}

void go_through(WINDOW *w, Person *p)
{

	int cy, cx;
	int ny, nx;

	if ( p->y == 2 && p->x == 29) 
	{
		return;
	}
	int next[4][2] = 
		{
			{0, 1},
			{1, 0},
			{0, -1},
			{-1, 0},
		};
	


		for (int i = 0; i < 4; ++i) 
		{
			ny = p->y + next[i][0];
			nx = p->x + next[i][1];
			cy = p->y;
			cx = p->x;

			if (ny > 10 || ny < 0 || nx > 29 || nx < 0)
			{
				continue;
			}

			if (book[ny][nx] == 0 && myth[ny][nx] == ' ')
			{
				book[ny][nx] = 1;
				moveTo(w, p, ny, nx);
				usleep(100000);
				go_through(w, p);
				book[ny][nx] = 0;
			}
		}
		return ;

		
		
}
