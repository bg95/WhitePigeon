/*
 * 2013-05-15  Token  <token@token-HP-ENVY-14-SPECTRE-Notebook-PC>

 * 
 * selfplayer
 * It is used to play the sound.
 * Get all the sound nearly and mix them together.
 */
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_mixer.h"

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <map>

#define tr(i, x) for (typeof(x.begin()) i = x.begin(); i != x.end(); ++ i)
#define pii std::pair <int, int>

namespace Player
{
	struct TFrac
	{
		int X, Y;
	};
	
	struct TScore
	{
		int ScoreType;
		int ScoreHigh;
		int ScoreForce;
		TFrac ScoreLen;
		double ScoreFreq;
		int Part;
		std::string Instrument;
	};
	
	std::map <std::string, TScore> MapStack;
	
	bool GetNextScore(std::ofstream &fin, TScore &sc)
	{
		return 0;
	}
	
	void AddScore(TScore sc)
	{
	}
	
	void Initialize()
	{
		MapStack.clear();
	}
	
	bool CanPop()
	{
		return 0;
	}
	
	void Pop()
	{
		
		tr (it, MapStack)
		{
		}
	}
	
	void Run(char *File)
	{
		std::ofstream fin(File);
		TScore sc;
		Initialize();
		while (GetNextScore(fin, sc))
		{
			AddScore(sc);
			while (CanPop())
				Pop();
		}
		fin.close();
	}
}
