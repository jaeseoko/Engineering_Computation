#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nCorrect = 0;
char answer[256];

class FlashCard
{
public:
	int a,b;
	void PrintCard(void);
	int CorrectAnswer(void);
};

void FlashCard::PrintCard(void)
{
	printf("%dx%d=",a,b);
	fgets(answer,255,stdin);
}
int FlashCard::CorrectAnswer(void)
{
	return a*b;
}
void SwapFlashCard(FlashCard &card1,FlashCard &card2)
{
	int x   =card1.a;
	int y   =card1.b;
	card1   =card2;
	card2.a =x;
	card2.b =y;
}

void ShuffleCards(FlashCard card[144])
{
	int i;
	for(i=0; i<144; i++)
	{
		card[i].a=i;
		card[i].b=i;
	}

	for(i=0; i<144; i++)
	{
		int r=rand()%144;
		SwapFlashCard(card[i],card[r]);
	}
}

int main(void)
{
	FlashCard card[144];
	int nCard;
	unsigned int t0;
	char nCards[256];
	
	for(;;)
	{
		printf("How many cards to work on?\n");
		printf(">");
		nCard = atoi(fgets(nCards,255,stdin));

		if(nCard<1 || 144<nCard)
		{
			printf("The number of cards must be between 1 and 144.\n");
		}
		else
		{
			break;
		}
	}

	t0=time(NULL);
	srand(t0);

	ShuffleCards(card);

	for(int i =0; i < nCard; ++i)
	{
		card[i].a=1+card[i].a%12;
		card[i].b=1+card[i].b/12;
		card[i].PrintCard();
		if(card[i].CorrectAnswer() == atoi(answer))
		{
			printf("Correct!\n");
			nCorrect++;
		}
		else
		{
			printf("Wrong!  Correct answer is %d\n",card[i].CorrectAnswer());
		}
	}

	printf("You have worked on %d problems.\n",nCard);
	printf("You answered %d problems correctly. (%d%%)\n",nCorrect,nCorrect*100/nCard);
	printf("You spent %d seconds to answer all problems.\n",time(NULL)-t0);

	return 0;
}

