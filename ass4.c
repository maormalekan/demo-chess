/***********
* Maor Malekan
* 321202962
* 01
* ass04
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "ass4.h"

// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, promotionPiece;
	int iSrc, jSrc, iDest, jDest;
	int isWhite, isCapture, isPromotion, isCheck, isLegal;
} Move;

// PGN and FEN characters
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';
const char BLACK_PAWN = 'p';
const char BLACK_ROOK = 'r';
const char BLACK_KNIGHT = 'n';
const char BLACK_BISHOP = 'b';
const char BLACK_QUEEN = 'q';
const char BLACK_KING = 'k';

const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';

const char FIRST_LEGAL_COL = 'a';
const char LAST_LEGAL_COL = 'a' + SIZE - 1;
const char FIRST_LEGAL_ROW = '1';
const char LAST_LEGAL_ROW = '1' + SIZE - 1;

const char EMPTY = ' ';

/***************************************************************************************************
* Function Name: createBoard
* Input: char board[][](2-D array), char fen[](string)
* Output: no output
* Function Operation: the function gets uninitialized 2-D array that represent the board of the game
						and string that represent the status of the board.
						the function change the boarg according to the string.
***************************************************************************************************/
void createBoard(char board[][SIZE], char fen[])
{
	// indicates the location of the loop while it run over the string   
	int loc = 0;
	for (int i = 0; i < SIZE; i++)
	{
		int j = 0;
		while (j < SIZE)
		{
			// if there are empty cells
			if (fen[loc] >= '1' && fen[loc] <= '9')
			{
				int digit = fen[loc] - '0';
				while (digit != 0)
				{
					board[i][j] = EMPTY;
					digit--;
					j++;
				}
			}
			else
			{
				board[i][j] = fen[loc];
				j++;
			}
			loc++;
		}
		loc++;
	}
}

/***************************************************************************************************
* Function Name: printColumns
* Input: no input
* Output: no output
* Function Operation: the function print the rows that show the columns of the board.
***************************************************************************************************/
void printColumns()
{
	char column = 'A';
	printf("* |");
	for (int i = 0; i < SIZE; i++) {
		if (i) {
			printf(" ");
		}
		printf("%c", column);
		column++;
	}
	printf("| *\n");
}

/***************************************************************************************************
* Function Name: printSpacers
* Input: no input
* Output: no output
* Function Operation: the function print the rows that will delimit the board.
***************************************************************************************************/
void printSpacers() {
	printf("* -");
	for (int i = 0; i < SIZE; i++) {
		printf("--");
	}
	printf(" *\n");
}

/***************************************************************************************************
* Function Name: printBoard
* Input: char board[][](2-D array)
* Output: no output
* Function Operation: the function get a board of a chess game and print it.
***************************************************************************************************/
void printBoard(char board[][SIZE])
{
	printColumns();
	printSpacers();
	int rowIdx = SIZE;
	for (int i = 0; i < SIZE; i++)
	{
		printf("%d ", rowIdx);
		for (int j = 0; j < SIZE; j++)
			printf("|%c", board[i][j]);
		printf("| %d\n", rowIdx);
		rowIdx--;
	}
	printSpacers();
	printColumns();
}

/***************************************************************************************************
* Function Name: myMin
* Input: two variables of int
* Output: int (the smaller int from the two it gets in the input)
* Function Operation: the function gets two numbers and return the smaller number out of the two.
***************************************************************************************************/
int myMin(int x, int y)
{
	if (x < y)
		return x;
	return y;
}

/***************************************************************************************************
* Function Name: myMax
* Input: two variables of int
* Output: int (the bigger int out of the two it gets in the input)
* Function Operation: the function gets two numbers and return the bigger number out of the two.
***************************************************************************************************/
int myMax(int x, int y)
{
	if (x > y)
		return x;
	return y;
}

/***************************************************************************************************
* Function Name: myIsUpper
* Input: char
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets char and check if it capital letter.
***************************************************************************************************/
int myIsUpper(char c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

/*****************************************************************************************************************************
* Function Name: buildMoveFromPgn
* Input: Move* move(pointer of move that initialized to 0), char pgn[](string of the move), char board[][](board of the game),
		int isWhite (1- for white turn, 0- for the black turn).
* Output: no output
* Function Operation: the function build the move he gets according to the string that represent the move,
						the board of the game and the variable that indicates if it's the white turn or the black turn.
*****************************************************************************************************************************/
void buildMoveFromPgn(Move* move, char pgn[], char board[][SIZE], int isWhite)
{
	(*move).isWhite = isWhite;
	int loc = 0;
	// if the first letter is capital letter, it represents the piece of the soldier that want to move.
	if (myIsUpper(pgn[loc]))
	{
		(*move).srcPiece = pgn[loc];
		loc++;
	}
	// if the first letter is not capital letter, the soldier that want to move is Pawn.
	else
		(*move).srcPiece = PAWN;

	/*
	in the board the capital letters are for the white player and the others are for the black player.
	in order that the board and the move will match each other if it's the white turn the source piece will stay capital letter.
	otherwise it will be change to small letter.
	*/
	if (!(*move).isWhite)
		(*move).srcPiece = tolower((*move).srcPiece);
	while (pgn[loc] != '\0')
	{
		// if the char in the 'pgn' represent a column
		if (pgn[loc] >= FIRST_LEGAL_COL && pgn[loc] <= LAST_LEGAL_COL)
		{

			/*
			at first the move is initialized to 0, so all it's fields are 0.
			if there are two chars that represent columns, the first char is for the source column
			and the second is for the destenation.
			*/
			if ((*move).destCol != 0)
			{
				(*move).srcCol = (*move).destCol;
				(*move).jSrc = (*move).srcCol - FIRST_LEGAL_COL;
			}
			(*move).destCol = pgn[loc];
			(*move).jDest = pgn[loc] - FIRST_LEGAL_COL;
		}
		// if the char in the 'pgn' represent a row
		if (pgn[loc] >= FIRST_LEGAL_ROW && pgn[loc] <= LAST_LEGAL_ROW)
		{

			/*
			if there are two chars that represent rows, the first char is for the source row
			and the second is for the destenation.
			*/
			if ((*move).destRow != 0)
			{
				(*move).srcRow = (*move).destRow;
				// the indicators of the printed board are  the opposite indicators of the board in the program.
				int oppositeRow = (*move).srcRow - FIRST_LEGAL_ROW;
				(*move).iSrc = SIZE - oppositeRow - 1;
			}
			(*move).destRow = pgn[loc];
			int oppositeRow = pgn[loc] - FIRST_LEGAL_ROW;
			(*move).iDest = SIZE - oppositeRow - 1;
		}
		if (pgn[loc] == CAPTURE)
			(*move).isCapture = 1;
		if (pgn[loc] == PROMOTION)
		{
			(*move).isPromotion = 1;
			// if the char in the 'pgn' indicates for promotion the next letter represent the promotion piece.
			(*move).promotionPiece = pgn[loc + 1];
			if (!(*move).isWhite)
				(*move).promotionPiece = tolower((*move).promotionPiece);
		}
		if (pgn[loc] == CHECK || pgn[loc] == MATE)
			(*move).isCheck = 1;
		loc++;
	}
	(*move).destPiece = board[(*move).iDest][(*move).jDest];
}

/*****************************************************************************************************************************
* Function Name: buildTempMove
* Input: char board[][](board of the game), Move* tempMove(pointer of move), int iSrc, int jSrc, int iDest, int jDest.
* Output: no output
* Function Operation: the function gets move, board and indicators for source cell and destenation cell that
						are in the board. according to these cells the function build the move it gets.
*****************************************************************************************************************************/
void buildTempMove(char board[][SIZE], Move* tempMove, int iSrc, int jSrc, int iDest, int jDest)
{
	(*tempMove).iSrc = iSrc;
	(*tempMove).jSrc = jSrc;
	(*tempMove).srcPiece = board[iSrc][jSrc];
	(*tempMove).iDest = iDest;
	(*tempMove).jDest = jDest;
	(*tempMove).destPiece = board[iDest][jDest];
	(*tempMove).isWhite = myIsUpper((*tempMove).srcPiece);
}

/*****************************************************************************************************************************
* Function Name: canMoveInRow
* Input: char board[][](board of the game), int iSrc, int jSrc, int iDest, int jDest.
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and indicators for source cell and destenation cell that are in the board.
					the function check if these cells are in the same row and if the road between the cells is clear.
*****************************************************************************************************************************/
int canMoveInRow(char board[][SIZE], int iSrc, int jSrc, int iDest, int jDest)
{
	if (iSrc != iDest)
		return 0;
	int jStart = myMin(jSrc, jDest);
	int jLast = myMax(jSrc, jDest);
	for (int j = jStart + 1; j < jLast; j++)
		if (board[iSrc][j] != EMPTY)
			return 0;
	return 1;
}

/*****************************************************************************************************************************
* Function Name: canMoveInCol
* Input: char board[][](board of the game), int iSrc, int jSrc, int iDest, int jDest.
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and indicators for source cell and destenation cell that are in the board.
					the function check if these cells are in the same column and if the road between the cells is clear.
*****************************************************************************************************************************/
int canMoveInCol(char board[][SIZE], int iSrc, int jSrc, int iDest, int jDest)
{
	if (jSrc != jDest)
		return 0;
	int iStart = myMin(iSrc, iDest);
	int iLast = myMax(iSrc, iDest);
	for (int i = iStart + 1; i < iLast; i++)
		if (board[i][jSrc] != EMPTY)
			return 0;
	return 1;
}

/*****************************************************************************************************************************
* Function Name: canMoveInDiagonal
* Input: char board[][](board of the game), int iSrc, int jSrc, int iDest, int jDest.
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and indicators for source cell and destenation cell that are in the board.
					the function check if these cells are in diagonal road and if the road between the cells is clear.
*****************************************************************************************************************************/
int canMoveInDiagonal(char board[][SIZE], int iSrc, int jSrc, int iDest, int jDest)
{
	//if the cells are in diagonal road the distance between their rows and the distance between their columns has to be equal.
	if (fabs((double)iSrc - (double)iDest) != fabs((double)jSrc - (double)jDest))
		return 0;
	// variables for the loop that will run over the diagonal
	int iStart;
	int jStart;
	int iEnd;
	int jEnd;

	/*
	there are two types of diagonal:
	- a diagonal with upper part that inclined to the left and with lower part that inclined to the right.
	- a diagonal with upper part that inclined to the right and with lower part that inclined to the left.
	these conditions are for each type of diagonal.
	*/
	if ((iSrc<iDest && jSrc>jDest) || (iSrc > iDest&& jSrc < jDest))
	{
		// diagonal with upper part that inclined to the right and with lower part that inclined to the left.
		iStart = myMin(iSrc, iDest);
		jStart = myMax(jSrc, jDest) - 1;
		iEnd = myMax(iSrc, iDest);
		jEnd = myMin(jSrc, jDest);
		for (int i = iStart + 1; i < iEnd; i++, jStart--)
			if (board[i][jStart] != EMPTY)
				return 0;
	}
	else
	{
		// diagonal with upper part that inclined to the left and with lower part that inclined to the right.
		iStart = myMin(iSrc, iDest);
		jStart = myMin(jSrc, jDest) + 1;
		iEnd = myMax(iSrc, iDest);
		jEnd = myMax(jSrc, jDest);
		for (int i = iStart + 1; i < iEnd; i++, jStart++)
			if (board[i][jStart] != EMPTY)
				return 0;
	}
	return 1;
}

/*****************************************************************************************************************************
* Function Name: queenCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a queen.
*****************************************************************************************************************************/
int queenCanMove(char board[][SIZE], Move* move)
{
	// queen can march in row, column or diagonal
	return(canMoveInRow(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest) ||
		canMoveInCol(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest) ||
		canMoveInDiagonal(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest));
}

/*****************************************************************************************************************************
* Function Name: rookCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a rook.
*****************************************************************************************************************************/
int rookCanMove(char board[][SIZE], Move* move)
{
	// rook can march in row or column.
	return(canMoveInRow(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest) ||
		canMoveInCol(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest));
}

/*****************************************************************************************************************************
* Function Name: bishopCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a bishop.
*****************************************************************************************************************************/
int bishopCanMove(char board[][SIZE], Move* move)
{
	// bishop can march in only in diagonal
	return(canMoveInDiagonal(board, (*move).iSrc, (*move).jSrc, (*move).iDest, (*move).jDest));
}

/*****************************************************************************************************************************
* Function Name: knightCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a knight.
*****************************************************************************************************************************/
int knightCanMove(char board[][SIZE], Move* move)
{
	/*
	we check this move with Pythagorean theorem:
	knight can march only if the distance between him and the destenation is root of 5.
	*/
	double length = (double)(*move).iSrc - (double)(*move).iDest;
	double width = (double)(*move).jSrc - (double)(*move).jDest;
	double diagonal = sqrt(pow(length, 2) + pow(width, 2));
	return(diagonal == sqrt(5.0));
}

/*****************************************************************************************************************************
* Function Name: kingCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a king.
*****************************************************************************************************************************/
int kingCanMove(char board[][SIZE], Move* move)
{
	//king can march only one cell distance.
	if (fabs((double)(*move).iDest - (double)(*move).iSrc) == 1.0 && 
		(((*move).jSrc == (*move).jDest) || (fabs((double)(*move).jDest - (*move).jSrc) == 1.0)))
			return 1;
	if (((*move).iSrc == (*move).iDest) && (fabs((double)(*move).jDest - (*move).jSrc) == 1.0))
		return 1;
	return 0;
}

/*****************************************************************************************************************************
* Function Name: pawnCanMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and pointer to a move and check if this move can march as a pawn.
*****************************************************************************************************************************/
int pawnCanMove(char board[][SIZE], Move* move)
{
	// white pawns can march to the upper side of the board and black pawns can march to the lower side of the board.
	if ((*move).isWhite)
	{
		if ((*move).isCapture)
		{
			// when pawn want to capture, it march one cell distance but only in diagonal way.
			if (((*move).iDest == (*move).iSrc - 1) && fabs((double)(*move).jDest - (double)(*move).jSrc) == 1.0)
				return 1;
			return 0;
		}
		// if pawn doesn't want to capture it can march one cell distance in straight way.
		if (((*move).jDest == (*move).jSrc) && ((*move).iDest == (*move).iSrc - 1))
			return 1;
		// if pawn in his second row it has option to march two cells distance in straight way.
		if (((*move).jDest == (*move).jSrc) &&	((*move).iSrc == LAST_LEGAL_ROW - FIRST_LEGAL_ROW - 1)
			&& ((*move).iDest == (*move).iSrc - 2) && (board[(*move).iSrc-1][(*move).jSrc] == EMPTY))
			return 1;
		return 0;
	}
	else
	{
		if ((*move).isCapture)
		{
			// when pawn want to capture, it march one cell distance but only in diagonal way.
			if (((*move).iDest == (*move).iSrc + 1) && fabs((double)(*move).jDest - (double)(*move).jSrc) == 1.0)
				return 1;
			return 0;
		}
		// if pawn doesn't want to capture it can march one cell distance in straight way.
		if (((*move).jDest == (*move).jSrc) && ((*move).iDest == (*move).iSrc + 1))
			return 1;
		// if pawn in his second row it has option to march two cells distance in straight way.
		if (((*move).jDest == (*move).jSrc) && ((*move).iSrc == FIRST_LEGAL_ROW - '0')
			&& ((*move).iDest == (*move).iSrc + 2) && (board[(*move).iSrc + 1][(*move).jSrc] == EMPTY))
			return 1;
		return 0;
	}
}

/*****************************************************************************************************************************
* Function Name: canMove
* Input: char board[][](board of the game), Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function check if the piece in the source can march to the destenation.
*****************************************************************************************************************************/
int canMove(char board[][SIZE], Move* move)
{
	if ((*move).srcPiece == QUEEN || (*move).srcPiece == BLACK_QUEEN)
		return queenCanMove(board, move);
	if ((*move).srcPiece == BISHOP || (*move).srcPiece == BLACK_BISHOP)
		return bishopCanMove(board, move);
	if ((*move).srcPiece == ROOK || (*move).srcPiece == BLACK_ROOK)
		return rookCanMove(board, move);
	if ((*move).srcPiece == KNIGHT || (*move).srcPiece == BLACK_KNIGHT)
		return knightCanMove(board, move);
	if ((*move).srcPiece == PAWN || (*move).srcPiece == BLACK_PAWN)
		return pawnCanMove(board, move);
	if ((*move).srcPiece == KING || (*move).srcPiece == BLACK_KING)
		return kingCanMove(board, move);
	return 0;
}

/*****************************************************************************************************************************
* Function Name: findMyKing
* Input: char board[][], int isWhite(1 - for white, 0- for black), int* row(pointer to int) , int* col(pointer to int)
* Output: no output
* Function Operation: the function gets board and indicators that represent the place of the king in the board.
						the function run all over the board and find the king with the same color in the input,
						and replace the value of the pointers in the input according the place of the king that found.
*****************************************************************************************************************************/
void findMyKing(char board[][SIZE], int isWhite, int* row, int* col)
{
	// char that represent the king that we need to find (white or black)
	char newKing;
	if (isWhite)
		newKing = KING;
	else
		newKing = BLACK_KING;
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (board[i][j] == newKing)
			{
				*row = i;
				*col = j;
				return;
			}
}

/*****************************************************************************************************************************
* Function Name: isMyKingInCheck
* Input: char board[][], int isWhite(1 - for white, 0- for black)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and color and check if the king in this color is in check.
*****************************************************************************************************************************/
int isMyKingInCheck(char board[][SIZE], int isWhite)
{
	// first we need to find where is the king.
	int iKing = 0, jKing = 0;
	findMyKing(board, isWhite, &iKing, &jKing);
	// the function run all over the board and check if there is piece that threaten the king.
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			// the piece that threaten the king has to be in different place, and with different color from the king.
			if (!(i == iKing && j == jKing) && (board[i][j])!= EMPTY && myIsUpper(board[i][j]) != isWhite)
			{
				/*
				we need to build move that represent move from the place that the loop reachd in the board
				to the place of the king.
				if that piece can march and arrive to the king, it means that the king is in check.
				*/
				Move tempMove = { 0 };
				buildTempMove(board, &tempMove, i, j, iKing, jKing);
				tempMove.isCapture = 1;
				if (canMove(board, &tempMove))
					return 1;
			}
	}
	return 0;
}

/*****************************************************************************************************************************
* Function Name: myKingInCheckAfterMove
* Input: char board[][], Move* move(pointer to a move), int isWhite(1 - for white, 0- for black)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function check if the king in the color from the input will be in check after the move.
*****************************************************************************************************************************/
int myKingInCheckAfterMove(char board[][SIZE], Move* move, int isWhite)
{
	// variable that indicates if the king will be in check after the move.
	int isCheck = 0;
	// first we need to change the board according to the move.
	if ((*move).isPromotion)
		board[(*move).iDest][(*move).jDest] = (*move).promotionPiece;
	else
		board[(*move).iDest][(*move).jDest] = (*move).srcPiece;
	board[(*move).iSrc][(*move).jSrc] = EMPTY;
	// we need to check if the king is in check after we changed the board.
	if (isMyKingInCheck(board, isWhite))
		isCheck = 1;
	// now we need to return the board to the same situation it was before the move.
	board[(*move).iSrc][(*move).jSrc] = (*move).srcPiece;
	board[(*move).iDest][(*move).jDest] = (*move).destPiece;
	return isCheck;
}

/*****************************************************************************************************************************
* Function Name: findTheRow
* Input: char board[][], Move* move(pointer to a move), int col(the column of the source piece)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board, move and column and try to find the row of the source piece.
						if it succeed it change the row of the source piece in the move and return 1.
						otherwise, it return 0.
*****************************************************************************************************************************/
int findTheRow(char board[][SIZE], Move* move, int col)
{
	int isWhite = (*move).isWhite;
	int srcPiece = (*move).srcPiece;
	for (int i = 0; i < SIZE; i++)
	{
		// if is the same piece and is the same color
		if (board[i][col] == srcPiece && isWhite == myIsUpper(board[i][col]))
		{
			/*
			we need to build move that represent move from the place that the loop reachd in the board
			to the place of the destenation.
			*/
			Move tempMove = { 0 };
			buildTempMove(board, &tempMove, i, col, (*move).iDest, (*move).jDest);
			tempMove.isCapture = (*move).isCapture;

			/*
			if that piece can arrive the destenation, and after his move there is no check on his king,
			it means that the piece is the source piece and his row is the row that we looked for.
			*/
			if (canMove(board, &tempMove) && !(myKingInCheckAfterMove(board, &tempMove, tempMove.isWhite)))
			{
				(*move).iSrc = i;
				return 1;
			}
		}
	}
	return 0;
}

/*****************************************************************************************************************************
* Function Name: findTheCol
* Input: char board[][], Move* move(pointer to a move), int row(the row of the source piece)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board, move and row and try to find the column of the source piece.
						if it succeed it change the column of the source piece in the move and return 1.
						otherwise, it return 0.
*****************************************************************************************************************************/
int findTheCol(char board[][SIZE], Move* move, int row)
{
	int isWhite = (*move).isWhite;
	int srcPiece = (*move).srcPiece;
	for (int j = 0; j < SIZE; j++)
	{
		// if is the same piece and is the same color
		if (board[row][j] == srcPiece && isWhite == myIsUpper(board[row][j]))
		{
			/*
			we need to build move that represent move from the place that the loop reachd in the board
			to the place of the destenation.
			*/
			Move tempMove = { 0 };
			buildTempMove(board, &tempMove, row, j, (*move).iDest, (*move).jDest);
			tempMove.isCapture = (*move).isCapture;

			/*
			if that piece can arrive the destenation, and after his move there is no check on his king,
			it means that the piece is the source piece and his column is the column that we looked for.
			*/
			if (canMove(board, &tempMove) && !myKingInCheckAfterMove(board, &tempMove, tempMove.isWhite))
			{
				(*move).jSrc = j;
				return 1;
			}
		}
	}
	return 0;
}

/*****************************************************************************************************************************
* Function Name: findStartingPoint
* Input: char board[][], Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and move and try to find the column or the row (or both) of the source piece.
						if it succeed it change the column or the row of the source piece in the move and return 1.
						otherwise, it return 0.
*****************************************************************************************************************************/
int findStartingPoint(char board[][SIZE], Move* move)
{
	// case that the row and the column of the source piece are unknown
	if ((*move).srcCol == '\0' && (*move).srcRow == '\0')
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (findTheCol(board, move, i))
			{
				(*move).iSrc = i;
				return 1;
			}
		}
		return 0;
	}
	// case that the column of the source piece is unknown
	if ((*move).srcCol == '\0')
		return findTheCol(board, move, (*move).iSrc);
	// case that the row of the source piece is unknown
	if ((*move).srcRow == '\0')
		return findTheRow(board, move, (*move).jSrc);
	return 0;
}

/*****************************************************************************************************************************
* Function Name: legalCapture
* Input: char board[][], Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and move, and check if the move do a legal capture.
*****************************************************************************************************************************/
int legalCapture(char board[][SIZE], Move* move)
{
	/*
	if the source piece doesn't want to capture but the destenation in not empty,
	or that the source piece want to capture but the destenation is empty - the move is illegal.
	*/
	if ((!(*move).isCapture && board[(*move).iDest][(*move).jDest] != EMPTY) ||
		((*move).isCapture && board[(*move).iDest][(*move).jDest] == EMPTY))
		return 0;
	// if the source piece want to captue a piece with a color like him - the move is illegal.
	if ((*move).isCapture && (myIsUpper((*move).srcPiece) == myIsUpper((*move).destPiece)))
		return 0;
	// otherwise, the move is legal.
	return 1;
}

/*****************************************************************************************************************************
* Function Name: legalPromotion
* Input: char board[][], Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and move, and check if the move do a legal promotion.
*****************************************************************************************************************************/
int legalPromotion(char board[][SIZE], Move* move)
{
	// if is the white turn
	if ((*move).isWhite)
	{
		if ((*move).isPromotion)
		{
			/*
			if the source piece want to promotion, it has to be a white pawn that move to the first row of the board,
			and the promotion piece can't be king or pawn.
			*/
			if ((*move).srcPiece == PAWN && (*move).iDest == 0 &&
				(*move).promotionPiece != KING && (*move).promotionPiece != PAWN)
				return 1;
			return 0;
		}
		else
		{
			// if the source piece doesn't do promotion but it move to the first row of the board - the move is illegal.
			if ((*move).srcPiece == PAWN && (*move).iDest == 0)
				return 0;
			return 1;
		}
	}
	// if is the black turn
	else
	{
		if ((*move).isPromotion)
		{
			/*
			if the source piece want to promotion, it has to be a black pawn that move to the last row of the board,
			and the promotion piece can't be king or pawn.
			*/
			if ((*move).srcPiece == BLACK_PAWN && (*move).iDest == SIZE - 1 &&
				(*move).promotionPiece != BLACK_KING && (*move).promotionPiece != BLACK_PAWN)
				return 1;
			return 0;
		}
		else
		{
			// if the source piece doesn't do promotion but it move to the last row of the board - the move is illegal.
			if ((*move).srcPiece == BLACK_PAWN && (*move).iDest == SIZE - 1)
				return 0;
			return 1;
		}
	}
}

/*****************************************************************************************************************************
* Function Name: legalCheck
* Input: char board[][], Move* move(pointer to a move)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function gets board and move, and check if the move do a legal check.
*****************************************************************************************************************************/
int legalCheck(char board[][SIZE], Move* move)
{
	// if after the move there is check on the king of the player that do the move - the move is illegal.
	int isCheck = myKingInCheckAfterMove(board, move, (*move).isWhite);
	if (isCheck)
		return 0;
	int opponentColor = !(*move).isWhite;
	int isOpponentKingInCheck = myKingInCheckAfterMove(board, move, opponentColor);

	/*
	if after the move there is check on the king of the opponent player but there is no claim to check, 
	or that after the move there is no check on the king of the opponent player but there is claim to check
	- the move is illegal.
	*/
	if (((*move).isCheck && isOpponentKingInCheck) || (!(*move).isCheck && !isOpponentKingInCheck))
		return 1;
	return 0;
}

/*****************************************************************************************************************************
* Function Name: changeBoard
* Input: char board[][], Move* move(pointer to a move)
* Output: no output
* Function Operation: the function gets board and move, change the board according to the move.
*****************************************************************************************************************************/
void changeBoard(char board[][SIZE], Move* move)
{
	// if there is promotion the piece in the destenation will be the promotion piece.
	if ((*move).isPromotion)
		board[(*move).iDest][(*move).jDest] = (*move).promotionPiece;
	// otherwise, the piece in the destenation will be the source piece.
	else
		board[(*move).iDest][(*move).jDest] = (*move).srcPiece;
	board[(*move).iSrc][(*move).jSrc] = EMPTY;
}

/*****************************************************************************************************************************
* Function Name: makeMove
* Input: char board[][], char pgn[](string of the move), int isWhite(1 - for white, 0- for black)
* Output: int (1 if it's TRUE, 0 if is FALSE)
* Function Operation: the function check if the move is legal  or not.
*****************************************************************************************************************************/
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn)
{
	Move move = { 0 };
	buildMoveFromPgn(&move, pgn, board, isWhiteTurn);
	// if the column of the source piece or the row of the source piece is unknown, we has to find them.
	if ((move).srcCol == '\0' || (move).srcRow == '\0')
	{
		int isStartingPoint = findStartingPoint(board, &move);
		if (!isStartingPoint)
			return 0;
	}
	// if the source piece can arrive to the destenation and does a legal capture, promotion and check - the move is legal.
	if (canMove(board,&move) && legalCapture(board, &move) && legalPromotion(board, &move) && legalCheck(board, &move))
	{
		move.isLegal = 1;
		changeBoard(board, &move);
		return 1;
	}
	return 0;
}