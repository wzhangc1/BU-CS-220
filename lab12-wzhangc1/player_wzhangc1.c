rps player_wzhangc1(int round, rps *myhist, rps *opphist)
{
	printf("%d\n", round);

	if (round <= 0)
	{
		return Paper;
	}

	int w = 0;
	int d = 0;
	int l = d;

	if ((myhist[round - 1] == Rock && opphist[round - 1] == Scissors) || (myhist[round - 1] == Paper && opphist[round - 1] == Rock) || (myhist[round - 1] == Scissors && opphist[round - 1] == Paper))
	{
		for (int i = 0; i < round - 1; i++)
		{
			if ((myhist[i] == Rock && opphist[i] == Scissors) || (myhist[i] == Paper && opphist[i] == Rock) || (myhist[i] == Scissors && opphist[i] == Paper))
			{
				if ((opphist[i] == Rock && opphist[i + 1] == Scissors) || (opphist[i] == Paper && opphist[i + 1] == Rock) || (opphist[i] == Scissors && opphist[i + 1] == Paper))
				{
					l++;
				}
				else if (opphist[i] == opphist[i + 1])
				{
					d++;
				}
				else
				{
					w++;
				}
			}
		}
	}
	else if (myhist[round - 1] == opphist[round - 1])
	{
		for (int i = 0; i < round - 1; i++)
		{
			if (myhist[i] == opphist[i])
			{
				if ((opphist[i] == Rock && opphist[i + 1] == Scissors) || (opphist[i] == Paper && opphist[i + 1] == Rock) || (opphist[i] == Scissors && opphist[i + 1] == Paper))
				{
					l++;
				}
				else if (opphist[i] == opphist[i + 1])
				{
					d++;
				}
				else
				{
					w++;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < round - 1; i++)
		{
			if ((myhist[i] == Rock && opphist[i] == Paper) || (myhist[i] == Paper && opphist[i] == Scissors) || (myhist[i] == Scissors && opphist[i] == Rock))
			{
				if ((opphist[i] == Rock && opphist[i + 1] == Scissors) || (opphist[i] == Paper && opphist[i + 1] == Rock) || (opphist[i] == Scissors && opphist[i + 1] == Paper))
				{
					l++;
				}
				else if (opphist[i] == opphist[i + 1])
				{
					d++;
				}
				else
				{
					w++;
				}
			}
		}
	}

	int i;

	if ((w + l + d) > 0)
	{
		i = ((round - w)*(round - l)*(round - d)) % (w + l + d);
	}
	else
	{
		i = 0;
	}

	if (opphist[round - 1] == Rock)
	{
		if (i < w)
		{
			return Scissors;
		}
		else if (i < w + d)
		{
			return Paper;
		}
		else
		{
			return Rock;
		}
	}
	else if (opphist[round - 1] == Paper)
	{
		if (i < w)
		{
			return Rock;
		}
		else if (i < w + d)
		{
			return Scissors;
		}
		else
		{
			return Paper;
		}
	}
	else
	{
		if (i < w)
		{
			return Paper;
		}
		else if (i < w + d)
		{
			return Rock;
		}
		else
		{
			return Scissors;
		}
	}
}

register_player(player_wzhangc1, "wzhangc1");
