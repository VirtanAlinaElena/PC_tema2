#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NITEMS 4
#define BUFMAX 100

typedef struct {
	char *intrebare;
	char *variantaA;
	char *variantaB;
	char *variantaC;
	char *variantaD;
} quiz;

// initializarea perechilor de culori
void initialise_colors()
{
	init_pair(1, COLOR_BLACK, COLOR_GREEN); 
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_RED, COLOR_BLACK);
}

// afisarea instructiunilor
void instructions(int row, int col)
{
	clear();
	attron(A_BOLD | COLOR_PAIR(2) | A_BLINK);
	mvprintw(row / 5, (col - strlen("## Instructiuni ##")) / 2,
										 "## Instructiuni ##");
	attroff(A_BOLD | COLOR_PAIR(2) | A_BLINK);
	attron(A_PROTECT);
	mvprintw(row / 2, (col - 
strlen("Alegeti raspunsul corect prin apasarea uneia din tastele A/B/C/D")) / 2,
 "Alegeti raspunsul corect prin apasarea  uneia din tastele A/B/C/D");
	mvprintw(row / 2 + 1, (col - 
						strlen("Un raspuns corect aduce jucatorului +10p")) / 2, 
						"Un raspuns corect aduce jucatorului +10p" );
	mvprintw(row / 2 + 2, (col - 
			strlen("Pentru fiecare raspuns gresit, jucatorul pierde -5p")) / 2, 
			"Pentru fiecare raspuns gresit, jucatorul pierde -5p");
	mvprintw(row / 2 + 3, (col - 
strlen("Dupa inceperea jocului se poate reveni la meniu apasand tasta Q")) / 2, 
"Dupa inceperea jocului se poate reveni la meniu apasand tasta Q");
	mvprintw(row / 2 + 6, (col - strlen("Mult success!")) / 2, "Mult success!");
	attroff(A_PROTECT);
	attron(A_BOLD | COLOR_PAIR(2));
	mvprintw(row-2, (col - 
strlen("## Apasati tasta ENTER pentru a reveni la meniul principal ##"))/2, 
"## Apasati tasta ENTER pentru a reveni la meniul principal ##");
	attroff(A_BOLD | COLOR_PAIR(2));
	refresh();
	int ch = getch();
	while (ch != 10) ch = getch();
	clear();
	refresh();
}

// crearea meniului principal cu 4 optiuni, afisarea lui si navigarea prin el
int menu() 
{
	WINDOW *menuw, *titlew;
	char list[NITEMS][15] = { "New Game",
				              "Resume Game",
			                  "Quit",
				              "Instructions" };
	char item[15];
	int ch, i = 0, row, col;


	getmaxyx(stdscr, row, col);
	menuw = newwin(7, 16, (row - NITEMS) / 2 - 2, (col - 
												strlen("Instructions")) / 2);
	box(menuw, ACS_VLINE, ACS_HLINE);

	titlew = newwin(3, 8, (row - NITEMS)/ 2 - 6, 
											(col - strlen("TRIVIA"))/ 2 + 1);
	wattrset(titlew, COLOR_PAIR(1) | A_BOLD);
	box(titlew, ACS_BULLET, ACS_BULLET);
	mvwprintw(titlew, 1, 1, "TRIVIA");
	wattroff(titlew, COLOR_PAIR(1) | A_BOLD);
	wrefresh(titlew);

	for (i = 0; i < NITEMS; i++)
	{
		if (i == 0)
			wattron(menuw, A_STANDOUT);
		else 
			wattroff(menuw, A_STANDOUT);
		sprintf(item, "%s", list[i]);
		mvwprintw(menuw, i + 1, 2, "%s", item);
	}
	wrefresh(menuw);

	i = 0;
	noecho();
	keypad(menuw, TRUE);
	curs_set(0);

	//get the input
	while ((ch = wgetch(menuw)))
	{
		sprintf(item,"%s", list[i]);
		mvwprintw(menuw, i + 1, 2, "%s", item);
		switch(ch)
		{
			case KEY_UP:
				i--;
				i = ( i < 0) ? (NITEMS -1) : i;
				break;
			case KEY_DOWN:
				i++;
				i = ( i > (NITEMS - 1) ) ? 0 : i;
				break;
			case 10:
				if (i == 0)
					return -1; // new game
				if (i == 1)
					return -2; // resume game
				if (i == 2)  // quit
					return -3;
				if (i == 3) // instructions
				{
				//	clear();
					return -4;
				}
				break;
		}
	  	wattron(menuw, A_STANDOUT);
		sprintf(item, "%s", list[i]);
		mvwprintw(menuw, i + 1, 2, "%s", item);
		wattroff(menuw, A_STANDOUT);	
	}
	delwin(menuw);
	keypad(menuw, FALSE);
	return 0;
}

// afisarea datei si timpului curent
void maketime(int* now_sec, int* now_min, int* now_hour, int* now_day, 
				int* now_month, int* now_year) 
{
		time_t now;
		struct tm *now_tm;

        now = time (NULL);
        now_tm = localtime (&now);
        *now_sec = now_tm->tm_sec;
        *now_min = now_tm->tm_min;
        *now_hour = now_tm->tm_hour;
        *now_day = now_tm->tm_mday;
        *now_month = now_tm->tm_mon + 1;
        *now_year = now_tm->tm_year + 1900;
}

// afisarea ecranului de final
void final_display(int score, int row, int col, int correct, int wrong)
{
			clear();
			refresh();
			WINDOW *gamew;
			gamew = newwin(row - 2, col - 2, 1, 1);
			box(gamew, ACS_BULLET, ACS_BULLET);

			int row_gamew, col_gamew;
			getmaxyx(gamew, row_gamew, col_gamew);
			mvwprintw(gamew, row_gamew / 2, (col_gamew - 
				strlen("Felicitari! Esti un triviador cu drepturi depline!"))/2, 
				"%s", "Felicitari! Esti un triviador cu drepturi depline!");
			mvwprintw(gamew, row_gamew/2 + 1, (col_gamew - 
									strlen("Raspunsuri corecte: ")) / 2, "%s", 
									"Raspunsuri corecte: ");
			mvwprintw(gamew, row_gamew/2 + 2, (col_gamew - 
			 strlen("Raspunsuri gresite: ")) / 2, "%s", "Raspunsuri gresite: ");
			mvwprintw(gamew, row_gamew/2 + 1, (col_gamew - 
								strlen("Raspunsuri corecte: ")) / 2 + 
								strlen("Raspunsuri corecte: "), "%d", correct);
			mvwprintw(gamew, row_gamew/ 2 + 2, (col_gamew - 
								strlen("Raspunsuri gresite: "))/2 + 
								strlen("Raspunsuri gresite: "), "%d", wrong);
			mvwprintw(gamew, row_gamew/2 + 4, 
				(col_gamew - strlen("Scor final: ")) / 2, "%s", "Scor final: ");
			mvwprintw(gamew, row_gamew / 2 + 4, 
				(col_gamew - strlen("Scor final: ")) / 2 + 
				strlen("Scor final: "), "%d", score);
			mvwprintw(gamew, row_gamew - 4, (col_gamew - 
				strlen("Apasa ENTER pentru a reveni la meniul principal")) / 2,
				 "%s", "Apasa ENTER pentru a reveni la meniul principal");
			wrefresh(gamew);
			int ch;
			ch = getch();
			while (ch != 10) ch = getch();
			clear();
			refresh();
}
 
// afisarea optiunilor ajutatoare cu atributele corespunzatoare starii lor: 
// activ sau inactiv
void display_help(int skip, int fifty, WINDOW *gamew, int row_gamew, 
					int col_gamew)
{
	// daca sunt disponibile ambele variante ajutatoare
	if (skip == 1 && fifty == 1)
	{
		wattron(gamew, COLOR_PAIR(5));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 - 12, "%s", "50/50");
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 + 2, "%s", 
														"Skip the question");
		wattroff(gamew, COLOR_PAIR(5));
	}

	// daca nu e disponibila nicio varianta ajutatoare
	if (skip == 0 && fifty == 0)
	{
		wattron(gamew, COLOR_PAIR(6));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 - 12, "%s", "50/50");
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 + 2, 
														"Skip the question");
		wattroff(gamew, COLOR_PAIR(6));
	}

	// daca e disponibila doar optiunea 50/50
	if (skip == 0 && fifty == 1)
	{
		wattron(gamew, COLOR_PAIR(5));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 - 12, "%s", "50/50");
		wattroff(gamew, COLOR_PAIR(5));

		wattron(gamew, COLOR_PAIR(6));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 + 2, 
														"Skip the question");
		wattroff(gamew, COLOR_PAIR(6));
	}

	// daca e disponibila doar optiunea skip
	if (skip == 1 && fifty == 0)
	{
		wattron(gamew, COLOR_PAIR(5));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 + 2, "%s", 
														"Skip the question");
		wattroff(gamew, COLOR_PAIR(5));

		wattron(gamew, COLOR_PAIR(6));
		mvwprintw(gamew, row_gamew / 3 + 1, col_gamew / 2 - 12, "%s", "50/50");
		wattroff(gamew, COLOR_PAIR(6));
	}
	mvwprintw(gamew, row_gamew / 3, col_gamew / 2 - 10, "%s", "|");
	mvwprintw(gamew, row_gamew / 3 - 1, col_gamew / 2 - 10, "%s", "^");
	mvwprintw(gamew, row_gamew / 3, col_gamew / 2 + 10, "%s", "|");
	mvwprintw(gamew, row_gamew / 3 - 1, col_gamew / 2 + 10, "%s", "^");
	mvwprintw(gamew, row_gamew / 3 - 3, col_gamew / 2 + 10, "%s", "S");
	mvwprintw(gamew, row_gamew / 3 - 3, col_gamew / 2 - 10, "%s", "F");


}

// actualizarea ecranului in cazul alegerii unui raspuns corect
void correct_answer(int tasta, WINDOW* gamew, int row_gamew, int col_gamew, 
	int* score, int* correct, quiz* question, int i)
{
	wattron(gamew, A_BOLD | COLOR_PAIR(3));
	if (tasta == 'A' || tasta == 'a')
		mvwprintw(gamew, row_gamew / 2 + 2, (col_gamew-
			strlen(question[i].variantaA)) / 2, "%s", question[i].variantaA);
	if (tasta == 'B' || tasta == 'b')
		mvwprintw(gamew, row_gamew / 2 + 3, (col_gamew-
			strlen(question[i].variantaB)) / 2, "%s", question[i].variantaB);
	if (tasta == 'C' || tasta == 'c')
		mvwprintw(gamew, row_gamew / 2 + 4, (col_gamew-
			strlen(question[i].variantaC)) / 2, "%s", question[i].variantaC);
	if (tasta == 'D' || tasta == 'd')
		mvwprintw(gamew, row_gamew / 2 + 5, (col_gamew-
			strlen(question[i].variantaD)) / 2, "%s", question[i].variantaD);
	wattroff(gamew, A_BOLD | COLOR_PAIR(3));
	*score += 10;
	(*correct)++;
}

// actualizarea ecranului in cazul alegerii unui raspuns gresit
void wrong_answer(int tasta, WINDOW* gamew, int row_gamew, int col_gamew, 
	int* score, int* wrong, quiz* question, int i, char  answer)
{
	wattron(gamew, A_BOLD | COLOR_PAIR(4));
	if (tasta == 'A' || tasta == 'a')
		mvwprintw(gamew, row_gamew / 2 + 2, (col_gamew - 
			strlen(question[i].variantaA))/2, "%s", question[i].variantaA);
	if (tasta == 'B' || tasta == 'b')
		mvwprintw(gamew, row_gamew/ 2 + 3, (col_gamew - 
			strlen(question[i].variantaB))/2, "%s", question[i].variantaB);
	if (tasta == 'C' || tasta == 'c')
		mvwprintw(gamew, row_gamew/ 2 + 4, (col_gamew - 
			strlen(question[i].variantaC))/2, "%s", question[i].variantaC);
	if (tasta == 'D' || tasta == 'd')
		mvwprintw(gamew, row_gamew/ 2 + 5, (col_gamew - 
			strlen(question[i].variantaD))/2, "%s", question[i].variantaD);
	wattroff(gamew, A_BOLD | COLOR_PAIR(3));
								
	wattron(gamew, A_BOLD | COLOR_PAIR(3) | A_BLINK);
	if (answer == 'A' || answer == 'a')
		mvwprintw(gamew, row_gamew/ 2 + 2, (col_gamew - 
			strlen(question[i].variantaA)) / 2, "%s", question[i].variantaA);
	if (answer == 'B' || answer == 'b')
		mvwprintw(gamew, row_gamew/ 2 + 3, (col_gamew - 
			strlen(question[i].variantaB)) / 2, "%s", question[i].variantaB);
	if (answer == 'C' || answer == 'c')
		mvwprintw(gamew, row_gamew/ 2 + 4, (col_gamew - 
			strlen(question[i].variantaC)) / 2, "%s", question[i].variantaC);
	if (answer == 'D' || answer == 'd')
		mvwprintw(gamew, row_gamew / 2 + 5, (col_gamew - 
			strlen(question[i].variantaD)) / 2, "%s", question[i].variantaD);
	wattroff(gamew, A_BOLD | COLOR_PAIR(3) | A_BLINK);

	*score -= 5;
	(*wrong)++;
	
}

// trecerea la urmatoare intrebare in cazul selectarii optiunii skip
void display_skip(WINDOW *gamew, int row_gamew, int col_gamew, quiz* question, 
					int score, int i)
{
	wclear(gamew);

	mvwprintw(gamew, row_gamew / 5 + 1, 5, "%d/10", i+1);
	mvwprintw(gamew, row_gamew / 2, (col_gamew-
		strlen(question[10].intrebare)) / 2,  question[10].intrebare);
	mvwprintw(gamew, row_gamew / 2 + 2, (col_gamew - 
		strlen(question[10].variantaA))/2, "%s", question[10].variantaA);
	mvwprintw(gamew, row_gamew / 2 + 3, (col_gamew - 
		strlen(question[10].variantaB))/2, "%s", question[10].variantaB);
	mvwprintw(gamew, row_gamew / 2 + 4, (col_gamew - 
		strlen(question[10].variantaC)) / 2, "%s", question[10].variantaC);
	mvwprintw(gamew, row_gamew / 2 + 5, (col_gamew - 
		strlen(question[10].variantaD)) / 2, "%s", question[10].variantaD);
	mvwprintw(gamew, row_gamew / 5, col_gamew - 25, "%s", "Score: ");
	mvwprintw(gamew, row_gamew / 5, col_gamew - 18, "%d  ", score);
	mvwprintw(gamew, row_gamew - 4, (col_gamew - 
			strlen("Apasa ENTER pentru a trece la intrebarea urmatoare")) / 2,
				 "%s", "Apasa ENTER pentru a trece la intrebarea urmatoare");
}

// implementarea optiunii 50/50
void fifty_option(WINDOW* gamew, char answer[], int i, int* rasp1, int* rasp2, 
					int row_gamew)
{
	int del1, del2;

	del1 = rand() % 4;
	while (del1 == answer[i] - 65)
		del1 = rand() % 4;

	del2 = rand() % 4;
	while (del2 == answer[i] - 65 || del2 == del1)
		del2 = rand() % 4;

	int j;
	for (j = 0 ; j < 4; j++)
	{	
		if (del1 == j)
		{
			wmove(gamew, row_gamew/2 + j + 2, 0);
			wclrtoeol(gamew);
		}
		else
			if (del2 == j)
			{
				wmove(gamew, row_gamew/2 + j + 2, 0);
				wclrtoeol(gamew);
			}
			else 
				if (*rasp1 == 0) 
					*rasp1 = j;
				else
					if (*rasp2 == 0) 
						*rasp2 = j;
	}
	
	box(gamew, ACS_VLINE, ACS_HLINE);
	wrefresh(gamew);
}

// afisarea intrebarii curente cu variantele de raspuns
void display_game_interface(WINDOW* gamew, int row_gamew, int col_gamew, 
							quiz* question, int score, int i)
{
	mvwprintw(gamew, row_gamew / 5 + 1, 5, "%d/10", i + 1);
	mvwprintw(gamew, row_gamew / 2, (col_gamew - 
		strlen(question[i].intrebare))/2, "%s", question[i].intrebare);
	mvwprintw(gamew, row_gamew / 2 + 2, (col_gamew - 
		strlen(question[i].variantaA))/2, "%s", question[i].variantaA);
	mvwprintw(gamew, row_gamew / 2 + 3, (col_gamew - 
		strlen(question[i].variantaB)) / 2, "%s", question[i].variantaB);
	mvwprintw(gamew, row_gamew / 2 + 4, (col_gamew -
		strlen(question[i].variantaC)) / 2, "%s", question[i].variantaC);
	mvwprintw(gamew, row_gamew/ 2 + 5, (col_gamew - 
		strlen(question[i].variantaD))/2, "%s", question[i].variantaD);
	mvwprintw(gamew, row_gamew / 5, col_gamew - 25, "%s", "Score: ");
	mvwprintw(gamew, row_gamew / 5, col_gamew - 18, "%d  ", score);
	mvwprintw(gamew, row_gamew - 4, (col_gamew - 
			strlen("Apasa ENTER pentru a trece la intrebarea urmatoare")) / 2,
				 "%s", "Apasa ENTER pentru a trece la intrebarea urmatoare");
}

int main(int argc, char** argv )
{
	if (argc == 1)
	{
		printf("[Eroare]: Nu s-au dat argumente in linia de comanda\n");
		return 1;
	}
	else
	{ 
		WINDOW *gamew;
		quiz* question;
		int i, row, col, row_gamew, col_gamew, fifty, skip;
		int now_sec, now_min, now_hour, now_day, now_month, now_year;
		 
		// initializarea modului ncurses
		initscr();

		// acum pot sa folosesc paleta de culori din ncurses
		start_color();

		// definirea perechilor de culori pe care le voi folosi 
		initialise_colors();

		nodelay(stdscr, FALSE);

		// afisarea meniului
		int play = menu();
		getmaxyx(stdscr, row, col);

		// atata timp cat nu selectez optiunea Quit
		while (play != -3)
		{
			// incep un joc nou
			if (play == -1)
			{
				// aloc memorie pentru structura cu intrebari
				question = (quiz*)malloc(11 * sizeof(quiz));
				
				FILE * fisier;
				char buffer[BUFMAX];

				//deschid fisierul dat ca argument
				fisier = fopen(argv[1], "r");
				if (fisier == NULL)
				{
					 printf("[Eroare]: Fisierul %s nu poate fi deschis\n", 
					 		 argv[1]);
				 	return 1;
				}
				else
				{
					int nr_intrebare = 0;
					int rand_intrebare = 0;

					//int c;

					// pp ca fisierul nu contine caractere neprintabile
					//int good = 1;
					//c = fgetc(fisier);
					//while (c ! = EOF && good == 1)
					//{
					//	if (c >= 0 && c <=31)
					//		good = 0;
					//	c = fgets(fisier);
					//}
					//if (good == 0)
					//{
					//	return 
					//}
				//aloc memorie pentru fiecare intrebare+raspunsurile aferente ei
					while (fgets(buffer, BUFMAX, fisier) != NULL)
					{
						rand_intrebare++;
						if (rand_intrebare == 1)
						{
							question[nr_intrebare].intrebare = 
									malloc((strlen(buffer) + 1) * sizeof(char));
							strcpy(question[nr_intrebare].intrebare, buffer);
						}
						if(rand_intrebare == 2)
						{	
							question[nr_intrebare].variantaA = 
									malloc((strlen(buffer) + 1) * sizeof(char));
							strcpy(question[nr_intrebare].variantaA, buffer);
						}
						if(rand_intrebare == 3)
						{	
							question[nr_intrebare].variantaB = 
									malloc((strlen(buffer) + 1) * sizeof(char));
							strcpy(question[nr_intrebare].variantaB, buffer);
						}
						if(rand_intrebare == 4)
						{	
							question[nr_intrebare].variantaC = 
									malloc((strlen(buffer) + 1) * sizeof(char));
							strcpy(question[nr_intrebare].variantaC, buffer);
						}
						if(rand_intrebare == 5)
						{	
							question[nr_intrebare].variantaD = 
									malloc((strlen(buffer) + 1) * sizeof(char));
							strcpy(question[nr_intrebare].variantaD, buffer); 
						}
						if (rand_intrebare == 5)
						{
							rand_intrebare = 0;
							nr_intrebare++;
						}
					}
					char answer[11] = {'B', 'A', 'B', 'C', 'A', 'D', 'C', 'C',
															 'A', 'A', 'D'};
					fclose(fisier);
		
				

					int score = 0;
					int correct = 0;
					int wrong = 0;
					fifty = 1;
					skip = 1; 
					// incep lucrul efectiv cu fereastra gamew-jocul
					
					gamew = newwin(row - 2, col - 2, 1, 1);
					getmaxyx(gamew, row_gamew, col_gamew);

					// jocul curent are statutul "in desfasurare"
					int ok = 1; 
					for (i = 0; i < 10 && play != -3 && ok == 1; i++)
					{
						clear();
						refresh();
						gamew = newwin(row - 2, col - 2, 1, 1);
						getmaxyx(gamew, row_gamew, col_gamew);
						display_game_interface(gamew, row_gamew, col_gamew, 
												question, score, i);
						maketime(&now_sec, &now_min, &now_hour, &now_day, 
												&now_month, &now_year);
						mvwprintw(gamew, row_gamew / 5 + 1, col_gamew - 25, 
								"TIME: %d:%d:%d", now_hour, now_min, now_sec);
						mvwprintw(gamew, row_gamew / 5 + 2, col_gamew - 25, 
								"DATE: %d-%d-%d", now_day, now_month, now_year);
						display_help(skip, fifty, gamew, row_gamew, col_gamew);
						box(gamew, ACS_VLINE, ACS_HLINE);
						wrefresh(gamew); 

						int tasta;
						tasta = getch();
						//while (tasta == KEY_UP || tasta == KEY_DOWN ) tasta = getch();
						if (skip == 1 && fifty == 1)
							while (tasta != 'A' && tasta != 'a' && 
								  tasta != 'B' && tasta != 'b' &&
								  tasta != 'C' && tasta != 'c' &&
								  tasta != 'D' && tasta != 'd' && 
							      tasta != 'S' && tasta != 's' &&
							      tasta != 'F' && tasta != 'f') 
							   tasta = getch();
						else
							if (skip == 0 && fifty == 1)
									while (tasta != 'A' && tasta!= 'a' && 
										   tasta != 'B' && tasta != 'b' &&
										   tasta != 'C' && tasta != 'c' &&
										   tasta != 'D' && tasta != 'd' &&
										   tasta != 'F' && tasta != 'f') 
										tasta = getch();
							else
								if (skip == 1 && fifty == 0)
									while (tasta != 'A' && tasta != 'a' &&
										   tasta != 'B' && tasta != 'b' &&
										   tasta != 'C' && tasta != 'c' &&
										   tasta != 'D' && tasta != 'd' &&
										   tasta != 'S' && tasta != 's')
									    tasta = getch();
								else
									if (skip == 0 && fifty == 0)
										while (tasta != 'A' && tasta != 'a' &&
											   tasta != 'B' && tasta != 'b' &&
											   tasta != 'C' && tasta != 'c' &&
											   tasta != 'D' && tasta != 'd') 
											tasta = getch();

						if (tasta == answer[i] || tasta == answer[i] + 32)
							correct_answer(tasta, gamew, row_gamew, col_gamew, 
											&score, &correct, question, i);

						if (tasta != answer[i] && tasta != answer[i] + 32
									&& tasta != 'S' && tasta != 's' && 
									tasta != 'F' && tasta != 'f')
							wrong_answer(tasta, gamew, row_gamew, col_gamew, 
										&score, &wrong, question, i, answer[i]);
						mvwprintw(gamew, row_gamew / 5, col_gamew - 18, "%d  ", 
																	score);
						
						// implementarea optiunii ajutatoare skip
						if ((tasta == 'S' || tasta == 's') && skip == 1) 
						{
							skip--;
							display_skip(gamew, row_gamew, col_gamew, question, 
																	score, i);
							maketime(&now_sec, &now_min, &now_hour, &now_day, 
														&now_month, &now_year);
							mvwprintw(gamew, row_gamew/5+1, col_gamew-25, 
								"TIME: %d:%d:%d", now_hour, now_min, now_sec);
							mvwprintw(gamew, row_gamew/5+2, col_gamew-25, 
								"DATE: %d-%d-%d", now_day, now_month, now_year);
							display_help(skip, fifty, gamew, row_gamew, 
															col_gamew);
							box(gamew, ACS_VLINE, ACS_HLINE);
							wrefresh(gamew);

							int tasta1 = getch();
							if (fifty == 1)
								while (tasta1 != 'A' && tasta1 != 'a' &&
									  tasta1 != 'B' && tasta1 != 'b' &&
									  tasta1 != 'C' && tasta1 != 'c' &&
									  tasta1 != 'D' && tasta1 != 'd' &&
									  tasta1 != 'F' && tasta1 != 'f') 
									tasta1 = getch();
							else
								while (tasta1 != 'A' && tasta1 == 'a' &&
									   tasta1 != 'B' && tasta1 !='b' &&
									   tasta1 != 'C' && tasta1 != 'c' &&
									   tasta1 != 'D' && tasta1 != 'd') 
									tasta1 = getch();
							if (tasta1 == answer[10] || tasta1 == answer[10]+32)
								correct_answer(tasta1, gamew, row_gamew, 
									col_gamew, &score, &correct, question, 10);
							if (tasta1 != answer[10] && tasta1 != answer[10] 
									+ 32 && tasta1 != 'F' && tasta1 != 'f')
							   wrong_answer(tasta1, gamew, row_gamew, col_gamew,
									 &score, &wrong, question, 10, answer[10]);
							mvwprintw(gamew, row_gamew/5, col_gamew-18, "%d ", 
																		score);
							wrefresh(gamew);

							if ((tasta1 == 'F' || tasta1 == 'f') && fifty == 1)
							{
								int rasp1 = 0, rasp2 = 0;
								fifty--;
								display_help(skip, fifty, gamew, row_gamew, 
																col_gamew);
								fifty_option(gamew, answer, 10, &rasp1, &rasp2, 
																	row_gamew);
							
								int tasta2 = getch();
								while (tasta2 != rasp1 + 65 && tasta2 != 
										rasp2 + 65 && tasta2 != rasp1 + 97 &&
									  	tasta2 != rasp2 + 97) 
									tasta2 = getch();
								if (tasta2 == answer[10] || 
													tasta2 == answer[10] + 32)
									correct_answer(tasta2, gamew, row_gamew, 
									 col_gamew, &score, &correct, question, 10);
								if (tasta2 != answer[10] && tasta2 != answer[10]
																		 + 32)
									wrong_answer(tasta2, gamew, row_gamew, 
										col_gamew, &score, &wrong, question, 10,
										 answer[10]);
								mvwprintw(gamew, row_gamew / 5, col_gamew - 18, 
															"%d ", score);
								wrefresh(gamew);
							}
						}

						if ((tasta == 'F' || tasta == 'f') && fifty == 1)
						{
							int rasp1 = 0, rasp2 = 0;
							fifty--;
							display_help(skip, fifty, gamew, row_gamew, 
															col_gamew);
							fifty_option(gamew, answer, i, &rasp1, &rasp2, 
																row_gamew);
							box(gamew, ACS_VLINE, ACS_HLINE);
							wrefresh(gamew);

							int tasta2 = getch();
							if (skip == 0)
								while (tasta2 != rasp1 + 65 && tasta2 != rasp2 
										+ 65 && tasta2 != rasp1 + 97 && 
										tasta2 != rasp2 + 97) 
									tasta2 = getch();
							else
								while (tasta2 != rasp1 + 65 && tasta2 != rasp2 +
										 65 && tasta2 != rasp1 + 97 && 
										 tasta2 != rasp2 + 97 && 
										 tasta2 != 'S' && tasta2 != 's') 
									tasta2 = getch();
							if (tasta2 == answer[i] || tasta2 == answer[i] + 32)
								correct_answer(tasta2, gamew, row_gamew, 
									col_gamew, &score, &correct, question, i);
							if (tasta2 != answer[i] && tasta2 != answer[i] + 32
									&& tasta2 != 'S' && tasta2 != 's')
								wrong_answer(tasta2, gamew, row_gamew, 
													col_gamew, &score, &wrong, 
													question, i, answer[i]);
							mvwprintw(gamew, row_gamew / 5, col_gamew - 18, 
															"%d ", score);
							wrefresh(gamew);

							if ((tasta2 == 'S' || tasta2 == 's') && skip == 1)
							{
								skip--;
								display_skip(gamew, row_gamew, col_gamew, 
														question, score, i);
								maketime(&now_sec, &now_min, &now_hour, &now_day
													   , &now_month, &now_year);
								mvwprintw(gamew, row_gamew / 5 + 1, 
											col_gamew - 25, "TIME: %d:%d:%d", 
											now_hour, now_min, now_sec);
								mvwprintw(gamew, row_gamew / 5 + 2, 
										col_gamew - 25, "DATE: %d-%d-%d", 
										now_day, now_month, now_year);
								display_help(skip, fifty, gamew, row_gamew, 
																col_gamew);
								box(gamew, ACS_VLINE, ACS_HLINE);
								wrefresh(gamew); 

								int tasta3;
								tasta3 = getch();
								//while (tasta == KEY_UP || tasta == KEY_DOWN ) tasta = getch();
								while (tasta3 != 'A' && tasta3 != 'a' &&
									   tasta3 != 'B' && tasta3 != 'b' && 
									   tasta3 != 'C' && tasta3 != 'c' &&
									   tasta3 != 'D' && tasta3 != 'd') 
									tasta3 = getch();
								if (tasta3 == answer[10] || 
										tasta3 == answer[10] + 32)
									correct_answer(tasta3, gamew, row_gamew, 
									 col_gamew, &score, &correct, question, 10);
								if (tasta3 != answer[10] &&
										tasta3 != answer[10] + 32)
									wrong_answer(tasta3, gamew, row_gamew, 
										col_gamew, &score, &wrong, question,
										 10, answer[10]);
								mvwprintw(gamew, row_gamew / 5, col_gamew - 18, 
																"%d ", score);
							
							}

						}

						box(gamew, ACS_VLINE, ACS_HLINE);
						wrefresh(gamew);

						int resume_ch;
						resume_ch = wgetch(gamew);
						while (resume_ch != 'Q' && resume_ch != 'q' 
												&& resume_ch != 10)
							resume_ch = wgetch(gamew);
						if (resume_ch == 'Q' || resume_ch == 'q')
							{
								wclear(gamew);
								wrefresh(gamew);
								wtimeout(gamew, -1);
								play = menu();

								// afisez instructiunile
								if (play == -4)
									while(play == -4)
									{
										instructions(row, col);
										play = menu();
									}

								// statului jocului "terminat"; incepe un joc nou
								if (play == -1)
									ok = 0;	

							}
					}
						// afisarea ecranului de final
					if (i == 10)
					{
						final_display(score, row_gamew, col_gamew, correct, 
																	wrong);
						play = menu();
					}

				}

			}	
			if (play == -3)
			{
				for (i = 0; i < 11; i++)
				{
					free(question[i].intrebare);
					free(question[i].variantaA);
					free(question[i].variantaB);
					free(question[i].variantaC);
					free(question[i].variantaD);
				}
				free (question);

				endwin();
			}
			if (play == -2)
				play = menu();
			if (play == -4)
			{
				instructions(row, col);
				play = menu();
			}
	    }
		endwin();
		return 0;
	}
} 

